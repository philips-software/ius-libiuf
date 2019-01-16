//
// Created by Ruijzendaal on 16/04/2018.
//
#include <unity.h>

#include <include/ius.h>
#include <testDataGenerators.h>



static const char *pPulseLabel = "pulseLabel";
static const char *pSourceLabel = "sourceLabel";
static const char *pChannelMapLabel = "channelMapLabel";
static const char *pApodizationLabel = "apodizationLabel";
static const char *pReceivesettingsLabel = "receivesettingsLabel";
static const char *pDemodulationLabel = "demodulationLabel";

void dgFillData
(
    iud_t data,
    float value
)
{
    int i;
    float *pData = iusDataGetPointer(data);
    int numSamples = iusDataGetSize(data);

    for (i=0; i<numSamples; i++)
    {
        pData[i] = value;
    }
}

iurs_t dgGenerateReceiveSettings
(
    int numSamplesPerLine
)
{
    float sampleFrequency=4000;
    int numTGCentries = 1;
    iurs_t obj = iusReceiveSettingsCreate(sampleFrequency, numSamplesPerLine, numTGCentries);

    return obj;
}

iudm_t dgGenerateDemodulation
(
	int numSamplesPerLine
)
{
	float sampleFrequency = 4000;
	int numTGCentries = 1;
	iudm_t obj = iusDemodulationCreate(sampleFrequency, numSamplesPerLine, numTGCentries);

	return obj;
}

iursd_t dgGenerateReceiveSettingsDict
(
    char *label,
    int numSamplesPerLine
)
{
    int status=0;

    // Create
    iursd_t  dict = iusReceiveSettingsDictCreate();

    // Fill
    iurs_t obj = dgGenerateReceiveSettings(numSamplesPerLine);
    status |= iusReceiveSettingsDictSet(dict,label,obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    return dict;
}

iudmd_t dgGenerateDemodulationDict
(
	char *label,
	int numSamplesPerLine
)
{
	int status = 0;

	// Create
	iudmd_t  dict = iusDemodulationDictCreate();

	// Fill
	iudm_t obj = dgGenerateDemodulation(numSamplesPerLine);
	status |= iusDemodulationDictSet(dict, label, obj);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);
	return dict;
}

iuhn_t dgGenerateHistoryNode
(

)
{
    char *type =  "TYPE3";
    iuhn_t node;

    node = iusHistoryNodeCreate(type);
    TEST_ASSERT_NOT_EQUAL(IUHN_INVALID,node);

    iupad_t parameterDict = dgGenerateParameterDict(10);
    iusHistoryNodeSetParameters(node, parameterDict);
    return node;
}

iupad_t dgGenerateParameterDict
(
    int numElements
)
{
    int elementID,status;
    iupad_t dict = iusParameterDictCreate();
    TEST_ASSERT(dict != IUPAD_INVALID);

    char generatedKey[1024];
    char generatedValue[1024];
    for (elementID=0;elementID<numElements;elementID++)
    {
        sprintf(generatedKey,"key_%d", elementID);
        sprintf(generatedValue,"value_%d", elementID);
        status = iusParameterDictSet(dict, generatedKey, generatedValue);
        TEST_ASSERT_EQUAL(IUS_E_OK,status);
    }

    return dict;
}

int dgDeleteIqFile
(
	iuiqf_t iusIqFile
)
{
	int status = iusFrameListDeepDelete(iusIqFileGetFrameList(iusIqFile));
	status |= iusReceiveChannelMapDictDeepDelete(iusInputFileGetReceiveChannelMapDict(iusIqFile));
	status |= iusTransducerDeepDelete(iusInputFileGetTransducer(iusIqFile));
	status |= iusTransmitApodizationDictDeepDelete(iusInputFileGetTransmitApodizationDict(iusIqFile));
	status |= iusPatternListDictDeepDelete(iusInputFileGetPatternListDict(iusIqFile));
	status |= iusPulseDictDeepDelete(iusInputFileGetPulseDict(iusIqFile));
	status |= iusSourceDictDeepDelete(iusInputFileGetSourceDict(iusIqFile));
	status |= iusAcquisitionDelete(iusInputFileGetAcquisition(iusIqFile));
	status |= iusIqFileDelete(iusIqFile);
	return status;
}

int dgDeleteInputFile
(
    iuif_t iusInputFile
)
{
    int status = iusFrameListDeepDelete(iusInputFileGetFrameList(iusInputFile));
    status |= iusReceiveChannelMapDictDeepDelete(iusInputFileGetReceiveChannelMapDict(iusInputFile));
    status |= iusTransducerDeepDelete(iusInputFileGetTransducer(iusInputFile));
    status |= iusTransmitApodizationDictDeepDelete(iusInputFileGetTransmitApodizationDict(iusInputFile));
    status |= iusReceiveSettingsDictDeepDelete(iusInputFileGetReceiveSettingsDict(iusInputFile));
    status |= iusPatternListDictDeepDelete(iusInputFileGetPatternListDict(iusInputFile));
    status |= iusPulseDictDeepDelete(iusInputFileGetPulseDict(iusInputFile));
    status |= iusSourceDictDeepDelete(iusInputFileGetSourceDict(iusInputFile));
    status |= iusAcquisitionDelete(iusInputFileGetAcquisition(iusInputFile));
    status |= iusInputFileDelete(iusInputFile);
    return status;
}

iuif_t dgGenerateInputFile
(
    char *ptestFileName,
    char *transducerName,
    char *label,
    int numFrames,
    int numSamplesPerLine,
    int numChannels
)
{
    // create
    iuif_t inputFile = iusInputFileCreate(ptestFileName);
    TEST_ASSERT(inputFile != IUIF_INVALID);

    // fill
    iufl_t frameList = dgGenerateFrameList(numFrames);
    int status = iusInputFileSetFrameList(inputFile,frameList);
    TEST_ASSERT(status == IUS_E_OK);

    iupd_t pulseDict = dgGeneratePulseDict();
    status = iusInputFileSetPulseDict(inputFile, pulseDict);
    TEST_ASSERT(status == IUS_E_OK);

    iusd_t sourceDict = dgGenerateSourceDict();
    status = iusInputFileSetSourceDict(inputFile, sourceDict);
    TEST_ASSERT(status == IUS_E_OK);

    iurcmd_t receiveChannelMapDict = dgGenerateReceiveChannelMapDict(label, numChannels);
    status = iusInputFileSetReceiveChannelMapDict(inputFile, receiveChannelMapDict);
    TEST_ASSERT(status == IUS_E_OK);

    iutad_t transmitApodizationDict = dgGenerateTransmitApodizationDict(label);
    status = iusInputFileSetTransmitApodizationDict(inputFile, transmitApodizationDict);
    TEST_ASSERT(status == IUS_E_OK);

    iursd_t receiveSettingsDict = dgGenerateReceiveSettingsDict(label, numSamplesPerLine);
    status = iusInputFileSetReceiveSettingsDict(inputFile, receiveSettingsDict);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);

    iupald_t patternListDict = dgGeneratePatternListDict(label,receiveSettingsDict,receiveChannelMapDict);
    status = iusInputFileSetPatternListDict(inputFile,patternListDict);
    TEST_ASSERT(status == IUS_E_OK);

    // save
    iua_t acquisition = dgGenerateAcquisition();
    status = iusInputFileSetAcquisition(inputFile, acquisition);
    TEST_ASSERT(status == IUS_E_OK);


    iut_t transducer = dgGenerateTransducer(transducerName);
    status = iusInputFileSetTransducer(inputFile, transducer);
    TEST_ASSERT(status == IUS_E_OK);

    return inputFile;
}

iuiqf_t dgGenerateIqFile
(
	char *ptestFileName,
	char *transducerName,
	char *label,
	int numFrames,
	int numSamplesPerLine,
	int numChannels
)
{
	// create
	iuiqf_t iqFile = iusIqFileCreate(ptestFileName);
	TEST_ASSERT(iqFile != IUIQF_INVALID);

	// fill
	iufl_t frameList = dgGenerateFrameList(numFrames);
	int status = iusIqFileSetFrameList(iqFile, frameList);
	TEST_ASSERT(status == IUS_E_OK);

	iupd_t pulseDict = dgGeneratePulseDict();
	status = iusIqFileSetPulseDict(iqFile, pulseDict);
	TEST_ASSERT(status == IUS_E_OK);

	iusd_t sourceDict = dgGenerateSourceDict();
	status = iusIqFileSetSourceDict(iqFile, sourceDict);
	TEST_ASSERT(status == IUS_E_OK);

	iurcmd_t receiveChannelMapDict = dgGenerateReceiveChannelMapDict(label, numChannels);
	status = iusIqFileSetReceiveChannelMapDict(iqFile, receiveChannelMapDict);
	TEST_ASSERT(status == IUS_E_OK);

	iutad_t transmitApodizationDict = dgGenerateTransmitApodizationDict(label);
	status = iusIqFileSetTransmitApodizationDict(iqFile, transmitApodizationDict);
	TEST_ASSERT(status == IUS_E_OK);

	TEST_ASSERT(numSamplesPerLine >= 0);
	iudmd_t demodulationDict = dgGenerateDemodulationDict(label, numSamplesPerLine);
	status = iusIqFileSetDemodulationDict(iqFile, demodulationDict);
	TEST_ASSERT(status == IUS_E_OK);

	iuiqpald_t iqPatternListDict = dgGenerateIqPatternListDict(label, demodulationDict, receiveChannelMapDict);
	status = iusIqFileSetPatternListDict(iqFile, iqPatternListDict);
	TEST_ASSERT(status == IUS_E_OK);

	// save
	iua_t acquisition = dgGenerateAcquisition();
	status = iusIqFileSetAcquisition(iqFile, acquisition);
	TEST_ASSERT(status == IUS_E_OK);


	iut_t transducer = dgGenerateTransducer(transducerName);
	status = iusIqFileSetTransducer(iqFile, transducer);
	TEST_ASSERT(status == IUS_E_OK);

	return iqFile;
}

int dgInputFileAddGeneratedData
(
    iuif_t inputFile,
    char *label,
    int numSamplesPerLine,
    int numChannels
)
{
    // fill
    iurcmd_t receiveChannelMapDict = iusInputFileGetReceiveChannelMapDict(inputFile);
    iurcm_t receiveChannelMap = dgGenerateReceiveChannelMap(numChannels);
    iusReceiveChannelMapDictSet(receiveChannelMapDict,label,receiveChannelMap);
    int status = iusInputFileSetReceiveChannelMapDict(inputFile, receiveChannelMapDict);
    TEST_ASSERT(status == IUS_E_OK);

    iursd_t receiveSettingsDict = iusInputFileGetReceiveSettingsDict(inputFile);
    iurs_t receiveSettings = dgGenerateReceiveSettings(numSamplesPerLine);
    iusReceiveSettingsDictSet(receiveSettingsDict,label,receiveSettings);
    status = iusInputFileSetReceiveSettingsDict(inputFile, receiveSettingsDict);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);

    iupald_t patternListDict = iusInputFileGetPatternListDict(inputFile);
    iupal_t patternList = dgGeneratePatternList(8,0.08f,receiveSettingsDict,receiveChannelMapDict);
    iusPatternListDictSet(patternListDict,label,patternList);
    status = iusInputFileSetPatternListDict(inputFile,patternListDict);
    TEST_ASSERT(status == IUS_E_OK);

    return status;
}

int dgIqFileAddGeneratedData
(
	iuiqf_t iqFile,
	char *label,
	int numSamplesPerLine,
	int numChannels
)
{
	// fill
	TEST_ASSERT(numSamplesPerLine >= 0);
	TEST_ASSERT(numChannels >= 0);

	iurcmd_t receiveChannelMapDict = iusInputFileGetReceiveChannelMapDict(iqFile);
	iurcm_t receiveChannelMap = dgGenerateReceiveChannelMap(numChannels);
	iusReceiveChannelMapDictSet(receiveChannelMapDict, label, receiveChannelMap);
	int status = iusIqFileSetReceiveChannelMapDict(iqFile, receiveChannelMapDict);
	TEST_ASSERT(status == IUS_E_OK);

	iudmd_t demodulationDict = iusIqFileGetDemodulationDict(iqFile);
	iudm_t demodulation = dgGenerateDemodulation(numSamplesPerLine);
	iusDemodulationDictSet(demodulationDict, label, demodulation);
	status = iusIqFileSetDemodulationDict(iqFile, demodulationDict);
	TEST_ASSERT(status == IUS_E_OK);

	iuiqpald_t patternListDict = iusIqFileGetPatternListDict(iqFile);
	iuiqpal_t patternList = dgGenerateIqPatternList(8, 0.08f, demodulationDict, receiveChannelMapDict);
	iusIqPatternListDictSet(patternListDict, label, patternList);
	status = iusIqFileSetPatternListDict(iqFile, patternListDict);
	TEST_ASSERT(status == IUS_E_OK);

	return status;
}

iufl_t dgGenerateFrameList
(
    int numFrames
)
{
    iufr_t obj;
    int status,i;
    iufl_t frameList = iusFrameListCreate(numFrames);
    TEST_ASSERT_NOT_EQUAL(IUFL_INVALID, frameList);

    for (i=0;i<numFrames;i++)
    {
        obj = iusFrameCreate("test",i+2,i*0.01f);
        status = iusFrameListSet(frameList, obj, i);
        TEST_ASSERT_EQUAL(IUS_E_OK, status);
    }
    return frameList;
}

iupal_t dgGeneratePatternList
(
    int numPatterns,
    float timeInterval,
    iursd_t receiveSettingsDict,
    iurcmd_t receiveChannelMapDict
)
{
    int i,status;
    iupal_t patternList = iusPatternListCreate(numPatterns,receiveSettingsDict,receiveChannelMapDict);
    TEST_ASSERT_NOT_EQUAL(IUPAL_INVALID, patternList);

    for (i=0;i<numPatterns;i++)
    {
        iupa_t pattern = iusPatternCreate((i + 1) * timeInterval,
                                          pPulseLabel,
                                          pSourceLabel,
                                          pChannelMapLabel,
                                          pApodizationLabel,
                                          pReceivesettingsLabel);
        status = iusPatternListSet(patternList, pattern, i);
        TEST_ASSERT_EQUAL(IUS_E_OK, status);
    }

    return patternList;
}

iupald_t dgGeneratePatternListDict
(
    char *label,
    iursd_t receiveSettingsDict,
    iurcmd_t receiveChannelMapDict
)
{
  int status;

  // fill list
  iupald_t patternListDict = iusPatternListDictCreate();
  iupal_t bmodePatternList = dgGeneratePatternList(1,0.01f,receiveSettingsDict,receiveChannelMapDict);
  status = iusPatternListDictSet(patternListDict, label, bmodePatternList);
  TEST_ASSERT_EQUAL(IUS_E_OK, status);
  return patternListDict;
}

iuiqpal_t dgGenerateIqPatternList
(
	int numPatterns,
	float timeInterval,
	iudmd_t demodulationDict,
	iurcmd_t receiveChannelMapDict
)
{
	int i, status;
	iuiqpal_t iqPatternList = iusIqPatternListCreate(numPatterns, demodulationDict, receiveChannelMapDict);
	TEST_ASSERT_NOT_EQUAL(IUIQPAL_INVALID, iqPatternList);

	for (i = 0; i<numPatterns; i++)
	{
		iuiqpa_t pattern = iusIqPatternCreate((i + 1) * timeInterval,
			pPulseLabel,
			pSourceLabel,
			pChannelMapLabel,
			pApodizationLabel,
			pDemodulationLabel);
		status = iusIqPatternListSet(iqPatternList, pattern, i);
		TEST_ASSERT_EQUAL(IUS_E_OK, status);
	}

	return iqPatternList;
}

iuiqpald_t dgGenerateIqPatternListDict
(
	char *label,
	iudmd_t demodulationDict,
	iurcmd_t receiveChannelMapDict
)
{
	int status;

	// fill list
	iuiqpald_t iqPatternListDict = iusIqPatternListDictCreate();
	iuiqpal_t bmodeIqPatternList = dgGenerateIqPatternList(1, 0.01f, demodulationDict, receiveChannelMapDict);
	status = iusIqPatternListDictSet(iqPatternListDict, label, bmodeIqPatternList);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);
	return iqPatternListDict;
}



iupd_t dgGeneratePulseDict
(
    void
)
{
  int     numPulseValues=10;
  float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
  float   pulseAmplitude=800.0f;       /**< (max) amplitude of the pulse in Volts */
  int     numPulses=10;               /**< number of cycles that the pulse represents */
  int     status;

  // create
  iupd_t dict = iusPulseDictCreate();
  TEST_ASSERT(dict != IUPD_INVALID);

  // fill
  char *parametricLabel = "parametricPulseLabel";
  char *nonParametricLabel = "nonParametricPulseLabel";
  iupp_t parametricPulse = iusParametricPulseCreate(pulseFrequency, pulseAmplitude, numPulses);
  iunpp_t nonParametricPulse = iusNonParametricPulseCreate(numPulseValues);
  iusNonParametricPulseSetValue(nonParametricPulse,0,10.0f,10.0f);
  iusNonParametricPulseSetValue(nonParametricPulse,1,20.0f,10.0f);
  status = iusPulseDictSet(dict,nonParametricLabel, (iup_t) nonParametricPulse);
  TEST_ASSERT(status == IUS_E_OK);
  status = iusPulseDictSet(dict,parametricLabel, (iup_t) parametricPulse);
  TEST_ASSERT(status == IUS_E_OK);
  return dict;
}

iusd_t dgGenerateSourceDict
(
    void
)
{
    int numLocationsTheta = 5; /**< number of locations */
	int numLocationsPhi = 5; /**< number of locations */
    float angularDelta = 0.13f;
    float FNumber = -0.955f;
    float startAngle = 3.14f;
    float startPhi = startAngle;
    float deltaPhi = angularDelta;
	float startTheta = startAngle;
	float deltaTheta = angularDelta;
    char *_3d_non_parametric_label = "label for 3d non parametric source";
    char *_3d_parametric_label = "label for 3d parametric source";

    iu3dps_t parametricSource = ius3DParametricSourceCreate(numLocationsTheta, numLocationsPhi, FNumber, deltaTheta, startTheta, deltaPhi, startPhi);

    TEST_ASSERT(parametricSource != IU3DPS_INVALID);
    iu3dnps_t nonParametricSource = ius3DNonParametricSourceCreate(numLocationsTheta*numLocationsPhi);
    TEST_ASSERT(nonParametricSource != IU3DNPS_INVALID);

    // create
    iusd_t dict = iusSourceDictCreate();
    TEST_ASSERT(dict != IUSD_INVALID);

    // fill
    int status = iusSourceDictSet(dict, _3d_parametric_label,(ius_t) parametricSource);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    status = iusSourceDictSet(dict, _3d_non_parametric_label,(ius_t) nonParametricSource);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    return dict;
}


iurcm_t dgGenerateReceiveChannelMap
(
    int numChannels
)
{
    int status,i;
    int channelMap[8] = {0, 1, 2, 3, 4, 5, 6, 7};

    // fill
    iurcm_t receiveChannelMap = iusReceiveChannelMapCreate(numChannels);
    TEST_ASSERT(receiveChannelMap != NULL);

    status = iusReceiveChannelMapSetMap(receiveChannelMap, channelMap);
    TEST_ASSERT(status == IUS_E_OK);

    status = iusReceiveChannelMapSetMap(receiveChannelMap, channelMap);
    TEST_ASSERT(status == IUS_E_OK);

    // Delays
    for(i=0;i<numChannels;i++)
    {
        float delay = i*2.0f;
        status |= iusReceiveChannelMapSetStartDelay(receiveChannelMap, i, delay);
        TEST_ASSERT(status == IUS_E_OK);
    }


    return receiveChannelMap;
}

iurcmd_t dgGenerateReceiveChannelMapDict
(
    char *label,
    int numChannels
)
{
    int status;
    iurcmd_t dict = iusReceiveChannelMapDictCreate();
    TEST_ASSERT(dict != IURCMD_INVALID);

    // fill
    iurcm_t receiveChannelMap = dgGenerateReceiveChannelMap(numChannels);
    status = iusReceiveChannelMapDictSet(dict, label, receiveChannelMap);
    TEST_ASSERT(status == IUS_E_OK);
    return dict;
}

iua_t dgGenerateAcquisition()
{
	int date = 20180416;
	char *pDescription = "a nice acquisition that almost won me the nobel prize";
	float speedOfSound = 1540.0f;

	iua_t acquisition = iusAcquisitionCreate(speedOfSound, date, pDescription);
	TEST_ASSERT(acquisition != IUA_INVALID);

	return acquisition;
}


iut_t dgGenerateTransducer
(
    char *transducerName
)
{
    const int numTransducerElements = 128;
    int i = 0;

    // create and fill
    const float transducerPitch = 0.000005f;

    iu3dt_t transducer = ius3DTransducerCreate(transducerName, IUS_PLANE, 2500000.0f, numTransducerElements);
    TEST_ASSERT(transducer != IU3DT_INVALID);
    for (i = 0; i < numTransducerElements; i++)
    {
        iu3dp_t elemPos = ius3DPositionCreate((float)(10.0-numTransducerElements/2.0)*transducerPitch, 0.0f, 0.0f);
        iu3ds_t elemSize = ius3DSizeCreate(0.0001f,0.0001f,0.0001f);
        iu3da_t elemAngle = ius3DAngleCreate(0.0f,0.3f);
        iu3dte_t element = ius3DTransducerElementCreate(elemPos, elemAngle, elemSize);
        ius3DTransducerSetElement(transducer, i, element);
    }

    return  (iut_t)transducer;
}

iutad_t dgGenerateTransmitApodizationDict
(
    char *label
)
{
	int status;
	int numElements = 8;
	float apodizaton[8] = { 0.5f, 1.0f, 1.0f, 1.0f, .0f, 1.0f, .0f, 0.5f };

	iutad_t dict = iusTransmitApodizationDictCreate();
	TEST_ASSERT(dict != IUTAD_INVALID);

	// fill
	iuta_t transmitApodization = iusTransmitApodizationCreate(numElements);
	TEST_ASSERT(transmitApodization != NULL);

	status = iusTransmitApodizationSetApodization(transmitApodization, apodizaton);
	TEST_ASSERT(status == IUS_E_OK);

	status = iusTransmitApodizationDictSet(dict, label, transmitApodization);
	TEST_ASSERT(status == IUS_E_OK);

	return dict;
}

