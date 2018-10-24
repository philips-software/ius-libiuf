//
// Created by nlv09165 on 31/07/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <ius.h>
#include <iusReceiveSettingsDictPrivate.h>
#include <testDataGenerators.h>

TEST_GROUP(IusReceiveSettingsDict);

TEST_SETUP(IusReceiveSettingsDict)
{
}

TEST_TEAR_DOWN(IusReceiveSettingsDict)
{
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
    int numDelays=10;
    int numSamplesPerLine=10;
    int numTGCentries = 1;
    int status;

    iurs_t obj = iusReceiveSettingsCreate(sampleFrequency, numDelays, numSamplesPerLine, numTGCentries);


    // Create
    iursd_t  dict = iusReceiveSettingsDictCreate();

    // Fill
    status = iusReceiveSettingsDictSet(dict,pObjLabel,obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    iurs_t retrievedObj = iusReceiveSettingsDictGet(dict,pObjLabel);
    TEST_ASSERT_EQUAL(IUS_TRUE, iusReceiveSettingsCompare(obj,retrievedObj));

    // Invalid params
    TEST_ASSERT_EQUAL(IURS_INVALID, iusReceiveSettingsDictGet(dict,NULL));
    TEST_ASSERT_EQUAL(IURS_INVALID, iusReceiveSettingsDictGet(NULL,pObjLabel));
    TEST_ASSERT_EQUAL(IURS_INVALID, iusReceiveSettingsDictGet(dict,"unknownLabel"));
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
    int numDelays=10;
    int numSamplesPerLine=10;
    int numTGCentries = 1;
    int status;

    iurs_t obj = iusReceiveSettingsCreate(sampleFrequency, numDelays, numSamplesPerLine, numTGCentries);
    iurs_t notherObj = iusReceiveSettingsCreate(sampleFrequency, numDelays, numSamplesPerLine, numTGCentries);
    iurs_t differentObj = iusReceiveSettingsCreate(sampleFrequency, numDelays, numSamplesPerLine, numTGCentries+1);

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
    iursd_t  dict = dgGenerateReceiveSettingsDict("bmode");

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
