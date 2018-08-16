

//
// Created by nlv09165 on 11/07/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <include/ius.h>
#include <include/iusError.h>
#include <include/iusTypes.h>
#include <include/iusHLInputFile.h>
#include <include/iusHLPulseDict.h>
#include <include/iusHLReceiveChannelMapDict.h>
#include <include/iusHLTransmitApodizationDict.h>

#include <include/iusHLParametricPulse.h>
#include <include/iusHLNonParametricPulse.h>

#include <testDataGenerators.h>

static const char *pFilename = "IusInputFile.hdf5";
static const char *pNotherFilename = "AnotherIusInputFile.hdf5";

TEST_GROUP(IusInputFile);

TEST_SETUP(IusInputFile)
{
}

TEST_TEAR_DOWN(IusInputFile)
{
    if( fileExists(pFilename) == IUS_TRUE )
    {
		remove(pFilename);
    }
    if( fileExists(pNotherFilename) == IUS_TRUE )
    {
		remove(pNotherFilename);
    }
}


TEST(IusInputFile, testIusInputFileCreate)
{
    const char *pEmptyFilename = "";
    const char *pSpecialCharsFilename = "*&/";

    iuif_t ifh = iusHLInputFileCreate(pFilename);
    TEST_ASSERT(ifh != IUIF_INVALID);

    // Create file that is already open should result in error
    iuif_t ifh2 = iusHLInputFileCreate(pFilename);
    TEST_ASSERT(ifh2 == IUIF_INVALID);

    int status = iusHLInputFileClose(ifh);
    TEST_ASSERT(status == IUS_E_OK);

    // Closing file that has already been closed results in error
    status = iusHLInputFileClose(ifh);
    TEST_ASSERT(status != IUS_E_OK);

    // Invalid argument should result in error.
    ifh = iusHLInputFileCreate(pEmptyFilename);
    TEST_ASSERT(ifh == IUIF_INVALID);
    ifh = iusHLInputFileCreate(pSpecialCharsFilename);
    TEST_ASSERT(ifh == IUIF_INVALID);
}

TEST(IusInputFile, testIusInputFileDelete)
{
    iuif_t obj = iusHLInputFileCreate(pFilename);
    TEST_ASSERT(obj != IUIF_INVALID);

	int status = iusHLInputFileClose(obj);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);
	
	status = iusHLInputFileDelete(obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // invalid params
    status = iusHLInputFileDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
}


TEST(IusInputFile, testIusInputFileCompare)
{
    IUS_BOOL equal;
    iuif_t obj = iusHLInputFileCreate(pFilename);
    iuif_t notherObj = iusHLInputFileCreate(pNotherFilename);
    TEST_ASSERT(obj != IUIF_INVALID);
    TEST_ASSERT(notherObj != IUIF_INVALID);
    equal = iusHLInputFileCompare(obj,obj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = iusHLInputFileCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    
    // invalid params
    equal = iusHLInputFileCompare(obj,NULL);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    equal = iusHLInputFileCompare(NULL,obj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

	int status = iusHLInputFileClose(obj);
	TEST_ASSERT(status == IUS_E_OK);
	status = iusHLInputFileClose(notherObj);
	TEST_ASSERT(status == IUS_E_OK);
	iusHLInputFileDelete(obj);
    iusHLInputFileDelete(notherObj);
}

TEST(IusInputFile, iusHLInputFileSetGetFrameList)
{
    IUS_BOOL equal;
    int status;
    iufl_t frameList = dgGenerateFrameList();
    iuif_t obj = iusHLInputFileCreate(pFilename);

    status = iusHLInputFileSetFrameList(obj,frameList);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    iufl_t gotMeAFrameList = iusHLInputFileGetFrameList(obj);
    TEST_ASSERT_NOT_EQUAL(IUFL_INVALID, gotMeAFrameList);

    equal = iusHLFrameListCompare(frameList,gotMeAFrameList);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);

    status = iusHLInputFileClose(obj);
    TEST_ASSERT(status == IUS_E_OK);
    iusHLInputFileDelete(obj);
}

TEST(IusInputFile, iusHLInputFileSetGetPatternList)
{
    IUS_BOOL equal;
    int status;
    iupal_t patternList = dgGeneratePatternList();
    iuif_t obj = iusHLInputFileCreate(pFilename);

    status = iusHLInputFileSetPatternList(obj,patternList);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    iupal_t gotMeAPatternList = iusHLInputFileGetPatternList(obj);
    TEST_ASSERT_NOT_EQUAL(IUPAL_INVALID, gotMeAPatternList);

    equal = iusHLPatternListCompare(patternList,gotMeAPatternList);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);

    status = iusHLInputFileClose(obj);
    TEST_ASSERT(status == IUS_E_OK);
    iusHLInputFileDelete(obj);
}


TEST(IusInputFile, iusHLInputFileSetGetPulseDict)
{
    iuif_t obj = iusHLInputFileCreate(pFilename);
    iupd_t pulseDict = dgGeneratePulseDict();

    int status = iusHLInputFileSetPulseDict(obj, pulseDict);
    TEST_ASSERT(status == IUS_E_OK);
    iupd_t gotMeAPulseDict = iusHLInputFileGetPulseDict(obj);
    TEST_ASSERT_NOT_EQUAL(NULL, gotMeAPulseDict);

    IUS_BOOL equal = iusHLPulseDictCompare(pulseDict,gotMeAPulseDict);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);

    status = iusHLInputFileClose(obj);
    TEST_ASSERT(status == IUS_E_OK);
    iusHLInputFileDelete(obj);
}

TEST(IusInputFile, iusHLInputFileSetGetSourceDict)
{
    iuif_t obj = iusHLInputFileCreate(pFilename);
    iusd_t sourceDict = dgGenerateSourceDict();

    int status = iusHLInputFileSetSourceDict(obj, sourceDict);
    TEST_ASSERT(status == IUS_E_OK);
    iusd_t gotMeASourceDict = iusHLInputFileGetSourceDict(obj);
    TEST_ASSERT_NOT_EQUAL(IUSD_INVALID, gotMeASourceDict);

    IUS_BOOL equal = iusHLSourceDictCompare(sourceDict,gotMeASourceDict);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);

    status = iusHLInputFileClose(obj);
    TEST_ASSERT(status == IUS_E_OK);
    iusHLInputFileDelete(obj);
}


TEST(IusInputFile, iusHLInputFileSetGetReceiveChannelMapDict)
{
    iuif_t obj = iusHLInputFileCreate(pFilename);
    iurcmd_t receiveChannelMapDict = iusHLReceiveChannelMapDictCreate();

    int status = iusHLInputFileSetReceiveChannelMapDict(obj, receiveChannelMapDict);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);

    iurcmd_t gotMeAReceiveChannelMapDict = iusHLInputFileGetReceiveChannelMapDict(obj);
    TEST_ASSERT_NOT_EQUAL(NULL, gotMeAReceiveChannelMapDict);

    IUS_BOOL equal = iusHLReceiveChannelMapDictCompare(receiveChannelMapDict, gotMeAReceiveChannelMapDict);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);

    status = iusHLInputFileClose(obj);
    TEST_ASSERT(status == IUS_E_OK);
    iusHLInputFileDelete(obj);
}

TEST(IusInputFile, iusHLInputFileSetGetTransmitApodizationDict)
{
    IUS_BOOL equal;
    int status;
	iutad_t transmitApodizationDict = iusHLTransmitApodizationDictCreate();
    iuif_t obj = iusHLInputFileCreate(pFilename);
    TEST_ASSERT(obj != IUIF_INVALID);

	// transmitApodizationDict param
	status = iusHLInputFileSetTransmitApodizationDict(obj, transmitApodizationDict);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);

	iutad_t gotMeATransmitApodizationDict = iusHLInputFileGetTransmitApodizationDict(obj);
	TEST_ASSERT_NOT_EQUAL(NULL, gotMeATransmitApodizationDict);

	equal = iusHLTransmitApodizationDictCompare(transmitApodizationDict, gotMeATransmitApodizationDict);
	TEST_ASSERT_EQUAL(IUS_TRUE, equal);

    // invalid param
	  //todo

	status = iusHLInputFileClose(obj);
	TEST_ASSERT(status == IUS_E_OK);
    iusHLInputFileDelete(obj);
}

TEST(IusInputFile, iusHLInputFileSetGetReceiveSettingsDict)
{
    IUS_BOOL equal;
    int status;
    iursd_t receiveSettingsDict = dgGenerateReceiveSettingsDict();
    iuif_t obj = iusHLInputFileCreate(pFilename);
    TEST_ASSERT(obj != IUIF_INVALID);

    status = iusHLInputFileSetReceiveSettingsDict(obj, receiveSettingsDict);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);

    iursd_t gotMeATransmitReceiveSettingsDict = iusHLInputFileGetReceiveSettingsDict(obj);
    TEST_ASSERT_NOT_EQUAL(NULL, gotMeATransmitReceiveSettingsDict);

    equal = iusHLReceiveSettingsDictCompare(receiveSettingsDict, gotMeATransmitReceiveSettingsDict);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);

    // invalid param
    //todo

    status = iusHLInputFileClose(obj);
    TEST_ASSERT(status == IUS_E_OK);
    iusHLInputFileDelete(obj);
}

TEST(IusInputFile, testIusInputFileSerialization)
{
    const char *ptestFileName = "testIusInputFileSerialization.hdf5";

    // create
    iuif_t inputFile = iusHLInputFileCreate(ptestFileName);
    TEST_ASSERT(inputFile != IUIF_INVALID);

    // fill
    iufl_t frameList = dgGenerateFrameList();
    int status = iusHLInputFileSetFrameList(inputFile,frameList);
    TEST_ASSERT(status == IUS_E_OK);

    iupal_t patternList = dgGeneratePatternList();
    status = iusHLInputFileSetPatternList(inputFile,patternList);
    TEST_ASSERT(status == IUS_E_OK);

    iupd_t pulseDict = dgGeneratePulseDict();
    status = iusHLInputFileSetPulseDict(inputFile, pulseDict);
    TEST_ASSERT(status == IUS_E_OK);

    iusd_t sourceDict = dgGenerateSourceDict();
    status = iusHLInputFileSetSourceDict(inputFile, sourceDict);
    TEST_ASSERT(status == IUS_E_OK);

    iurcmd_t receiveChannelMapDict = dgGenerateReceiveChannelMapDict();
	status = iusHLInputFileSetReceiveChannelMapDict(inputFile, receiveChannelMapDict);
	TEST_ASSERT(status == IUS_E_OK);

	iutad_t transmitApodizationDict = dgGenerateTransmitApodizationDict();
	status = iusHLInputFileSetTransmitApodizationDict(inputFile, transmitApodizationDict);
	TEST_ASSERT(status == IUS_E_OK);

    iursd_t receiveSettingsDict = dgGenerateReceiveSettingsDict();
    status = iusHLInputFileSetReceiveSettingsDict(inputFile, receiveSettingsDict);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);

    // save
	iue_t experiment = dgGenerateExperiment();
	status = iusHLInputFileSetExperiment(inputFile, experiment);
	TEST_ASSERT(status == IUS_E_OK);

	// save
    status = iusHLInputFileSave(inputFile);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    status = iusHLInputFileClose(inputFile);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // read back
    iuif_t savedObj = iusHLInputFileLoad(ptestFileName);
    TEST_ASSERT(savedObj != NULL);
    TEST_ASSERT_EQUAL(IUS_TRUE, iusHLInputFileCompare(inputFile,savedObj));

	status = iusHLInputFileClose(savedObj);
	TEST_ASSERT(status == IUS_E_OK);

    iusHLPulseDictDelete(pulseDict);
	iusHLReceiveChannelMapDictDelete(receiveChannelMapDict);
	iusHLTransmitApodizationDictDelete(transmitApodizationDict);

	iusHLExperimentDelete(experiment);
    iusHLInputFileDelete(inputFile);
    iusHLInputFileDelete(savedObj);
}

TEST_GROUP_RUNNER(IusInputFile)
{
    RUN_TEST_CASE(IusInputFile, testIusInputFileCreate);
    RUN_TEST_CASE(IusInputFile, testIusInputFileDelete);
    RUN_TEST_CASE(IusInputFile, testIusInputFileCompare);
    RUN_TEST_CASE(IusInputFile, iusHLInputFileSetGetFrameList);
    RUN_TEST_CASE(IusInputFile, iusHLInputFileSetGetPatternList);
    RUN_TEST_CASE(IusInputFile, iusHLInputFileSetGetPulseDict);
    RUN_TEST_CASE(IusInputFile, iusHLInputFileSetGetSourceDict);
    RUN_TEST_CASE(IusInputFile, iusHLInputFileSetGetReceiveChannelMapDict);
    RUN_TEST_CASE(IusInputFile, iusHLInputFileSetGetTransmitApodizationDict);
    RUN_TEST_CASE(IusInputFile, iusHLInputFileSetGetReceiveSettingsDict);
    RUN_TEST_CASE(IusInputFile, testIusInputFileSerialization);
}
