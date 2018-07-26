

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
#include <include/iusHLPatternList.h>

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
    TEST_ASSERT(ifh != IUF_INVALID);

    // Create file that is already open should result in error
    iuif_t ifh2 = iusHLInputFileCreate(pFilename);
    TEST_ASSERT(ifh2 == IUF_INVALID);

    int status = iusHLInputFileClose(ifh);
    TEST_ASSERT(status == IUS_E_OK);

    // Closing file that has already been closed results in error
    status = iusHLInputFileClose(ifh);
    TEST_ASSERT(status != IUS_E_OK);

    // Invalid argument should result in error.
    ifh = iusHLInputFileCreate(pEmptyFilename);
    TEST_ASSERT(ifh == IUF_INVALID);
    ifh = iusHLInputFileCreate(pSpecialCharsFilename);
    TEST_ASSERT(ifh == IUF_INVALID);
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
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    
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


TEST(IusInputFile, testIusInputFileSetGet)
{
    IUS_BOOL equal;
    iupd_t pulseDict = iusHLPulseDictCreate();
	iurcmd_t receiveChannelMapDict = iusHLReceiveChannelMapDictCreate();
	iutad_t transmitApodizationDict = iusHLTransmitApodizationDictCreate();
    iuif_t obj = iusHLInputFileCreate(pFilename);
    TEST_ASSERT(obj != IUIF_INVALID);


    // pulseDict param
    int status = iusHLInputFileSetPulseDict(obj,pulseDict);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    iupd_t gotMeAPulseDict = iusHLInputFileGetPulseDict(obj);
    TEST_ASSERT_NOT_EQUAL(NULL, gotMeAPulseDict);

    equal = iusHLPulseDictCompare(pulseDict,gotMeAPulseDict);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);


	// receiveChannelMapDict param
	status = iusHLInputFileSetReceiveChannelMapDict(obj, receiveChannelMapDict);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);

	iurcmd_t gotMeAReceiveChannelMapDict = iusHLInputFileGetReceiveChannelMapDict(obj);
	TEST_ASSERT_NOT_EQUAL(NULL, gotMeAReceiveChannelMapDict);

	equal = iusHLReceiveChannelMapDictCompare(receiveChannelMapDict, gotMeAReceiveChannelMapDict);
	TEST_ASSERT_EQUAL(IUS_TRUE, equal);

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


TEST(IusInputFile, testIusInputFileSerialization)
{
    const char *ptestFileName = "testIusInputFileSerialization.hdf5";

    // create
    iuif_t inputFile = iusHLInputFileCreate(ptestFileName);
    TEST_ASSERT(inputFile != IUIF_INVALID);

    // fill
    iupd_t pulseDict = dgGeneratePulseDict();
    int status = iusHLInputFileSetPulseDict(inputFile, pulseDict);
    TEST_ASSERT(status == IUS_E_OK);


	iurcmd_t receiveChannelMapDict = dgGenerateReceiveChannelMapDict();
	status = iusHLInputFileSetReceiveChannelMapDict(inputFile, receiveChannelMapDict);
	TEST_ASSERT(status == IUS_E_OK);


	iutad_t transmitApodizationDict = dgGenerateTransmitApodizationDict();
	status = iusHLInputFileSetTransmitApodizationDict(inputFile, transmitApodizationDict);
	TEST_ASSERT(status == IUS_E_OK);

    // save
    iupal_t patternList = dgGeneratePatternList();
    status = iusHLInputFileSetPatternList(inputFile,patternList);
    TEST_ASSERT(status == IUS_E_OK);

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
    RUN_TEST_CASE(IusInputFile, testIusInputFileSetGet);
    RUN_TEST_CASE(IusInputFile, testIusInputFileSerialization);
}
