
//
// Created by nlv09165 on 18/07/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <ius.h>
#include <iusPatternListPrivate.h>
#include <testDataGenerators.h>

static const char *pPulseLabel = "pulseLabel";
static const char *pSourceLabel = "sourceLabel";
static const char *pChannelMapLabel = "channelMapLabel";
static const char *pApodizationLabel = "apodizationLabel";
static const char *pReceivesettingsLabel = "receivesettingsLabel";

TEST_GROUP(IusPatternList);

TEST_SETUP(IusPatternList)
{
}

TEST_TEAR_DOWN(IusPatternList)
{
}

TEST(IusPatternList, testIusCreatePatternList)
{
    // Put your test code here
    int numPatterns = 100;
    iupal_t patternList = iusPatternListCreate(numPatterns,NULL,NULL);
    TEST_ASSERT_NOT_EQUAL(IUPAL_INVALID, patternList);
    TEST_ASSERT_EQUAL(numPatterns, iusPatternListGetSize(patternList));
    iusPatternListDelete(patternList);

    patternList = iusPatternListCreate(-1,NULL,NULL);
    TEST_ASSERT_EQUAL(IUPAL_INVALID, patternList);
}


TEST(IusPatternList, testIusSetPatternList)
{
    // Put your test code here
    int numPatterns = 100;
    int numSamplesPerLine = 10;
    int numChannels = 8;
    int status;
    char *pLabel20SampsPerLine = "pLabel20SampsPerLine";
    char *pLabel6channels = "pLabel6channels";
    iursd_t receiveSettingsDict = dgGenerateReceiveSettingsDict((char *)pReceivesettingsLabel, numSamplesPerLine);
    iurcmd_t receiveChannelMapDict = dgGenerateReceiveChannelMapDict((char *) pChannelMapLabel, numChannels);
    iurs_t receiveSettings = dgGenerateReceiveSettings(20);
    iusReceiveSettingsDictSet(receiveSettingsDict,pLabel20SampsPerLine,receiveSettings);
    iurcm_t receiveChannelMap = dgGenerateReceiveChannelMap(6);
    iusReceiveChannelMapDictSet(receiveChannelMapDict,pLabel6channels,receiveChannelMap);

    // Empty lists should be equal
    iupal_t patternList = iusPatternListCreate(numPatterns,receiveSettingsDict,receiveChannelMapDict);
    TEST_ASSERT_NOT_EQUAL(IUPAL_INVALID, patternList);

    iupa_t pattern1 = iusPatternCreate(0.01f,
                                           pPulseLabel,
                                           pSourceLabel,
                                           pChannelMapLabel,
                                           pApodizationLabel,
                                           pReceivesettingsLabel);

    iupa_t pattern2 = iusPatternCreate(0.01f,
                                       pPulseLabel,
                                       pSourceLabel,
                                       pLabel6channels,
                                       pApodizationLabel,
                                       pReceivesettingsLabel);

    iupa_t pattern3 = iusPatternCreate(0.01f,
                                             pPulseLabel,
                                             pSourceLabel,
                                             pChannelMapLabel,
                                             pApodizationLabel,
                                             pLabel20SampsPerLine);

    // Allowed
    status = iusPatternListSet(patternList, pattern1, 0);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);
    // Different numChannels or samplesper line should not be accepted
    status = iusPatternListSet(patternList, pattern3, 2);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
    status = iusPatternListSet(patternList, pattern2, 1);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);


    iusReceiveSettingsDictDeepDelete(receiveSettingsDict);
    iusReceiveChannelMapDictDeepDelete(receiveChannelMapDict);
    iusPatternListDelete(patternList);
    iusPatternDelete(pattern1);
    iusPatternDelete(pattern2);
    iusPatternDelete(pattern3);
}

TEST(IusPatternList, testIusComparePatternList)
{
    // Put your test code here
    int numPatterns = 100;
    int numSamplesPerLine = 10;
    int numChannels = 8;
    int status;
    IUS_BOOL equal;
    iursd_t receiveSettingsDict = dgGenerateReceiveSettingsDict((char *)pReceivesettingsLabel, numSamplesPerLine);
    iurcmd_t receiveChannelMapDict = dgGenerateReceiveChannelMapDict((char *) pChannelMapLabel, numChannels);

    // Empty lists should be equal
    iupal_t patternList = iusPatternListCreate(numPatterns,receiveSettingsDict,receiveChannelMapDict);
    TEST_ASSERT_NOT_EQUAL(IUPAL_INVALID, patternList);
    iupal_t notherPatternList = iusPatternListCreate(numPatterns,receiveSettingsDict,receiveChannelMapDict);
    TEST_ASSERT_NOT_EQUAL(IUPAL_INVALID, notherPatternList);
    equal = iusPatternListCompare(patternList, notherPatternList);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);

    iupa_t bmodePattern = iusPatternCreate(0.01f,
                                           pPulseLabel,
                                           pSourceLabel,
                                           pChannelMapLabel,
                                           pApodizationLabel,
                                           pReceivesettingsLabel);

    iupa_t dopplerPattern = iusPatternCreate(0.01f,
                                             pPulseLabel,
                                             pSourceLabel,
                                             pChannelMapLabel,
                                             pApodizationLabel,
                                             pReceivesettingsLabel);

    // Change one list..add bmode
    status = iusPatternListSet(patternList, bmodePattern, 0);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);
    equal = iusPatternListCompare(patternList, notherPatternList);
    TEST_ASSERT_EQUAL(IUS_FALSE, equal);

    // Change other
    status = iusPatternListSet(notherPatternList, bmodePattern, 0);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);
    equal = iusPatternListCompare(patternList, notherPatternList);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);


    // Change one list..add doppler
    status = iusPatternListSet(patternList, dopplerPattern, 1);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);
    equal = iusPatternListCompare(patternList, notherPatternList);
    TEST_ASSERT_EQUAL(IUS_FALSE, equal);

    // Change other
    status = iusPatternListSet(notherPatternList, dopplerPattern, 1);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);
    equal = iusPatternListCompare(patternList, notherPatternList);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);


    iusReceiveSettingsDictDeepDelete(receiveSettingsDict);
    iusReceiveChannelMapDictDeepDelete(receiveChannelMapDict);
    iusPatternListDelete(patternList);
    iusPatternListDelete(notherPatternList);
    iusPatternDelete(bmodePattern);
    iusPatternDelete(dopplerPattern);
}

TEST(IusPatternList, testIusSerialization)
{
    int numPatterns = 2;
    int status;
    IUS_BOOL equal;
    char *pFilename = "testIusPatternListSerialization.hdf5";
    //char *pPatternListPath = "/PatternList";

    // fill list
    iupal_t patternList = iusPatternListCreate(numPatterns,NULL,NULL);
    TEST_ASSERT_NOT_EQUAL(IUPAL_INVALID, patternList);

    iupa_t bmodePattern = iusPatternCreate(0.01f,
                                           pPulseLabel,
                                           pSourceLabel,
                                           pChannelMapLabel,
                                           pApodizationLabel,
                                           pReceivesettingsLabel);

    iupa_t dopplerPattern = iusPatternCreate(0.02f,
                                             pPulseLabel,
                                             pSourceLabel,
                                             pChannelMapLabel,
                                             pApodizationLabel,
                                             pReceivesettingsLabel);
    status = iusPatternListSet(patternList, bmodePattern, 0);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);
    status = iusPatternListSet(patternList, dopplerPattern, 1);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);

    // save
    hid_t handle = H5Fcreate(pFilename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    TEST_ASSERT(handle > 0);
    status = iusPatternListSave(patternList, handle);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);
    status = H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);

    // read back
    handle = H5Fopen(pFilename, H5F_ACC_RDONLY, H5P_DEFAULT);
    iupal_t savedPatternList = iusPatternListLoad(handle);
    TEST_ASSERT_NOT_EQUAL(IUPAL_INVALID, savedPatternList);
    status |= H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);

    // compare
    equal = iusPatternListCompare(patternList, savedPatternList);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);

    iusPatternListDelete(patternList);
    iusPatternListDelete(savedPatternList);
    iusPatternDelete(bmodePattern);
    iusPatternDelete(dopplerPattern);

}

TEST_GROUP_RUNNER(IusPatternList)
{
    RUN_TEST_CASE(IusPatternList, testIusCreatePatternList);
    RUN_TEST_CASE(IusPatternList, testIusComparePatternList);
    RUN_TEST_CASE(IusPatternList, testIusSetPatternList);
    RUN_TEST_CASE(IusPatternList, testIusSerialization);
}
