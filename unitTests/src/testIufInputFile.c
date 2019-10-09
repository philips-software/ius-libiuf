#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <iuf.h>
#include <dg/dataGenerators.h>

static const char *pFilename = "IufInputFile.hdf5";
static const char *pNotherFilename = "AnotherIufInputFile.hdf5";

static char *pErrorFilename = "IufInputFile.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(IufInputFile);

TEST_SETUP(IufInputFile)
{
    iufErrorLogClear();
    iufErrorLog(IUF_TRUE);
    iufErrorAutoReportSet(IUF_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iufErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IufInputFile)
{
    iufErrorLogClear();
    iufErrorLog(IUF_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iufErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
}

TEST(IufInputFile, testIufInputFileCreate)
{
    const char *pEmptyFilename = "";
    const char *pSpecialCharsFilename = "*&/";

    iuif_t ifh = iufInputFileCreate(pFilename);
    TEST_ASSERT(ifh != IUIF_INVALID);

    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    // Create file that is already open should result in error
    iuif_t ifh2 = iufInputFileCreate(pFilename);
    TEST_ASSERT(ifh2 == IUIF_INVALID);
	TEST_ASSERT_EQUAL(1, iufErrorGetCount());

    int status = iufInputFileClose(ifh);
    TEST_ASSERT(status == IUF_E_OK);

    // Closing file that has already been closed results in error
    status = iufInputFileClose(ifh);
	TEST_ASSERT_EQUAL(2, iufErrorGetCount());
    iufInputFileDelete(ifh);
    TEST_ASSERT(status != IUF_E_OK);
    // Invalid argument should result in error.
    ifh = iufInputFileCreate(pEmptyFilename);
    TEST_ASSERT(ifh == IUIF_INVALID);
	TEST_ASSERT_EQUAL(3, iufErrorGetCount());
    ifh = iufInputFileCreate(pSpecialCharsFilename);
    TEST_ASSERT(ifh == IUIF_INVALID);
    TEST_ASSERT_EQUAL(4,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

}

TEST(IufInputFile, testIufInputFileDelete)
{
    iuif_t obj = iufInputFileCreate(pFilename);
    TEST_ASSERT(obj != IUIF_INVALID);

	int status = iufInputFileClose(obj);
	TEST_ASSERT_EQUAL(IUF_E_OK, status);
	
	status = iufInputFileDelete(obj);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    // invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    status = iufInputFileDelete(NULL);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, status);

    TEST_ASSERT_EQUAL(1, iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
}


TEST(IufInputFile, testIufInputFileCompare)
{
    IUF_BOOL equal;
    iuif_t obj = iufInputFileCreate(pFilename);
    iuif_t notherObj = iufInputFileCreate(pNotherFilename);
    TEST_ASSERT(obj != IUIF_INVALID);
    TEST_ASSERT(notherObj != IUIF_INVALID);
    equal = iufInputFileCompare(obj,obj);
    TEST_ASSERT_EQUAL(IUF_TRUE,equal);
    equal = iufInputFileCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);
    
    // invalid params
    equal = iufInputFileCompare(obj,NULL);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);
    equal = iufInputFileCompare(NULL,obj);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);

	int status = iufInputFileClose(obj);
	TEST_ASSERT(status == IUF_E_OK);
	status = iufInputFileClose(notherObj);
	TEST_ASSERT(status == IUF_E_OK);
	iufInputFileDelete(obj);
    iufInputFileDelete(notherObj);
}

TEST(IufInputFile, iufInputFileSetGetFrameList)
{
    IUF_BOOL equal;
    int status;
    iufl_t frameList = dgGenerateFrameList(10);
    iuif_t obj = iufInputFileCreate(pFilename);

    status = iufInputFileSetFrameList(obj,frameList);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    iufl_t gotMeAFrameList = iufInputFileGetFrameList(obj);
    TEST_ASSERT_NOT_EQUAL(IUFL_INVALID, gotMeAFrameList);

    equal = iufFrameListCompare(frameList,gotMeAFrameList);
    TEST_ASSERT_EQUAL(IUF_TRUE, equal);

    status = iufInputFileClose(obj);
    TEST_ASSERT(status == IUF_E_OK);
    iufFrameListDeepDelete(frameList);
    iufInputFileDelete(obj);
}

TEST(IufInputFile, iufInputFileSetGetPatternListDict)
{
    IUF_BOOL equal;
    int status;
    iupald_t patternListDict = dgGeneratePatternListDict("mylabel",NULL,NULL);
    iuif_t obj = iufInputFileCreate(pFilename);

    status = iufInputFileSetPatternListDict(obj, patternListDict);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    iupald_t gotMeAPatternListDict = iufInputFileGetPatternListDict(obj);
    TEST_ASSERT_NOT_EQUAL(IUPALD_INVALID, gotMeAPatternListDict);

    equal = iufPatternListDictCompare(patternListDict,gotMeAPatternListDict);
    TEST_ASSERT_EQUAL(IUF_TRUE, equal);

    status = iufInputFileClose(obj);
    TEST_ASSERT(status == IUF_E_OK);
    iufInputFileDelete(obj);
    iufPatternListDictDeepDelete(patternListDict);
}


TEST(IufInputFile, iufInputFileSetGetPulseDict)
{
    iuif_t obj = iufInputFileCreate(pFilename);
    iupd_t pulseDict = dgGeneratePulseDict();

    int status = iufInputFileSetPulseDict(obj, pulseDict);
    TEST_ASSERT(status == IUF_E_OK);
    iupd_t gotMeAPulseDict = iufInputFileGetPulseDict(obj);
    TEST_ASSERT_NOT_EQUAL(NULL, gotMeAPulseDict);

    IUF_BOOL equal = iufPulseDictCompare(pulseDict,gotMeAPulseDict);
    TEST_ASSERT_EQUAL(IUF_TRUE, equal);

    status = iufInputFileClose(obj);
    TEST_ASSERT(status == IUF_E_OK);
    iufInputFileDelete(obj);
    iufPulseDictDeepDelete(pulseDict);
}

TEST(IufInputFile, iufInputFileSetGetSourceDict)
{
    iuif_t obj = iufInputFileCreate(pFilename);
    iusd_t sourceDict = dgGenerateSourceDict();

    int status = iufInputFileSetSourceDict(obj, sourceDict);
    TEST_ASSERT(status == IUF_E_OK);
    iusd_t gotMeASourceDict = iufInputFileGetSourceDict(obj);
    TEST_ASSERT_NOT_EQUAL(IUFD_INVALID, gotMeASourceDict);

    IUF_BOOL equal = iufSourceDictCompare(sourceDict,gotMeASourceDict);
    TEST_ASSERT_EQUAL(IUF_TRUE, equal);

    status = iufInputFileClose(obj);
    TEST_ASSERT(status == IUF_E_OK);
    iufSourceDictDeepDelete(sourceDict);
    iufInputFileDelete(obj);
}


TEST(IufInputFile, iufInputFileSetGetReceiveChannelMapDict)
{
    iuif_t obj = iufInputFileCreate(pFilename);
    iurcmd_t receiveChannelMapDict = iufReceiveChannelMapDictCreate();

    int status = iufInputFileSetReceiveChannelMapDict(obj, receiveChannelMapDict);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);

    iurcmd_t gotMeAReceiveChannelMapDict = iufInputFileGetReceiveChannelMapDict(obj);
    TEST_ASSERT_NOT_EQUAL(NULL, gotMeAReceiveChannelMapDict);

    IUF_BOOL equal = iufReceiveChannelMapDictCompare(receiveChannelMapDict, gotMeAReceiveChannelMapDict);
    TEST_ASSERT_EQUAL(IUF_TRUE, equal);

    status = iufInputFileClose(obj);
    TEST_ASSERT(status == IUF_E_OK);
    iufInputFileDelete(obj);
    iufReceiveChannelMapDictDelete(receiveChannelMapDict);
}

TEST(IufInputFile, iufInputFileSetGetTransmitApodizationDict)
{
    IUF_BOOL equal;
    int status;
	iutad_t transmitApodizationDict = iufTransmitApodizationDictCreate();
    iuif_t obj = iufInputFileCreate(pFilename);
    TEST_ASSERT(obj != IUIF_INVALID);

	// transmitApodizationDict param
	status = iufInputFileSetTransmitApodizationDict(obj, transmitApodizationDict);
	TEST_ASSERT_EQUAL(IUF_E_OK, status);

	iutad_t gotMeATransmitApodizationDict = iufInputFileGetTransmitApodizationDict(obj);
	TEST_ASSERT_NOT_EQUAL(NULL, gotMeATransmitApodizationDict);

	equal = iufTransmitApodizationDictCompare(transmitApodizationDict, gotMeATransmitApodizationDict);
	TEST_ASSERT_EQUAL(IUF_TRUE, equal);

    // invalid param
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    status = iufInputFileSetTransmitApodizationDict(obj, NULL);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, status);

    status = iufInputFileSetTransmitApodizationDict(NULL, transmitApodizationDict);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, status);

    gotMeATransmitApodizationDict = iufInputFileGetTransmitApodizationDict(NULL);
    TEST_ASSERT_EQUAL(IUTAD_INVALID, gotMeATransmitApodizationDict);

    TEST_ASSERT_EQUAL(3, iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

	status = iufInputFileClose(obj);
	TEST_ASSERT(status == IUF_E_OK);
    iufInputFileDelete(obj);
    iufTransmitApodizationDictDelete(transmitApodizationDict);
}

TEST(IufInputFile, iufInputFileSetGetReceiveSettingsDict)
{
    IUF_BOOL equal;
    int status;
    iursd_t receiveSettingsDict = dgGenerateReceiveSettingsDict("bmode", 0);
    iuif_t obj = iufInputFileCreate(pFilename);
    TEST_ASSERT(obj != IUIF_INVALID);

    status = iufInputFileSetReceiveSettingsDict(obj, receiveSettingsDict);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);

    iursd_t gotMeATransmitReceiveSettingsDict = iufInputFileGetReceiveSettingsDict(obj);
    TEST_ASSERT_NOT_EQUAL(NULL, gotMeATransmitReceiveSettingsDict);

    equal = iufReceiveSettingsDictCompare(receiveSettingsDict, gotMeATransmitReceiveSettingsDict);
    TEST_ASSERT_EQUAL(IUF_TRUE, equal);

    // invalid param
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    status = iufInputFileSetReceiveSettingsDict(obj, NULL);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, status);

    status = iufInputFileSetReceiveSettingsDict(NULL, receiveSettingsDict);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, status);

    gotMeATransmitReceiveSettingsDict = iufInputFileGetReceiveSettingsDict(NULL);
    TEST_ASSERT_EQUAL(IURSD_INVALID, gotMeATransmitReceiveSettingsDict);

    TEST_ASSERT_EQUAL(3, iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));


    status = iufInputFileClose(obj);
    TEST_ASSERT(status == IUF_E_OK);
    iufInputFileDelete(obj);
    iufReceiveSettingsDictDeepDelete(receiveSettingsDict);
}

TEST(IufInputFile, iufInputFileSetGetAcquisition)
{
    IUF_BOOL equal;
    int status;
    iua_t acquisition = dgGenerateAcquisition();
    iuif_t obj = iufInputFileCreate(pFilename);
    TEST_ASSERT(obj != IUIF_INVALID);

    status = iufInputFileSetAcquisition(obj, acquisition);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);

    iua_t gotMeAnAcquisition = iufInputFileGetAcquisition(obj);
    TEST_ASSERT_NOT_EQUAL(NULL, gotMeAnAcquisition);

    equal = iufAcquisitionCompare(acquisition, gotMeAnAcquisition);
    TEST_ASSERT_EQUAL(IUF_TRUE, equal);

    // invalid param
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    status = iufInputFileSetAcquisition(obj, NULL);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, status);

    status = iufInputFileSetAcquisition(NULL, acquisition);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, status);

    gotMeAnAcquisition = iufInputFileGetAcquisition(NULL);
    TEST_ASSERT_EQUAL(IURSD_INVALID, gotMeAnAcquisition);

    TEST_ASSERT_EQUAL(3, iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));


    status = iufInputFileClose(obj);
    TEST_ASSERT(status == IUF_E_OK);
    iufInputFileDelete(obj);
    iufAcquisitionDelete(acquisition);
}

TEST(IufInputFile, iufInputFileSetGetTransducer)
{
    IUF_BOOL equal;
    int status;
    iut_t transducer = dgGenerateTransducer("S5-1");
    iuif_t obj = iufInputFileCreate(pFilename);
    TEST_ASSERT(obj != IUIF_INVALID);

    status = iufInputFileSetTransducer(obj, transducer);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);

    iut_t gotMeATransducer = iufInputFileGetTransducer(obj);
    TEST_ASSERT_NOT_EQUAL(NULL, gotMeATransducer);

    equal = iufTransducerCompare(transducer, gotMeATransducer);
    TEST_ASSERT_EQUAL(IUF_TRUE, equal);

    // invalid param
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    status = iufInputFileSetTransducer(obj, NULL);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, status);

    status = iufInputFileSetTransducer(NULL, transducer);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, status);

    gotMeATransducer = iufInputFileGetTransducer(NULL);
    TEST_ASSERT_EQUAL(IURSD_INVALID, gotMeATransducer);

    TEST_ASSERT_EQUAL(3, iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

    status = iufInputFileClose(obj);
    TEST_ASSERT(status == IUF_E_OK);
    iufInputFileDelete(obj);
    iufTransducerDeepDelete(transducer);
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
    iud_t frame = iufInputFileFrameCreate(inputFile, label);
    iuo_t offset = iufOffsetCreate();

    for (i=0; i<numFrames; i++)
    {
        dgFillData(frame, 1 + i * 1.0f);
        offset->t = i;
        status |= iufInputFileFrameSave(inputFile, label, frame, offset);
    }

    iufOffsetDelete(offset);
    iufDataDelete(frame);
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
    iud_t response = iufInputFileResponseCreate(inputFile, label);
    iuo_t offset = iufOffsetCreate();
    int numResponses = iufInputFileGetNumResponses(inputFile, label);

    for (i=0; i<numFrames; i++)
    {
        dgFillData(response, 1 + i * 1.0f);
        // Save Frame in responses
        for (j=0; j<numResponses; j++)
        {
            offset->y = j;
            offset->t = i;
            status |= iufInputFileResponseSave(inputFile, label, response, offset);
        }
    }

    iufDataDelete(response);
    iufOffsetDelete(offset);
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
    iud_t channel = iufInputFileChannelCreate(inputFile, label);
    iuo_t offset = iufOffsetCreate();
    int numResponses = iufInputFileGetNumResponses(inputFile, label);
    int numChannels = iufInputFileGetNumChannels(inputFile, label);

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
                status |= iufInputFileChannelSave(inputFile, label, channel, offset);
            }
        }
    }

    iufDataDelete(channel);
    iufOffsetDelete(offset);
    return status;
}


static IUF_BOOL validateFrames
(
    iuif_t inputFile,
    char *label,
    int numFrames
)
{
    int status=0;
    int i;
    IUF_BOOL equal = IUF_FALSE;

    iud_t referenceFrame = iufInputFileFrameCreate(inputFile, label);
    iud_t actualFrame = iufInputFileFrameCreate(inputFile, label);
    iuo_t offset = iufOffsetCreate();
    for (i=0; i<numFrames; i++)
    {
        dgFillData(referenceFrame, 1 + i * 1.0f);
        offset->t = i;
        status |= iufInputFileFrameLoad(inputFile, label, actualFrame, offset);
        equal = iufDataCompare(referenceFrame,actualFrame);
        if (equal == IUF_FALSE) break;
    }

    iufDataDelete(referenceFrame);
    iufDataDelete(actualFrame);
    iufOffsetDelete(offset);
    return equal;
}


static IUF_BOOL validateResponses
(
    iuif_t inputFile,
    char *label,
    int numFrames
)
{
    int status=0;
    int i,j;
    IUF_BOOL equal = IUF_FALSE;

    iud_t referenceResponse = iufInputFileResponseCreate(inputFile, label);
    iud_t actualResponse = iufInputFileResponseCreate(inputFile, label);
    iuo_t offset = iufOffsetCreate();
    int numResponses = iufInputFileGetNumResponses(inputFile, label);

    for (i=0; i<numFrames; i++)
    {
        dgFillData(referenceResponse, 1 + i * 1.0f);
        for (j=0; j<numResponses; j++)
        {
            offset->y = j;
            offset->t = i;
            status |= iufInputFileResponseLoad(inputFile, label, actualResponse, offset);
            equal = iufDataCompare(referenceResponse,actualResponse);
            if (equal == IUF_FALSE) break;
        }
        if (equal == IUF_FALSE) break;
    }



    iufDataDelete(referenceResponse);
    iufDataDelete(actualResponse);
    iufOffsetDelete(offset);
    return equal;
}

#if 0
static IUF_BOOL validateChannels
(
    iuif_t inputFile,
    char *label,
    int numFrames
)
{
    int status=0;
    int i,j,k;
    IUF_BOOL equal = IUF_FALSE;

    iud_t referenceChannel = iufInputFileChannelCreate(inputFile, label);
    iud_t actualChannel = iufInputFileChannelCreate(inputFile, label);
    iuo_t offset = iufOffsetCreate();
    int numResponses = iufInputFileGetNumResponses(inputFile, label);
    int numChannels = iufInputFileGetNumChannels(inputFile, label);

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
                status |= iufInputFileChannelLoad(inputFile, label, actualChannel, offset);
                equal = iufDataCompare(referenceChannel, actualChannel);
                if (equal == IUF_FALSE) break;
            }
            if (equal == IUF_FALSE) break;
        }
        if (equal == IUF_FALSE) break;
    }



    iufDataDelete(referenceChannel);
    iufDataDelete(actualChannel);
    iufOffsetDelete(offset);
    return equal;
}
#endif

TEST(IufInputFile, testIufInputFileDataIOSaveFrame)
{
    char *ptestFileName = "testIufInputFileDataIOSaveFrame.hdf5";
    char *pDopplerLabel = "doppler";
    int numFrames = 10;
    int numSamplesPerLine = 10;
    int numChannels = 8;
    char *pBmodeLabel = "bmode";
    int status = 0;

    // create
    iuif_t inputFile = dgGenerateInputFile(ptestFileName, "S5-1", pDopplerLabel, numFrames, numSamplesPerLine, numChannels);
    status = dgInputFileAddGeneratedData(inputFile, pBmodeLabel, numSamplesPerLine, numChannels);
    TEST_ASSERT_EQUAL(status, IUF_E_OK);

    // SaveFrames
    status = iufInputFileNodeSave(inputFile);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);
    status = saveFrames(inputFile,pDopplerLabel,numFrames);
    status |= saveFrames(inputFile,pBmodeLabel,numFrames);
    TEST_ASSERT_EQUAL(status, IUF_E_OK);
    status = iufInputFileClose(inputFile);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    // read back
    iuif_t savedObj = iufInputFileNodeLoad(ptestFileName);
    TEST_ASSERT(savedObj != NULL);
    TEST_ASSERT_EQUAL(IUF_TRUE, iufInputFileCompare(inputFile,savedObj));
    TEST_ASSERT_EQUAL(IUF_TRUE, validateFrames(savedObj,pDopplerLabel,numFrames));
    TEST_ASSERT_EQUAL(IUF_TRUE, validateFrames(savedObj,pBmodeLabel,numFrames));


    // create channel
    iufInputFileClose(savedObj);
    iufInputFileDelete(savedObj);
    dgDeleteInputFile(inputFile);
}

TEST(IufInputFile, testIufInputFileDataIOSaveResponse)
{
    char *ptestFileName = "testIufInputFileDataIOSaveResponse.hdf5";
    char *pDopplerLabel = "doppler";
    int numFrames = 10;
    int numSamplesPerLine = 10;
    int numChannels = 8;
    char *pBmodeLabel = "bmode";
    int status = 0;

    // create
    iuif_t inputFile = dgGenerateInputFile(ptestFileName, "S5-1", pDopplerLabel, numFrames, numSamplesPerLine, numChannels);
    status = dgInputFileAddGeneratedData(inputFile, pBmodeLabel, numSamplesPerLine, numChannels);
    TEST_ASSERT_EQUAL(status, IUF_E_OK);


    // SaveFrames
    status = iufInputFileNodeSave(inputFile);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);
    status = saveResponses(inputFile,pDopplerLabel,numFrames);
    status |= saveResponses(inputFile,pBmodeLabel,numFrames);
    TEST_ASSERT_EQUAL(status, IUF_E_OK);
    status = iufInputFileClose(inputFile);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    // read back
    iuif_t savedObj = iufInputFileNodeLoad(ptestFileName);
    TEST_ASSERT(savedObj != NULL);
    TEST_ASSERT_EQUAL(IUF_TRUE, iufInputFileCompare(inputFile,savedObj));
    TEST_ASSERT_EQUAL(IUF_TRUE, validateResponses(savedObj,pDopplerLabel,numFrames));
    TEST_ASSERT_EQUAL(IUF_TRUE, validateResponses(savedObj,pBmodeLabel,numFrames));


    // create channel
    iufInputFileClose(savedObj);
    iufInputFileDelete(savedObj);
    dgDeleteInputFile(inputFile);
}

TEST(IufInputFile, testIufInputFileDataIOSaveChannel)
{
    char *ptestFileName = "testIufInputFileDataIOSaveChannel.hdf5";
    char *pDopplerLabel = "doppler";
    int numFrames = 10;
    int numSamplesPerLine = 10;
    int numChannels = 8;
    char *pBmodeLabel = "bmode";
    int status = 0;

    // create
    iuif_t inputFile = dgGenerateInputFile(ptestFileName, "S5-1", pDopplerLabel, numFrames, numSamplesPerLine, numChannels);
    status = dgInputFileAddGeneratedData(inputFile, pBmodeLabel, numSamplesPerLine, numChannels);
    TEST_ASSERT_EQUAL(status, IUF_E_OK);


    // SaveFrames
    status = iufInputFileNodeSave(inputFile);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);
    status = saveChannels(inputFile, pDopplerLabel, numFrames);
//    status |= saveChannels(inputFile, pBmodeLabel, numFrames);
    TEST_ASSERT_EQUAL(status, IUF_E_OK);
    status = iufInputFileClose(inputFile);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    // read back
    iuif_t savedObj = iufInputFileNodeLoad(ptestFileName);
    TEST_ASSERT(savedObj != NULL);
    TEST_ASSERT_EQUAL(IUF_TRUE, iufInputFileCompare(inputFile,savedObj));
    // Todo:
    // Fix Errors in validateChannels:
//    TEST_ASSERT_EQUAL(IUF_TRUE, validateChannels(savedObj,pDopplerLabel,numFrames));
//    TEST_ASSERT_EQUAL(IUF_TRUE, validateChannels(savedObj,pBmodeLabel,numFrames));


    // create channel
    iufInputFileClose(savedObj);
    iufInputFileDelete(savedObj);
    dgDeleteInputFile(inputFile);
}


TEST(IufInputFile, testIufInputFileSerialization)
{
    int numFrames = 10;
    int numSamplesPerLine = 10;
    int numChannels = 8;
    char *ptestFileName = "testIufInputFileSerialization.hdf5";

    // create
    iuif_t inputFile = dgGenerateInputFile(ptestFileName, "S5-1", "bmode", numFrames, numSamplesPerLine, numChannels);
    TEST_ASSERT(inputFile != IUIF_INVALID);

    // save
    int status = iufInputFileNodeSave(inputFile);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);
    status = iufInputFileClose(inputFile);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    // read back
    iuif_t savedObj = iufInputFileNodeLoad(ptestFileName);
    TEST_ASSERT(savedObj != NULL);
    TEST_ASSERT_EQUAL(IUF_TRUE, iufInputFileCompare(inputFile,savedObj));

    status = iufInputFileClose(savedObj);
    TEST_ASSERT(status == IUF_E_OK);

    dgDeleteInputFile(inputFile);
    iufInputFileDelete(savedObj);

}

TEST(IufInputFile, testIufInputFileVeraSonics)
{
    int numFrames = 3;
    int numSamplesPerLine = 2048;
    int numChannels = 128;
    char *ptestFileName = "testIufInputFileVerasonics.hdf5";

    // create
    iuif_t inputFile = dgGenerateInputFileVerasonics(ptestFileName, "S5-1", "bmode", numFrames, numSamplesPerLine, numChannels);
    TEST_ASSERT(inputFile != IUIF_INVALID);

    // save
    int status = iufInputFileNodeSave(inputFile);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);
    status = iufInputFileClose(inputFile);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    // read back
    iuif_t savedObj = iufInputFileNodeLoad(ptestFileName);
    TEST_ASSERT(savedObj != NULL);
    TEST_ASSERT_EQUAL(IUF_TRUE, iufInputFileCompare(inputFile,savedObj));

    status = iufInputFileClose(savedObj);
    TEST_ASSERT(status == IUF_E_OK);

    dgDeleteInputFile(inputFile);
    iufInputFileDelete(savedObj);
}


TEST_GROUP_RUNNER(IufInputFile)
{
    RUN_TEST_CASE(IufInputFile, testIufInputFileCreate);
    RUN_TEST_CASE(IufInputFile, testIufInputFileDelete);
    RUN_TEST_CASE(IufInputFile, testIufInputFileCompare);
    RUN_TEST_CASE(IufInputFile, iufInputFileSetGetFrameList);
    RUN_TEST_CASE(IufInputFile, iufInputFileSetGetPatternListDict);
    RUN_TEST_CASE(IufInputFile, iufInputFileSetGetPulseDict);
    RUN_TEST_CASE(IufInputFile, iufInputFileSetGetSourceDict);
    RUN_TEST_CASE(IufInputFile, iufInputFileSetGetReceiveChannelMapDict);
    RUN_TEST_CASE(IufInputFile, iufInputFileSetGetTransmitApodizationDict);
    RUN_TEST_CASE(IufInputFile, iufInputFileSetGetReceiveSettingsDict);
    RUN_TEST_CASE(IufInputFile, iufInputFileSetGetAcquisition);
    RUN_TEST_CASE(IufInputFile, iufInputFileSetGetTransducer);
    RUN_TEST_CASE(IufInputFile, testIufInputFileSerialization);
    RUN_TEST_CASE(IufInputFile, testIufInputFileDataIOSaveFrame);
    RUN_TEST_CASE(IufInputFile, testIufInputFileDataIOSaveResponse);
    RUN_TEST_CASE(IufInputFile, testIufInputFileDataIOSaveChannel);
}
