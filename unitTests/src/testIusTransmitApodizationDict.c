//
// Created by nlv09165 on 03/05/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <ius.h>
#include <util.h>
#include <iusTransmitApodizationDictPrivate.h>
#include <string.h>

static char *pErrorFilename = "IusTransmitApodizationDict.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(IusTransmitApodizationDict);

TEST_SETUP(IusTransmitApodizationDict)
{
	iusErrorLogClear();
    iusErrorLog(IUS_TRUE);
    iusErrorAutoReportSet(IUS_TRUE);
	fpErrorLogging = fopen(pErrorFilename, "w+");
	iusErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IusTransmitApodizationDict)
{
    iusErrorLogClear();
    iusErrorLog(IUS_FALSE);
    if (fpErrorLogging != NULL)
    	fclose(fpErrorLogging);
	fpErrorLogging=stderr;
	iusErrorSetStream(fpErrorLogging);
	remove(pErrorFilename);
}


TEST(IusTransmitApodizationDict, testIusTransmitApodizationDictCreate)
{
	iutad_t obj = iusTransmitApodizationDictCreate();
	iutad_t notherObj = iusTransmitApodizationDictCreate();

	TEST_ASSERT(obj != IUTAD_INVALID);
	TEST_ASSERT(notherObj != IUTAD_INVALID);
	iusTransmitApodizationDictDelete(obj);
	iusTransmitApodizationDictDelete(notherObj);
}

TEST(IusTransmitApodizationDict, testIusTransmitApodizationDictSetGet)
{
	const int numElements = 8;
	float ones[8] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
	int status;
	char *pObjLabel = "ones";
	iuta_t obj = iusTransmitApodizationCreate(ones, numElements);

	iutad_t dict = iusTransmitApodizationDictCreate();
	status = iusTransmitApodizationDictSet(dict, pObjLabel, obj);
	TEST_ASSERT(status == IUS_E_OK);


	iuta_t retrievedObj = iusTransmitApodizationDictGet(dict, pObjLabel);
	TEST_ASSERT_EQUAL(IUS_TRUE, iusTransmitApodizationCompare(obj, retrievedObj));

	// Invalid params
	long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());
	TEST_ASSERT_EQUAL(IUTA_INVALID, iusTransmitApodizationDictGet(dict,NULL));
	TEST_ASSERT_EQUAL(IUTA_INVALID, iusTransmitApodizationDictGet(NULL,pObjLabel));
	TEST_ASSERT_EQUAL(IUTA_INVALID, iusTransmitApodizationDictGet(dict,"unknownLabel"));

	TEST_ASSERT_EQUAL(IUS_ERR_VALUE, iusTransmitApodizationDictSet(dict, pObjLabel, obj));
	TEST_ASSERT_EQUAL(IUS_ERR_VALUE, iusTransmitApodizationDictSet(NULL, pObjLabel, obj));
	TEST_ASSERT_EQUAL(IUS_ERR_VALUE, iusTransmitApodizationDictSet(dict, pObjLabel, NULL));

	TEST_ASSERT_EQUAL(6,iusErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
	iusTransmitApodizationDelete(obj);
	iusTransmitApodizationDictDelete(dict);
}


TEST(IusTransmitApodizationDict, testIusTransmitApodizationDictKeys)
{
    const int numElements = 8;
    float ones[8] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
    int status;
    char *labels[] = { "one" , "two" , "three", "four" , "five"};
    iuta_t obj = iusTransmitApodizationCreate(ones, numElements);

    iutad_t dict = iusTransmitApodizationDictCreate();

    // Fill dict
    int i;
    int keySize = sizeof(labels)/sizeof(labels[0]);
    for (i=0; i<keySize; i++)
    {
        status = iusTransmitApodizationDictSet(dict, labels[i], obj);
        TEST_ASSERT(status == IUS_E_OK);
    }

    // Get keys
    size_t dictSize = iusTransmitApodizationDictGetSize(dict);
    TEST_ASSERT_EQUAL(5, dictSize);
    char **keys = iusTransmitApodizationDictGetKeys(dict);

    // Validate keys
    for (i=0; i<keySize; i++)
    {
        TEST_ASSERT_EQUAL(IUS_TRUE, aInB(keys[i], labels));
    }

    // Invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());

    keys = iusTransmitApodizationDictGetKeys(NULL);
    TEST_ASSERT_EQUAL(NULL,keys);

    TEST_ASSERT_EQUAL(1,iusErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
    iusTransmitApodizationDelete(obj);
    iusTransmitApodizationDictDelete(dict);
}

TEST(IusTransmitApodizationDict, testIusTransmitApodizationDictCompare)
{
	IUS_BOOL equal;
	const int numElements = 8;
	float ones[8] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
	float ramp[8] = { 0.1f, 0.2f, 0.3f, 0.4f, 0.4f, 0.3f, 0.2f, 0.1f };
	int     status;
	iuta_t transmitApodization = iusTransmitApodizationCreate(ones, numElements);
	iuta_t otherTransmitApodization = iusTransmitApodizationCreate(ramp, numElements);

	iutad_t dict = iusTransmitApodizationDictCreate();
	status = iusTransmitApodizationDictSet(dict, "ones", transmitApodization);
	TEST_ASSERT(status == IUS_E_OK);

	iutad_t notherDict = iusTransmitApodizationDictCreate();
	status = iusTransmitApodizationDictSet(dict, "triangle", transmitApodization);
    TEST_ASSERT(status == IUS_E_OK);

	TEST_ASSERT(dict != IUTAD_INVALID);
	TEST_ASSERT(notherDict != IUTAD_INVALID);

	// Same empty lists...
	equal = iusTransmitApodizationDictCompare(dict, dict);
	TEST_ASSERT_EQUAL(IUS_TRUE, equal);
	equal = iusTransmitApodizationDictCompare(dict, notherDict);
	TEST_ASSERT_EQUAL(IUS_FALSE, equal);

	// invalid params
	equal = iusTransmitApodizationDictCompare(dict, NULL);
	TEST_ASSERT_EQUAL(IUS_FALSE, equal);
	equal = iusTransmitApodizationDictCompare(NULL, dict);
	TEST_ASSERT_EQUAL(IUS_FALSE, equal);

	iusTransmitApodizationDelete(transmitApodization);
	iusTransmitApodizationDelete(otherTransmitApodization);
	iusTransmitApodizationDictDelete(dict);
	iusTransmitApodizationDictDelete(notherDict);
}

TEST(IusTransmitApodizationDict, testIusTransmitApodizationDictSerialization)
{
	char *filename = "testIusTransmitApodizationSerialization.hdf5";

	int numElements = 8;
	char *label = "ones";
	float ones[8] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };

	// create and save
	iuta_t obj = iusTransmitApodizationCreate(ones, numElements);

	iutad_t transmitApodizationDict = iusTransmitApodizationDictCreate();
	int status = iusTransmitApodizationDictSet(transmitApodizationDict, label, obj);
    TEST_ASSERT(status == IUS_E_OK);

	hid_t handle = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
	TEST_ASSERT(handle > 0);

	status = iusTransmitApodizationDictSave(transmitApodizationDict, handle);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);
	H5Fclose(handle);

	// read back
	handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT);
	TEST_ASSERT(handle > 0);

	iutad_t savedDict = iusTransmitApodizationDictLoad(handle);
	TEST_ASSERT_NOT_EQUAL(NULL, savedDict);
	H5Fclose(handle);

	TEST_ASSERT_EQUAL(IUS_TRUE, iusTransmitApodizationDictCompare(transmitApodizationDict, savedDict));

    // invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());

    status = iusTransmitApodizationDictSave(transmitApodizationDict, -1);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
    status = iusTransmitApodizationDictSave(NULL, handle);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
    savedDict = iusTransmitApodizationDictLoad(-1);
    TEST_ASSERT_EQUAL(NULL, savedDict);

    TEST_ASSERT_EQUAL(3,iusErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

	iusTransmitApodizationDelete(obj);
	iusTransmitApodizationDictDelete(transmitApodizationDict);
	iusTransmitApodizationDictDelete(savedDict);
}

TEST_GROUP_RUNNER(IusTransmitApodizationDict)
{
	RUN_TEST_CASE(IusTransmitApodizationDict, testIusTransmitApodizationDictCreate);
    RUN_TEST_CASE(IusTransmitApodizationDict, testIusTransmitApodizationDictKeys);
	RUN_TEST_CASE(IusTransmitApodizationDict, testIusTransmitApodizationDictSetGet);
	RUN_TEST_CASE(IusTransmitApodizationDict, testIusTransmitApodizationDictCompare);
	RUN_TEST_CASE(IusTransmitApodizationDict, testIusTransmitApodizationDictSerialization);
}
