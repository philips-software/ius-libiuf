#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <iuf.h>
#include <util.h>
#include <iufReceiveSettingsDictPrivate.h>
#include <dg/dataGenerators.h>

static char *pErrorFilename = "IufReceiveSettingsDict.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(IufReceiveSettingsDict);

TEST_SETUP(IufReceiveSettingsDict)
{
    iufErrorLogClear();
    iufErrorLog(IUF_TRUE);
    iufErrorAutoReportSet(IUF_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iufErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IufReceiveSettingsDict)
{
    iufErrorLogClear();
    iufErrorLog(IUF_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iufErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
}



TEST(IufReceiveSettingsDict, testIufCreateDict)
{
    iursd_t obj = iufReceiveSettingsDictCreate();
    iursd_t notherObj = iufReceiveSettingsDictCreate();

    TEST_ASSERT(obj != IURSD_INVALID);
    TEST_ASSERT(notherObj != IURSD_INVALID);
    iufReceiveSettingsDictDelete(obj);
    iufReceiveSettingsDictDelete(notherObj);
}


TEST(IufReceiveSettingsDict, testIufSetGetDict)
{
    char *pObjLabel = "bmode";
    float sampleFrequency=4000;
    int numSamplesPerLine=10;
    int numTGCentries = 1;
    int status;

    iurs_t obj = iufReceiveSettingsCreate(sampleFrequency, numSamplesPerLine, numTGCentries);


    // Create
    iursd_t  dict = iufReceiveSettingsDictCreate();

    // Fill
    status = iufReceiveSettingsDictSet(dict,pObjLabel,obj);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);
    iurs_t retrievedObj = iufReceiveSettingsDictGet(dict,pObjLabel);
    TEST_ASSERT_EQUAL(IUF_TRUE, iufReceiveSettingsCompare(obj,retrievedObj));

    // Invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());
    TEST_ASSERT_EQUAL(IUTA_INVALID, iufReceiveSettingsDictGet(dict,NULL));
    TEST_ASSERT_EQUAL(IUTA_INVALID, iufReceiveSettingsDictGet(NULL,pObjLabel));
    TEST_ASSERT_EQUAL(IUTA_INVALID, iufReceiveSettingsDictGet(dict,"unknownLabel"));

    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, iufReceiveSettingsDictSet(dict, pObjLabel, obj));
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, iufReceiveSettingsDictSet(NULL, pObjLabel, obj));
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, iufReceiveSettingsDictSet(dict, pObjLabel, NULL));

    TEST_ASSERT_EQUAL(6,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

    iufReceiveSettingsDelete(obj);
    iufReceiveSettingsDictDelete(dict);
}


TEST(IufReceiveSettingsDict, testIufDictKeys)
{
    float sampleFrequency=4000;
    int numSamplesPerLine=10;
    int numTGCentries = 1;
    int status;
    char *labels[] = { "one" , "two" , "three", "four" , "five"};

    iurs_t obj = iufReceiveSettingsCreate(sampleFrequency, numSamplesPerLine, numTGCentries);


    // Create
    iursd_t  dict = iufReceiveSettingsDictCreate();

    // Fill
    int i;
    int keySize = sizeof(labels)/sizeof(labels[0]);
    for (i=0; i<keySize; i++)
    {
        status = iufReceiveSettingsDictSet(dict, labels[i], obj);
        TEST_ASSERT(status == IUF_E_OK);
    }

    size_t dictSize = iufReceiveSettingsDictGetSize(dict);
    TEST_ASSERT_EQUAL(5, dictSize);
    char **keys = iufReceiveSettingsDictGetKeys(dict);

    // Validate keys
    for (i=0; i<keySize; i++)
    {
        TEST_ASSERT_EQUAL(IUF_TRUE, aInB(keys[i], labels));
    }

    // Invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    keys = iufReceiveSettingsDictGetKeys(NULL);
    TEST_ASSERT_EQUAL(NULL,keys);

    TEST_ASSERT_EQUAL(1,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

    iufReceiveSettingsDelete(obj);
    iufReceiveSettingsDictDelete(dict);
}

TEST(IufReceiveSettingsDict, testIufCompareDict)
{
    IUF_BOOL equal;
    char *pObjLabel = "Label for IufReceiveSettingsDict, created in testIufCompareSourceDict";
    char *pNotherObjLabel = "Another Label for IufReceiveSettingsDict, created in testIufCompareSourceDict";
    char *pDifferentLabel = "Different Label for IufReceiveSettingsDict, created in testIufCompareSourceDict";
    float sampleFrequency=4000;
    int numSamplesPerLine=10;
    int numTGCentries = 1;
    int status;

    iurs_t obj = iufReceiveSettingsCreate(sampleFrequency, numSamplesPerLine, numTGCentries);
    iurs_t notherObj = iufReceiveSettingsCreate(sampleFrequency, numSamplesPerLine, numTGCentries);
    iurs_t differentObj = iufReceiveSettingsCreate(sampleFrequency, numSamplesPerLine, numTGCentries + 1);

    // Create
    iursd_t  dict = iufReceiveSettingsDictCreate();
    iursd_t  notherDict = iufReceiveSettingsDictCreate();

    // Same empty lists...
    equal = iufReceiveSettingsDictCompare(dict, dict);
    TEST_ASSERT_EQUAL(IUF_TRUE, equal);
    equal = iufReceiveSettingsDictCompare(dict, notherDict);
    TEST_ASSERT_EQUAL(IUF_TRUE, equal);

    // Fill
    status = iufReceiveSettingsDictSet(dict,pObjLabel,obj);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);
    status = iufReceiveSettingsDictSet(dict,pNotherObjLabel,notherObj);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    equal = iufReceiveSettingsDictCompare(dict, notherDict);
    TEST_ASSERT_EQUAL(IUF_FALSE, equal);

    status = iufReceiveSettingsDictSet(notherDict,pObjLabel,obj);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);
    status = iufReceiveSettingsDictSet(notherDict,pNotherObjLabel,notherObj);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    equal = iufReceiveSettingsDictCompare(dict, notherDict);
    TEST_ASSERT_EQUAL(IUF_TRUE, equal);

    status = iufReceiveSettingsDictSet(notherDict,pDifferentLabel,differentObj);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    equal = iufReceiveSettingsDictCompare(dict, notherDict);
    TEST_ASSERT_EQUAL(IUF_FALSE, equal);

    iufReceiveSettingsDelete(obj);
    iufReceiveSettingsDelete(differentObj);
    iufReceiveSettingsDelete(notherObj);
    iufReceiveSettingsDictDelete(dict);
    iufReceiveSettingsDictDelete(notherDict);
}

TEST(IufReceiveSettingsDict, testIufSerialization)
{
    char *filename = "testIufReceiveSettingsDictSerialization.hdf5";
    //char *ReceiveChannelMapDictPath = "/ReceiveSettingsDict";

    // Create and fill
    iursd_t  dict = dgGenerateReceiveSettingsDict("bmode", 0);

    // Save
    hid_t handle = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    TEST_ASSERT(handle > 0);
    int status = iufReceiveSettingsDictSave(dict, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT);
    iursd_t savedDict = iufReceiveSettingsDictLoad(handle);
    TEST_ASSERT_NOT_EQUAL(NULL, savedDict);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUF_TRUE, iufReceiveSettingsDictCompare(dict, savedDict));

    iurs_t rs = iufReceiveSettingsDictGet(dict,"bmode");
    iufReceiveSettingsDelete(rs);
    iufReceiveSettingsDictDelete(dict);
    iufReceiveSettingsDictDelete(savedDict);
}


TEST_GROUP_RUNNER(IufReceiveSettingsDict)
{
    RUN_TEST_CASE(IufReceiveSettingsDict, testIufCreateDict);
    RUN_TEST_CASE(IufReceiveSettingsDict, testIufSetGetDict);
    RUN_TEST_CASE(IufReceiveSettingsDict, testIufDictKeys)
    RUN_TEST_CASE(IufReceiveSettingsDict, testIufCompareDict);
    RUN_TEST_CASE(IufReceiveSettingsDict, testIufSerialization);
}
