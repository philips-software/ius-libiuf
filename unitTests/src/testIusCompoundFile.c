

//
// Created by nlv12901 on 28/01/2019.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <ius.h>
#include <testDataGenerators.h>

static const char *pFilename = "IusCompoundFile.hdf5";
static const char *pNotherFilename = "AnotherIusCompoundFile.hdf5";

TEST_GROUP(IusCompoundFile);

TEST_SETUP(IusCompoundFile)
{
}

TEST_TEAR_DOWN(IusCompoundFile)
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


TEST(IusCompoundFile, testIusCompoundFileCreate)
{
    const char *pEmptyFilename = "";
    const char *pSpecialCharsFilename = "*&/";

    iucf_t cfh = iusCompoundFileCreate(pFilename);
    TEST_ASSERT(cfh != IUCF_INVALID);

    // Create file that is already open should result in error
    iucf_t cfh2 = iusCompoundFileCreate(pFilename);
    TEST_ASSERT(cfh2 == IUCF_INVALID);

    int status = iusCompoundFileClose(cfh);
    TEST_ASSERT(status == IUS_E_OK);

    // Closing file that has already been closed results in error
    status = iusCompoundFileClose(cfh);
    iusCompoundFileDelete(cfh);
    TEST_ASSERT(status != IUS_E_OK);

    // Invalid argument should result in error.
    cfh = iusCompoundFileCreate(pEmptyFilename);
    TEST_ASSERT(cfh == IUCF_INVALID);
    cfh = iusCompoundFileCreate(pSpecialCharsFilename);
    TEST_ASSERT(cfh == IUCF_INVALID);
}

TEST(IusCompoundFile, testIusCompoundFileDelete)
{
    iucf_t obj = iusCompoundFileCreate(pFilename);
    TEST_ASSERT(obj != IUCF_INVALID);

	int status = iusCompoundFileClose(obj);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);
	
	status = iusCompoundFileDelete(obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // invalid params
    status = iusCompoundFileDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
}


TEST(IusCompoundFile, testIusCompoundFileCompare)
{
    IUS_BOOL equal;
    iucf_t obj = iusCompoundFileCreate(pFilename);
    iucf_t notherObj = iusCompoundFileCreate(pNotherFilename);
    TEST_ASSERT(obj != IUCF_INVALID);
    TEST_ASSERT(notherObj != IUCF_INVALID);
    equal = iusCompoundFileCompare(obj,obj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = iusCompoundFileCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    
    // invalid params
    equal = iusCompoundFileCompare(obj,NULL);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    equal = iusCompoundFileCompare(NULL,obj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

	int status = iusCompoundFileClose(obj);
	TEST_ASSERT(status == IUS_E_OK);
	status = iusCompoundFileClose(notherObj);
	TEST_ASSERT(status == IUS_E_OK);
	iusCompoundFileDelete(obj);
    iusCompoundFileDelete(notherObj);
}

TEST(IusCompoundFile, iusCompoundFileSetGetFrameList)
{
    IUS_BOOL equal;
    int status;
    iufl_t frameList = dgGenerateFrameList(10);
    iucf_t obj = iusCompoundFileCreate(pFilename);

    status = iusCompoundFileSetFrameList(obj,frameList);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    iufl_t gotMeAFrameList = iusCompoundFileGetFrameList(obj);
    TEST_ASSERT_NOT_EQUAL(IUFL_INVALID, gotMeAFrameList);

    equal = iusFrameListCompare(frameList,gotMeAFrameList);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);

    status = iusCompoundFileClose(obj);
    TEST_ASSERT(status == IUS_E_OK);
    iusFrameListDeepDelete(frameList);
    iusCompoundFileDelete(obj);
}

TEST(IusCompoundFile, iusCompoundFileSetGetPatternListDict)
{
    IUS_BOOL equal;
    int status;
    iuiqpald_t iqPatternListDict = dgGenerateIqPatternListDict("mylabel",NULL,NULL);
    iucf_t obj = iusCompoundFileCreate(pFilename);

    status = iusCompoundFileSetPatternListDict(obj, iqPatternListDict);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    iuiqpald_t gotMeAPatternListDict = iusCompoundFileGetPatternListDict(obj);
    TEST_ASSERT_NOT_EQUAL(IUIQPALD_INVALID, gotMeAPatternListDict);

    equal = iusIqPatternListDictCompare(iqPatternListDict,gotMeAPatternListDict);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);

    status = iusCompoundFileClose(obj);
    TEST_ASSERT(status == IUS_E_OK);
    iusCompoundFileDelete(obj);
    iusIqPatternListDictDeepDelete(iqPatternListDict);
}


TEST(IusCompoundFile, iusCompoundFileSetGetPulseDict)
{
    iucf_t obj = iusCompoundFileCreate(pFilename);
    iupd_t pulseDict = dgGeneratePulseDict();

    int status = iusCompoundFileSetPulseDict(obj, pulseDict);
    TEST_ASSERT(status == IUS_E_OK);
    iupd_t gotMeAPulseDict = iusCompoundFileGetPulseDict(obj);
    TEST_ASSERT_NOT_EQUAL(NULL, gotMeAPulseDict);

    IUS_BOOL equal = iusPulseDictCompare(pulseDict,gotMeAPulseDict);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);

    status = iusCompoundFileClose(obj);
    TEST_ASSERT(status == IUS_E_OK);
    iusCompoundFileDelete(obj);
    iusPulseDictDeepDelete(pulseDict);
}

TEST(IusCompoundFile, iusCompoundFileSetGetSourceDict)
{
    iucf_t obj = iusCompoundFileCreate(pFilename);
    iusd_t sourceDict = dgGenerateSourceDict();

    int status = iusCompoundFileSetSourceDict(obj, sourceDict);
    TEST_ASSERT(status == IUS_E_OK);
    iusd_t gotMeASourceDict = iusCompoundFileGetSourceDict(obj);
    TEST_ASSERT_NOT_EQUAL(IUSD_INVALID, gotMeASourceDict);

    IUS_BOOL equal = iusSourceDictCompare(sourceDict,gotMeASourceDict);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);

    status = iusCompoundFileClose(obj);
    TEST_ASSERT(status == IUS_E_OK);
    iusSourceDictDeepDelete(sourceDict);
    iusCompoundFileDelete(obj);
}


TEST(IusCompoundFile, iusCompoundFileSetGetReceiveChannelMapDict)
{
    iucf_t obj = iusCompoundFileCreate(pFilename);
    iurcmd_t receiveChannelMapDict = iusReceiveChannelMapDictCreate();

    int status = iusCompoundFileSetReceiveChannelMapDict(obj, receiveChannelMapDict);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);

    iurcmd_t gotMeAReceiveChannelMapDict = iusCompoundFileGetReceiveChannelMapDict(obj);
    TEST_ASSERT_NOT_EQUAL(NULL, gotMeAReceiveChannelMapDict);

    IUS_BOOL equal = iusReceiveChannelMapDictCompare(receiveChannelMapDict, gotMeAReceiveChannelMapDict);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);

    status = iusCompoundFileClose(obj);
    TEST_ASSERT(status == IUS_E_OK);
    iusCompoundFileDelete(obj);
    iusReceiveChannelMapDictDelete(receiveChannelMapDict);
}

TEST(IusCompoundFile, iusCompoundFileSetGetTransmitApodizationDict)
{
    IUS_BOOL equal;
    int status;
	iutad_t transmitApodizationDict = iusTransmitApodizationDictCreate();
    iucf_t obj = iusCompoundFileCreate(pFilename);
    TEST_ASSERT(obj != IUCF_INVALID);

	// transmitApodizationDict param
	status = iusCompoundFileSetTransmitApodizationDict(obj, transmitApodizationDict);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);

	iutad_t gotMeATransmitApodizationDict = iusCompoundFileGetTransmitApodizationDict(obj);
	TEST_ASSERT_NOT_EQUAL(NULL, gotMeATransmitApodizationDict);

	equal = iusTransmitApodizationDictCompare(transmitApodizationDict, gotMeATransmitApodizationDict);
	TEST_ASSERT_EQUAL(IUS_TRUE, equal);

    // invalid param
    status = iusCompoundFileSetTransmitApodizationDict(obj, NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);

    status = iusCompoundFileSetTransmitApodizationDict(NULL, transmitApodizationDict);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);

    gotMeATransmitApodizationDict = iusCompoundFileGetTransmitApodizationDict(NULL);
    TEST_ASSERT_EQUAL(IUTAD_INVALID, gotMeATransmitApodizationDict);

	status = iusCompoundFileClose(obj);
	TEST_ASSERT(status == IUS_E_OK);
    iusCompoundFileDelete(obj);
    iusTransmitApodizationDictDelete(transmitApodizationDict);
}

//receiveseting -> demodulation
TEST(IusCompoundFile, iusCompoundFileSetGetDemodulationDict)
{
    IUS_BOOL equal;
    int status;
    iudmd_t demodulationDict = dgGenerateDemodulationDict("bmode", 0);
    iucf_t obj = iusCompoundFileCreate(pFilename);
    TEST_ASSERT(obj != IUCF_INVALID);

    status = iusCompoundFileSetDemodulationDict(obj, demodulationDict);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);

    iudmd_t gotMeADemodulationDict = iusCompoundFileGetDemodulationDict(obj);
    TEST_ASSERT_NOT_EQUAL(NULL, gotMeADemodulationDict);

    equal = iusDemodulationDictCompare(demodulationDict, gotMeADemodulationDict);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);

    // invalid param
    status = iusCompoundFileSetDemodulationDict(obj, NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);

    status = iusCompoundFileSetDemodulationDict(NULL, demodulationDict);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);

    gotMeADemodulationDict = iusCompoundFileGetDemodulationDict(NULL);
    TEST_ASSERT_EQUAL(IUDMD_INVALID, gotMeADemodulationDict);

    status = iusCompoundFileClose(obj);
    TEST_ASSERT(status == IUS_E_OK);
    iusCompoundFileDelete(obj);
    iusDemodulationDictDeepDelete(demodulationDict);
}


TEST(IusCompoundFile, iusCompoundFileSetGetAcquisition)
{
    IUS_BOOL equal;
    int status;
    iua_t acquisition = dgGenerateAcquisition();
    iucf_t obj = iusCompoundFileCreate(pFilename);
    TEST_ASSERT(obj != IUCF_INVALID);

    status = iusCompoundFileSetAcquisition(obj, acquisition);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);

    iua_t gotMeAnAcquisition = iusCompoundFileGetAcquisition(obj);
    TEST_ASSERT_NOT_EQUAL(NULL, gotMeAnAcquisition);

    equal = iusAcquisitionCompare(acquisition, gotMeAnAcquisition);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);

    // invalid param
    status = iusCompoundFileSetAcquisition(obj, NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);

    status = iusCompoundFileSetAcquisition(NULL, acquisition);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);

    gotMeAnAcquisition = iusCompoundFileGetAcquisition(NULL);
    TEST_ASSERT_EQUAL(IURSD_INVALID, gotMeAnAcquisition);

    status = iusCompoundFileClose(obj);
    TEST_ASSERT(status == IUS_E_OK);
    iusCompoundFileDelete(obj);
    iusAcquisitionDelete(acquisition);
}

TEST(IusCompoundFile, iusCompoundFileSetGetTransducer)
{
    IUS_BOOL equal;
    int status;
    iut_t transducer = dgGenerateTransducer("S5-1");
    iucf_t obj = iusCompoundFileCreate(pFilename);
    TEST_ASSERT(obj != IUCF_INVALID);

    status = iusCompoundFileSetTransducer(obj, transducer);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);

    iut_t gotMeATransducer = iusCompoundFileGetTransducer(obj);
    TEST_ASSERT_NOT_EQUAL(NULL, gotMeATransducer);

    equal = iusTransducerCompare(transducer, gotMeATransducer);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);

    // invalid param
    status = iusCompoundFileSetTransducer(obj, NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);

    status = iusCompoundFileSetTransducer(NULL, transducer);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);

    gotMeATransducer = iusCompoundFileGetTransducer(NULL);
    TEST_ASSERT_EQUAL(IURSD_INVALID, gotMeATransducer);

    status = iusCompoundFileClose(obj);
    TEST_ASSERT(status == IUS_E_OK);
    iusCompoundFileDelete(obj);
    iusTransducerDeepDelete(transducer);
}

TEST(IusCompoundFile, testIusCompoundFileSerialization)
{
    int numFrames = 10;
    int numSamplesPerLine = 10;
    int numChannels = 8;
    char *ptestFileName = "testIusCompoundFileSerialization.hdf5";

    // create
    iuiqf_t iqFile = dgGenerateIqFile(ptestFileName, "S5-1", "bmode", numFrames, numSamplesPerLine, numChannels);
    TEST_ASSERT(iqFile != IUIQF_INVALID);

	// save
    int status = iusCompoundFileNodeSave(iqFile);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    status = iusCompoundFileClose(iqFile);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // read back
    iuiqf_t savedObj = iusCompoundFileNodeLoad(ptestFileName);
    TEST_ASSERT(savedObj != NULL);
    TEST_ASSERT_EQUAL(IUS_TRUE, iusCompoundFileCompare(iqFile,savedObj));

	status = iusCompoundFileClose(savedObj);
	TEST_ASSERT(status == IUS_E_OK);

	dgDeleteIqFile(iqFile);
    iusCompoundFileDelete(savedObj);
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

	if (!iusCompoundFileFrameCreate(iqFile, label, &iFrame, &qFrame)) return -1;
    iuo_t offset = iusOffsetCreate();

    for (i=0; i<numFrames; i++)
    {
        dgFillData(iFrame,  1 + i * 1.0f);
		dgFillData(qFrame, -1 - i * 1.0f);
        offset->t = i;
        status |= iusCompoundFileFrameSave(iqFile, label, iFrame, qFrame, offset);
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

	if (!iusCompoundFileResponseCreate(iqFile, label, &iResponse, &qResponse)) return -1;
    iuo_t offset = iusOffsetCreate();
    int numResponses = iusCompoundFileGetNumResponses(iqFile, label);

    for (i=0; i<numFrames; i++)
    {
        dgFillData(iResponse, 1 + i * 1.0f);
		dgFillData(qResponse, -1 - i * 1.0f);
        // Save Frame in responses
        for (j=0; j<numResponses; j++)
        {
            offset->y = j;
            offset->t = i;
            status |= iusCompoundFileResponseSave(iqFile, label, iResponse, qResponse, offset);
        }
    }

    iusDataDelete(iResponse);
	iusDataDelete(qResponse);
    iusOffsetDelete(offset);
    return status;
}

static int saveChannels
(
    iucf_t iqFile,
    char *label,
    int numFrames
)
{
    int status=0;
    int i,j,k;
	iud_t iChannel;
	iud_t qChannel;
	
	if (!iusCompoundFileChannelCreate(iqFile, label, &iChannel, &qChannel)) return -1;
    iuo_t offset = iusOffsetCreate();
    int numResponses = iusCompoundFileGetNumResponses(iqFile, label);
    int numChannels = iusCompoundFileGetNumChannels(iqFile, label);

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
                status |= iusCompoundFileChannelSave(iqFile, label, iChannel, qChannel, offset);
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
    iucf_t iqFile,
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

	if (!iusCompoundFileFrameCreate(iqFile, label, &referenceIFrame, &referenceQFrame)) return IUS_FALSE;
	if (!iusCompoundFileFrameCreate(iqFile, label, &actualIFrame, &actualQFrame)) return IUS_FALSE;

	iuo_t offset = iusOffsetCreate();
    for (i=0; i<numFrames; i++)
    {
        dgFillData(referenceIFrame, 1 + i * 1.0f);
		dgFillData(referenceQFrame, -1 - i * 1.0f);
        offset->t = i;
        status |= iusCompoundFileFrameLoad(iqFile, label, actualIFrame, actualQFrame, offset);
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
    iucf_t iqFile,
    char *label,
    int numFrames
)
{
    int status=0;
    int i,j;
    IUS_BOOL equal = IUS_FALSE;

	iud_t referenceIResponse;
	iud_t referenceQResponse;
	if (!iusCompoundFileResponseCreate(iqFile, label, &referenceIResponse, &referenceQResponse)) return -1;
	iud_t actualIResponse;
	iud_t actualQResponse;
	if (!iusCompoundFileResponseCreate(iqFile, label, &actualIResponse, &actualQResponse)) return -1;

    iuo_t offset = iusOffsetCreate();
    int numResponses = iusCompoundFileGetNumResponses(iqFile, label);

    for (i=0; i<numFrames; i++)
    {
        dgFillData(referenceIResponse, 1 + i * 1.0f);
		dgFillData(referenceQResponse, -1 - i * 1.0f);
        for (j=0; j<numResponses; j++)
        {
            offset->y = j;
            offset->t = i;
            status |= iusCompoundFileResponseLoad(iqFile, label, actualIResponse, actualQResponse, offset);
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
    iucf_t iqFile,
    char *label,
    int numFrames
)
{
    int status=0;
    int i,j,k;
    IUS_BOOL equal = IUS_FALSE;

    iud_t referenceChannel = iusCompoundFileChannelCreate(iqFile, label);
    iud_t actualChannel = iusCompoundFileChannelCreate(iqFile, label);
    iuo_t offset = iusOffsetCreate();
    int numResponses = iusCompoundFileGetNumResponses(iqFile, label);
    int numChannels = iusCompoundFileGetNumChannels(iqFile, label);

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
                status |= iusCompoundFileChannelLoad(iqFile, label, actualChannel, offset);
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

TEST(IusCompoundFile, testIusCompoundFileDataIOSaveFrame)
{
    char *ptestFileName = "testIusCompoundFileDataIOSaveFrame.hdf5";
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
    status = iusCompoundFileNodeSave(iqFile);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    status = saveFrames(iqFile,pDopplerLabel,numFrames);
    status |= saveFrames(iqFile,pBmodeLabel,numFrames);
    TEST_ASSERT_EQUAL(status, IUS_E_OK);
    status = iusCompoundFileClose(iqFile);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // read back
    iucf_t savedObj = iusCompoundFileNodeLoad(ptestFileName);
    TEST_ASSERT(savedObj != NULL);
    TEST_ASSERT_EQUAL(IUS_TRUE, iusCompoundFileCompare(iqFile,savedObj));
    TEST_ASSERT_EQUAL(IUS_TRUE, validateFrames(savedObj,pDopplerLabel,numFrames));
    TEST_ASSERT_EQUAL(IUS_TRUE, validateFrames(savedObj,pBmodeLabel,numFrames));


    // create channel
    iusCompoundFileClose(savedObj);
    iusCompoundFileDelete(savedObj);
    dgDeleteIqFile(iqFile);
}

TEST(IusCompoundFile, testIusCompoundFileDataIOSaveResponse)
{
    char *ptestFileName = "testIusCompoundFileDataIOSaveResponse.hdf5";
    char *pDopplerLabel = "doppler";
    int numFrames = 10;
    int numSamplesPerLine = 10;
    int numChannels = 8;
    char *pBmodeLabel = "bmode";
    int status = 0;

    // create
    iucf_t iqFile = dgGenerateIqFile(ptestFileName, "S5-1", pDopplerLabel, numFrames, numSamplesPerLine, numChannels);
    status = dgIqFileAddGeneratedData(iqFile, pBmodeLabel, numSamplesPerLine, numChannels);
    TEST_ASSERT_EQUAL(status, IUS_E_OK);


    // SaveFrames
    status = iusCompoundFileNodeSave(iqFile);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    status = saveResponses(iqFile,pDopplerLabel,numFrames);
    status |= saveResponses(iqFile,pBmodeLabel,numFrames);
    TEST_ASSERT_EQUAL(status, IUS_E_OK);
    status = iusCompoundFileClose(iqFile);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // read back
    iucf_t savedObj = iusCompoundFileNodeLoad(ptestFileName);
    TEST_ASSERT(savedObj != NULL);
    TEST_ASSERT_EQUAL(IUS_TRUE, iusCompoundFileCompare(iqFile,savedObj));
    TEST_ASSERT_EQUAL(IUS_TRUE, validateResponses(savedObj,pDopplerLabel,numFrames));
    TEST_ASSERT_EQUAL(IUS_TRUE, validateResponses(savedObj,pBmodeLabel,numFrames));


    // create channel
    iusCompoundFileClose(savedObj);
    iusCompoundFileDelete(savedObj);
    dgDeleteIqFile(iqFile);
}

TEST(IusCompoundFile, testIusCompoundFileDataIOSaveChannel)
{
    char *ptestFileName = "testIusCompoundFileDataIOSaveChannel.hdf5";
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
    status = iusCompoundFileNodeSave(iqFile);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    status = saveChannels(iqFile, pDopplerLabel, numFrames);
//    status |= saveChannels(iqFile, pBmodeLabel, numFrames);
    TEST_ASSERT_EQUAL(status, IUS_E_OK);
    status = iusCompoundFileClose(iqFile);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // read back
    iucf_t savedObj = iusCompoundFileNodeLoad(ptestFileName);
    TEST_ASSERT(savedObj != NULL);
    TEST_ASSERT_EQUAL(IUS_TRUE, iusCompoundFileCompare(iqFile,savedObj));
    // Todo:
    // Fix Errors in validateChannels:
//    TEST_ASSERT_EQUAL(IUS_TRUE, validateChannels(savedObj,pDopplerLabel,numFrames));
//    TEST_ASSERT_EQUAL(IUS_TRUE, validateChannels(savedObj,pBmodeLabel,numFrames));


    // create channel
    iusCompoundFileClose(savedObj);
    iusCompoundFileDelete(savedObj);
    dgDeleteIqFile(iqFile);
}

TEST_GROUP_RUNNER(IusCompoundFile)
{
    RUN_TEST_CASE(IusCompoundFile, testIusCompoundFileCreate);
    RUN_TEST_CASE(IusCompoundFile, testIusCompoundFileDelete);
    RUN_TEST_CASE(IusCompoundFile, testIusCompoundFileCompare);
    RUN_TEST_CASE(IusCompoundFile, iusCompoundFileSetGetFrameList);
    RUN_TEST_CASE(IusCompoundFile, iusCompoundFileSetGetPatternListDict);
    RUN_TEST_CASE(IusCompoundFile, iusCompoundFileSetGetPulseDict);
    RUN_TEST_CASE(IusCompoundFile, iusCompoundFileSetGetSourceDict);
    RUN_TEST_CASE(IusCompoundFile, iusCompoundFileSetGetReceiveChannelMapDict);
    RUN_TEST_CASE(IusCompoundFile, iusCompoundFileSetGetTransmitApodizationDict);
    //RUN_TEST_CASE(IusCompoundFile, iusCompoundFileSetGetReceiveSettingsDict);
    RUN_TEST_CASE(IusCompoundFile, iusCompoundFileSetGetAcquisition);
    RUN_TEST_CASE(IusCompoundFile, iusCompoundFileSetGetTransducer);
    RUN_TEST_CASE(IusCompoundFile, testIusCompoundFileSerialization);
    RUN_TEST_CASE(IusCompoundFile, testIusCompoundFileDataIOSaveFrame);
    RUN_TEST_CASE(IusCompoundFile, testIusCompoundFileDataIOSaveResponse);
    RUN_TEST_CASE(IusCompoundFile, testIusCompoundFileDataIOSaveChannel);
}
