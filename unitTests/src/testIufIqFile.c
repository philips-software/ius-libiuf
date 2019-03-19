

//
// Created by nlv09165 on 14/01/2019.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <iuf.h>
#include <dg/dataGenerators.h>

static const char *pFilename = "IufIqFile.hdf5";
static const char *pNotherFilename = "AnotherIufIqFile.hdf5";

static char *pErrorFilename = "IufIqFile.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(IufIqFile);

TEST_SETUP(IufIqFile)
{
    iufErrorLogClear();
    iufErrorLog(IUF_TRUE);
    iufErrorAutoReportSet(IUF_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iufErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IufIqFile)
{
    iufErrorLogClear();
    iufErrorLog(IUF_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iufErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
}

TEST(IufIqFile, testIufIqFileCreate)
{
    const char *pEmptyFilename = "";
    const char *pSpecialCharsFilename = "*&/";

    iuif_t ifh = iufIqFileCreate(pFilename);
    TEST_ASSERT(ifh != IUIF_INVALID);

    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    // Create file that is already open should result in error
    iuif_t ifh2 = iufIqFileCreate(pFilename);
    TEST_ASSERT(ifh2 == IUIF_INVALID);
	TEST_ASSERT_EQUAL(1, iufErrorGetCount());

    int status = iufIqFileClose(ifh);
    TEST_ASSERT(status == IUF_E_OK);

    // Closing file that has already been closed results in error
    status = iufIqFileClose(ifh);
	TEST_ASSERT_EQUAL(2, iufErrorGetCount());
    iufIqFileDelete(ifh);
    TEST_ASSERT(status != IUF_E_OK);

    // Invalid argument should result in error.
    ifh = iufIqFileCreate(pEmptyFilename);
	TEST_ASSERT_EQUAL(3, iufErrorGetCount());
    TEST_ASSERT(ifh == IUIF_INVALID);
    ifh = iufIqFileCreate(pSpecialCharsFilename);
    TEST_ASSERT(ifh == IUIF_INVALID);
    TEST_ASSERT_EQUAL(4,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
}

TEST(IufIqFile, testIufIqFileDelete)
{
    iuif_t obj = iufIqFileCreate(pFilename);
    TEST_ASSERT(obj != IUIF_INVALID);

	int status = iufIqFileClose(obj);
	TEST_ASSERT_EQUAL(IUF_E_OK, status);
	
	status = iufIqFileDelete(obj);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    // invalid params
    status = iufIqFileDelete(NULL);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, status);
}


TEST(IufIqFile, testIufIqFileCompare)
{
    IUF_BOOL equal;
    iuif_t obj = iufIqFileCreate(pFilename);
    iuif_t notherObj = iufIqFileCreate(pNotherFilename);
    TEST_ASSERT(obj != IUIF_INVALID);
    TEST_ASSERT(notherObj != IUIF_INVALID);
    equal = iufIqFileCompare(obj,obj);
    TEST_ASSERT_EQUAL(IUF_TRUE,equal);
    equal = iufIqFileCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);
    
    // invalid params
    equal = iufIqFileCompare(obj,NULL);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);
    equal = iufIqFileCompare(NULL,obj);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);

	int status = iufIqFileClose(obj);
	TEST_ASSERT(status == IUF_E_OK);
	status = iufIqFileClose(notherObj);
	TEST_ASSERT(status == IUF_E_OK);
	iufIqFileDelete(obj);
    iufIqFileDelete(notherObj);
}

TEST(IufIqFile, iufIqFileSetGetFrameList)
{
    IUF_BOOL equal;
    int status;
    iufl_t frameList = dgGenerateFrameList(10);
    iuif_t obj = iufIqFileCreate(pFilename);

    status = iufIqFileSetFrameList(obj,frameList);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    iufl_t gotMeAFrameList = iufIqFileGetFrameList(obj);
    TEST_ASSERT_NOT_EQUAL(IUFL_INVALID, gotMeAFrameList);

    equal = iufFrameListCompare(frameList,gotMeAFrameList);
    TEST_ASSERT_EQUAL(IUF_TRUE, equal);

    status = iufIqFileClose(obj);
    TEST_ASSERT(status == IUF_E_OK);
    iufFrameListDeepDelete(frameList);
    iufIqFileDelete(obj);
}

TEST(IufIqFile, iufIqFileSetGetPatternListDict)
{
    IUF_BOOL equal;
    int status;
    iuiqpald_t iqPatternListDict = dgGenerateIqPatternListDict("mylabel",NULL,NULL);
    iuif_t obj = iufIqFileCreate(pFilename);

    status = iufIqFileSetPatternListDict(obj, iqPatternListDict);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    iuiqpald_t gotMeAPatternListDict = iufIqFileGetIqPatternListDict(obj);
    TEST_ASSERT_NOT_EQUAL(IUIQPALD_INVALID, gotMeAPatternListDict);

    equal = iufIqPatternListDictCompare(iqPatternListDict,gotMeAPatternListDict);
    TEST_ASSERT_EQUAL(IUF_TRUE, equal);

    status = iufIqFileClose(obj);
    TEST_ASSERT(status == IUF_E_OK);
    iufIqFileDelete(obj);
    iufIqPatternListDictDeepDelete(iqPatternListDict);
}


TEST(IufIqFile, iufIqFileSetGetPulseDict)
{
    iuif_t obj = iufIqFileCreate(pFilename);
    iupd_t pulseDict = dgGeneratePulseDict();

    int status = iufIqFileSetPulseDict(obj, pulseDict);
    TEST_ASSERT(status == IUF_E_OK);
    iupd_t gotMeAPulseDict = iufIqFileGetPulseDict(obj);
    TEST_ASSERT_NOT_EQUAL(NULL, gotMeAPulseDict);

    IUF_BOOL equal = iufPulseDictCompare(pulseDict,gotMeAPulseDict);
    TEST_ASSERT_EQUAL(IUF_TRUE, equal);

    status = iufIqFileClose(obj);
    TEST_ASSERT(status == IUF_E_OK);
    iufIqFileDelete(obj);
    iufPulseDictDeepDelete(pulseDict);
}

TEST(IufIqFile, iufIqFileSetGetSourceDict)
{
    iuif_t obj = iufIqFileCreate(pFilename);
    iusd_t sourceDict = dgGenerateSourceDict();

    int status = iufIqFileSetSourceDict(obj, sourceDict);
    TEST_ASSERT(status == IUF_E_OK);
    iusd_t gotMeASourceDict = iufIqFileGetSourceDict(obj);
    TEST_ASSERT_NOT_EQUAL(IUFD_INVALID, gotMeASourceDict);

    IUF_BOOL equal = iufSourceDictCompare(sourceDict,gotMeASourceDict);
    TEST_ASSERT_EQUAL(IUF_TRUE, equal);

    status = iufIqFileClose(obj);
    TEST_ASSERT(status == IUF_E_OK);
    iufSourceDictDeepDelete(sourceDict);
    iufIqFileDelete(obj);
}


TEST(IufIqFile, iufIqFileSetGetReceiveChannelMapDict)
{
    iuif_t obj = iufIqFileCreate(pFilename);
    iurcmd_t receiveChannelMapDict = iufReceiveChannelMapDictCreate();

    int status = iufIqFileSetReceiveChannelMapDict(obj, receiveChannelMapDict);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);

    iurcmd_t gotMeAReceiveChannelMapDict = iufIqFileGetReceiveChannelMapDict(obj);
    TEST_ASSERT_NOT_EQUAL(NULL, gotMeAReceiveChannelMapDict);

    IUF_BOOL equal = iufReceiveChannelMapDictCompare(receiveChannelMapDict, gotMeAReceiveChannelMapDict);
    TEST_ASSERT_EQUAL(IUF_TRUE, equal);

    status = iufIqFileClose(obj);
    TEST_ASSERT(status == IUF_E_OK);
    iufIqFileDelete(obj);
    iufReceiveChannelMapDictDelete(receiveChannelMapDict);
}

TEST(IufIqFile, iufIqFileSetGetTransmitApodizationDict)
{
    IUF_BOOL equal;
    int status;
	iutad_t transmitApodizationDict = iufTransmitApodizationDictCreate();
    iuif_t obj = iufIqFileCreate(pFilename);
    TEST_ASSERT(obj != IUIF_INVALID);

	// transmitApodizationDict param
	status = iufIqFileSetTransmitApodizationDict(obj, transmitApodizationDict);
	TEST_ASSERT_EQUAL(IUF_E_OK, status);

	iutad_t gotMeATransmitApodizationDict = iufIqFileGetTransmitApodizationDict(obj);
	TEST_ASSERT_NOT_EQUAL(NULL, gotMeATransmitApodizationDict);

	equal = iufTransmitApodizationDictCompare(transmitApodizationDict, gotMeATransmitApodizationDict);
	TEST_ASSERT_EQUAL(IUF_TRUE, equal);

    // invalid param
    status = iufIqFileSetTransmitApodizationDict(obj, NULL);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, status);

    status = iufIqFileSetTransmitApodizationDict(NULL, transmitApodizationDict);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, status);

    gotMeATransmitApodizationDict = iufIqFileGetTransmitApodizationDict(NULL);
    TEST_ASSERT_EQUAL(IUTAD_INVALID, gotMeATransmitApodizationDict);

	status = iufIqFileClose(obj);
	TEST_ASSERT(status == IUF_E_OK);
    iufIqFileDelete(obj);
    iufTransmitApodizationDictDelete(transmitApodizationDict);
}

#if 0
// receiveseting -> demodulation
TEST(IufIqFile, iufIqFileSetGetReceiveSettingsDict)
{
    IUF_BOOL equal;
    int status;
    iursd_t receiveSettingsDict = dgGenerateReceiveSettingsDict("bmode", 0);
    iuif_t obj = iufIqFileCreate(pFilename);
    TEST_ASSERT(obj != IUIF_INVALID);

    status = iufIqFileSetReceiveSettingsDict(obj, receiveSettingsDict);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);

    iursd_t gotMeATransmitReceiveSettingsDict = iufIqFileGetReceiveSettingsDict(obj);
    TEST_ASSERT_NOT_EQUAL(NULL, gotMeATransmitReceiveSettingsDict);

    equal = iufReceiveSettingsDictCompare(receiveSettingsDict, gotMeATransmitReceiveSettingsDict);
    TEST_ASSERT_EQUAL(IUF_TRUE, equal);

    // invalid param
    status = iufIqFileSetReceiveSettingsDict(obj, NULL);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, status);

    status = iufIqFileSetReceiveSettingsDict(NULL, receiveSettingsDict);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, status);

    gotMeATransmitReceiveSettingsDict = iufIqFileGetReceiveSettingsDict(NULL);
    TEST_ASSERT_EQUAL(IURSD_INVALID, gotMeATransmitReceiveSettingsDict);

    status = iufIqFileClose(obj);
    TEST_ASSERT(status == IUF_E_OK);
    iufIqFileDelete(obj);
    iufReceiveSettingsDictDeepDelete(receiveSettingsDict);
}
#endif

TEST(IufIqFile, iufIqFileSetGetAcquisition)
{
    IUF_BOOL equal;
    int status;
    iua_t acquisition = dgGenerateAcquisition();
    iuif_t obj = iufIqFileCreate(pFilename);
    TEST_ASSERT(obj != IUIF_INVALID);

    status = iufIqFileSetAcquisition(obj, acquisition);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);

    iua_t gotMeAnAcquisition = iufIqFileGetAcquisition(obj);
    TEST_ASSERT_NOT_EQUAL(NULL, gotMeAnAcquisition);

    equal = iufAcquisitionCompare(acquisition, gotMeAnAcquisition);
    TEST_ASSERT_EQUAL(IUF_TRUE, equal);

    // invalid param
    status = iufIqFileSetAcquisition(obj, NULL);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, status);

    status = iufIqFileSetAcquisition(NULL, acquisition);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, status);

    gotMeAnAcquisition = iufIqFileGetAcquisition(NULL);
    TEST_ASSERT_EQUAL(IURSD_INVALID, gotMeAnAcquisition);

    status = iufIqFileClose(obj);
    TEST_ASSERT(status == IUF_E_OK);
    iufIqFileDelete(obj);
    iufAcquisitionDelete(acquisition);
}

TEST(IufIqFile, iufIqFileSetGetTransducer)
{
    IUF_BOOL equal;
    int status;
    iut_t transducer = dgGenerateTransducer("S5-1");
    iuif_t obj = iufIqFileCreate(pFilename);
    TEST_ASSERT(obj != IUIF_INVALID);

    status = iufIqFileSetTransducer(obj, transducer);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);

    iut_t gotMeATransducer = iufIqFileGetTransducer(obj);
    TEST_ASSERT_NOT_EQUAL(NULL, gotMeATransducer);

    equal = iufTransducerCompare(transducer, gotMeATransducer);
    TEST_ASSERT_EQUAL(IUF_TRUE, equal);

    // invalid param
    status = iufIqFileSetTransducer(obj, NULL);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, status);

    status = iufIqFileSetTransducer(NULL, transducer);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, status);

    gotMeATransducer = iufIqFileGetTransducer(NULL);
    TEST_ASSERT_EQUAL(IURSD_INVALID, gotMeATransducer);

    status = iufIqFileClose(obj);
    TEST_ASSERT(status == IUF_E_OK);
    iufIqFileDelete(obj);
    iufTransducerDeepDelete(transducer);
}

TEST(IufIqFile, testIufIqFileSerialization)
{
    int numFrames = 10;
    int numSamplesPerLine = 10;
    int numChannels = 8;
    char *ptestFileName = "testIufIqFileSerialization.hdf5";

    // create
    iuiqf_t iqFile = dgGenerateIqFile(ptestFileName, "S5-1", "bmode", numFrames, numSamplesPerLine, numChannels);
    TEST_ASSERT(iqFile != IUIQF_INVALID);

	// save
    int status = iufIqFileNodeSave(iqFile);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);
    status = iufIqFileClose(iqFile);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    // read back
    iuiqf_t savedObj = iufIqFileNodeLoad(ptestFileName);
    TEST_ASSERT(savedObj != NULL);
    TEST_ASSERT_EQUAL(IUF_TRUE, iufIqFileCompare(iqFile,savedObj));

	status = iufIqFileClose(savedObj);
	TEST_ASSERT(status == IUF_E_OK);

	dgDeleteIqFile(iqFile);
    iufIqFileDelete(savedObj);
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
    iud_t iFrame = iufIqFileFrameCreate(iqFile, label);
	iud_t qFrame = iufIqFileFrameCreate(iqFile, label);
    iuo_t offset = iufOffsetCreate();

    for (i=0; i<numFrames; i++)
    {
        dgFillData(iFrame, 1 + i * 1.0f);
		dgFillData(qFrame, 100 - i * 1.0f);
        offset->t = i;
        status |= iufIqFileFrameSave(iqFile, label, IUF_IQ_COMPONENT_I, iFrame, offset);
		status |= iufIqFileFrameSave(iqFile, label, IUF_IQ_COMPONENT_Q, qFrame, offset);
    }

    iufOffsetDelete(offset);
    iufDataDelete(iFrame);
	iufDataDelete(qFrame);
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
    iud_t iResponse = iufIqFileResponseCreate(iqFile, label);
	iud_t qResponse = iufIqFileResponseCreate(iqFile, label);
    iuo_t offset = iufOffsetCreate();
    int numResponses = iufIqFileGetNumResponses(iqFile, label);

    for (i=0; i<numFrames; i++)
    {
        dgFillData(iResponse, 1 + i * 1.0f);
		dgFillData(qResponse, 100 - i * 1.0f);
        // Save Frame in responses
        for (j=0; j<numResponses; j++)
        {
            offset->y = j;
            offset->t = i;
            status |= iufIqFileResponseSave(iqFile, label, IUF_IQ_COMPONENT_I, iResponse, offset);
			status |= iufIqFileResponseSave(iqFile, label, IUF_IQ_COMPONENT_Q, qResponse, offset);
        }
    }

    iufDataDelete(iResponse);
	iufDataDelete(qResponse);
    iufOffsetDelete(offset);
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
    iud_t iChannel = iufIqFileChannelCreate(iqFile, label);
	iud_t qChannel = iufIqFileChannelCreate(iqFile, label);
    iuo_t offset = iufOffsetCreate();
    int numResponses = iufIqFileGetNumResponses(iqFile, label);
    int numChannels = iufIqFileGetNumChannels(iqFile, label);

    for (i=0; i<numFrames; i++)
    {
        dgFillData(iChannel, 1 + i * 1.0f);
		dgFillData(qChannel, 100 - i * 1.0f);
        // Save Frame in responses
        for (j=0; j<numResponses; j++)
        {
            for (k=0; k<numChannels; k++)
            {
                offset->z = k;
                offset->y = j;
                offset->t = i;
                status |= iufIqFileChannelSave(iqFile, label, IUF_IQ_COMPONENT_I, iChannel, offset);
				status |= iufIqFileChannelSave(iqFile, label, IUF_IQ_COMPONENT_Q, qChannel, offset);
            }
        }
    }

    iufDataDelete(iChannel);
	iufDataDelete(qChannel);
    iufOffsetDelete(offset);
    return status;
}


static IUF_BOOL validateFrames
(
    iuif_t iqFile,
    char *label,
    int numFrames
)
{
    int status=0;
    int i;
    IUF_BOOL equal = IUF_FALSE;

    iud_t referenceIFrame = iufIqFileFrameCreate(iqFile, label);
	iud_t referenceQFrame = iufIqFileFrameCreate(iqFile, label);
    iud_t actualIFrame = iufIqFileFrameCreate(iqFile, label);
	iud_t actualQFrame = iufIqFileFrameCreate(iqFile, label);
    iuo_t offset = iufOffsetCreate();
    for (i=0; i<numFrames; i++)
    {
        dgFillData(referenceIFrame, 1 + i * 1.0f);
		//dgFillData(actualIFrame, 1 + i * 1.0f);

		dgFillData(referenceQFrame, 100 - i * 1.0f);
		//dgFillData(actualQFrame, 100 - i * 1.0f);
        offset->t = i;
        status |= iufIqFileFrameLoad(iqFile, label, IUF_IQ_COMPONENT_I, actualIFrame, offset);
        equal = iufDataCompare(referenceIFrame, actualIFrame);
        if (equal == IUF_FALSE) break;

		status |= iufIqFileFrameLoad(iqFile, label, IUF_IQ_COMPONENT_Q, actualQFrame, offset);
		equal = iufDataCompare(referenceQFrame, actualQFrame);
		if (equal == IUF_FALSE) break;
    }

    iufDataDelete(referenceIFrame);
    iufDataDelete(actualIFrame);
	iufDataDelete(referenceQFrame);
	iufDataDelete(actualQFrame);
    iufOffsetDelete(offset);
    return equal;
}


static IUF_BOOL validateResponses
(
    iuif_t iqFile,
    char *label,
    int numFrames
)
{
    int status=0;
    int i,j;
    IUF_BOOL equal = IUF_FALSE;

    iud_t referenceIResponse = iufIqFileResponseCreate(iqFile, label);
    iud_t actualIResponse    = iufIqFileResponseCreate(iqFile, label);
	iud_t referenceQResponse = iufIqFileResponseCreate(iqFile, label);
	iud_t actualQResponse    = iufIqFileResponseCreate(iqFile, label);
    iuo_t offset = iufOffsetCreate();
    int numResponses = iufIqFileGetNumResponses(iqFile, label);

    for (i=0; i<numFrames; i++)
    {
        dgFillData(referenceIResponse, 1 + i * 1.0f);
		dgFillData(referenceQResponse, 100 - i * 1.0f);
        for (j=0; j<numResponses; j++)
        {
            offset->y = j;
            offset->t = i;
            status |= iufIqFileResponseLoad(iqFile, label, IUF_IQ_COMPONENT_I, actualIResponse, offset);
			status |= iufIqFileResponseLoad(iqFile, label, IUF_IQ_COMPONENT_Q, actualQResponse, offset);
            equal = iufDataCompare(referenceIResponse,actualIResponse);
			if (equal == IUF_FALSE) break;

			equal = iufDataCompare(referenceQResponse, actualQResponse);
			if (equal == IUF_FALSE) break;
        }
    }

    iufDataDelete(referenceIResponse);
    iufDataDelete(actualIResponse);
	iufDataDelete(referenceQResponse);
	iufDataDelete(actualQResponse);

    iufOffsetDelete(offset);
    return equal;
}

#if 0
static IUF_BOOL validateChannels
(
    iuif_t iqFile,
    char *label,
    int numFrames
)
{
    int status=0;
    int i,j,k;
    IUF_BOOL equal = IUF_FALSE;

    iud_t referenceChannel = iufIqFileChannelCreate(iqFile, label);
    iud_t actualChannel = iufIqFileChannelCreate(iqFile, label);
    iuo_t offset = iufOffsetCreate();
    int numResponses = iufIqFileGetNumResponses(iqFile, label);
    int numChannels = iufIqFileGetNumChannels(iqFile, label);

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
                status |= iufIqFileChannelLoad(iqFile, label, actualChannel, offset);
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

TEST(IufIqFile, testIufIqFileDataIOSaveFrame)
{
    char *ptestFileName = "testIufIqFileDataIOSaveFrame.hdf5";
    char *pDopplerLabel = "doppler";
    int numFrames = 10;
    int numSamplesPerLine = 10;
    int numChannels = 8;
    char *pBmodeLabel = "bmode";
    int status = 0;

    // create
    iuiqf_t iqFile = dgGenerateIqFile(ptestFileName, "S5-1", pDopplerLabel, numFrames, numSamplesPerLine, numChannels);
    status = dgIqFileAddGeneratedData(iqFile, pBmodeLabel, numSamplesPerLine, numChannels);
    TEST_ASSERT_EQUAL(status, IUF_E_OK);

    // SaveFrames
    status = iufIqFileNodeSave(iqFile);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);
    status = saveFrames(iqFile,pDopplerLabel,numFrames);
    status |= saveFrames(iqFile,pBmodeLabel,numFrames);
    TEST_ASSERT_EQUAL(status, IUF_E_OK);
    status = iufIqFileClose(iqFile);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    // read back
    iuif_t savedObj = iufIqFileNodeLoad(ptestFileName);
    TEST_ASSERT(savedObj != NULL);
    TEST_ASSERT_EQUAL(IUF_TRUE, iufIqFileCompare(iqFile,savedObj));
    TEST_ASSERT_EQUAL(IUF_TRUE, validateFrames(savedObj,pDopplerLabel,numFrames));
    TEST_ASSERT_EQUAL(IUF_TRUE, validateFrames(savedObj,pBmodeLabel,numFrames));


    // create channel
    iufIqFileClose(savedObj);
    iufIqFileDelete(savedObj);
    dgDeleteIqFile(iqFile);
}

TEST(IufIqFile, testIufIqFileDataIOSaveResponse)
{
    char *ptestFileName = "testIufIqFileDataIOSaveResponse.hdf5";
    char *pDopplerLabel = "doppler";
    int numFrames = 10;
    int numSamplesPerLine = 10;
    int numChannels = 8;
    char *pBmodeLabel = "bmode";
    int status = 0;

    // create
    iuif_t iqFile = dgGenerateIqFile(ptestFileName, "S5-1", pDopplerLabel, numFrames, numSamplesPerLine, numChannels);
    status = dgIqFileAddGeneratedData(iqFile, pBmodeLabel, numSamplesPerLine, numChannels);
    TEST_ASSERT_EQUAL(status, IUF_E_OK);


    // SaveFrames
    status = iufIqFileNodeSave(iqFile);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);
    status = saveResponses(iqFile,pDopplerLabel,numFrames);
    status |= saveResponses(iqFile,pBmodeLabel,numFrames);
    TEST_ASSERT_EQUAL(status, IUF_E_OK);
    status = iufIqFileClose(iqFile);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    // read back
    iuif_t savedObj = iufIqFileNodeLoad(ptestFileName);
    TEST_ASSERT(savedObj != NULL);
    TEST_ASSERT_EQUAL(IUF_TRUE, iufIqFileCompare(iqFile,savedObj));
    TEST_ASSERT_EQUAL(IUF_TRUE, validateResponses(savedObj,pDopplerLabel,numFrames));
    TEST_ASSERT_EQUAL(IUF_TRUE, validateResponses(savedObj,pBmodeLabel,numFrames));


    // create channel
    iufIqFileClose(savedObj);
    iufIqFileDelete(savedObj);
    dgDeleteIqFile(iqFile);
}

TEST(IufIqFile, testIufIqFileDataIOSaveChannel)
{
    char *ptestFileName = "testIufIqFileDataIOSaveChannel.hdf5";
    char *pDopplerLabel = "doppler";
    int numFrames = 10;
    int numSamplesPerLine = 10;
    int numChannels = 8;
    char *pBmodeLabel = "bmode";
    int status = 0;

    // create
    iuiqf_t iqFile = dgGenerateIqFile(ptestFileName, "S5-1", pDopplerLabel, numFrames, numSamplesPerLine, numChannels);
    status = dgIqFileAddGeneratedData(iqFile, pBmodeLabel, numSamplesPerLine, numChannels);
    TEST_ASSERT_EQUAL(status, IUF_E_OK);


    // SaveFrames
    status = iufIqFileNodeSave(iqFile);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);
    status = saveChannels(iqFile, pDopplerLabel, numFrames);
//    status |= saveChannels(iqFile, pBmodeLabel, numFrames);
    TEST_ASSERT_EQUAL(status, IUF_E_OK);
    status = iufIqFileClose(iqFile);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    // read back
    iuif_t savedObj = iufIqFileNodeLoad(ptestFileName);
    TEST_ASSERT(savedObj != NULL);
    TEST_ASSERT_EQUAL(IUF_TRUE, iufIqFileCompare(iqFile,savedObj));
    // Todo:
    // Fix Errors in validateChannels:
//    TEST_ASSERT_EQUAL(IUF_TRUE, validateChannels(savedObj,pDopplerLabel,numFrames));
//    TEST_ASSERT_EQUAL(IUF_TRUE, validateChannels(savedObj,pBmodeLabel,numFrames));


    // create channel
    iufIqFileClose(savedObj);
    iufIqFileDelete(savedObj);
    dgDeleteIqFile(iqFile);
}

TEST_GROUP_RUNNER(IufIqFile)
{
    RUN_TEST_CASE(IufIqFile, testIufIqFileCreate);
    RUN_TEST_CASE(IufIqFile, testIufIqFileDelete);
    RUN_TEST_CASE(IufIqFile, testIufIqFileCompare);
    RUN_TEST_CASE(IufIqFile, iufIqFileSetGetFrameList);
    RUN_TEST_CASE(IufIqFile, iufIqFileSetGetPatternListDict);
    RUN_TEST_CASE(IufIqFile, iufIqFileSetGetPulseDict);
    RUN_TEST_CASE(IufIqFile, iufIqFileSetGetSourceDict);
    RUN_TEST_CASE(IufIqFile, iufIqFileSetGetReceiveChannelMapDict);
    RUN_TEST_CASE(IufIqFile, iufIqFileSetGetTransmitApodizationDict);
    //RUN_TEST_CASE(IufIqFile, iufIqFileSetGetReceiveSettingsDict);
    RUN_TEST_CASE(IufIqFile, iufIqFileSetGetAcquisition);
    RUN_TEST_CASE(IufIqFile, iufIqFileSetGetTransducer);
    RUN_TEST_CASE(IufIqFile, testIufIqFileSerialization);
    RUN_TEST_CASE(IufIqFile, testIufIqFileDataIOSaveFrame);
    RUN_TEST_CASE(IufIqFile, testIufIqFileDataIOSaveResponse);
    RUN_TEST_CASE(IufIqFile, testIufIqFileDataIOSaveChannel);
}
