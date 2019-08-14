#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <iuf.h>
#include <util.h>
#include <iufDemodulationDictPrivate.h>
#include <dg/dataGenerators.h>


static char *pErrorFilename = "IufDemodulationDict.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(IufDemodulationDict);

TEST_SETUP(IufDemodulationDict)
{
    iufErrorLogClear();
    iufErrorLog(IUF_TRUE);
    iufErrorAutoReportSet(IUF_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iufErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IufDemodulationDict)
{
    iufErrorLogClear();
    iufErrorLog(IUF_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iufErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
}


TEST(IufDemodulationDict, testIufCreateDeleteDict)
{
	iudmd_t obj = iufDemodulationDictCreate();
	iudmd_t notherObj = iufDemodulationDictCreate();

	TEST_ASSERT(obj != IUDMD_INVALID);
	TEST_ASSERT(notherObj != IUDMD_INVALID);
    TEST_ASSERT(iufDemodulationDictDelete(obj) == IUF_E_OK);
    TEST_ASSERT(iufDemodulationDictDelete(notherObj) == IUF_E_OK);
    TEST_ASSERT(iufDemodulationDictDelete(NULL) != IUF_E_OK);
    TEST_ASSERT(iufDemodulationDictDeepDelete(NULL) != IUF_E_OK);
}


TEST(IufDemodulationDict, testIufSetGetDict)
{
	char *pObjLabel = "bmode";
	float sampleFrequency = 4000;
	float centerFrequency = 2000;
	int numSamplesPerLine = 10;
	int numTGCentries = 1;
	IufDemodulationMethod method = IUF_DEMODULATION_FOURX;
	int kernelSize = 5;
	int status;


	iudm_t obj = iufDemodulationCreate(method, sampleFrequency, centerFrequency, numSamplesPerLine, numTGCentries, kernelSize);


	// Create
	iudmd_t  dict = iufDemodulationDictCreate();

	// Fill
	status = iufDemodulationDictSet(dict, pObjLabel, obj);
	TEST_ASSERT_EQUAL(IUF_E_OK, status);
	iudm_t retrievedObj = iufDemodulationDictGet(dict, pObjLabel);
	TEST_ASSERT_EQUAL(IUF_TRUE, iufDemodulationCompare(obj, retrievedObj));

	// Invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());
    TEST_ASSERT_EQUAL(IUTA_INVALID, iufDemodulationDictGet(dict,NULL));
    TEST_ASSERT_EQUAL(IUTA_INVALID, iufDemodulationDictGet(NULL,pObjLabel));
    TEST_ASSERT_EQUAL(IUTA_INVALID, iufDemodulationDictGet(dict,"unknownLabel"));

    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, iufDemodulationDictSet(dict, pObjLabel, obj));
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, iufDemodulationDictSet(NULL, pObjLabel, obj));
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, iufDemodulationDictSet(dict, pObjLabel, NULL));

    TEST_ASSERT_EQUAL(6,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
    iufDemodulationDelete(obj);
	iufDemodulationDictDelete(dict);
}

TEST(IufDemodulationDict, testIufDictGetKeys)
{
    char *labels[] = { "one" , "two" , "three", "four" , "five"};
    float sampleFrequency = 4000;
    float centerFrequency = 2000;
    int numSamplesPerLine = 10;
    int numTGCentries = 1;
    IufDemodulationMethod method = IUF_DEMODULATION_FOURX;
    int kernelSize = 5;
    int status;


    iudm_t obj = iufDemodulationCreate(method, sampleFrequency, centerFrequency, numSamplesPerLine, numTGCentries, kernelSize);


    // Create
    iudmd_t  dict = iufDemodulationDictCreate();

    // Fill
    int i;
    int keySize = sizeof(labels)/sizeof(labels[0]);
    for (i=0; i<keySize; i++)
    {
        status = iufDemodulationDictSet(dict, labels[i], obj);
        TEST_ASSERT(status == IUF_E_OK);
    }

    size_t dictSize = iufDemodulationDictGetSize(dict);
    TEST_ASSERT_EQUAL(5, dictSize);
    char **keys = iufDemodulationDictGetKeys(dict);

    // Validate keys
    for (i=0; i<keySize; i++)
    {
        TEST_ASSERT_EQUAL(IUF_TRUE, aInB(keys[i], labels));
    }

    // Invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    keys = iufDemodulationDictGetKeys(NULL);
    TEST_ASSERT_EQUAL(NULL,keys);

    TEST_ASSERT_EQUAL(1,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
    iufDemodulationDelete(obj);
    iufDemodulationDictDelete(dict);
}

TEST(IufDemodulationDict, testIufCompareDict)
{
	IUF_BOOL equal;
	char *pObjLabel = "Label for IufDemodulationDict, created in testIufCompareSourceDict";
	char *pNotherObjLabel = "Another Label for IufDemodulationDict, created in testIufCompareSourceDict";
	char *pDifferentLabel = "Different Label for IufDemodulationDict, created in testIufCompareSourceDict";
	float sampleFrequency = 4000;
	float centerFrequency = 2000;
	int numSamplesPerLine = 10;
	int numTGCentries = 1;
	IufDemodulationMethod method = IUF_DEMODULATION_FOURX;
	int kernelSize = 5;
	int status;

	iudm_t obj = iufDemodulationCreate(method, sampleFrequency, centerFrequency, numSamplesPerLine, numTGCentries, kernelSize);
	iudm_t notherObj = iufDemodulationCreate(method, sampleFrequency, centerFrequency, numSamplesPerLine, numTGCentries, kernelSize);
	iudm_t differentObj = iufDemodulationCreate(method, sampleFrequency, centerFrequency, numSamplesPerLine, numTGCentries + 1, kernelSize);

	// Create
	iudmd_t  dict = iufDemodulationDictCreate();
	iudmd_t  notherDict = iufDemodulationDictCreate();

	// Same empty lists...
	equal = iufDemodulationDictCompare(dict, dict);
	TEST_ASSERT_EQUAL(IUF_TRUE, equal);
	equal = iufDemodulationDictCompare(dict, notherDict);
	TEST_ASSERT_EQUAL(IUF_TRUE, equal);
    equal = iufDemodulationDictCompare(notherDict, dict);
    TEST_ASSERT_EQUAL(IUF_TRUE, equal);

	// Fill
	status = iufDemodulationDictSet(dict, pObjLabel, obj);
	TEST_ASSERT_EQUAL(IUF_E_OK, status);
	status = iufDemodulationDictSet(dict, pNotherObjLabel, notherObj);
	TEST_ASSERT_EQUAL(IUF_E_OK, status);

	equal = iufDemodulationDictCompare(dict, notherDict);
	TEST_ASSERT_EQUAL(IUF_FALSE, equal);
    equal = iufDemodulationDictCompare(notherDict, dict);
    TEST_ASSERT_EQUAL(IUF_FALSE, equal);

	status = iufDemodulationDictSet(notherDict, pObjLabel, obj);
	TEST_ASSERT_EQUAL(IUF_E_OK, status);
	status = iufDemodulationDictSet(notherDict, pNotherObjLabel, notherObj);
	TEST_ASSERT_EQUAL(IUF_E_OK, status);

	equal = iufDemodulationDictCompare(dict, notherDict);
	TEST_ASSERT_EQUAL(IUF_TRUE, equal);
    equal = iufDemodulationDictCompare(notherDict, dict);
    TEST_ASSERT_EQUAL(IUF_TRUE, equal);

	status = iufDemodulationDictSet(notherDict, pDifferentLabel, differentObj);
	TEST_ASSERT_EQUAL(IUF_E_OK, status);

	equal = iufDemodulationDictCompare(dict, notherDict);
	TEST_ASSERT_EQUAL(IUF_FALSE, equal);
    equal = iufDemodulationDictCompare(notherDict, dict);
    TEST_ASSERT_EQUAL(IUF_FALSE, equal);

	iufDemodulationDelete(obj);
	iufDemodulationDelete(differentObj);
	iufDemodulationDelete(notherObj);
	iufDemodulationDictDelete(dict);
	iufDemodulationDictDelete(notherDict);
}

TEST(IufDemodulationDict, testIufSerialization)
{
	char *filename = "testIufDemodulationDictSerialization.hdf5";
	//char *ReceiveChannelMapDictPath = "/DemodulationDict";

	// Create and fill
	iudmd_t  dict = dgGenerateDemodulationDict("bmode", 2000);

	// Save
	hid_t handle = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
	TEST_ASSERT(handle > 0);
	int status = iufDemodulationDictSave(dict, handle);
	H5Fclose(handle);
	TEST_ASSERT_EQUAL(IUF_E_OK, status);

	// read back
	handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT);
	iudmd_t savedDict = iufDemodulationDictLoad(handle);
	TEST_ASSERT_NOT_EQUAL(NULL, savedDict);
	H5Fclose(handle);

	TEST_ASSERT_EQUAL(IUF_TRUE, iufDemodulationDictCompare(dict, savedDict));

	iudm_t rs = iufDemodulationDictGet(dict, "bmode");
	iufDemodulationDelete(rs);
	iufDemodulationDictDelete(dict);
	iufDemodulationDictDelete(savedDict);
}


TEST_GROUP_RUNNER(IufDemodulationDict)
{
	RUN_TEST_CASE(IufDemodulationDict, testIufCreateDeleteDict);
	RUN_TEST_CASE(IufDemodulationDict, testIufSetGetDict);
    RUN_TEST_CASE(IufDemodulationDict, testIufDictGetKeys)
	RUN_TEST_CASE(IufDemodulationDict, testIufCompareDict);
	RUN_TEST_CASE(IufDemodulationDict, testIufSerialization);
}
