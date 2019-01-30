

//
// Created by nlv09165 on 14/01/2019.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <ius.h>
#include <testDataGenerators.h>

static const char *pFilename = "IusIqFile.hdf5";
static const char *pNotherFilename = "AnotherIusIqFile.hdf5";

static char *pErrorFilename = "IusIqFile.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(IusIqFile);

TEST_SETUP(IusIqFile)
{
    iusErrorLogClear();
    iusErrorLog(IUS_TRUE);
    iusErrorAutoReport(IUS_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iusErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IusIqFile)
{
    iusErrorLogClear();
    iusErrorLog(IUS_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iusErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
}

TEST(IusIqFile, testIusIqFileCreate)
{
    const char *pEmptyFilename = "";
    const char *pSpecialCharsFilename = "*&/";

    iuif_t ifh = iusIqFileCreate(pFilename);
    TEST_ASSERT(ifh != IUIF_INVALID);

    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());

    // Create file that is already open should result in error
    iuif_t ifh2 = iusIqFileCreate(pFilename);
    TEST_ASSERT(ifh2 == IUIF_INVALID);

    int status = iusIqFileClose(ifh);
    TEST_ASSERT(status == IUS_E_OK);

    // Closing file that has already been closed results in error
    status = iusIqFileClose(ifh);
    iusIqFileDelete(ifh);
    TEST_ASSERT(status != IUS_E_OK);

    // Invalid argument should result in error.
    ifh = iusIqFileCreate(pEmptyFilename);
    TEST_ASSERT(ifh == IUIF_INVALID);
    ifh = iusIqFileCreate(pSpecialCharsFilename);
    TEST_ASSERT(ifh == IUIF_INVALID);

    TEST_ASSERT_EQUAL(3,iusErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
}

TEST(IusIqFile, testIusIqFileDelete)
{
    iuif_t obj = iusIqFileCreate(pFilename);
    TEST_ASSERT(obj != IUIF_INVALID);

	int status = iusIqFileClose(obj);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);
	
	status = iusIqFileDelete(obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // invalid params
    status = iusIqFileDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
}


TEST(IusIqFile, testIusIqFileCompare)
{
    IUS_BOOL equal;
    iuif_t obj = iusIqFileCreate(pFilename);
    iuif_t notherObj = iusIqFileCreate(pNotherFilename);
    TEST_ASSERT(obj != IUIF_INVALID);
    TEST_ASSERT(notherObj != IUIF_INVALID);
    equal = iusIqFileCompare(obj,obj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = iusIqFileCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    
    // invalid params
    equal = iusIqFileCompare(obj,NULL);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    equal = iusIqFileCompare(NULL,obj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

	int status = iusIqFileClose(obj);
	TEST_ASSERT(status == IUS_E_OK);
	status = iusIqFileClose(notherObj);
	TEST_ASSERT(status == IUS_E_OK);
	iusIqFileDelete(obj);
    iusIqFileDelete(notherObj);
}

TEST(IusIqFile, iusIqFileSetGetFrameList)
{
    IUS_BOOL equal;
    int status;
    iufl_t frameList = dgGenerateFrameList(10);
    iuiqf_t obj = iusIqFileCreate(pFilename);

    status = iusIqFileSetFrameList(obj,frameList);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    iufl_t gotMeAFrameList = iusIqFileGetFrameList(obj);
    TEST_ASSERT_NOT_EQUAL(IUFL_INVALID, gotMeAFrameList);

    equal = iusFrameListCompare(frameList,gotMeAFrameList);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);

    status = iusIqFileClose(obj);
    TEST_ASSERT(status == IUS_E_OK);
    iusFrameListDeepDelete(frameList);
    iusIqFileDelete(obj);
}

TEST(IusIqFile, iusIqFileSetGetPatternListDict)
{
    IUS_BOOL equal;
    int status;
    iuiqpald_t iqPatternListDict = dgGenerateIqPatternListDict("mylabel",NULL,NULL);
    iuif_t obj = iusIqFileCreate(pFilename);

    status = iusIqFileSetPatternListDict(obj, iqPatternListDict);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    iuiqpald_t gotMeAPatternListDict = iusIqFileGetPatternListDict(obj);
    TEST_ASSERT_NOT_EQUAL(IUIQPALD_INVALID, gotMeAPatternListDict);

    equal = iusIqPatternListDictCompare(iqPatternListDict,gotMeAPatternListDict);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);

    status = iusIqFileClose(obj);
    TEST_ASSERT(status == IUS_E_OK);
    iusIqFileDelete(obj);
    iusIqPatternListDictDeepDelete(iqPatternListDict);
}


TEST(IusIqFile, iusIqFileSetGetPulseDict)
{
    iuif_t obj = iusIqFileCreate(pFilename);
    iupd_t pulseDict = dgGeneratePulseDict();

    int status = iusIqFileSetPulseDict(obj, pulseDict);
    TEST_ASSERT(status == IUS_E_OK);
    iupd_t gotMeAPulseDict = iusIqFileGetPulseDict(obj);
    TEST_ASSERT_NOT_EQUAL(NULL, gotMeAPulseDict);

    IUS_BOOL equal = iusPulseDictCompare(pulseDict,gotMeAPulseDict);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);

    status = iusIqFileClose(obj);
    TEST_ASSERT(status == IUS_E_OK);
    iusIqFileDelete(obj);
    iusPulseDictDeepDelete(pulseDict);
}

TEST(IusIqFile, iusIqFileSetGetSourceDict)
{
    iuif_t obj = iusIqFileCreate(pFilename);
    iusd_t sourceDict = dgGenerateSourceDict();

    int status = iusIqFileSetSourceDict(obj, sourceDict);
    TEST_ASSERT(status == IUS_E_OK);
    iusd_t gotMeASourceDict = iusIqFileGetSourceDict(obj);
    TEST_ASSERT_NOT_EQUAL(IUSD_INVALID, gotMeASourceDict);

    IUS_BOOL equal = iusSourceDictCompare(sourceDict,gotMeASourceDict);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);

    status = iusIqFileClose(obj);
    TEST_ASSERT(status == IUS_E_OK);
    iusSourceDictDeepDelete(sourceDict);
    iusIqFileDelete(obj);
}


TEST(IusIqFile, iusIqFileSetGetReceiveChannelMapDict)
{
    iuif_t obj = iusIqFileCreate(pFilename);
    iurcmd_t receiveChannelMapDict = iusReceiveChannelMapDictCreate();

    int status = iusIqFileSetReceiveChannelMapDict(obj, receiveChannelMapDict);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);

    iurcmd_t gotMeAReceiveChannelMapDict = iusIqFileGetReceiveChannelMapDict(obj);
    TEST_ASSERT_NOT_EQUAL(NULL, gotMeAReceiveChannelMapDict);

    IUS_BOOL equal = iusReceiveChannelMapDictCompare(receiveChannelMapDict, gotMeAReceiveChannelMapDict);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);

    status = iusIqFileClose(obj);
    TEST_ASSERT(status == IUS_E_OK);
    iusIqFileDelete(obj);
    iusReceiveChannelMapDictDelete(receiveChannelMapDict);
}

TEST(IusIqFile, iusIqFileSetGetTransmitApodizationDict)
{
    IUS_BOOL equal;
    int status;
	iutad_t transmitApodizationDict = iusTransmitApodizationDictCreate();
    iuif_t obj = iusIqFileCreate(pFilename);
    TEST_ASSERT(obj != IUIF_INVALID);

	// transmitApodizationDict param
	status = iusIqFileSetTransmitApodizationDict(obj, transmitApodizationDict);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);

	iutad_t gotMeATransmitApodizationDict = iusIqFileGetTransmitApodizationDict(obj);
	TEST_ASSERT_NOT_EQUAL(NULL, gotMeATransmitApodizationDict);

	equal = iusTransmitApodizationDictCompare(transmitApodizationDict, gotMeATransmitApodizationDict);
	TEST_ASSERT_EQUAL(IUS_TRUE, equal);

    // invalid param
    status = iusIqFileSetTransmitApodizationDict(obj, NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);

    status = iusIqFileSetTransmitApodizationDict(NULL, transmitApodizationDict);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);

    gotMeATransmitApodizationDict = iusIqFileGetTransmitApodizationDict(NULL);
    TEST_ASSERT_EQUAL(IUTAD_INVALID, gotMeATransmitApodizationDict);

	status = iusIqFileClose(obj);
	TEST_ASSERT(status == IUS_E_OK);
    iusIqFileDelete(obj);
    iusTransmitApodizationDictDelete(transmitApodizationDict);
}

//receiveseting -> demodulation
TEST(IusIqFile, iusIqFileSetGetDemodulationDict)
{
    IUS_BOOL equal;
    int status;
    iudmd_t demodulationDict = dgGenerateDemodulationDict("bmode", 0);
    iuif_t obj = iusIqFileCreate(pFilename);
    TEST_ASSERT(obj != IUIF_INVALID);

    status = iusIqFileSetDemodulationDict(obj, demodulationDict);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);

    iudmd_t gotMeADemodulationDict = iusIqFileGetDemodulationDict(obj);
    TEST_ASSERT_NOT_EQUAL(NULL, gotMeADemodulationDict);

    equal = iusDemodulationDictCompare(demodulationDict, gotMeADemodulationDict);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);

    // invalid param
    status = iusIqFileSetDemodulationDict(obj, NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);

    status = iusIqFileSetDemodulationDict(NULL, demodulationDict);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);

    gotMeADemodulationDict = iusIqFileGetDemodulationDict(NULL);
    TEST_ASSERT_EQUAL(IUDMD_INVALID, gotMeADemodulationDict);

    status = iusIqFileClose(obj);
    TEST_ASSERT(status == IUS_E_OK);
    iusIqFileDelete(obj);
    iusDemodulationDictDeepDelete(demodulationDict);
}


TEST(IusIqFile, iusIqFileSetGetAcquisition)
{
    IUS_BOOL equal;
    int status;
    iua_t acquisition = dgGenerateAcquisition();
    iuif_t obj = iusIqFileCreate(pFilename);
    TEST_ASSERT(obj != IUIF_INVALID);

    status = iusIqFileSetAcquisition(obj, acquisition);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);

    iua_t gotMeAnAcquisition = iusIqFileGetAcquisition(obj);
    TEST_ASSERT_NOT_EQUAL(NULL, gotMeAnAcquisition);

    equal = iusAcquisitionCompare(acquisition, gotMeAnAcquisition);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);

    // invalid param
    status = iusIqFileSetAcquisition(obj, NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);

    status = iusIqFileSetAcquisition(NULL, acquisition);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);

    gotMeAnAcquisition = iusIqFileGetAcquisition(NULL);
    TEST_ASSERT_EQUAL(IURSD_INVALID, gotMeAnAcquisition);

    status = iusIqFileClose(obj);
    TEST_ASSERT(status == IUS_E_OK);
    iusIqFileDelete(obj);
    iusAcquisitionDelete(acquisition);
}

TEST(IusIqFile, iusIqFileSetGetTransducer)
{
    IUS_BOOL equal;
    int status;
    iut_t transducer = dgGenerateTransducer("S5-1");
    iuif_t obj = iusIqFileCreate(pFilename);
    TEST_ASSERT(obj != IUIF_INVALID);

    status = iusIqFileSetTransducer(obj, transducer);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);

    iut_t gotMeATransducer = iusIqFileGetTransducer(obj);
    TEST_ASSERT_NOT_EQUAL(NULL, gotMeATransducer);

    equal = iusTransducerCompare(transducer, gotMeATransducer);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);

    // invalid param
    status = iusIqFileSetTransducer(obj, NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);

    status = iusIqFileSetTransducer(NULL, transducer);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);

    gotMeATransducer = iusIqFileGetTransducer(NULL);
    TEST_ASSERT_EQUAL(IURSD_INVALID, gotMeATransducer);

    status = iusIqFileClose(obj);
    TEST_ASSERT(status == IUS_E_OK);
    iusIqFileDelete(obj);
    iusTransducerDeepDelete(transducer);
}

TEST(IusIqFile, testIusIqFileSerialization)
{
    int numFrames = 10;
    int numSamplesPerLine = 10;
    int numChannels = 8;
    char *ptestFileName = "testIusIqFileSerialization.hdf5";

    // create
    iuiqf_t iqFile = dgGenerateIqFile(ptestFileName, "S5-1", "bmode", numFrames, numSamplesPerLine, numChannels);
    TEST_ASSERT(iqFile != IUIQF_INVALID);

	// save
    int status = iusIqFileNodeSave(iqFile);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    status = iusIqFileClose(iqFile);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // read back
    iuiqf_t savedObj = iusIqFileNodeLoad(ptestFileName);
    TEST_ASSERT(savedObj != NULL);
    TEST_ASSERT_EQUAL(IUS_TRUE, iusIqFileCompare(iqFile,savedObj));

	status = iusIqFileClose(savedObj);
	TEST_ASSERT(status == IUS_E_OK);

	dgDeleteIqFile(iqFile);
    iusIqFileDelete(savedObj);
}

static int saveFrames
(
    iuiqf_t iqFile,
    char *label,
    int numFrames
)
{
    int status=0;
    int i;
	iud_t iFrame;
	iud_t qFrame;

	if (!iusIqFileFrameCreate(iqFile, label, &iFrame, &qFrame)) return -1;
    iuo_t offset = iusOffsetCreate();

    for (i=0; i<numFrames; i++)
    {
        dgFillData(iFrame,  1 + i * 1.0f);
		dgFillData(qFrame, -1 - i * 1.0f);
        offset->t = i;
        status |= iusIqFileFrameSave(iqFile, label, iFrame, qFrame, offset);
    }

    iusOffsetDelete(offset);
    iusDataDelete(iFrame);
	iusDataDelete(qFrame);
    return status;
}

static int saveResponses
(
    iuiqf_t iqFile,
    char *label,
    int numFrames
)
{
    int status=0;
    int i,j;
	iud_t iResponse;
	iud_t qResponse;

	if (!iusIqFileResponseCreate(iqFile, label, &iResponse, &qResponse)) return -1;
    iuo_t offset = iusOffsetCreate();
    int numResponses = iusIqFileGetNumResponses(iqFile, label);

    for (i=0; i<numFrames; i++)
    {
        dgFillData(iResponse, 1 + i * 1.0f);
		dgFillData(qResponse, -1 - i * 1.0f);
        // Save Frame in responses
        for (j=0; j<numResponses; j++)
        {
            offset->y = j;
            offset->t = i;
            status |= iusIqFileResponseSave(iqFile, label, iResponse, qResponse, offset);
        }
    }

    iusDataDelete(iResponse);
	iusDataDelete(qResponse);
    iusOffsetDelete(offset);
    return status;
}

static int saveChannels
(
    iuif_t iqFile,
    char *label,
    int numFrames
)
{
    int status=0;
    int i,j,k;
	iud_t iChannel;
	iud_t qChannel;
	
	if (!iusIqFileChannelCreate(iqFile, label, &iChannel, &qChannel)) return -1;
    iuo_t offset = iusOffsetCreate();
    int numResponses = iusIqFileGetNumResponses(iqFile, label);
    int numChannels = iusIqFileGetNumChannels(iqFile, label);

    for (i=0; i<numFrames; i++)
    {
        dgFillData(iChannel, 1 + i * 1.0f);
		dgFillData(qChannel, -1 - i * 1.0f);
        // Save Frame in responses
        for (j=0; j<numResponses; j++)
        {
            for (k=0; k<numChannels; k++)
            {
                offset->z = k;
                offset->y = j;
                offset->t = i;
                status |= iusIqFileChannelSave(iqFile, label, iChannel, qChannel, offset);
            }
        }
    }

    iusDataDelete(iChannel);
	iusDataDelete(qChannel);
    iusOffsetDelete(offset);
    return status;
}


static IUS_BOOL validateFrames
(
    iuif_t iqFile,
    char *label,
    int numFrames
)
{
    int status=0;
    int i;
    IUS_BOOL equal = IUS_FALSE;

	iud_t referenceIFrame;
	iud_t referenceQFrame;
	iud_t actualIFrame;
	iud_t actualQFrame;

	if (!iusIqFileFrameCreate(iqFile, label, &referenceIFrame, &referenceQFrame)) return IUS_FALSE;
	if (!iusIqFileFrameCreate(iqFile, label, &actualIFrame, &actualQFrame)) return IUS_FALSE;

	iuo_t offset = iusOffsetCreate();
    for (i=0; i<numFrames; i++)
    {
        dgFillData(referenceIFrame, 1 + i * 1.0f);
		dgFillData(referenceQFrame, -1 - i * 1.0f);
        offset->t = i;
        status |= iusIqFileFrameLoad(iqFile, label, actualIFrame, actualQFrame, offset);
        equal = iusDataCompare(referenceIFrame, actualIFrame);
		equal &= iusDataCompare(referenceQFrame, actualQFrame);
        if (equal == IUS_FALSE) break;
    }

    iusDataDelete(referenceIFrame);
	iusDataDelete(referenceQFrame);
    iusDataDelete(actualIFrame);
	iusDataDelete(actualQFrame);
    iusOffsetDelete(offset);
    return equal;
}


static IUS_BOOL validateResponses
(
    iuif_t iqFile,
    char *label,
    int numFrames
)
{
    int status=0;
    int i,j;
    IUS_BOOL equal = IUS_FALSE;

	iud_t referenceIResponse;
	iud_t referenceQResponse;
	if (!iusIqFileResponseCreate(iqFile, label, &referenceIResponse, &referenceQResponse)) return -1;
	iud_t actualIResponse;
	iud_t actualQResponse;
	if (!iusIqFileResponseCreate(iqFile, label, &actualIResponse, &actualQResponse)) return -1;

    iuo_t offset = iusOffsetCreate();
    int numResponses = iusIqFileGetNumResponses(iqFile, label);

    for (i=0; i<numFrames; i++)
    {
        dgFillData(referenceIResponse, 1 + i * 1.0f);
		dgFillData(referenceQResponse, -1 - i * 1.0f);
        for (j=0; j<numResponses; j++)
        {
            offset->y = j;
            offset->t = i;
            status |= iusIqFileResponseLoad(iqFile, label, actualIResponse, actualQResponse, offset);
            equal = iusDataCompare(referenceIResponse,actualIResponse);
			equal &= iusDataCompare(referenceQResponse, actualQResponse);
            if (equal == IUS_FALSE) break;
        }
        if (equal == IUS_FALSE) break;
    }

    iusDataDelete(referenceIResponse);
	iusDataDelete(referenceQResponse);
    iusDataDelete(actualIResponse);
	iusDataDelete(actualQResponse);
    iusOffsetDelete(offset);
    return equal;
}

#if 0
static IUS_BOOL validateChannels
(
    iuif_t iqFile,
    char *label,
    int numFrames
)
{
    int status=0;
    int i,j,k;
    IUS_BOOL equal = IUS_FALSE;

    iud_t referenceChannel = iusIqFileChannelCreate(iqFile, label);
    iud_t actualChannel = iusIqFileChannelCreate(iqFile, label);
    iuo_t offset = iusOffsetCreate();
    int numResponses = iusIqFileGetNumResponses(iqFile, label);
    int numChannels = iusIqFileGetNumChannels(iqFile, label);

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
                status |= iusIqFileChannelLoad(iqFile, label, actualChannel, offset);
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

TEST(IusIqFile, testIusIqFileDataIOSaveFrame)
{
    char *ptestFileName = "testIusIqFileDataIOSaveFrame.hdf5";
    char *pDopplerLabel = "doppler";
    int numFrames = 10;
    int numSamplesPerLine = 10;
    int numChannels = 8;
    char *pBmodeLabel = "bmode";
    int status = 0;

    // create
    iuiqf_t iqFile = dgGenerateIqFile(ptestFileName, "S5-1", pDopplerLabel, numFrames, numSamplesPerLine, numChannels);
    status = dgIqFileAddGeneratedData(iqFile, pBmodeLabel, numSamplesPerLine, numChannels);
    TEST_ASSERT_EQUAL(status, IUS_E_OK);

    // SaveFrames
    status = iusIqFileNodeSave(iqFile);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    status = saveFrames(iqFile,pDopplerLabel,numFrames);
    status |= saveFrames(iqFile,pBmodeLabel,numFrames);
    TEST_ASSERT_EQUAL(status, IUS_E_OK);
    status = iusIqFileClose(iqFile);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // read back
    iuif_t savedObj = iusIqFileNodeLoad(ptestFileName);
    TEST_ASSERT(savedObj != NULL);
    TEST_ASSERT_EQUAL(IUS_TRUE, iusIqFileCompare(iqFile,savedObj));
    TEST_ASSERT_EQUAL(IUS_TRUE, validateFrames(savedObj,pDopplerLabel,numFrames));
    TEST_ASSERT_EQUAL(IUS_TRUE, validateFrames(savedObj,pBmodeLabel,numFrames));


    // create channel
    iusIqFileClose(savedObj);
    iusIqFileDelete(savedObj);
    dgDeleteIqFile(iqFile);
}

TEST(IusIqFile, testIusIqFileDataIOSaveResponse)
{
    char *ptestFileName = "testIusIqFileDataIOSaveResponse.hdf5";
    char *pDopplerLabel = "doppler";
    int numFrames = 10;
    int numSamplesPerLine = 10;
    int numChannels = 8;
    char *pBmodeLabel = "bmode";
    int status = 0;

    // create
    iuif_t iqFile = dgGenerateIqFile(ptestFileName, "S5-1", pDopplerLabel, numFrames, numSamplesPerLine, numChannels);
    status = dgIqFileAddGeneratedData(iqFile, pBmodeLabel, numSamplesPerLine, numChannels);
    TEST_ASSERT_EQUAL(status, IUS_E_OK);


    // SaveFrames
    status = iusIqFileNodeSave(iqFile);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    status = saveResponses(iqFile,pDopplerLabel,numFrames);
    status |= saveResponses(iqFile,pBmodeLabel,numFrames);
    TEST_ASSERT_EQUAL(status, IUS_E_OK);
    status = iusIqFileClose(iqFile);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // read back
    iuif_t savedObj = iusIqFileNodeLoad(ptestFileName);
    TEST_ASSERT(savedObj != NULL);
    TEST_ASSERT_EQUAL(IUS_TRUE, iusIqFileCompare(iqFile,savedObj));
    TEST_ASSERT_EQUAL(IUS_TRUE, validateResponses(savedObj,pDopplerLabel,numFrames));
    TEST_ASSERT_EQUAL(IUS_TRUE, validateResponses(savedObj,pBmodeLabel,numFrames));


    // create channel
    iusIqFileClose(savedObj);
    iusIqFileDelete(savedObj);
    dgDeleteIqFile(iqFile);
}

TEST(IusIqFile, testIusIqFileDataIOSaveChannel)
{
    char *ptestFileName = "testIusIqFileDataIOSaveChannel.hdf5";
    char *pDopplerLabel = "doppler";
    int numFrames = 10;
    int numSamplesPerLine = 10;
    int numChannels = 8;
    char *pBmodeLabel = "bmode";
    int status = 0;

    // create
    iuiqf_t iqFile = dgGenerateIqFile(ptestFileName, "S5-1", pDopplerLabel, numFrames, numSamplesPerLine, numChannels);
    status = dgIqFileAddGeneratedData(iqFile, pBmodeLabel, numSamplesPerLine, numChannels);
    TEST_ASSERT_EQUAL(status, IUS_E_OK);


    // SaveFrames
    status = iusIqFileNodeSave(iqFile);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    status = saveChannels(iqFile, pDopplerLabel, numFrames);
//    status |= saveChannels(iqFile, pBmodeLabel, numFrames);
    TEST_ASSERT_EQUAL(status, IUS_E_OK);
    status = iusIqFileClose(iqFile);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // read back
    iuif_t savedObj = iusIqFileNodeLoad(ptestFileName);
    TEST_ASSERT(savedObj != NULL);
    TEST_ASSERT_EQUAL(IUS_TRUE, iusIqFileCompare(iqFile,savedObj));
    // Todo:
    // Fix Errors in validateChannels:
//    TEST_ASSERT_EQUAL(IUS_TRUE, validateChannels(savedObj,pDopplerLabel,numFrames));
//    TEST_ASSERT_EQUAL(IUS_TRUE, validateChannels(savedObj,pBmodeLabel,numFrames));


    // create channel
    iusIqFileClose(savedObj);
    iusIqFileDelete(savedObj);
    dgDeleteIqFile(iqFile);
}

TEST_GROUP_RUNNER(IusIqFile)
{
    RUN_TEST_CASE(IusIqFile, testIusIqFileCreate);
    RUN_TEST_CASE(IusIqFile, testIusIqFileDelete);
    RUN_TEST_CASE(IusIqFile, testIusIqFileCompare);
    RUN_TEST_CASE(IusIqFile, iusIqFileSetGetFrameList);
    RUN_TEST_CASE(IusIqFile, iusIqFileSetGetPatternListDict);
    RUN_TEST_CASE(IusIqFile, iusIqFileSetGetPulseDict);
    RUN_TEST_CASE(IusIqFile, iusIqFileSetGetSourceDict);
    RUN_TEST_CASE(IusIqFile, iusIqFileSetGetReceiveChannelMapDict);
    RUN_TEST_CASE(IusIqFile, iusIqFileSetGetTransmitApodizationDict);
    //RUN_TEST_CASE(IusIqFile, iusIqFileSetGetReceiveSettingsDict);
    RUN_TEST_CASE(IusIqFile, iusIqFileSetGetAcquisition);
    RUN_TEST_CASE(IusIqFile, iusIqFileSetGetTransducer);
    RUN_TEST_CASE(IusIqFile, testIusIqFileSerialization);
    RUN_TEST_CASE(IusIqFile, testIusIqFileDataIOSaveFrame);
    RUN_TEST_CASE(IusIqFile, testIusIqFileDataIOSaveResponse);
    RUN_TEST_CASE(IusIqFile, testIusIqFileDataIOSaveChannel);
}
