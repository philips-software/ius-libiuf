

//
// Created by nlv09165 on 11/07/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <include/ius.h>
#include <include/iusError.h>
#include <include/iusTypes.h>
#include <include/iusInputFile.h>
#include <include/iusPulseDict.h>
#include <include/iusReceiveChannelMapDict.h>
#include <include/iusTransmitApodizationDict.h>

#include <include/iusParametricPulse.h>
#include <include/iusNonParametricPulse.h>

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

    iuif_t ifh = iusInputFileCreate(pFilename);
    TEST_ASSERT(ifh != IUIF_INVALID);

    // Create file that is already open should result in error
    iuif_t ifh2 = iusInputFileCreate(pFilename);
    TEST_ASSERT(ifh2 == IUIF_INVALID);

    int status = iusInputFileClose(ifh);
    TEST_ASSERT(status == IUS_E_OK);

    // Closing file that has already been closed results in error
    status = iusInputFileClose(ifh);
    TEST_ASSERT(status != IUS_E_OK);

    // Invalid argument should result in error.
    ifh = iusInputFileCreate(pEmptyFilename);
    TEST_ASSERT(ifh == IUIF_INVALID);
    ifh = iusInputFileCreate(pSpecialCharsFilename);
    TEST_ASSERT(ifh == IUIF_INVALID);
}

TEST(IusInputFile, testIusInputFileDelete)
{
    iuif_t obj = iusInputFileCreate(pFilename);
    TEST_ASSERT(obj != IUIF_INVALID);

	int status = iusInputFileClose(obj);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);
	
	status = iusInputFileDelete(obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // invalid params
    status = iusInputFileDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
}


TEST(IusInputFile, testIusInputFileCompare)
{
    IUS_BOOL equal;
    iuif_t obj = iusInputFileCreate(pFilename);
    iuif_t notherObj = iusInputFileCreate(pNotherFilename);
    TEST_ASSERT(obj != IUIF_INVALID);
    TEST_ASSERT(notherObj != IUIF_INVALID);
    equal = iusInputFileCompare(obj,obj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = iusInputFileCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    
    // invalid params
    equal = iusInputFileCompare(obj,NULL);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    equal = iusInputFileCompare(NULL,obj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

	int status = iusInputFileClose(obj);
	TEST_ASSERT(status == IUS_E_OK);
	status = iusInputFileClose(notherObj);
	TEST_ASSERT(status == IUS_E_OK);
	iusInputFileDelete(obj);
    iusInputFileDelete(notherObj);
}

TEST(IusInputFile, iusInputFileSetGetFrameList)
{
    IUS_BOOL equal;
    int status;
    iufl_t frameList = dgGenerateFrameList();
    iuif_t obj = iusInputFileCreate(pFilename);

    status = iusInputFileSetFrameList(obj,frameList);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    iufl_t gotMeAFrameList = iusInputFileGetFrameList(obj);
    TEST_ASSERT_NOT_EQUAL(IUFL_INVALID, gotMeAFrameList);

    equal = iusFrameListCompare(frameList,gotMeAFrameList);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);

    status = iusInputFileClose(obj);
    TEST_ASSERT(status == IUS_E_OK);
    iusInputFileDelete(obj);
}

TEST(IusInputFile, iusInputFileSetGetPatternList)
{
    IUS_BOOL equal;
    int status;
    iupal_t patternList = dgGeneratePatternList();
    iuif_t obj = iusInputFileCreate(pFilename);

    status = iusInputFileSetPatternList(obj,patternList);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    iupal_t gotMeAPatternList = iusInputFileGetPatternList(obj);
    TEST_ASSERT_NOT_EQUAL(IUPAL_INVALID, gotMeAPatternList);

    equal = iusPatternListCompare(patternList,gotMeAPatternList);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);

    status = iusInputFileClose(obj);
    TEST_ASSERT(status == IUS_E_OK);
    iusInputFileDelete(obj);
}


TEST(IusInputFile, iusInputFileSetGetPulseDict)
{
    iuif_t obj = iusInputFileCreate(pFilename);
    iupd_t pulseDict = dgGeneratePulseDict();

    int status = iusInputFileSetPulseDict(obj, pulseDict);
    TEST_ASSERT(status == IUS_E_OK);
    iupd_t gotMeAPulseDict = iusInputFileGetPulseDict(obj);
    TEST_ASSERT_NOT_EQUAL(NULL, gotMeAPulseDict);

    IUS_BOOL equal = iusPulseDictCompare(pulseDict,gotMeAPulseDict);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);

    status = iusInputFileClose(obj);
    TEST_ASSERT(status == IUS_E_OK);
    iusInputFileDelete(obj);
}

TEST(IusInputFile, iusInputFileSetGetSourceDict)
{
    iuif_t obj = iusInputFileCreate(pFilename);
    iusd_t sourceDict = dgGenerateSourceDict();

    int status = iusInputFileSetSourceDict(obj, sourceDict);
    TEST_ASSERT(status == IUS_E_OK);
    iusd_t gotMeASourceDict = iusInputFileGetSourceDict(obj);
    TEST_ASSERT_NOT_EQUAL(IUSD_INVALID, gotMeASourceDict);

    IUS_BOOL equal = iusSourceDictCompare(sourceDict,gotMeASourceDict);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);

    status = iusInputFileClose(obj);
    TEST_ASSERT(status == IUS_E_OK);
    iusInputFileDelete(obj);
}


TEST(IusInputFile, iusInputFileSetGetReceiveChannelMapDict)
{
    iuif_t obj = iusInputFileCreate(pFilename);
    iurcmd_t receiveChannelMapDict = iusReceiveChannelMapDictCreate();

    int status = iusInputFileSetReceiveChannelMapDict(obj, receiveChannelMapDict);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);

    iurcmd_t gotMeAReceiveChannelMapDict = iusInputFileGetReceiveChannelMapDict(obj);
    TEST_ASSERT_NOT_EQUAL(NULL, gotMeAReceiveChannelMapDict);

    IUS_BOOL equal = iusReceiveChannelMapDictCompare(receiveChannelMapDict, gotMeAReceiveChannelMapDict);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);

    status = iusInputFileClose(obj);
    TEST_ASSERT(status == IUS_E_OK);
    iusInputFileDelete(obj);
}

TEST(IusInputFile, iusInputFileSetGetTransmitApodizationDict)
{
    IUS_BOOL equal;
    int status;
	iutad_t transmitApodizationDict = iusTransmitApodizationDictCreate();
    iuif_t obj = iusInputFileCreate(pFilename);
    TEST_ASSERT(obj != IUIF_INVALID);

	// transmitApodizationDict param
	status = iusInputFileSetTransmitApodizationDict(obj, transmitApodizationDict);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);

	iutad_t gotMeATransmitApodizationDict = iusInputFileGetTransmitApodizationDict(obj);
	TEST_ASSERT_NOT_EQUAL(NULL, gotMeATransmitApodizationDict);

	equal = iusTransmitApodizationDictCompare(transmitApodizationDict, gotMeATransmitApodizationDict);
	TEST_ASSERT_EQUAL(IUS_TRUE, equal);

    // invalid param
    status = iusInputFileSetTransmitApodizationDict(obj, NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);

    status = iusInputFileSetTransmitApodizationDict(NULL, transmitApodizationDict);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);

    gotMeATransmitApodizationDict = iusInputFileGetTransmitApodizationDict(NULL);
    TEST_ASSERT_EQUAL(IUTAD_INVALID, gotMeATransmitApodizationDict);

	status = iusInputFileClose(obj);
	TEST_ASSERT(status == IUS_E_OK);
    iusInputFileDelete(obj);
}

TEST(IusInputFile, iusInputFileSetGetReceiveSettingsDict)
{
    IUS_BOOL equal;
    int status;
    iursd_t receiveSettingsDict = dgGenerateReceiveSettingsDict();
    iuif_t obj = iusInputFileCreate(pFilename);
    TEST_ASSERT(obj != IUIF_INVALID);

    status = iusInputFileSetReceiveSettingsDict(obj, receiveSettingsDict);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);

    iursd_t gotMeATransmitReceiveSettingsDict = iusInputFileGetReceiveSettingsDict(obj);
    TEST_ASSERT_NOT_EQUAL(NULL, gotMeATransmitReceiveSettingsDict);

    equal = iusReceiveSettingsDictCompare(receiveSettingsDict, gotMeATransmitReceiveSettingsDict);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);

    // invalid param
    status = iusInputFileSetReceiveSettingsDict(obj, NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);

    status = iusInputFileSetReceiveSettingsDict(NULL, receiveSettingsDict);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);

    gotMeATransmitReceiveSettingsDict = iusInputFileGetReceiveSettingsDict(NULL);
    TEST_ASSERT_EQUAL(IURSD_INVALID, gotMeATransmitReceiveSettingsDict);

    status = iusInputFileClose(obj);
    TEST_ASSERT(status == IUS_E_OK);
    iusInputFileDelete(obj);
}

TEST(IusInputFile, iusInputFileSetGetExperiment)
{
    IUS_BOOL equal;
    int status;
    iue_t experiment = dgGenerateExperiment();
    iuif_t obj = iusInputFileCreate(pFilename);
    TEST_ASSERT(obj != IUIF_INVALID);

    status = iusInputFileSetExperiment(obj, experiment);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);

    iue_t gotMeAnExperiment = iusInputFileGetExperiment(obj);
    TEST_ASSERT_NOT_EQUAL(NULL, gotMeAnExperiment);

    equal = iusExperimentCompare(experiment, gotMeAnExperiment);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);

    // invalid param
    status = iusInputFileSetExperiment(obj, NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);

    status = iusInputFileSetExperiment(NULL, experiment);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);

    gotMeAnExperiment = iusInputFileGetExperiment(NULL);
    TEST_ASSERT_EQUAL(IURSD_INVALID, gotMeAnExperiment);

    status = iusInputFileClose(obj);
    TEST_ASSERT(status == IUS_E_OK);
    iusInputFileDelete(obj);
}

TEST(IusInputFile, iusInputFileSetGetTransducer)
{
    IUS_BOOL equal;
    int status;
    iut_t transducer = dgGenerateTransducer("S5-1");
    iuif_t obj = iusInputFileCreate(pFilename);
    TEST_ASSERT(obj != IUIF_INVALID);

    status = iusInputFileSetTransducer(obj, transducer);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);

    iut_t gotMeATransducer = iusInputFileGetTransducer(obj);
    TEST_ASSERT_NOT_EQUAL(NULL, gotMeATransducer);

    equal = iusTransducerCompare(transducer, gotMeATransducer);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);

    // invalid param
    status = iusInputFileSetTransducer(obj, NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);

    status = iusInputFileSetTransducer(NULL, transducer);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);

    gotMeATransducer = iusInputFileGetTransducer(NULL);
    TEST_ASSERT_EQUAL(IURSD_INVALID, gotMeATransducer);

    status = iusInputFileClose(obj);
    TEST_ASSERT(status == IUS_E_OK);
    iusInputFileDelete(obj);
}

TEST(IusInputFile, testIusInputFileSerialization)
{
    char *ptestFileName = "testIusInputFileSerialization.hdf5";

    // create
    iuif_t inputFile = dgGenerateInputFile(ptestFileName,"S5-1");
    TEST_ASSERT(inputFile != IUIF_INVALID);

	// save
    int status = iusInputFileSave(inputFile);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    status = iusInputFileClose(inputFile);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // read back
    iuif_t savedObj = iusInputFileLoad(ptestFileName);
    TEST_ASSERT(savedObj != NULL);
    TEST_ASSERT_EQUAL(IUS_TRUE, iusInputFileCompare(inputFile,savedObj));

	status = iusInputFileClose(savedObj);
	TEST_ASSERT(status == IUS_E_OK);

    iusInputFileDelete(inputFile);
    iusInputFileDelete(savedObj);
}

TEST_GROUP_RUNNER(IusInputFile)
{
    RUN_TEST_CASE(IusInputFile, testIusInputFileCreate);
    RUN_TEST_CASE(IusInputFile, testIusInputFileDelete);
    RUN_TEST_CASE(IusInputFile, testIusInputFileCompare);
    RUN_TEST_CASE(IusInputFile, iusInputFileSetGetFrameList);
    RUN_TEST_CASE(IusInputFile, iusInputFileSetGetPatternList);
    RUN_TEST_CASE(IusInputFile, iusInputFileSetGetPulseDict);
    RUN_TEST_CASE(IusInputFile, iusInputFileSetGetSourceDict);
    RUN_TEST_CASE(IusInputFile, iusInputFileSetGetReceiveChannelMapDict);
    RUN_TEST_CASE(IusInputFile, iusInputFileSetGetTransmitApodizationDict);
    RUN_TEST_CASE(IusInputFile, iusInputFileSetGetReceiveSettingsDict);
    RUN_TEST_CASE(IusInputFile, iusInputFileSetGetExperiment);
    RUN_TEST_CASE(IusInputFile, iusInputFileSetGetTransducer);
    RUN_TEST_CASE(IusInputFile, testIusInputFileSerialization);
}
