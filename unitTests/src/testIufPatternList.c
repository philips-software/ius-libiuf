#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <iuf.h>
#include <iufPatternListPrivate.h>
#include <dg/dataGenerators.h>

static const char *pPulseLabel = "pulseLabel";
static const char *pSourceLabel = "sourceLabel";
static const char *pChannelMapLabel = "channelMapLabel";
static const char *pApodizationLabel = "apodizationLabel";
static const char *pReceivesettingsLabel = "receivesettingsLabel";
static char *pErrorFilename = "IufPatternList.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(IufPatternList);

TEST_SETUP(IufPatternList)
{
    iufErrorLogClear();
    iufErrorLog(IUF_TRUE);
    iufErrorAutoReportSet(IUF_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iufErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IufPatternList)
{
    iufErrorLogClear();
    iufErrorLog(IUF_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iufErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
}


TEST(IufPatternList, testIufCreatePatternList)
{
    // Put your test code here
    int numPatterns = 100;
    iupal_t patternList = iufPatternListCreate(numPatterns,NULL,NULL);
    TEST_ASSERT_NOT_EQUAL(IUPAL_INVALID, patternList);
    TEST_ASSERT_EQUAL(numPatterns, iufPatternListGetSize(patternList));
    iufPatternListDelete(patternList);


    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    patternList = iufPatternListCreate(-1,NULL,NULL);
    TEST_ASSERT_EQUAL(IUPAL_INVALID, patternList);

    TEST_ASSERT_EQUAL(1,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

}


TEST(IufPatternList, testIufSetPatternList)
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
    iufReceiveSettingsDictSet(receiveSettingsDict,pLabel20SampsPerLine,receiveSettings);
    iurcm_t receiveChannelMap = dgGenerateReceiveChannelMap(6);
    iufReceiveChannelMapDictSet(receiveChannelMapDict,pLabel6channels,receiveChannelMap);

    // Empty lists should be equal
    iupal_t patternList = iufPatternListCreate(numPatterns,receiveSettingsDict,receiveChannelMapDict);
    TEST_ASSERT_NOT_EQUAL(IUPAL_INVALID, patternList);

    iupa_t pattern1 = iufPatternCreate(0.01f,
                                           pPulseLabel,
                                           pSourceLabel,
                                           pChannelMapLabel,
                                           pApodizationLabel,
                                           pReceivesettingsLabel);

    iupa_t pattern2 = iufPatternCreate(0.01f,
                                       pPulseLabel,
                                       pSourceLabel,
                                       pLabel6channels,
                                       pApodizationLabel,
                                       pReceivesettingsLabel);

    iupa_t pattern3 = iufPatternCreate(0.01f,
                                             pPulseLabel,
                                             pSourceLabel,
                                             pChannelMapLabel,
                                             pApodizationLabel,
                                             pLabel20SampsPerLine);

    // Allowed
    status = iufPatternListSet(patternList, pattern1, 0);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);
    // Different numChannels or samplesper line should not be accepted
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    status = iufPatternListSet(patternList, pattern3, 2);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, status);
    status = iufPatternListSet(patternList, pattern2, 1);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, status);

    TEST_ASSERT_EQUAL(2,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

    iufReceiveSettingsDictDeepDelete(receiveSettingsDict);
    iufReceiveChannelMapDictDeepDelete(receiveChannelMapDict);
    iufPatternListDelete(patternList);
    iufPatternDelete(pattern1);
    iufPatternDelete(pattern2);
    iufPatternDelete(pattern3);
}

TEST(IufPatternList, testIufComparePatternList)
{
    // Put your test code here
    int numPatterns = 100;
    int numSamplesPerLine = 10;
    int numChannels = 8;
    int status;
    IUF_BOOL equal;
    iursd_t receiveSettingsDict = dgGenerateReceiveSettingsDict((char *)pReceivesettingsLabel, numSamplesPerLine);
    iurcmd_t receiveChannelMapDict = dgGenerateReceiveChannelMapDict((char *) pChannelMapLabel, numChannels);

    // Empty lists should be equal
    iupal_t patternList = iufPatternListCreate(numPatterns,receiveSettingsDict,receiveChannelMapDict);
    TEST_ASSERT_NOT_EQUAL(IUPAL_INVALID, patternList);
    iupal_t notherPatternList = iufPatternListCreate(numPatterns,receiveSettingsDict,receiveChannelMapDict);
    TEST_ASSERT_NOT_EQUAL(IUPAL_INVALID, notherPatternList);
    equal = iufPatternListCompare(patternList, notherPatternList);
    TEST_ASSERT_EQUAL(IUF_TRUE, equal);

    iupa_t bmodePattern = iufPatternCreate(0.01f,
                                           pPulseLabel,
                                           pSourceLabel,
                                           pChannelMapLabel,
                                           pApodizationLabel,
                                           pReceivesettingsLabel);

    iupa_t dopplerPattern = iufPatternCreate(0.01f,
                                             pPulseLabel,
                                             pSourceLabel,
                                             pChannelMapLabel,
                                             pApodizationLabel,
                                             pReceivesettingsLabel);

    // Change one list..add bmode
    status = iufPatternListSet(patternList, bmodePattern, 0);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);
    equal = iufPatternListCompare(patternList, notherPatternList);
    TEST_ASSERT_EQUAL(IUF_FALSE, equal);

    // Change other
    status = iufPatternListSet(notherPatternList, bmodePattern, 0);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);
    equal = iufPatternListCompare(patternList, notherPatternList);
    TEST_ASSERT_EQUAL(IUF_TRUE, equal);


    // Change one list..add doppler
    status = iufPatternListSet(patternList, dopplerPattern, 1);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);
    equal = iufPatternListCompare(patternList, notherPatternList);
    TEST_ASSERT_EQUAL(IUF_FALSE, equal);

    // Change other
    status = iufPatternListSet(notherPatternList, dopplerPattern, 1);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);
    equal = iufPatternListCompare(patternList, notherPatternList);
    TEST_ASSERT_EQUAL(IUF_TRUE, equal);


    iufReceiveSettingsDictDeepDelete(receiveSettingsDict);
    iufReceiveChannelMapDictDeepDelete(receiveChannelMapDict);
    iufPatternListDelete(patternList);
    iufPatternListDelete(notherPatternList);
    iufPatternDelete(bmodePattern);
    iufPatternDelete(dopplerPattern);
}

TEST(IufPatternList, testIufSerialization)
{
    int numPatterns = 2;
    int status;
    IUF_BOOL equal;
    char *pFilename = "testIufPatternListSerialization.hdf5";
    //char *pPatternListPath = "/PatternList";

    // fill list
    iupal_t patternList = iufPatternListCreate(numPatterns,NULL,NULL);
    TEST_ASSERT_NOT_EQUAL(IUPAL_INVALID, patternList);

    iupa_t bmodePattern = iufPatternCreate(0.01f,
                                           pPulseLabel,
                                           pSourceLabel,
                                           pChannelMapLabel,
                                           pApodizationLabel,
                                           pReceivesettingsLabel);

    iupa_t dopplerPattern = iufPatternCreate(0.02f,
                                             pPulseLabel,
                                             pSourceLabel,
                                             pChannelMapLabel,
                                             pApodizationLabel,
                                             pReceivesettingsLabel);
    status = iufPatternListSet(patternList, bmodePattern, 0);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);
    status = iufPatternListSet(patternList, dopplerPattern, 1);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);

    // save
    hid_t handle = H5Fcreate(pFilename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    TEST_ASSERT(handle > 0);
    status = iufPatternListSave(patternList, handle);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);
    status = H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);

    // read back
    handle = H5Fopen(pFilename, H5F_ACC_RDONLY, H5P_DEFAULT);
    iupal_t savedPatternList = iufPatternListLoad(handle);
    TEST_ASSERT_NOT_EQUAL(IUPAL_INVALID, savedPatternList);
    status |= H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);

    // compare
    equal = iufPatternListCompare(patternList, savedPatternList);
    TEST_ASSERT_EQUAL(IUF_TRUE, equal);

    iufPatternListDelete(patternList);
    iufPatternListDelete(savedPatternList);
    iufPatternDelete(bmodePattern);
    iufPatternDelete(dopplerPattern);

}

TEST_GROUP_RUNNER(IufPatternList)
{
    RUN_TEST_CASE(IufPatternList, testIufCreatePatternList);
    RUN_TEST_CASE(IufPatternList, testIufComparePatternList);
    RUN_TEST_CASE(IufPatternList, testIufSetPatternList);
    RUN_TEST_CASE(IufPatternList, testIufSerialization);
}
