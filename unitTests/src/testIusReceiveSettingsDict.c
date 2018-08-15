//
// Created by nlv09165 on 31/07/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <hdf5.h>
#include <ius.h>
#include <iusHLReceiveSettingsDictImp.h>
#include <testDataGenerators.h>

TEST_GROUP(IusReceiveSettingsDict);

TEST_SETUP(IusReceiveSettingsDict)
{
}

TEST_TEAR_DOWN(IusReceiveSettingsDict)
{
}


TEST(IusReceiveSettingsDict, testIusCreateSourceDict)
{
    iursd_t obj = iusHLReceiveSettingsDictCreate();
    iursd_t notherObj = iusHLReceiveSettingsDictCreate();

    TEST_ASSERT(obj != IURSD_INVALID);
    TEST_ASSERT(notherObj != IURSD_INVALID);
    iusHLReceiveSettingsDictDelete(obj);
    iusHLReceiveSettingsDictDelete(notherObj);
}


TEST(IusReceiveSettingsDict, testIusCompareSourceDict)
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

    iurs_t obj = iusHLReceiveSettingsCreate(pObjLabel, sampleFrequency, numDelays, numSamplesPerLine, numTGCentries);
    iurs_t notherObj = iusHLReceiveSettingsCreate(pNotherObjLabel, sampleFrequency, numDelays, numSamplesPerLine, numTGCentries);
    iurs_t differentObj = iusHLReceiveSettingsCreate(pDifferentLabel, sampleFrequency, numDelays, numSamplesPerLine, numTGCentries+1);

    // Create
    iursd_t  dict = iusHLReceiveSettingsDictCreate();
    iursd_t  notherDict = iusHLReceiveSettingsDictCreate();

    // Same empty lists...
    equal = iusHLReceiveSettingsDictCompare(dict, dict);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);
    equal = iusHLReceiveSettingsDictCompare(dict, notherDict);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);

    // Fill
    status = iusHLReceiveSettingsDictSet(dict,pObjLabel,obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    status = iusHLReceiveSettingsDictSet(dict,pNotherObjLabel,notherObj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    equal = iusHLReceiveSettingsDictCompare(dict, notherDict);
    TEST_ASSERT_EQUAL(IUS_FALSE, equal);

    status = iusHLReceiveSettingsDictSet(notherDict,pObjLabel,obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    status = iusHLReceiveSettingsDictSet(notherDict,pNotherObjLabel,notherObj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    equal = iusHLReceiveSettingsDictCompare(dict, notherDict);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);

    status = iusHLReceiveSettingsDictSet(notherDict,pDifferentLabel,differentObj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    equal = iusHLReceiveSettingsDictCompare(dict, notherDict);
    TEST_ASSERT_EQUAL(IUS_FALSE, equal);

    iusHLReceiveSettingsDelete(obj);
    iusHLReceiveSettingsDelete(differentObj);
    iusHLReceiveSettingsDelete(notherObj);
    iusHLReceiveSettingsDictDelete(dict);
    iusHLReceiveSettingsDictDelete(notherDict);
}

TEST(IusReceiveSettingsDict, testIusSerialization)
{
    char *filename = "testIusReceiveSettingsDictSerialization.hdf5";
    char *ReceiveChannelMapDictPath = "/ReceiveSettingsDict";

    // Create and fill
    iursd_t  dict = dgGenerateReceiveSettingsDict();

    // Save
    hid_t handle = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    TEST_ASSERT(handle > 0);
    int status = iusHLReceiveSettingsDictSave(dict, ReceiveChannelMapDictPath, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT);
    iursd_t savedDict = iusHLReceiveSettingsDictLoad(handle, ReceiveChannelMapDictPath);
    TEST_ASSERT_NOT_EQUAL(NULL, savedDict);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUS_TRUE, iusHLReceiveSettingsDictCompare(dict, savedDict));
    iusHLReceiveSettingsDictDelete(dict);
    iusHLReceiveSettingsDictDelete(savedDict);
}


TEST_GROUP_RUNNER(IusReceiveSettingsDict)
{
    RUN_TEST_CASE(IusReceiveSettingsDict, testIusCreateSourceDict);
    RUN_TEST_CASE(IusReceiveSettingsDict, testIusCompareSourceDict);
    RUN_TEST_CASE(IusReceiveSettingsDict, testIusSerialization);
}
