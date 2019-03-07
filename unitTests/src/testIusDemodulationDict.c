//
// Created by nlv12901 on 15/01/2019.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <ius.h>
#include <util.h>
#include <iusDemodulationDictPrivate.h>
#include <dg/dataGenerators.h>


static char *pErrorFilename = "IusDemodulationDict.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(IusDemodulationDict);

TEST_SETUP(IusDemodulationDict)
{
    iusErrorLogClear();
    iusErrorLog(IUS_TRUE);
    iusErrorAutoReportSet(IUS_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iusErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IusDemodulationDict)
{
    iusErrorLogClear();
    iusErrorLog(IUS_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iusErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
}


TEST(IusDemodulationDict, testIusCreateDict)
{
	iudmd_t obj = iusDemodulationDictCreate();
	iudmd_t notherObj = iusDemodulationDictCreate();

	TEST_ASSERT(obj != IUDMD_INVALID);
	TEST_ASSERT(notherObj != IUDMD_INVALID);
	iusDemodulationDictDelete(obj);
	iusDemodulationDictDelete(notherObj);
}


TEST(IusDemodulationDict, testIusSetGetDict)
{
	char *pObjLabel = "bmode";
	float sampleFrequency = 4000;
	int numSamplesPerLine = 10;
	int numTGCentries = 1;
	IusDemodulationMethod method = IUS_DEMODULATION_FOURX;
	int kernelSize = 5;
	int status;


	iudm_t obj = iusDemodulationCreate(method, sampleFrequency, numSamplesPerLine, numTGCentries, kernelSize);


	// Create
	iudmd_t  dict = iusDemodulationDictCreate();

	// Fill
	status = iusDemodulationDictSet(dict, pObjLabel, obj);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);
	iudm_t retrievedObj = iusDemodulationDictGet(dict, pObjLabel);
	TEST_ASSERT_EQUAL(IUS_TRUE, iusDemodulationCompare(obj, retrievedObj));

	// Invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());
    TEST_ASSERT_EQUAL(IUTA_INVALID, iusDemodulationDictGet(dict,NULL));
    TEST_ASSERT_EQUAL(IUTA_INVALID, iusDemodulationDictGet(NULL,pObjLabel));
    TEST_ASSERT_EQUAL(IUTA_INVALID, iusDemodulationDictGet(dict,"unknownLabel"));

    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, iusDemodulationDictSet(dict, pObjLabel, obj));
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, iusDemodulationDictSet(NULL, pObjLabel, obj));
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, iusDemodulationDictSet(dict, pObjLabel, NULL));

    TEST_ASSERT_EQUAL(6,iusErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
    iusDemodulationDelete(obj);
	iusDemodulationDictDelete(dict);
}

TEST(IusDemodulationDict, testIusDictGetKeys)
{
    char *labels[] = { "one" , "two" , "three", "four" , "five"};
    float sampleFrequency = 4000;
    int numSamplesPerLine = 10;
    int numTGCentries = 1;
    IusDemodulationMethod method = IUS_DEMODULATION_FOURX;
    int kernelSize = 5;
    int status;


    iudm_t obj = iusDemodulationCreate(method, sampleFrequency, numSamplesPerLine, numTGCentries, kernelSize);


    // Create
    iudmd_t  dict = iusDemodulationDictCreate();

    // Fill
    int i;
    int keySize = sizeof(labels)/sizeof(labels[0]);
    for (i=0; i<keySize; i++)
    {
        status = iusDemodulationDictSet(dict, labels[i], obj);
        TEST_ASSERT(status == IUS_E_OK);
    }

    size_t dictSize = iusDemodulationDictGetSize(dict);
    TEST_ASSERT_EQUAL(5, dictSize);
    char **keys = iusDemodulationDictGetKeys(dict);

    // Validate keys
    for (i=0; i<keySize; i++)
    {
        TEST_ASSERT_EQUAL(IUS_TRUE, aInB(keys[i], labels));
    }

    // Invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());

    keys = iusDemodulationDictGetKeys(NULL);
    TEST_ASSERT_EQUAL(NULL,keys);

    TEST_ASSERT_EQUAL(1,iusErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
    iusDemodulationDelete(obj);
    iusDemodulationDictDelete(dict);
}

TEST(IusDemodulationDict, testIusCompareDict)
{
	IUS_BOOL equal;
	char *pObjLabel = "Label for IusDemodulationDict, created in testIusCompareSourceDict";
	char *pNotherObjLabel = "Another Label for IusDemodulationDict, created in testIusCompareSourceDict";
	char *pDifferentLabel = "Different Label for IusDemodulationDict, created in testIusCompareSourceDict";
	float sampleFrequency = 4000;
	int numSamplesPerLine = 10;
	int numTGCentries = 1;
	IusDemodulationMethod method = IUS_DEMODULATION_FOURX;
	int kernelSize = 5;
	int status;

	iudm_t obj = iusDemodulationCreate(method, sampleFrequency, numSamplesPerLine, numTGCentries, kernelSize);
	iudm_t notherObj = iusDemodulationCreate(method, sampleFrequency, numSamplesPerLine, numTGCentries, kernelSize);
	iudm_t differentObj = iusDemodulationCreate(method, sampleFrequency, numSamplesPerLine, numTGCentries + 1, kernelSize);

	// Create
	iudmd_t  dict = iusDemodulationDictCreate();
	iudmd_t  notherDict = iusDemodulationDictCreate();

	// Same empty lists...
	equal = iusDemodulationDictCompare(dict, dict);
	TEST_ASSERT_EQUAL(IUS_TRUE, equal);
	equal = iusDemodulationDictCompare(dict, notherDict);
	TEST_ASSERT_EQUAL(IUS_TRUE, equal);

	// Fill
	status = iusDemodulationDictSet(dict, pObjLabel, obj);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);
	status = iusDemodulationDictSet(dict, pNotherObjLabel, notherObj);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);

	equal = iusDemodulationDictCompare(dict, notherDict);
	TEST_ASSERT_EQUAL(IUS_FALSE, equal);

	status = iusDemodulationDictSet(notherDict, pObjLabel, obj);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);
	status = iusDemodulationDictSet(notherDict, pNotherObjLabel, notherObj);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);

	equal = iusDemodulationDictCompare(dict, notherDict);
	TEST_ASSERT_EQUAL(IUS_TRUE, equal);

	status = iusDemodulationDictSet(notherDict, pDifferentLabel, differentObj);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);

	equal = iusDemodulationDictCompare(dict, notherDict);
	TEST_ASSERT_EQUAL(IUS_FALSE, equal);

	iusDemodulationDelete(obj);
	iusDemodulationDelete(differentObj);
	iusDemodulationDelete(notherObj);
	iusDemodulationDictDelete(dict);
	iusDemodulationDictDelete(notherDict);
}

TEST(IusDemodulationDict, testIusSerialization)
{
	char *filename = "testIusDemodulationDictSerialization.hdf5";
	//char *ReceiveChannelMapDictPath = "/DemodulationDict";

	// Create and fill
	iudmd_t  dict = dgGenerateDemodulationDict("bmode", 2000);

	// Save
	hid_t handle = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
	TEST_ASSERT(handle > 0);
	int status = iusDemodulationDictSave(dict, handle);
	H5Fclose(handle);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);

	// read back
	handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT);
	iudmd_t savedDict = iusDemodulationDictLoad(handle);
	TEST_ASSERT_NOT_EQUAL(NULL, savedDict);
	H5Fclose(handle);

	TEST_ASSERT_EQUAL(IUS_TRUE, iusDemodulationDictCompare(dict, savedDict));

	iudm_t rs = iusDemodulationDictGet(dict, "bmode");
	iusDemodulationDelete(rs);
	iusDemodulationDictDelete(dict);
	iusDemodulationDictDelete(savedDict);
}


TEST_GROUP_RUNNER(IusDemodulationDict)
{
	RUN_TEST_CASE(IusDemodulationDict, testIusCreateDict);
	RUN_TEST_CASE(IusDemodulationDict, testIusSetGetDict);
    RUN_TEST_CASE(IusDemodulationDict, testIusDictGetKeys)
	RUN_TEST_CASE(IusDemodulationDict, testIusCompareDict);
	RUN_TEST_CASE(IusDemodulationDict, testIusSerialization);
}
