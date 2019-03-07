

//
// Created by nlv09165 on 11/07/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <ius.h>
#include <testDataGenerators.h>

static const char *pFilename = "IusInputFile.hdf5";
static const char *pNotherFilename = "AnotherIusInputFile.hdf5";

static char *pErrorFilename = "IusInputFile.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(IusInputFile);

TEST_SETUP(IusInputFile)
{
    iusErrorLogClear();
    iusErrorLog(IUS_TRUE);
    iusErrorAutoReport(IUS_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iusErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IusInputFile)
{
    iusErrorLogClear();
    iusErrorLog(IUS_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iusErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
}

TEST(IusInputFile, testIusInputFileCreate)
{
    const char *pEmptyFilename = "";
    const char *pSpecialCharsFilename = "*&/";

    iuif_t ifh = iusInputFileCreate(pFilename);
    TEST_ASSERT(ifh != IUIF_INVALID);

    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());

    // Create file that is already open should result in error
    iuif_t ifh2 = iusInputFileCreate(pFilename);
    TEST_ASSERT(ifh2 == IUIF_INVALID);
	TEST_ASSERT_EQUAL(1, iusErrorGetCount());

    int status = iusInputFileClose(ifh);
    TEST_ASSERT(status == IUS_E_OK);

    // Closing file that has already been closed results in error
    status = iusInputFileClose(ifh);
	TEST_ASSERT_EQUAL(2, iusErrorGetCount());
    iusInputFileDelete(ifh);
    TEST_ASSERT(status != IUS_E_OK);
    // Invalid argument should result in error.
    ifh = iusInputFileCreate(pEmptyFilename);
    TEST_ASSERT(ifh == IUIF_INVALID);
	TEST_ASSERT_EQUAL(3, iusErrorGetCount());
    ifh = iusInputFileCreate(pSpecialCharsFilename);
    TEST_ASSERT(ifh == IUIF_INVALID);
    TEST_ASSERT_EQUAL(4,iusErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

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
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());

    status = iusInputFileDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);

    TEST_ASSERT_EQUAL(1, iusErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
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
    iufl_t frameList = dgGenerateFrameList(10);
    iuif_t obj = iusInputFileCreate(pFilename);

    status = iusInputFileSetFrameList(obj,frameList);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    iufl_t gotMeAFrameList = iusInputFileGetFrameList(obj);
    TEST_ASSERT_NOT_EQUAL(IUFL_INVALID, gotMeAFrameList);

    equal = iusFrameListCompare(frameList,gotMeAFrameList);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);

    status = iusInputFileClose(obj);
    TEST_ASSERT(status == IUS_E_OK);
    iusFrameListDeepDelete(frameList);
    iusInputFileDelete(obj);
}

TEST(IusInputFile, iusInputFileSetGetPatternListDict)
{
    IUS_BOOL equal;
    int status;
    iupald_t patternListDict = dgGeneratePatternListDict("mylabel",NULL,NULL);
    iuif_t obj = iusInputFileCreate(pFilename);

    status = iusInputFileSetPatternListDict(obj, patternListDict);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    iupald_t gotMeAPatternListDict = iusInputFileGetPatternListDict(obj);
    TEST_ASSERT_NOT_EQUAL(IUPALD_INVALID, gotMeAPatternListDict);

    equal = iusPatternListDictCompare(patternListDict,gotMeAPatternListDict);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);

    status = iusInputFileClose(obj);
    TEST_ASSERT(status == IUS_E_OK);
    iusInputFileDelete(obj);
    iusPatternListDictDeepDelete(patternListDict);
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
    iusPulseDictDeepDelete(pulseDict);
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
    iusSourceDictDeepDelete(sourceDict);
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
    iusReceiveChannelMapDictDelete(receiveChannelMapDict);
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
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());

    status = iusInputFileSetTransmitApodizationDict(obj, NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);

    status = iusInputFileSetTransmitApodizationDict(NULL, transmitApodizationDict);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);

    gotMeATransmitApodizationDict = iusInputFileGetTransmitApodizationDict(NULL);
    TEST_ASSERT_EQUAL(IUTAD_INVALID, gotMeATransmitApodizationDict);

    TEST_ASSERT_EQUAL(3, iusErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

	status = iusInputFileClose(obj);
	TEST_ASSERT(status == IUS_E_OK);
    iusInputFileDelete(obj);
    iusTransmitApodizationDictDelete(transmitApodizationDict);
}

TEST(IusInputFile, iusInputFileSetGetReceiveSettingsDict)
{
    IUS_BOOL equal;
    int status;
    iursd_t receiveSettingsDict = dgGenerateReceiveSettingsDict("bmode", 0);
    iuif_t obj = iusInputFileCreate(pFilename);
    TEST_ASSERT(obj != IUIF_INVALID);

    status = iusInputFileSetReceiveSettingsDict(obj, receiveSettingsDict);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);

    iursd_t gotMeATransmitReceiveSettingsDict = iusInputFileGetReceiveSettingsDict(obj);
    TEST_ASSERT_NOT_EQUAL(NULL, gotMeATransmitReceiveSettingsDict);

    equal = iusReceiveSettingsDictCompare(receiveSettingsDict, gotMeATransmitReceiveSettingsDict);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);

    // invalid param
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());

    status = iusInputFileSetReceiveSettingsDict(obj, NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);

    status = iusInputFileSetReceiveSettingsDict(NULL, receiveSettingsDict);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);

    gotMeATransmitReceiveSettingsDict = iusInputFileGetReceiveSettingsDict(NULL);
    TEST_ASSERT_EQUAL(IURSD_INVALID, gotMeATransmitReceiveSettingsDict);

    TEST_ASSERT_EQUAL(3, iusErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));


    status = iusInputFileClose(obj);
    TEST_ASSERT(status == IUS_E_OK);
    iusInputFileDelete(obj);
    iusReceiveSettingsDictDeepDelete(receiveSettingsDict);
}

TEST(IusInputFile, iusInputFileSetGetAcquisition)
{
    IUS_BOOL equal;
    int status;
    iua_t acquisition = dgGenerateAcquisition();
    iuif_t obj = iusInputFileCreate(pFilename);
    TEST_ASSERT(obj != IUIF_INVALID);

    status = iusInputFileSetAcquisition(obj, acquisition);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);

    iua_t gotMeAnAcquisition = iusInputFileGetAcquisition(obj);
    TEST_ASSERT_NOT_EQUAL(NULL, gotMeAnAcquisition);

    equal = iusAcquisitionCompare(acquisition, gotMeAnAcquisition);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);

    // invalid param
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());

    status = iusInputFileSetAcquisition(obj, NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);

    status = iusInputFileSetAcquisition(NULL, acquisition);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);

    gotMeAnAcquisition = iusInputFileGetAcquisition(NULL);
    TEST_ASSERT_EQUAL(IURSD_INVALID, gotMeAnAcquisition);

    TEST_ASSERT_EQUAL(3, iusErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));


    status = iusInputFileClose(obj);
    TEST_ASSERT(status == IUS_E_OK);
    iusInputFileDelete(obj);
    iusAcquisitionDelete(acquisition);
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
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());

    status = iusInputFileSetTransducer(obj, NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);

    status = iusInputFileSetTransducer(NULL, transducer);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);

    gotMeATransducer = iusInputFileGetTransducer(NULL);
    TEST_ASSERT_EQUAL(IURSD_INVALID, gotMeATransducer);

    TEST_ASSERT_EQUAL(3, iusErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

    status = iusInputFileClose(obj);
    TEST_ASSERT(status == IUS_E_OK);
    iusInputFileDelete(obj);
    iusTransducerDeepDelete(transducer);
}

static int saveFrames
(
    iuif_t inputFile,
    char *label,
    int numFrames
)
{
    int status=0;
    int i;
    iud_t frame = iusInputFileFrameCreate(inputFile, label);
    iuo_t offset = iusOffsetCreate();

    for (i=0; i<numFrames; i++)
    {
        dgFillData(frame, 1 + i * 1.0f);
        offset->t = i;
        status |= iusInputFileFrameSave(inputFile, label, frame, offset);
    }

    iusOffsetDelete(offset);
    iusDataDelete(frame);
    return status;
}

static int saveResponses
(
    iuif_t inputFile,
    char *label,
    int numFrames
)
{
    int status=0;
    int i,j;
    iud_t response = iusInputFileResponseCreate(inputFile, label);
    iuo_t offset = iusOffsetCreate();
    int numResponses = iusInputFileGetNumResponses(inputFile, label);

    for (i=0; i<numFrames; i++)
    {
        dgFillData(response, 1 + i * 1.0f);
        // Save Frame in responses
        for (j=0; j<numResponses; j++)
        {
            offset->y = j;
            offset->t = i;
            status |= iusInputFileResponseSave(inputFile, label, response, offset);
        }
    }

    iusDataDelete(response);
    iusOffsetDelete(offset);
    return status;
}

static int saveChannels
(
    iuif_t inputFile,
    char *label,
    int numFrames
)
{
    int status=0;
    int i,j,k;
    iud_t channel = iusInputFileChannelCreate(inputFile, label);
    iuo_t offset = iusOffsetCreate();
    int numResponses = iusInputFileGetNumResponses(inputFile, label);
    int numChannels = iusInputFileGetNumChannels(inputFile, label);

    for (i=0; i<numFrames; i++)
    {
        dgFillData(channel, 1 + i * 1.0f);
        // Save Frame in responses
        for (j=0; j<numResponses; j++)
        {
            for (k=0; k<numChannels; k++)
            {
                offset->z = k;
                offset->y = j;
                offset->t = i;
                status |= iusInputFileChannelSave(inputFile, label, channel, offset);
            }
        }
    }

    iusDataDelete(channel);
    iusOffsetDelete(offset);
    return status;
}


static IUS_BOOL validateFrames
(
    iuif_t inputFile,
    char *label,
    int numFrames
)
{
    int status=0;
    int i;
    IUS_BOOL equal = IUS_FALSE;

    iud_t referenceFrame = iusInputFileFrameCreate(inputFile, label);
    iud_t actualFrame = iusInputFileFrameCreate(inputFile, label);
    iuo_t offset = iusOffsetCreate();
    for (i=0; i<numFrames; i++)
    {
        dgFillData(referenceFrame, 1 + i * 1.0f);
        offset->t = i;
        status |= iusInputFileFrameLoad(inputFile, label, actualFrame, offset);
        equal = iusDataCompare(referenceFrame,actualFrame);
        if (equal == IUS_FALSE) break;
    }

    iusDataDelete(referenceFrame);
    iusDataDelete(actualFrame);
    iusOffsetDelete(offset);
    return equal;
}


static IUS_BOOL validateResponses
(
    iuif_t inputFile,
    char *label,
    int numFrames
)
{
    int status=0;
    int i,j;
    IUS_BOOL equal = IUS_FALSE;

    iud_t referenceResponse = iusInputFileResponseCreate(inputFile, label);
    iud_t actualResponse = iusInputFileResponseCreate(inputFile, label);
    iuo_t offset = iusOffsetCreate();
    int numResponses = iusInputFileGetNumResponses(inputFile, label);

    for (i=0; i<numFrames; i++)
    {
        dgFillData(referenceResponse, 1 + i * 1.0f);
        for (j=0; j<numResponses; j++)
        {
            offset->y = j;
            offset->t = i;
            status |= iusInputFileResponseLoad(inputFile, label, actualResponse, offset);
            equal = iusDataCompare(referenceResponse,actualResponse);
            if (equal == IUS_FALSE) break;
        }
        if (equal == IUS_FALSE) break;
    }



    iusDataDelete(referenceResponse);
    iusDataDelete(actualResponse);
    iusOffsetDelete(offset);
    return equal;
}

#if 0
static IUS_BOOL validateChannels
(
    iuif_t inputFile,
    char *label,
    int numFrames
)
{
    int status=0;
    int i,j,k;
    IUS_BOOL equal = IUS_FALSE;

    iud_t referenceChannel = iusInputFileChannelCreate(inputFile, label);
    iud_t actualChannel = iusInputFileChannelCreate(inputFile, label);
    iuo_t offset = iusOffsetCreate();
    int numResponses = iusInputFileGetNumResponses(inputFile, label);
    int numChannels = iusInputFileGetNumChannels(inputFile, label);

    for (i=0; i<numFrames; i++)
    {
        dgFillData(referenceChannel, 1 + i * 1.0f);
        // Save Frame in responses
        for (j=0; j<numResponses; j++)
        {
            for (k=0; k<numChannels; k++)
            {
                offset->z = k;
                offset->y = j;
                offset->t = i;
                status |= iusInputFileChannelLoad(inputFile, label, actualChannel, offset);
                equal = iusDataCompare(referenceChannel, actualChannel);
                if (equal == IUS_FALSE) break;
            }
            if (equal == IUS_FALSE) break;
        }
        if (equal == IUS_FALSE) break;
    }



    iusDataDelete(referenceChannel);
    iusDataDelete(actualChannel);
    iusOffsetDelete(offset);
    return equal;
}
#endif

TEST(IusInputFile, testIusInputFileDataIOSaveFrame)
{
    char *ptestFileName = "testIusInputFileDataIOSaveFrame.hdf5";
    char *pDopplerLabel = "doppler";
    int numFrames = 10;
    int numSamplesPerLine = 10;
    int numChannels = 8;
    char *pBmodeLabel = "bmode";
    int status = 0;

    // create
    iuif_t inputFile = dgGenerateInputFile(ptestFileName, "S5-1", pDopplerLabel, numFrames, numSamplesPerLine, numChannels);
    status = dgInputFileAddGeneratedData(inputFile, pBmodeLabel, numSamplesPerLine, numChannels);
    TEST_ASSERT_EQUAL(status, IUS_E_OK);

    // SaveFrames
    status = iusInputFileNodeSave(inputFile);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    status = saveFrames(inputFile,pDopplerLabel,numFrames);
    status |= saveFrames(inputFile,pBmodeLabel,numFrames);
    TEST_ASSERT_EQUAL(status, IUS_E_OK);
    status = iusInputFileClose(inputFile);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // read back
    iuif_t savedObj = iusInputFileNodeLoad(ptestFileName);
    TEST_ASSERT(savedObj != NULL);
    TEST_ASSERT_EQUAL(IUS_TRUE, iusInputFileCompare(inputFile,savedObj));
    TEST_ASSERT_EQUAL(IUS_TRUE, validateFrames(savedObj,pDopplerLabel,numFrames));
    TEST_ASSERT_EQUAL(IUS_TRUE, validateFrames(savedObj,pBmodeLabel,numFrames));


    // create channel
    iusInputFileClose(savedObj);
    iusInputFileDelete(savedObj);
    dgDeleteInputFile(inputFile);
}

TEST(IusInputFile, testIusInputFileDataIOSaveResponse)
{
    char *ptestFileName = "testIusInputFileDataIOSaveResponse.hdf5";
    char *pDopplerLabel = "doppler";
    int numFrames = 10;
    int numSamplesPerLine = 10;
    int numChannels = 8;
    char *pBmodeLabel = "bmode";
    int status = 0;

    // create
    iuif_t inputFile = dgGenerateInputFile(ptestFileName, "S5-1", pDopplerLabel, numFrames, numSamplesPerLine, numChannels);
    status = dgInputFileAddGeneratedData(inputFile, pBmodeLabel, numSamplesPerLine, numChannels);
    TEST_ASSERT_EQUAL(status, IUS_E_OK);


    // SaveFrames
    status = iusInputFileNodeSave(inputFile);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    status = saveResponses(inputFile,pDopplerLabel,numFrames);
    status |= saveResponses(inputFile,pBmodeLabel,numFrames);
    TEST_ASSERT_EQUAL(status, IUS_E_OK);
    status = iusInputFileClose(inputFile);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // read back
    iuif_t savedObj = iusInputFileNodeLoad(ptestFileName);
    TEST_ASSERT(savedObj != NULL);
    TEST_ASSERT_EQUAL(IUS_TRUE, iusInputFileCompare(inputFile,savedObj));
    TEST_ASSERT_EQUAL(IUS_TRUE, validateResponses(savedObj,pDopplerLabel,numFrames));
    TEST_ASSERT_EQUAL(IUS_TRUE, validateResponses(savedObj,pBmodeLabel,numFrames));


    // create channel
    iusInputFileClose(savedObj);
    iusInputFileDelete(savedObj);
    dgDeleteInputFile(inputFile);
}

TEST(IusInputFile, testIusInputFileDataIOSaveChannel)
{
    char *ptestFileName = "testIusInputFileDataIOSaveChannel.hdf5";
    char *pDopplerLabel = "doppler";
    int numFrames = 10;
    int numSamplesPerLine = 10;
    int numChannels = 8;
    char *pBmodeLabel = "bmode";
    int status = 0;

    // create
    iuif_t inputFile = dgGenerateInputFile(ptestFileName, "S5-1", pDopplerLabel, numFrames, numSamplesPerLine, numChannels);
    status = dgInputFileAddGeneratedData(inputFile, pBmodeLabel, numSamplesPerLine, numChannels);
    TEST_ASSERT_EQUAL(status, IUS_E_OK);


    // SaveFrames
    status = iusInputFileNodeSave(inputFile);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    status = saveChannels(inputFile, pDopplerLabel, numFrames);
//    status |= saveChannels(inputFile, pBmodeLabel, numFrames);
    TEST_ASSERT_EQUAL(status, IUS_E_OK);
    status = iusInputFileClose(inputFile);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // read back
    iuif_t savedObj = iusInputFileNodeLoad(ptestFileName);
    TEST_ASSERT(savedObj != NULL);
    TEST_ASSERT_EQUAL(IUS_TRUE, iusInputFileCompare(inputFile,savedObj));
    // Todo:
    // Fix Errors in validateChannels:
//    TEST_ASSERT_EQUAL(IUS_TRUE, validateChannels(savedObj,pDopplerLabel,numFrames));
//    TEST_ASSERT_EQUAL(IUS_TRUE, validateChannels(savedObj,pBmodeLabel,numFrames));


    // create channel
    iusInputFileClose(savedObj);
    iusInputFileDelete(savedObj);
    dgDeleteInputFile(inputFile);
}


TEST(IusInputFile, testIusInputFileSerialization)
{
    int numFrames = 10;
    int numSamplesPerLine = 10;
    int numChannels = 8;
    char *ptestFileName = "testIusInputFileSerialization.hdf5";

    // create
    iuif_t inputFile = dgGenerateInputFile(ptestFileName, "S5-1", "bmode", numFrames, numSamplesPerLine, numChannels);
    TEST_ASSERT(inputFile != IUIF_INVALID);

    // save
    int status = iusInputFileNodeSave(inputFile);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    status = iusInputFileClose(inputFile);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // read back
    iuif_t savedObj = iusInputFileNodeLoad(ptestFileName);
    TEST_ASSERT(savedObj != NULL);
    TEST_ASSERT_EQUAL(IUS_TRUE, iusInputFileCompare(inputFile,savedObj));

    status = iusInputFileClose(savedObj);
    TEST_ASSERT(status == IUS_E_OK);

    dgDeleteInputFile(inputFile);
    iusInputFileDelete(savedObj);

}

TEST_GROUP_RUNNER(IusInputFile)
{
    RUN_TEST_CASE(IusInputFile, testIusInputFileCreate);
    RUN_TEST_CASE(IusInputFile, testIusInputFileDelete);
    RUN_TEST_CASE(IusInputFile, testIusInputFileCompare);
    RUN_TEST_CASE(IusInputFile, iusInputFileSetGetFrameList);
    RUN_TEST_CASE(IusInputFile, iusInputFileSetGetPatternListDict);
    RUN_TEST_CASE(IusInputFile, iusInputFileSetGetPulseDict);
    RUN_TEST_CASE(IusInputFile, iusInputFileSetGetSourceDict);
    RUN_TEST_CASE(IusInputFile, iusInputFileSetGetReceiveChannelMapDict);
    RUN_TEST_CASE(IusInputFile, iusInputFileSetGetTransmitApodizationDict);
    RUN_TEST_CASE(IusInputFile, iusInputFileSetGetReceiveSettingsDict);
    RUN_TEST_CASE(IusInputFile, iusInputFileSetGetAcquisition);
    RUN_TEST_CASE(IusInputFile, iusInputFileSetGetTransducer);
    RUN_TEST_CASE(IusInputFile, testIusInputFileSerialization);
    RUN_TEST_CASE(IusInputFile, testIusInputFileDataIOSaveFrame);
    RUN_TEST_CASE(IusInputFile, testIusInputFileDataIOSaveResponse);
    RUN_TEST_CASE(IusInputFile, testIusInputFileDataIOSaveChannel);
}
