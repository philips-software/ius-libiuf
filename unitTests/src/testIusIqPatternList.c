
//
// Created by nlv09165 on 18/07/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <ius.h>
#include <iusIqPatternListPrivate.h>
#include <testDataGenerators.h>

static const char *pPulseLabel = "pulseLabel";
static const char *pSourceLabel = "sourceLabel";
static const char *pChannelMapLabel = "channelMapLabel";
static const char *pApodizationLabel = "apodizationLabel";
static const char *pDemodulationLabel = "demodulationLabel";
static char *pErrorFilename = "IusIqPatternList.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(IusIqPatternList);

TEST_SETUP(IusIqPatternList)
{
    iusErrorLogClear();
    iusErrorLog(IUS_TRUE);
    iusErrorAutoReport(IUS_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iusErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IusIqPatternList)
{
    iusErrorLogClear();
    iusErrorLog(IUS_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iusErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
}


TEST(IusIqPatternList, testIusCreatePatternList)
{
    // Put your test code here
    int numPatterns = 100;
    iuiqpal_t patternList = iusIqPatternListCreate(numPatterns,NULL,NULL);
    TEST_ASSERT_NOT_EQUAL(IUIQPAL_INVALID, patternList);
    TEST_ASSERT_EQUAL(numPatterns, iusIqPatternListGetSize(patternList));
    iusIqPatternListDelete(patternList);


    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());

    patternList = iusIqPatternListCreate(-1,NULL,NULL);
    TEST_ASSERT_EQUAL(IUPAL_INVALID, patternList);

    TEST_ASSERT_EQUAL(1,iusErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

}


TEST(IusIqPatternList, testIusSetPatternList)
{
    // Put your test code here
    int numPatterns = 100;
    int numSamplesPerLine = 10;
    int numChannels = 8;
    int status;
    char *pLabel20SampsPerLine = "pLabel20SampsPerLine";
    char *pLabel6channels = "pLabel6channels";
    iudmd_t demodulationDict = dgGenerateDemodulationDict((char *)pDemodulationLabel, numSamplesPerLine);
    iurcmd_t receiveChannelMapDict = dgGenerateReceiveChannelMapDict((char *) pChannelMapLabel, numChannels);
    iudm_t demodulationSettings = dgGenerateDemodulation(20);
    iusDemodulationDictSet(demodulationDict, pLabel20SampsPerLine, demodulationSettings);
    iurcm_t receiveChannelMap = dgGenerateReceiveChannelMap(6);
    iusReceiveChannelMapDictSet(receiveChannelMapDict,pLabel6channels,receiveChannelMap);

    // Empty lists should be equal
    iuiqpal_t patternList = iusIqPatternListCreate(numPatterns,demodulationDict,receiveChannelMapDict);
    TEST_ASSERT_NOT_EQUAL(IUIQPAL_INVALID, patternList);

    iuiqpa_t pattern1 = iusIqPatternCreate(0.01f,
                                           pPulseLabel,
                                           pSourceLabel,
                                           pChannelMapLabel,
                                           pApodizationLabel,
                                           pDemodulationLabel);

    iuiqpa_t pattern2 = iusIqPatternCreate(0.01f,
                                       pPulseLabel,
                                       pSourceLabel,
                                       pLabel6channels,
                                       pApodizationLabel,
                                       pDemodulationLabel);

    iuiqpa_t pattern3 = iusIqPatternCreate(0.01f,
                                             pPulseLabel,
                                             pSourceLabel,
                                             pChannelMapLabel,
                                             pApodizationLabel,
                                             pLabel20SampsPerLine);

    // Allowed
    status = iusIqPatternListSet(patternList, pattern1, 0);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);
    // Different numChannels or samplesper line should not be accepted
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());

    status = iusIqPatternListSet(patternList, pattern3, 2);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
    status = iusIqPatternListSet(patternList, pattern2, 1);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);

    TEST_ASSERT_EQUAL(2,iusErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

    iusDemodulationDictDeepDelete(demodulationDict);
    iusReceiveChannelMapDictDeepDelete(receiveChannelMapDict);
    iusIqPatternListDelete(patternList);
    iusIqPatternDelete(pattern1);
    iusIqPatternDelete(pattern2);
    iusIqPatternDelete(pattern3);
}

TEST(IusIqPatternList, testIusComparePatternList)
{
    // Put your test code here
    int numPatterns = 100;
    int numSamplesPerLine = 10;
    int numChannels = 8;
    int status;
    IUS_BOOL equal;
    iudmd_t demodulationDict = dgGenerateDemodulationDict((char *)pDemodulationLabel, numSamplesPerLine);
    iurcmd_t receiveChannelMapDict = dgGenerateReceiveChannelMapDict((char *) pChannelMapLabel, numChannels);

    // Empty lists should be equal
    iuiqpal_t patternList = iusIqPatternListCreate(numPatterns,demodulationDict,receiveChannelMapDict);
    TEST_ASSERT_NOT_EQUAL(IUIQPAL_INVALID, patternList);
    iuiqpal_t notherPatternList = iusIqPatternListCreate(numPatterns,demodulationDict,receiveChannelMapDict);
    TEST_ASSERT_NOT_EQUAL(IUIQPAL_INVALID, notherPatternList);
    equal = iusIqPatternListCompare(patternList, notherPatternList);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);

    iuiqpa_t bmodePattern = iusIqPatternCreate(0.01f,
                                           pPulseLabel,
                                           pSourceLabel,
                                           pChannelMapLabel,
                                           pApodizationLabel,
                                           pDemodulationLabel);

    iuiqpa_t dopplerPattern = iusIqPatternCreate(0.01f,
                                             pPulseLabel,
                                             pSourceLabel,
                                             pChannelMapLabel,
                                             pApodizationLabel,
                                             pDemodulationLabel);

    // Change one list..add bmode
    status = iusIqPatternListSet(patternList, bmodePattern, 0);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);
    equal = iusIqPatternListCompare(patternList, notherPatternList);
    TEST_ASSERT_EQUAL(IUS_FALSE, equal);

    // Change other
    status = iusIqPatternListSet(notherPatternList, bmodePattern, 0);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);
    equal = iusIqPatternListCompare(patternList, notherPatternList);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);


    // Change one list..add doppler
    status = iusIqPatternListSet(patternList, dopplerPattern, 1);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);
    equal = iusIqPatternListCompare(patternList, notherPatternList);
    TEST_ASSERT_EQUAL(IUS_FALSE, equal);

    // Change other
    status = iusIqPatternListSet(notherPatternList, dopplerPattern, 1);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);
    equal = iusIqPatternListCompare(patternList, notherPatternList);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);


    iusDemodulationDictDeepDelete(demodulationDict);
    iusReceiveChannelMapDictDeepDelete(receiveChannelMapDict);
    iusIqPatternListDelete(patternList);
    iusIqPatternListDelete(notherPatternList);
    iusIqPatternDelete(bmodePattern);
    iusIqPatternDelete(dopplerPattern);
}

TEST(IusIqPatternList, testIusSerialization)
{
    int numPatterns = 2;
    int status;
    IUS_BOOL equal;
    char *pFilename = "testIqIusPatternListSerialization.hdf5";
    //char *pPatternListPath = "/PatternList";

    // fill list
    iuiqpal_t patternList = iusIqPatternListCreate(numPatterns,NULL,NULL);
    TEST_ASSERT_NOT_EQUAL(IUIQPAL_INVALID, patternList);

    iuiqpa_t bmodePattern = iusIqPatternCreate(0.01f,
                                           pPulseLabel,
                                           pSourceLabel,
                                           pChannelMapLabel,
                                           pApodizationLabel,
                                           pDemodulationLabel);

    iuiqpa_t dopplerPattern = iusIqPatternCreate(0.02f,
                                             pPulseLabel,
                                             pSourceLabel,
                                             pChannelMapLabel,
                                             pApodizationLabel,
                                             pDemodulationLabel);
    status = iusIqPatternListSet(patternList, bmodePattern, 0);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);
    status = iusIqPatternListSet(patternList, dopplerPattern, 1);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);

    // save
    hid_t handle = H5Fcreate(pFilename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    TEST_ASSERT(handle > 0);
    status = iusIqPatternListSave(patternList, handle);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);
    status = H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);

    // read back
    handle = H5Fopen(pFilename, H5F_ACC_RDONLY, H5P_DEFAULT);
    iuiqpal_t savedPatternList = iusIqPatternListLoad(handle);
    TEST_ASSERT_NOT_EQUAL(IUIQPAL_INVALID, savedPatternList);
    status |= H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);

    // compare
    equal = iusIqPatternListCompare(patternList, savedPatternList);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);

    iusIqPatternListDelete(patternList);
    iusIqPatternListDelete(savedPatternList);
    iusIqPatternDelete(bmodePattern);
    iusIqPatternDelete(dopplerPattern);

}

TEST_GROUP_RUNNER(IusIqPatternList)
{
    RUN_TEST_CASE(IusIqPatternList, testIusCreatePatternList);
    RUN_TEST_CASE(IusIqPatternList, testIusComparePatternList);
    RUN_TEST_CASE(IusIqPatternList, testIusSetPatternList);
    RUN_TEST_CASE(IusIqPatternList, testIusSerialization);
}
