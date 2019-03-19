//
// Created by nlv09165 on 03/05/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <iuf.h>
#include <util.h>
#include <iufTransmitApodizationDictPrivate.h>
#include <string.h>

static char *pErrorFilename = "IufTransmitApodizationDict.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(IufTransmitApodizationDict);

TEST_SETUP(IufTransmitApodizationDict)
{
	iufErrorLogClear();
    iufErrorLog(IUF_TRUE);
    iufErrorAutoReportSet(IUF_TRUE);
	fpErrorLogging = fopen(pErrorFilename, "w+");
	iufErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IufTransmitApodizationDict)
{
    iufErrorLogClear();
    iufErrorLog(IUF_FALSE);
    if (fpErrorLogging != NULL)
    	fclose(fpErrorLogging);
	fpErrorLogging=stderr;
	iufErrorSetStream(fpErrorLogging);
	remove(pErrorFilename);
}


TEST(IufTransmitApodizationDict, testIufTransmitApodizationDictCreate)
{
	iutad_t obj = iufTransmitApodizationDictCreate();
	iutad_t notherObj = iufTransmitApodizationDictCreate();

	TEST_ASSERT(obj != IUTAD_INVALID);
	TEST_ASSERT(notherObj != IUTAD_INVALID);
	iufTransmitApodizationDictDelete(obj);
	iufTransmitApodizationDictDelete(notherObj);
}

TEST(IufTransmitApodizationDict, testIufTransmitApodizationDictSetGet)
{
	const int numElements = 8;
	float ones[8] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
	int status;
	char *pObjLabel = "ones";
	iuta_t obj = iufTransmitApodizationCreate(ones, numElements);

	iutad_t dict = iufTransmitApodizationDictCreate();
	status = iufTransmitApodizationDictSet(dict, pObjLabel, obj);
	TEST_ASSERT(status == IUF_E_OK);


	iuta_t retrievedObj = iufTransmitApodizationDictGet(dict, pObjLabel);
	TEST_ASSERT_EQUAL(IUF_TRUE, iufTransmitApodizationCompare(obj, retrievedObj));

	// Invalid params
	long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());
	TEST_ASSERT_EQUAL(IUTA_INVALID, iufTransmitApodizationDictGet(dict,NULL));
	TEST_ASSERT_EQUAL(IUTA_INVALID, iufTransmitApodizationDictGet(NULL,pObjLabel));
	TEST_ASSERT_EQUAL(IUTA_INVALID, iufTransmitApodizationDictGet(dict,"unknownLabel"));

	TEST_ASSERT_EQUAL(IUF_ERR_VALUE, iufTransmitApodizationDictSet(dict, pObjLabel, obj));
	TEST_ASSERT_EQUAL(IUF_ERR_VALUE, iufTransmitApodizationDictSet(NULL, pObjLabel, obj));
	TEST_ASSERT_EQUAL(IUF_ERR_VALUE, iufTransmitApodizationDictSet(dict, pObjLabel, NULL));

	TEST_ASSERT_EQUAL(6,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
	iufTransmitApodizationDelete(obj);
	iufTransmitApodizationDictDelete(dict);
}


TEST(IufTransmitApodizationDict, testIufTransmitApodizationDictKeys)
{
    const int numElements = 8;
    float ones[8] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
    int status;
    char *labels[] = { "one" , "two" , "three", "four" , "five"};
    iuta_t obj = iufTransmitApodizationCreate(ones, numElements);

    iutad_t dict = iufTransmitApodizationDictCreate();

    // Fill dict
    int i;
    int keySize = sizeof(labels)/sizeof(labels[0]);
    for (i=0; i<keySize; i++)
    {
        status = iufTransmitApodizationDictSet(dict, labels[i], obj);
        TEST_ASSERT(status == IUF_E_OK);
    }

    // Get keys
    size_t dictSize = iufTransmitApodizationDictGetSize(dict);
    TEST_ASSERT_EQUAL(5, dictSize);
    char **keys = iufTransmitApodizationDictGetKeys(dict);

    // Validate keys
    for (i=0; i<keySize; i++)
    {
        TEST_ASSERT_EQUAL(IUF_TRUE, aInB(keys[i], labels));
    }

    // Invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    keys = iufTransmitApodizationDictGetKeys(NULL);
    TEST_ASSERT_EQUAL(NULL,keys);

    TEST_ASSERT_EQUAL(1,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
    iufTransmitApodizationDelete(obj);
    iufTransmitApodizationDictDelete(dict);
}

TEST(IufTransmitApodizationDict, testIufTransmitApodizationDictCompare)
{
	IUF_BOOL equal;
	const int numElements = 8;
	float ones[8] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
	float ramp[8] = { 0.1f, 0.2f, 0.3f, 0.4f, 0.4f, 0.3f, 0.2f, 0.1f };
	int     status;
	iuta_t transmitApodization = iufTransmitApodizationCreate(ones, numElements);
	iuta_t otherTransmitApodization = iufTransmitApodizationCreate(ramp, numElements);

	iutad_t dict = iufTransmitApodizationDictCreate();
	status = iufTransmitApodizationDictSet(dict, "ones", transmitApodization);
	TEST_ASSERT(status == IUF_E_OK);

	iutad_t notherDict = iufTransmitApodizationDictCreate();
	status = iufTransmitApodizationDictSet(dict, "triangle", transmitApodization);
    TEST_ASSERT(status == IUF_E_OK);

	TEST_ASSERT(dict != IUTAD_INVALID);
	TEST_ASSERT(notherDict != IUTAD_INVALID);

	// Same empty lists...
	equal = iufTransmitApodizationDictCompare(dict, dict);
	TEST_ASSERT_EQUAL(IUF_TRUE, equal);
	equal = iufTransmitApodizationDictCompare(dict, notherDict);
	TEST_ASSERT_EQUAL(IUF_FALSE, equal);

	// invalid params
	equal = iufTransmitApodizationDictCompare(dict, NULL);
	TEST_ASSERT_EQUAL(IUF_FALSE, equal);
	equal = iufTransmitApodizationDictCompare(NULL, dict);
	TEST_ASSERT_EQUAL(IUF_FALSE, equal);

	iufTransmitApodizationDelete(transmitApodization);
	iufTransmitApodizationDelete(otherTransmitApodization);
	iufTransmitApodizationDictDelete(dict);
	iufTransmitApodizationDictDelete(notherDict);
}

TEST(IufTransmitApodizationDict, testIufTransmitApodizationDictSerialization)
{
	char *filename = "testIufTransmitApodizationSerialization.hdf5";

	int numElements = 8;
	char *label = "ones";
	float ones[8] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };

	// create and save
	iuta_t obj = iufTransmitApodizationCreate(ones, numElements);

	iutad_t transmitApodizationDict = iufTransmitApodizationDictCreate();
	int status = iufTransmitApodizationDictSet(transmitApodizationDict, label, obj);
    TEST_ASSERT(status == IUF_E_OK);

	hid_t handle = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
	TEST_ASSERT(handle > 0);

	status = iufTransmitApodizationDictSave(transmitApodizationDict, handle);
	TEST_ASSERT_EQUAL(IUF_E_OK, status);
	H5Fclose(handle);

	// read back
	handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT);
	TEST_ASSERT(handle > 0);

	iutad_t savedDict = iufTransmitApodizationDictLoad(handle);
	TEST_ASSERT_NOT_EQUAL(NULL, savedDict);
	H5Fclose(handle);

	TEST_ASSERT_EQUAL(IUF_TRUE, iufTransmitApodizationDictCompare(transmitApodizationDict, savedDict));

    // invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    status = iufTransmitApodizationDictSave(transmitApodizationDict, -1);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, status);
    status = iufTransmitApodizationDictSave(NULL, handle);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, status);
    savedDict = iufTransmitApodizationDictLoad(-1);
    TEST_ASSERT_EQUAL(NULL, savedDict);

    TEST_ASSERT_EQUAL(3,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

	iufTransmitApodizationDelete(obj);
	iufTransmitApodizationDictDelete(transmitApodizationDict);
	iufTransmitApodizationDictDelete(savedDict);
}

TEST_GROUP_RUNNER(IufTransmitApodizationDict)
{
	RUN_TEST_CASE(IufTransmitApodizationDict, testIufTransmitApodizationDictCreate);
    RUN_TEST_CASE(IufTransmitApodizationDict, testIufTransmitApodizationDictKeys);
	RUN_TEST_CASE(IufTransmitApodizationDict, testIufTransmitApodizationDictSetGet);
	RUN_TEST_CASE(IufTransmitApodizationDict, testIufTransmitApodizationDictCompare);
	RUN_TEST_CASE(IufTransmitApodizationDict, testIufTransmitApodizationDictSerialization);
}
