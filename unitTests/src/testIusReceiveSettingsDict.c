//
// Created by nlv09165 on 31/07/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <ius.h>
#include <iusReceiveSettingsDictPrivate.h>
#include <testDataGenerators.h>

static char *pErrorFilename = "IusReceiveSettingsDict.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(IusReceiveSettingsDict);

TEST_SETUP(IusReceiveSettingsDict)
{
    iusErrorLogClear();
    iusErrorLog(IUS_TRUE);
    iusErrorAutoReport(IUS_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iusErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IusReceiveSettingsDict)
{
    iusErrorLogClear();
    iusErrorLog(IUS_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iusErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
}



TEST(IusReceiveSettingsDict, testIusCreateDict)
{
    iursd_t obj = iusReceiveSettingsDictCreate();
    iursd_t notherObj = iusReceiveSettingsDictCreate();

    TEST_ASSERT(obj != IURSD_INVALID);
    TEST_ASSERT(notherObj != IURSD_INVALID);
    iusReceiveSettingsDictDelete(obj);
    iusReceiveSettingsDictDelete(notherObj);
}


TEST(IusReceiveSettingsDict, testIusSetGetDict)
{
    char *pObjLabel = "bmode";
    float sampleFrequency=4000;
    int numSamplesPerLine=10;
    int numTGCentries = 1;
    int status;

    iurs_t obj = iusReceiveSettingsCreate(sampleFrequency, numSamplesPerLine, numTGCentries);


    // Create
    iursd_t  dict = iusReceiveSettingsDictCreate();

    // Fill
    status = iusReceiveSettingsDictSet(dict,pObjLabel,obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    iurs_t retrievedObj = iusReceiveSettingsDictGet(dict,pObjLabel);
    TEST_ASSERT_EQUAL(IUS_TRUE, iusReceiveSettingsCompare(obj,retrievedObj));

    // Invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());
    TEST_ASSERT_EQUAL(IUTA_INVALID, iusReceiveSettingsDictGet(dict,NULL));
    TEST_ASSERT_EQUAL(IUTA_INVALID, iusReceiveSettingsDictGet(NULL,pObjLabel));
    TEST_ASSERT_EQUAL(IUTA_INVALID, iusReceiveSettingsDictGet(dict,"unknownLabel"));

    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, iusReceiveSettingsDictSet(dict, pObjLabel, obj));
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, iusReceiveSettingsDictSet(NULL, pObjLabel, obj));
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, iusReceiveSettingsDictSet(dict, pObjLabel, NULL));

    TEST_ASSERT_EQUAL(6,iusErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

    iusReceiveSettingsDelete(obj);
    iusReceiveSettingsDictDelete(dict);
}

TEST(IusReceiveSettingsDict, testIusCompareDict)
{
    IUS_BOOL equal;
    char *pObjLabel = "Label for IusReceiveSettingsDict, created in testIusCompareSourceDict";
    char *pNotherObjLabel = "Another Label for IusReceiveSettingsDict, created in testIusCompareSourceDict";
    char *pDifferentLabel = "Different Label for IusReceiveSettingsDict, created in testIusCompareSourceDict";
    float sampleFrequency=4000;
    int numSamplesPerLine=10;
    int numTGCentries = 1;
    int status;

    iurs_t obj = iusReceiveSettingsCreate(sampleFrequency, numSamplesPerLine, numTGCentries);
    iurs_t notherObj = iusReceiveSettingsCreate(sampleFrequency, numSamplesPerLine, numTGCentries);
    iurs_t differentObj = iusReceiveSettingsCreate(sampleFrequency, numSamplesPerLine, numTGCentries + 1);

    // Create
    iursd_t  dict = iusReceiveSettingsDictCreate();
    iursd_t  notherDict = iusReceiveSettingsDictCreate();

    // Same empty lists...
    equal = iusReceiveSettingsDictCompare(dict, dict);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);
    equal = iusReceiveSettingsDictCompare(dict, notherDict);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);

    // Fill
    status = iusReceiveSettingsDictSet(dict,pObjLabel,obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    status = iusReceiveSettingsDictSet(dict,pNotherObjLabel,notherObj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    equal = iusReceiveSettingsDictCompare(dict, notherDict);
    TEST_ASSERT_EQUAL(IUS_FALSE, equal);

    status = iusReceiveSettingsDictSet(notherDict,pObjLabel,obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    status = iusReceiveSettingsDictSet(notherDict,pNotherObjLabel,notherObj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    equal = iusReceiveSettingsDictCompare(dict, notherDict);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);

    status = iusReceiveSettingsDictSet(notherDict,pDifferentLabel,differentObj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    equal = iusReceiveSettingsDictCompare(dict, notherDict);
    TEST_ASSERT_EQUAL(IUS_FALSE, equal);

    iusReceiveSettingsDelete(obj);
    iusReceiveSettingsDelete(differentObj);
    iusReceiveSettingsDelete(notherObj);
    iusReceiveSettingsDictDelete(dict);
    iusReceiveSettingsDictDelete(notherDict);
}

TEST(IusReceiveSettingsDict, testIusSerialization)
{
    char *filename = "testIusReceiveSettingsDictSerialization.hdf5";
    //char *ReceiveChannelMapDictPath = "/ReceiveSettingsDict";

    // Create and fill
    iursd_t  dict = dgGenerateReceiveSettingsDict("bmode", 0);

    // Save
    hid_t handle = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    TEST_ASSERT(handle > 0);
    int status = iusReceiveSettingsDictSave(dict, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT);
    iursd_t savedDict = iusReceiveSettingsDictLoad(handle);
    TEST_ASSERT_NOT_EQUAL(NULL, savedDict);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUS_TRUE, iusReceiveSettingsDictCompare(dict, savedDict));

    iurs_t rs = iusReceiveSettingsDictGet(dict,"bmode");
    iusReceiveSettingsDelete(rs);
    iusReceiveSettingsDictDelete(dict);
    iusReceiveSettingsDictDelete(savedDict);
}


TEST_GROUP_RUNNER(IusReceiveSettingsDict)
{
    RUN_TEST_CASE(IusReceiveSettingsDict, testIusCreateDict);
    RUN_TEST_CASE(IusReceiveSettingsDict, testIusSetGetDict);
    RUN_TEST_CASE(IusReceiveSettingsDict, testIusCompareDict);
    RUN_TEST_CASE(IusReceiveSettingsDict, testIusSerialization);
}
