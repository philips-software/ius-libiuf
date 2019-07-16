#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <iuf.h>
#include <iufIqPatternListPrivate.h>
#include <dg/dataGenerators.h>

static const char *pPulseLabel = "pulseLabel";
static const char *pSourceLabel = "sourceLabel";
static const char *pChannelMapLabel = "channelMapLabel";
static const char *pApodizationLabel = "apodizationLabel";
static const char *pDemodulationLabel = "demodulationLabel";
static char *pErrorFilename = "IufIqPatternList.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(IufIqPatternList);

TEST_SETUP(IufIqPatternList)
{
    iufErrorLogClear();
    iufErrorLog(IUF_TRUE);
    iufErrorAutoReportSet(IUF_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iufErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IufIqPatternList)
{
    iufErrorLogClear();
    iufErrorLog(IUF_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iufErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
}


TEST(IufIqPatternList, testIufCreatePatternList)
{
    // Put your test code here
    int numPatterns = 100;
    iuiqpal_t patternList = iufIqPatternListCreate(numPatterns,NULL,NULL);
    TEST_ASSERT_NOT_EQUAL(IUIQPAL_INVALID, patternList);
    TEST_ASSERT_EQUAL(numPatterns, iufIqPatternListGetSize(patternList));
    iufIqPatternListDelete(patternList);


    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    patternList = iufIqPatternListCreate(-1,NULL,NULL);
    TEST_ASSERT_EQUAL(IUPAL_INVALID, patternList);

    TEST_ASSERT_EQUAL(1,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

}


TEST(IufIqPatternList, testIufSetPatternList)
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
    iufDemodulationDictSet(demodulationDict, pLabel20SampsPerLine, demodulationSettings);
    iurcm_t receiveChannelMap = dgGenerateReceiveChannelMap(6);
    iufReceiveChannelMapDictSet(receiveChannelMapDict,pLabel6channels,receiveChannelMap);

    // Empty lists should be equal
    iuiqpal_t patternList = iufIqPatternListCreate(numPatterns,demodulationDict,receiveChannelMapDict);
    TEST_ASSERT_NOT_EQUAL(IUIQPAL_INVALID, patternList);

    iuiqpa_t pattern1 = iufIqPatternCreate(0.01f,
                                           pPulseLabel,
                                           pSourceLabel,
                                           pChannelMapLabel,
                                           pApodizationLabel,
                                           pDemodulationLabel);

    iuiqpa_t pattern2 = iufIqPatternCreate(0.01f,
                                       pPulseLabel,
                                       pSourceLabel,
                                       pLabel6channels,
                                       pApodizationLabel,
                                       pDemodulationLabel);

    iuiqpa_t pattern3 = iufIqPatternCreate(0.01f,
                                             pPulseLabel,
                                             pSourceLabel,
                                             pChannelMapLabel,
                                             pApodizationLabel,
                                             pLabel20SampsPerLine);

    // Allowed
    status = iufIqPatternListSet(patternList, pattern1, 0);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);
    // Different numChannels or samplesper line should not be accepted
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    status = iufIqPatternListSet(patternList, pattern3, 2);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, status);
    status = iufIqPatternListSet(patternList, pattern2, 1);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, status);

    TEST_ASSERT_EQUAL(2,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

    iufDemodulationDictDeepDelete(demodulationDict);
    iufReceiveChannelMapDictDeepDelete(receiveChannelMapDict);
    iufIqPatternListDelete(patternList);
    iufIqPatternDelete(pattern1);
    iufIqPatternDelete(pattern2);
    iufIqPatternDelete(pattern3);
}

TEST(IufIqPatternList, testIufComparePatternList)
{
    // Put your test code here
    int numPatterns = 100;
    int numSamplesPerLine = 10;
    int numChannels = 8;
    int status;
    IUF_BOOL equal;
    iudmd_t demodulationDict = dgGenerateDemodulationDict((char *)pDemodulationLabel, numSamplesPerLine);
    iurcmd_t receiveChannelMapDict = dgGenerateReceiveChannelMapDict((char *) pChannelMapLabel, numChannels);

    // Empty lists should be equal
    iuiqpal_t patternList = iufIqPatternListCreate(numPatterns,demodulationDict,receiveChannelMapDict);
    TEST_ASSERT_NOT_EQUAL(IUIQPAL_INVALID, patternList);
    iuiqpal_t notherPatternList = iufIqPatternListCreate(numPatterns,demodulationDict,receiveChannelMapDict);
    TEST_ASSERT_NOT_EQUAL(IUIQPAL_INVALID, notherPatternList);
    equal = iufIqPatternListCompare(patternList, notherPatternList);
    TEST_ASSERT_EQUAL(IUF_TRUE, equal);

    iuiqpa_t bmodePattern = iufIqPatternCreate(0.01f,
                                           pPulseLabel,
                                           pSourceLabel,
                                           pChannelMapLabel,
                                           pApodizationLabel,
                                           pDemodulationLabel);

    iuiqpa_t dopplerPattern = iufIqPatternCreate(0.01f,
                                             pPulseLabel,
                                             pSourceLabel,
                                             pChannelMapLabel,
                                             pApodizationLabel,
                                             pDemodulationLabel);

    // Change one list..add bmode
    status = iufIqPatternListSet(patternList, bmodePattern, 0);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);
    equal = iufIqPatternListCompare(patternList, notherPatternList);
    TEST_ASSERT_EQUAL(IUF_FALSE, equal);

    // Change other
    status = iufIqPatternListSet(notherPatternList, bmodePattern, 0);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);
    equal = iufIqPatternListCompare(patternList, notherPatternList);
    TEST_ASSERT_EQUAL(IUF_TRUE, equal);


    // Change one list..add doppler
    status = iufIqPatternListSet(patternList, dopplerPattern, 1);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);
    equal = iufIqPatternListCompare(patternList, notherPatternList);
    TEST_ASSERT_EQUAL(IUF_FALSE, equal);

    // Change other
    status = iufIqPatternListSet(notherPatternList, dopplerPattern, 1);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);
    equal = iufIqPatternListCompare(patternList, notherPatternList);
    TEST_ASSERT_EQUAL(IUF_TRUE, equal);


    iufDemodulationDictDeepDelete(demodulationDict);
    iufReceiveChannelMapDictDeepDelete(receiveChannelMapDict);
    iufIqPatternListDelete(patternList);
    iufIqPatternListDelete(notherPatternList);
    iufIqPatternDelete(bmodePattern);
    iufIqPatternDelete(dopplerPattern);
}

TEST(IufIqPatternList, testIufSerialization)
{
    int numPatterns = 2;
    int status;
    IUF_BOOL equal;
    char *pFilename = "testIqIufPatternListSerialization.hdf5";
    //char *pPatternListPath = "/PatternList";

    // fill list
    iuiqpal_t patternList = iufIqPatternListCreate(numPatterns,NULL,NULL);
    TEST_ASSERT_NOT_EQUAL(IUIQPAL_INVALID, patternList);

    iuiqpa_t bmodePattern = iufIqPatternCreate(0.01f,
                                           pPulseLabel,
                                           pSourceLabel,
                                           pChannelMapLabel,
                                           pApodizationLabel,
                                           pDemodulationLabel);

    iuiqpa_t dopplerPattern = iufIqPatternCreate(0.02f,
                                             pPulseLabel,
                                             pSourceLabel,
                                             pChannelMapLabel,
                                             pApodizationLabel,
                                             pDemodulationLabel);
    status = iufIqPatternListSet(patternList, bmodePattern, 0);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);
    status = iufIqPatternListSet(patternList, dopplerPattern, 1);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);

    // save
    hid_t handle = H5Fcreate(pFilename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    TEST_ASSERT(handle > 0);
    status = iufIqPatternListSave(patternList, handle);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);
    status = H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);

    // read back
    handle = H5Fopen(pFilename, H5F_ACC_RDONLY, H5P_DEFAULT);
    iuiqpal_t savedPatternList = iufIqPatternListLoad(handle);
    TEST_ASSERT_NOT_EQUAL(IUIQPAL_INVALID, savedPatternList);
    status |= H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);

    // compare
    equal = iufIqPatternListCompare(patternList, savedPatternList);
    TEST_ASSERT_EQUAL(IUF_TRUE, equal);

    iufIqPatternListDelete(patternList);
    iufIqPatternListDelete(savedPatternList);
    iufIqPatternDelete(bmodePattern);
    iufIqPatternDelete(dopplerPattern);

}

TEST_GROUP_RUNNER(IufIqPatternList)
{
    RUN_TEST_CASE(IufIqPatternList, testIufCreatePatternList);
    RUN_TEST_CASE(IufIqPatternList, testIufComparePatternList);
    RUN_TEST_CASE(IufIqPatternList, testIufSetPatternList);
    RUN_TEST_CASE(IufIqPatternList, testIufSerialization);
}
