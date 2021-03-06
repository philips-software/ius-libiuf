#include <unity.h>

#include <iuf.h>
#include <dg/dataGenerators.h>



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
    int numSamples = iufDataGetSize(data);
    double *doubletjes = malloc(numSamples * sizeof(double));

    for (i=0; i<numSamples; i++)
    {
        doubletjes[i] = (float) value;
    }

    iufDataFill(data, doubletjes, numSamples);
    free(doubletjes);
}

iurs_t dgGenerateReceiveSettings
(
    int numSamplesPerLine
)
{
    float sampleFrequency=4000;
    int numTGCEntries = 1;
    iurs_t obj = iufReceiveSettingsCreate(sampleFrequency, numSamplesPerLine, numTGCEntries);

    return obj;
}

iudm_t dgGenerateDemodulation
(
	int numSamplesPerLine
)
{
	float sampleFrequency = 4000;
	float centerFrequency = 4000;
	int numTGCEntries = 1;
	IufDemodulationMethod method = IUF_DEMODULATION_FOURX;
	int kernelSize = 7;
    int i;

	iudm_t obj = iufDemodulationCreate(method, sampleFrequency, centerFrequency, numSamplesPerLine, numTGCEntries, kernelSize);

    iutgc_t myTGC = iufTGCCreate(numTGCEntries);
    iufTGCSet(myTGC, 0, 0.0, 1.0); //idx, time, gain
    iufDemodulationSetTGC(obj, myTGC);

    iuff_t preFilter = iufFirFilterCreate(kernelSize);
    for (i=0; i<kernelSize; i++)
    {
      iufFirFilterSetCoefficient(preFilter, i, 1.0/(float)kernelSize); //an averaging filter
    }
    iufDemodulationSetPreFilter(obj, preFilter);

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
    iursd_t  dict = iufReceiveSettingsDictCreate();

    // Fill
    iurs_t obj = dgGenerateReceiveSettings(numSamplesPerLine);
    status |= iufReceiveSettingsDictSet(dict,label,obj);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);
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
	iudmd_t  dict = iufDemodulationDictCreate();

	// Fill
	iudm_t obj = dgGenerateDemodulation(numSamplesPerLine);
	status |= iufDemodulationDictSet(dict, label, obj);
	TEST_ASSERT_EQUAL(IUF_E_OK, status);
	return dict;
}

iuhn_t dgGenerateHistoryNode
(

)
{
    char *type =  "TYPE3";
    iuhn_t node;

    node = iufHistoryNodeCreate(type);
    TEST_ASSERT_NOT_EQUAL(IUHN_INVALID,node);

    iupad_t parameterDict = dgGenerateParameterDict(10);
    iufHistoryNodeSetParameters(node, parameterDict);
    return node;
}

iupad_t dgGenerateParameterDict
(
    int numElements
)
{
    int elementID,status;
    iupad_t dict = iufParameterDictCreate();
    TEST_ASSERT(dict != IUPAD_INVALID);

    char generatedKey[1024];
    char generatedValue[1024];
    for (elementID=0;elementID<numElements;elementID++)
    {
        snprintf(generatedKey,sizeof(generatedKey),"key_%d", elementID);
        snprintf(generatedValue,sizeof(generatedValue),"value_%d", elementID);
        status = iufParameterDictSet(dict, generatedKey, generatedValue);
        TEST_ASSERT_EQUAL(IUF_E_OK,status);
    }

    return dict;
}

int dgDeleteIqFile
(
	iuiqf_t iufIqFile
)
{
	int status = iufFrameListDeepDelete(iufIqFileGetFrameList(iufIqFile));
	status |= iufReceiveChannelMapDictDeepDelete(iufIqFileGetReceiveChannelMapDict(iufIqFile));
	status |= iufTransducerDeepDelete(iufIqFileGetTransducer(iufIqFile));
	status |= iufTransmitApodizationDictDeepDelete(iufIqFileGetTransmitApodizationDict(iufIqFile));
	status |= iufIqPatternListDictDeepDelete(iufIqFileGetIqPatternListDict(iufIqFile));
	status |= iufPulseDictDeepDelete(iufIqFileGetPulseDict(iufIqFile));
	status |= iufSourceDictDeepDelete(iufIqFileGetSourceDict(iufIqFile));
	status |= iufAcquisitionDelete(iufIqFileGetAcquisition(iufIqFile));
	status |= iufIqFileDelete(iufIqFile);
	return status;
}

int dgDeleteInputFile
(
    iuif_t iufInputFile
)
{
    int status = iufFrameListDeepDelete(iufInputFileGetFrameList(iufInputFile));
    status |= iufReceiveChannelMapDictDeepDelete(iufInputFileGetReceiveChannelMapDict(iufInputFile));
    status |= iufTransducerDeepDelete(iufInputFileGetTransducer(iufInputFile));
    status |= iufTransmitApodizationDictDeepDelete(iufInputFileGetTransmitApodizationDict(iufInputFile));
    status |= iufReceiveSettingsDictDeepDelete(iufInputFileGetReceiveSettingsDict(iufInputFile));
    status |= iufPatternListDictDeepDelete(iufInputFileGetPatternListDict(iufInputFile));
    status |= iufPulseDictDeepDelete(iufInputFileGetPulseDict(iufInputFile));
    status |= iufSourceDictDeepDelete(iufInputFileGetSourceDict(iufInputFile));
    status |= iufAcquisitionDelete(iufInputFileGetAcquisition(iufInputFile));
    status |= iufInputFileDelete(iufInputFile);
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
    iuif_t inputFile = iufInputFileCreate(ptestFileName);
    TEST_ASSERT(inputFile != IUIF_INVALID);

    // fill
    iufl_t frameList = dgGenerateFrameList(numFrames);
    int status = iufInputFileSetFrameList(inputFile,frameList);
    TEST_ASSERT(status == IUF_E_OK);

    iupd_t pulseDict = dgGeneratePulseDict();
    status = iufInputFileSetPulseDict(inputFile, pulseDict);
    TEST_ASSERT(status == IUF_E_OK);

    iusd_t sourceDict = dgGenerateSourceDict();
    status = iufInputFileSetSourceDict(inputFile, sourceDict);
    TEST_ASSERT(status == IUF_E_OK);

    iurcmd_t receiveChannelMapDict = dgGenerateReceiveChannelMapDict(label, numChannels);
    status = iufInputFileSetReceiveChannelMapDict(inputFile, receiveChannelMapDict);
    TEST_ASSERT(status == IUF_E_OK);

    iutad_t transmitApodizationDict = dgGenerateTransmitApodizationDict(label);
    status = iufInputFileSetTransmitApodizationDict(inputFile, transmitApodizationDict);
    TEST_ASSERT(status == IUF_E_OK);

    iursd_t receiveSettingsDict = dgGenerateReceiveSettingsDict(label, numSamplesPerLine);
    status = iufInputFileSetReceiveSettingsDict(inputFile, receiveSettingsDict);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);

    iupald_t patternListDict = dgGeneratePatternListDict(label,receiveSettingsDict,receiveChannelMapDict);
    status = iufInputFileSetPatternListDict(inputFile,patternListDict);
    TEST_ASSERT(status == IUF_E_OK);

    // save
    iua_t acquisition = dgGenerateAcquisition();
    status = iufInputFileSetAcquisition(inputFile, acquisition);
    TEST_ASSERT(status == IUF_E_OK);


    iut_t transducer = dgGenerateTransducer(transducerName);
    status = iufInputFileSetTransducer(inputFile, transducer);
    TEST_ASSERT(status == IUF_E_OK);

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
	iuiqf_t iqFile = iufIqFileCreate(ptestFileName);
	TEST_ASSERT(iqFile != IUIQF_INVALID);

	// fill
	iufl_t frameList = dgGenerateFrameList(numFrames);
	int status = iufIqFileSetFrameList(iqFile, frameList);
	TEST_ASSERT(status == IUF_E_OK);

	iupd_t pulseDict = dgGeneratePulseDict();
	status = iufIqFileSetPulseDict(iqFile, pulseDict);
	TEST_ASSERT(status == IUF_E_OK);

	iusd_t sourceDict = dgGenerateSourceDict();
	status = iufIqFileSetSourceDict(iqFile, sourceDict);
	TEST_ASSERT(status == IUF_E_OK);

	iurcmd_t receiveChannelMapDict = dgGenerateReceiveChannelMapDict(label, numChannels);
	status = iufIqFileSetReceiveChannelMapDict(iqFile, receiveChannelMapDict);
	TEST_ASSERT(status == IUF_E_OK);

	iutad_t transmitApodizationDict = dgGenerateTransmitApodizationDict(label);
	status = iufIqFileSetTransmitApodizationDict(iqFile, transmitApodizationDict);
	TEST_ASSERT(status == IUF_E_OK);

	TEST_ASSERT(numSamplesPerLine >= 0);
	iudmd_t demodulationDict = dgGenerateDemodulationDict(label, numSamplesPerLine);
	status = iufIqFileSetDemodulationDict(iqFile, demodulationDict);
	TEST_ASSERT(status == IUF_E_OK);

	iuiqpald_t iqPatternListDict = dgGenerateIqPatternListDict(label, demodulationDict, receiveChannelMapDict);
	status = iufIqFileSetPatternListDict(iqFile, iqPatternListDict);
	TEST_ASSERT(status == IUF_E_OK);

	// save
	iua_t acquisition = dgGenerateAcquisition();
	status = iufIqFileSetAcquisition(iqFile, acquisition);
	TEST_ASSERT(status == IUF_E_OK);


	iut_t transducer = dgGenerateTransducer(transducerName);
	status = iufIqFileSetTransducer(iqFile, transducer);
	TEST_ASSERT(status == IUF_E_OK);

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
    iurcmd_t receiveChannelMapDict = iufInputFileGetReceiveChannelMapDict(inputFile);
    iurcm_t receiveChannelMap = dgGenerateReceiveChannelMap(numChannels);
    iufReceiveChannelMapDictSet(receiveChannelMapDict,label,receiveChannelMap);
    int status = iufInputFileSetReceiveChannelMapDict(inputFile, receiveChannelMapDict);
    TEST_ASSERT(status == IUF_E_OK);

    iursd_t receiveSettingsDict = iufInputFileGetReceiveSettingsDict(inputFile);
    iurs_t receiveSettings = dgGenerateReceiveSettings(numSamplesPerLine);
    iufReceiveSettingsDictSet(receiveSettingsDict,label,receiveSettings);
    status = iufInputFileSetReceiveSettingsDict(inputFile, receiveSettingsDict);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);

    iupald_t patternListDict = iufInputFileGetPatternListDict(inputFile);
    iupal_t patternList = dgGeneratePatternList(8,0.08f,receiveSettingsDict,receiveChannelMapDict);
    iufPatternListDictSet(patternListDict,label,patternList);
    status = iufInputFileSetPatternListDict(inputFile,patternListDict);
    TEST_ASSERT(status == IUF_E_OK);
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

	iurcmd_t receiveChannelMapDict = iufInputFileGetReceiveChannelMapDict(iqFile);
	iurcm_t receiveChannelMap = dgGenerateReceiveChannelMap(numChannels);
	iufReceiveChannelMapDictSet(receiveChannelMapDict, label, receiveChannelMap);
	int status = iufIqFileSetReceiveChannelMapDict(iqFile, receiveChannelMapDict);
	TEST_ASSERT(status == IUF_E_OK);

	iudmd_t demodulationDict = iufIqFileGetDemodulationDict(iqFile);
	iudm_t demodulation = dgGenerateDemodulation(numSamplesPerLine);
	iufDemodulationDictSet(demodulationDict, label, demodulation);
	status = iufIqFileSetDemodulationDict(iqFile, demodulationDict);
	TEST_ASSERT(status == IUF_E_OK);

	iuiqpald_t patternListDict = iufIqFileGetIqPatternListDict(iqFile);
	iuiqpal_t patternList = dgGenerateIqPatternList(8, 0.08f, demodulationDict, receiveChannelMapDict);
	iufIqPatternListDictSet(patternListDict, label, patternList);
	status = iufIqFileSetPatternListDict(iqFile, patternListDict);
	TEST_ASSERT(status == IUF_E_OK);

	return status;
}

iufl_t dgGenerateFrameList
(
    int numFrames
)
{
    iufr_t obj;
    int status,i;
    iufl_t frameList = iufFrameListCreate(numFrames);
    TEST_ASSERT_NOT_EQUAL(IUFL_INVALID, frameList);

    for (i=0;i<numFrames;i++)
    {
        obj = iufFrameCreate("test",i+2,i*0.01f);
        status = iufFrameListSet(frameList, obj, i);
        TEST_ASSERT_EQUAL(IUF_E_OK, status);
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
    iupal_t patternList = iufPatternListCreate(numPatterns,receiveSettingsDict,receiveChannelMapDict);
    TEST_ASSERT_NOT_EQUAL(IUPAL_INVALID, patternList);

    for (i=0;i<numPatterns;i++)
    {
        iupa_t pattern = iufPatternCreate((i + 1) * timeInterval,
                                          pPulseLabel,
                                          pSourceLabel,
                                          pChannelMapLabel,
                                          pApodizationLabel,
                                          pReceivesettingsLabel);
        status = iufPatternListSet(patternList, pattern, i);
        TEST_ASSERT_EQUAL(IUF_E_OK, status);
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
  iupald_t patternListDict = iufPatternListDictCreate();
  iupal_t bmodePatternList = dgGeneratePatternList(1,0.01f,receiveSettingsDict,receiveChannelMapDict);
  status = iufPatternListDictSet(patternListDict, label, bmodePatternList);
  TEST_ASSERT_EQUAL(IUF_E_OK, status);
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
	iuiqpal_t iqPatternList = iufIqPatternListCreate(numPatterns, demodulationDict, receiveChannelMapDict);
	TEST_ASSERT_NOT_EQUAL(IUIQPAL_INVALID, iqPatternList);

	for (i = 0; i<numPatterns; i++)
	{
		iuiqpa_t pattern = iufIqPatternCreate((i + 1) * timeInterval,
			pPulseLabel,
			pSourceLabel,
			pChannelMapLabel,
			pApodizationLabel,
			pDemodulationLabel);
		status = iufIqPatternListSet(iqPatternList, pattern, i);
		TEST_ASSERT_EQUAL(IUF_E_OK, status);
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
	iuiqpald_t iqPatternListDict = iufIqPatternListDictCreate();
	iuiqpal_t bmodeIqPatternList = dgGenerateIqPatternList(1, 0.01f, demodulationDict, receiveChannelMapDict);
	status = iufIqPatternListDictSet(iqPatternListDict, label, bmodeIqPatternList);
	TEST_ASSERT_EQUAL(IUF_E_OK, status);
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
  iupd_t dict = iufPulseDictCreate();
  TEST_ASSERT(dict != IUPD_INVALID);

  // fill
  char *parametricLabel = "parametricPulseLabel";
  char *nonParametricLabel = "nonParametricPulseLabel";
  iupp_t parametricPulse = iufParametricPulseCreate(pulseFrequency, pulseAmplitude, numPulses);
  iunpp_t nonParametricPulse = iufNonParametricPulseCreate(numPulseValues);
  iufNonParametricPulseSetValue(nonParametricPulse,0,10.0f,10.0f);
  iufNonParametricPulseSetValue(nonParametricPulse,1,20.0f,10.0f);
  status = iufPulseDictSet(dict,nonParametricLabel, (iup_t) nonParametricPulse);
  TEST_ASSERT(status == IUF_E_OK);
  status = iufPulseDictSet(dict,parametricLabel, (iup_t) parametricPulse);
  TEST_ASSERT(status == IUF_E_OK);
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

    iu3dps_t parametricSource = iuf3DParametricSourceCreate(numLocationsTheta, numLocationsPhi, FNumber, deltaTheta, startTheta, deltaPhi, startPhi);

    TEST_ASSERT(parametricSource != IU3DPS_INVALID);
    iu3dnps_t nonParametricSource = iuf3DNonParametricSourceCreate(numLocationsTheta*numLocationsPhi);
    TEST_ASSERT(nonParametricSource != IU3DNPS_INVALID);

    // create
    iusd_t dict = iufSourceDictCreate();
    TEST_ASSERT(dict != IUFD_INVALID);

    // fill
    int status = iufSourceDictSet(dict, _3d_parametric_label,(ius_t) parametricSource);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);
    status = iufSourceDictSet(dict, _3d_non_parametric_label,(ius_t) nonParametricSource);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);
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
    iurcm_t receiveChannelMap = iufReceiveChannelMapCreate(numChannels);
    TEST_ASSERT(receiveChannelMap != NULL);

    status = iufReceiveChannelMapSetMap(receiveChannelMap, channelMap);
    TEST_ASSERT(status == IUF_E_OK);

    status = iufReceiveChannelMapSetMap(receiveChannelMap, channelMap);
    TEST_ASSERT(status == IUF_E_OK);

    // Delays
    for(i=0;i<numChannels;i++)
    {
        float delay = i*2.0f;
        status |= iufReceiveChannelMapSetStartDelay(receiveChannelMap, i, delay);
        TEST_ASSERT(status == IUF_E_OK);
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
    iurcmd_t dict = iufReceiveChannelMapDictCreate();
    TEST_ASSERT(dict != IURCMD_INVALID);

    // fill
    iurcm_t receiveChannelMap = dgGenerateReceiveChannelMap(numChannels);
    status = iufReceiveChannelMapDictSet(dict, label, receiveChannelMap);
    TEST_ASSERT(status == IUF_E_OK);
    return dict;
}

iua_t dgGenerateAcquisition()
{
	int date = 20180416;
	char *pDescription = "a nice acquisition that almost won me the nobel prize";
	float speedOfSound = 1540.0f;

	iua_t acquisition = iufAcquisitionCreate(speedOfSound, date, pDescription);
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

    iu3dt_t transducer = iuf3DTransducerCreate(transducerName, IUF_PLANE, 2500000.0f, numTransducerElements);
    TEST_ASSERT(transducer != IU3DT_INVALID);
    for (i = 0; i < numTransducerElements; i++)
    {
        iu3dp_t elemPos = iuf3DPositionCreate((float)(10.0-numTransducerElements/2.0)*transducerPitch, 0.0f, 0.0f);
        iu3ds_t elemSize = iuf3DSizeCreate(0.0001f,0.0001f,0.0001f);
        iu3da_t elemAngle = iuf3DAngleCreate(0.0f,0.3f);
        iu3dte_t element = iuf3DTransducerElementCreate(elemPos, elemAngle, elemSize);
        iuf3DTransducerSetElement(transducer, i, element);
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

	iutad_t dict = iufTransmitApodizationDictCreate();
	TEST_ASSERT(dict != IUTAD_INVALID);

	// fill
	iuta_t transmitApodization = iufTransmitApodizationCreate(apodizaton, numElements);
	TEST_ASSERT(transmitApodization != NULL);


	status = iufTransmitApodizationDictSet(dict, label, transmitApodization);
	TEST_ASSERT(status == IUF_E_OK);

	return dict;
}

