//
// Created by Ruijzendaal on 16/04/2018.
//
#include <unity.h>

#include <include/ius.h>
#include <include/iusExperiment.h>
#include <include/iusNonParametricPulse.h>
#include <include/iusParametricPulse.h>
#include <include/iusPulseDict.h>
#include <include/iusPatternListDict.h>
#include <include/iusReceiveChannelMapDict.h>
#include <include/iusTransmitApodizationDict.h>

#include <testDataGenerators.h>
#include <include/iusSourceDict.h>
#include <include/ius3DParametricSource.h>
#include <include/ius3DNonParametricSource.h>
#include <include/ius3DTransducer.h>


static const char *pPulseLabel = "pulseLabel";
static const char *pSourceLabel = "sourceLabel";
static const char *pChannelMapLabel = "channelMapLabel";
static const char *pApodizationLabel = "apodizationLabel";
static const char *pReceivesettingsLabel = "receivesettingsLabel";


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
    void
)
{
    float sampleFrequency=4000;
    int numDelays=10;
    int numSamplesPerLine=10;
    int numTGCentries = 1;
    int status=0,i;

    iurs_t obj = iusReceiveSettingsCreate(sampleFrequency, numDelays, numSamplesPerLine, numTGCentries);

    // Delays
    for(i=0;i<numDelays;i++)
    {
        float delay = i*2.0f;
        status |= iusReceiveSettingsSetStartDelay(obj, i, delay);
        TEST_ASSERT(status == IUS_E_OK);
    }

    return obj;
}

iursd_t dgGenerateReceiveSettingsDict
(
    char *label
)
{
    int status=0;

    // Create
    iursd_t  dict = iusReceiveSettingsDictCreate();

    // Fill
    iurs_t obj = dgGenerateReceiveSettings();
    status |= iusReceiveSettingsDictSet(dict,label,obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    return dict;
}

iuhn_t dgGenerateHistoryNode
(

)
{
    char *type =  "TYPE3";
    int numParents = 0;
    iuhn_t node;

    node = iusHistoryNodeCreate(type,numParents);
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

iuif_t dgGenerateInputFile
(
    char *ptestFileName,
    char *transducerName,
    char *label,
    int numFrames
)
{
    // create
    iuif_t inputFile = iusInputFileCreate(ptestFileName);
    TEST_ASSERT(inputFile != IUIF_INVALID);

    // fill
    iufl_t frameList = dgGenerateFrameList(numFrames);
    int status = iusInputFileSetFrameList(inputFile,frameList);
    TEST_ASSERT(status == IUS_E_OK);

    iupald_t patternListDict = dgGeneratePatternListDict(label);
    status = iusInputFileSetPatternListDict(inputFile,patternListDict);
    TEST_ASSERT(status == IUS_E_OK);

    iupd_t pulseDict = dgGeneratePulseDict();
    status = iusInputFileSetPulseDict(inputFile, pulseDict);
    TEST_ASSERT(status == IUS_E_OK);

    iusd_t sourceDict = dgGenerateSourceDict();
    status = iusInputFileSetSourceDict(inputFile, sourceDict);
    TEST_ASSERT(status == IUS_E_OK);

    iurcmd_t receiveChannelMapDict = dgGenerateReceiveChannelMapDict(label);
    status = iusInputFileSetReceiveChannelMapDict(inputFile, receiveChannelMapDict);
    TEST_ASSERT(status == IUS_E_OK);

    iutad_t transmitApodizationDict = dgGenerateTransmitApodizationDict(label);
    status = iusInputFileSetTransmitApodizationDict(inputFile, transmitApodizationDict);
    TEST_ASSERT(status == IUS_E_OK);

    iursd_t receiveSettingsDict = dgGenerateReceiveSettingsDict(label);
    status = iusInputFileSetReceiveSettingsDict(inputFile, receiveSettingsDict);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);

    // save
    iue_t experiment = dgGenerateExperiment();
    status = iusInputFileSetExperiment(inputFile, experiment);
    TEST_ASSERT(status == IUS_E_OK);


    iut_t transducer = dgGenerateTransducer(transducerName);
    status = iusInputFileSetTransducer(inputFile, transducer);
    TEST_ASSERT(status == IUS_E_OK);

    return inputFile;
}



int dgInputFileAddGeneratedData
(
    iuif_t inputFile,
    char *label
)
{
    // fill
    iupald_t patternListDict = iusInputFileGetPatternListDict(inputFile);
    iupal_t patternList = dgGeneratePatternList(8,0.08f);
    iusPatternListDictSet(patternListDict,label,patternList);
    int status = iusInputFileSetPatternListDict(inputFile,patternListDict);
    TEST_ASSERT(status == IUS_E_OK);

    iurcmd_t receiveChannelMapDict = iusInputFileGetReceiveChannelMapDict(inputFile);
    iurcm_t receiveChannelMap = dgGenerateReceiveChannelMap();
    iusReceiveChannelMapDictSet(receiveChannelMapDict,label,receiveChannelMap);
    status = iusInputFileSetReceiveChannelMapDict(inputFile, receiveChannelMapDict);
    TEST_ASSERT(status == IUS_E_OK);

    iursd_t receiveSettingsDict = iusInputFileGetReceiveSettingsDict(inputFile);
    iurs_t receiveSettings = dgGenerateReceiveSettings();
    iusReceiveSettingsDictSet(receiveSettingsDict,label,receiveSettings);
    status = iusInputFileSetReceiveSettingsDict(inputFile, receiveSettingsDict);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);

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
    float timeInterval
)
{
    int i,status;
    iupal_t patternList = iusPatternListCreate(numPatterns);
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
    char *label
)
{
  int status;

  // fill list
  iupald_t patternListDict = iusPatternListDictCreate();
  iupal_t bmodePatternList = dgGeneratePatternList(1,0.01f);
  status = iusPatternListDictSet(patternListDict, label, bmodePatternList);
  TEST_ASSERT_EQUAL(IUS_E_OK, status);
  return patternListDict;
}

iupd_t dgGeneratePulseDict
(
    void
)
{
  int     numPulseValues=10;
  float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
  float   pulseAmplitude=800.0f;       /**< (max) amplitude of the pulse in Volts */
  int     pulseCount=10;               /**< number of cycles that the pulse represents */
  int     status=10;

  // create
  iupd_t dict = iusPulseDictCreate();
  TEST_ASSERT(dict != IUPD_INVALID);

  // fill
  char *parametricLabel = "parametricPulseLabel";
  char *nonParametricLabel = "nonParametricPulseLabel";
  iupp_t parametricPulse = iusParametricPulseCreate(pulseFrequency, pulseAmplitude, pulseCount);
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
    int locationCount = 5; /**< number of locations */
    float angularDelta = 0.13f;
    float FNumber = -0.955f;
    float startAngle = 3.14f;
    float startPhi = startAngle;
    float deltaPhi = angularDelta;
    char *_3d_non_parametric_label = "label for 3d non parametric source";
    char *_3d_parametric_label = "label for 3d parametric source";

    iu3dps_t parametricSource = ius3DParametricSourceCreate(locationCount, FNumber,
                                                              angularDelta, startAngle, deltaPhi, startPhi);

    TEST_ASSERT(parametricSource != IU3DPS_INVALID);
    iu3dps_t _nother3dps = ius3DParametricSourceCreate(locationCount, FNumber,
                                                         angularDelta, startAngle, deltaPhi, startPhi);

    TEST_ASSERT(_nother3dps != IU3DPS_INVALID);
    iu3dnps_t nonParametricSource = ius3DNonParametricSourceCreate(locationCount);
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
    void
)
{
    int status;
    int numChannels = 8;
    int channelMap[8] = {0, 1, 2, 3, 4, 5, 6, 7};

    // fill
    iurcm_t receiveChannelMap = iusReceiveChannelMapCreate(numChannels);
    TEST_ASSERT(receiveChannelMap != NULL);

    status = iusReceiveChannelMapSetMap(receiveChannelMap, channelMap);
    TEST_ASSERT(status == IUS_E_OK);

    status = iusReceiveChannelMapSetMap(receiveChannelMap, channelMap);
    TEST_ASSERT(status == IUS_E_OK);

    return receiveChannelMap;
}

iurcmd_t dgGenerateReceiveChannelMapDict
(
    char *label
)
{
    int status;
    iurcmd_t dict = iusReceiveChannelMapDictCreate();
    TEST_ASSERT(dict != IURCMD_INVALID);

    // fill
    iurcm_t receiveChannelMap = dgGenerateReceiveChannelMap();
    status = iusReceiveChannelMapDictSet(dict, label, receiveChannelMap);
    TEST_ASSERT(status == IUS_E_OK);
    return dict;
}

iue_t dgGenerateExperiment()
{
	int date = 20180416;
	char *pDescription = "a nice experiment that almost won me the nobel prize";
	float speedOfSound = 1540.0f;

	iue_t experiment = iusExperimentCreate(speedOfSound, date, pDescription);
	TEST_ASSERT(experiment != IUE_INVALID);

	return experiment;
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
        iu3dp_t elemPos = ius3DPositionCreate((10 - numTransducerElements / 2)*transducerPitch, 0.0f, 0.0f);
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

#if 0

iut_t create3DTransducer()
{
    char *pTransducerName = "created in testIusHLCreateTransducer";   /**< descriptive name of the ultrasound probe */
    float centerFrequency = 8000000;   /**< operating frequency of the transducer */
    int numElements = 1;          /**< number of transducer Elements in the probe */
    int elementIndex = 0;
    IusTransducerShape shape = IUS_PLANE;
    iut_t transducer;
    float x, y, z;
    float sx, sy, sz;
    float theta, phi;

    transducer = iusCreateTransducer(pTransducerName, shape, centerFrequency, numElements);
    TEST_ASSERT(transducer != IUT_INVALID);

    x = y = z = 0.0f;
    theta = phi = 0.0f;
    sx = 0.150f / 1000.0f; // meter
    sy = 0.0f;
    sz = 6.0f / 1000.0f;
    iu3dte_t _3dElement = iusUtilCreate3DElement(x, y, z, sx, sy, sz, theta, phi);
    int status = iusTransducerSetElement(transducer, elementIndex, _3dElement);
    TEST_ASSERT(status == IUS_E_OK);

    return transducer;
}

iut_t create2DTransducer()
{
    char *pTransducerName = "created in testIusHLCreate2DTransducer";   /**< descriptive name of the ultrasound probe */
    float centerFrequency = 8000000;   /**< operating frequency of the transducer */
    int numElements = 1;          /**< number of transducer Elements in the probe */
    int elementIndex = 0;
    IusTransducerShape shape = IUS_LINE;
    iut_t transducer;
    float x, z;
    float sx,  sz;
    float phi;

    transducer = iusCreateTransducer(pTransducerName, shape, centerFrequency, numElements);
    TEST_ASSERT(transducer != IUT_INVALID);

    x = z = 0.0f;
    phi = 0.0f;
    sx = 0.150f / 1000.0f; // meter
    sz = 6.0f / 1000.0f;
    iu2dte_t _2dElement = iusUtilCreate2DElement(x, z, sx, sz, phi);
    int status = iusTransducerSetElement(transducer, elementIndex, _2dElement);
    TEST_ASSERT(status == IUS_E_OK);

    return transducer;
}


int fillpatternlist(iutpal_t transmitPatternList, int size, float baseTime)
{
    int i;
    int pulseIndex,sourceIndex;
    int status=0;
    float time;

    for(i=0; i < size ; i++)
    {
        time = baseTime * i;
        pulseIndex = size - 1 - i;
        sourceIndex = size - 1 - i;
        status |= iusTransmitPatternListSet(transmitPatternList,time,sourceIndex,pulseIndex,i);
    }
    return status;
}

int filltransmitAppodization
(
    iuds_t drivingScheme,
    int numElements,
    int numTransmitPulses

)
{
    // By default, apodization should be initialized to 1.0f for all elements.
    float elementApodization;
    int pulseIndex;
    int elementIndex;
    int status=IUS_E_OK;

    for(pulseIndex=0 ;pulseIndex < numTransmitPulses ; pulseIndex++)
    {
        for (elementIndex = 0; elementIndex < numElements; elementIndex++)
        {
            elementApodization = 1.0f / ((pulseIndex % 10) + 1);
            status = iusDrivingSchemeSetTransmitApodization(drivingScheme, elementApodization, pulseIndex,
                                                            elementIndex);
            TEST_ASSERT(status == IUS_E_OK);
            TEST_ASSERT_EQUAL_FLOAT(elementApodization,
                                    iusDrivingSchemeGetTransmitApodization(drivingScheme, pulseIndex, elementIndex));
        }
    }
    return status;
}

iuds_t createDrivingScheme(IusShape shape)
{
    int numElements = 32;
    int numTransmitPulses = 26;
    int numTransmitSources = 13;
    int status = 0;
    iuds_t dri;
    IusSourceLocationType locationType = IUS_PARAMETRIC_3D_SOURCE_LOCATION;

    // Create transmit sources
    iusll_t transmitSources = iusCreateSourceLocationList( locationType, numTransmitSources );
    // fill
    status = fill3DSourceLocationList(transmitSources);

    // Create transmit pulses
    iutpl_t transmitPulses = iusCreateTransmitPulseList( numTransmitPulses );

    // fill
    status = fillTransmitPulseList(transmitPulses);

    // Create transmit pattern
    iutpal_t transmitPatterns = iusCreateTransmitPatternList( numTransmitPulses );
    status = fillTransmitPatternList(transmitPatterns);

    dri = iusCreateDrivingScheme( IUS_DIVERGING_WAVES_PARAMETRIZED,
                                    shape,
                                    transmitSources,
                                    transmitPulses,
                                    transmitPatterns,
                                    numElements );

    status = filltransmitAppodization(dri,numTransmitPulses,numTransmitSources);

    return dri;
}



iuh_t create2DTransducerHeader()
{
    // Generic settings
    float speedOfSound = 1498.1f;
    int date = 20160124;
    int status;
    char *pDescription = "My important experiment notes, by create3DTransducerHeader()";
    IusSourceLocationType locationType = IUS_PARAMETRIC_3D_SOURCE_LOCATION;


    iuh_t iuhHeader;
    iut_t tra;
    iue_t ex;
    iuds_t dri;

    iuhHeader = iusCreateInputHeader();
    TEST_ASSERT(iuhHeader != IUH_INVALID);

//    IusReceiveSettings * pReceiveSettings; /**< data receive settings */
//    IusDrivingScheme   * pDrivingScheme;   /**< data transmit settings */

    ex = iusCreateExperiment(speedOfSound, date, pDescription);
    status = iusHeaderSetExperiment(iuhHeader, ex);
    if (status != IUS_E_OK) return IUH_INVALID;

    tra = create2DTransducer();
    status = iusHeaderSetTransducer(iuhHeader, tra);
    if (status != IUS_E_OK) return IUH_INVALID;


    dri = createDrivingScheme(IUS_2D_SHAPE);
    status = iusHeaderSetDrivingScheme(iuhHeader, dri);
    if (status != IUS_E_OK) return IUH_INVALID;

    return iuhHeader;
}

iuh_t create3DTransducerHeader()
{
    // Generic settings
    float speedOfSound = 1498.1f;
    int date = 20160124;
    int status;
    char *pDescription = "My important experiment notes, by create3DTransducerHeader()";

    iuh_t iuhHeader;
    iut_t tra;
    iue_t ex;
    iuds_t dri;


    iuhHeader = iusCreateInputHeader();
    TEST_ASSERT(iuhHeader != IUH_INVALID);

//   Todo:
// IusReceiveSettings * pReceiveSettings; /**< data receive settings */

    ex = iusCreateExperiment(speedOfSound, date, pDescription);
    status = iusHeaderSetExperiment(iuhHeader, ex);
    if (status != IUS_E_OK) return IUH_INVALID;


    tra = create3DTransducer();
    status = iusHeaderSetTransducer(iuhHeader, tra);
    if (status != IUS_E_OK) return IUH_INVALID;

    dri = createDrivingScheme(IUS_3D_SHAPE);
    status = iusHeaderSetDrivingScheme(iuhHeader, dri);
    if (status != IUS_E_OK) return IUH_INVALID;

    return iuhHeader;
}


int fill3DSourceLocationList
(
    iusl_t sourceLocationList
)
{
    int status=0;
    int numTransmitSources;
    int index = 0;
    iu3dp_t _3dpos;
    float x,y,z;

    numTransmitSources= iusSourceListGetSize( sourceLocationList );
    for( index = 0 ; index < numTransmitSources; index++)
    {
        x = 0.05f*index;
        y = 0.01f*index;
        z = -0.05f;
        iu3dp_t _3dpos = iusCreate3DPosition(x, y, z);
        TEST_ASSERT_NOT_EQUAL(_3dpos,NULL);
        status = iusSourceListSet3DPosition( sourceLocationList, _3dpos, index );
        TEST_ASSERT_EQUAL(IUS_E_OK,status);
    }
    return status;

}

int nonParametricPulseFillAmplitudeTime2
(
    iunpp_t transmitPulse,
    float offset
)
{
    int i;
    int status=0;
    float pulseAmplitude;
    float pulseTimes;
    int numPulseValues = iusNonParametricPulseGetNumValues(transmitPulse);
    for(i=0; i< numPulseValues; i++)
    {
        pulseAmplitude = offset / ((i % 10) + 1.0f);
        pulseTimes = offset / ((i % 10) + 1.0f);
        status = iusNonParametricPulseSetAmplitudeTime(transmitPulse,pulseTimes,pulseAmplitude,i);
        if( status != IUS_E_OK )
        {
            return status;
        }
    }
    return status;
}

int nonParametricPulseFillAmplitudeTime
(
    iunpp_t transmitPulse
)
{
    int i;
    int status=0;
    float pulseAmplitude;
    float pulseTimes;
    int numPulseValues = iusNonParametricPulseGetNumValues(transmitPulse);
    for(i=0; i< numPulseValues; i++)
    {
        pulseAmplitude = ((i%3)-1)*40.0f;
        pulseTimes = i*0.00001;
        status = iusNonParametricPulseSetAmplitudeTime(transmitPulse,pulseTimes,pulseAmplitude,i);
        if( status != IUS_E_OK )
        {
            return status;
        }
    }
    return status;
}


int fillTransmitPulseList
(
    iupl_t transmitPulses
)
{
    int status=0;
    int index = 0;
    int numTransmitPulses=iusPulseListGetSize(transmitPulses);

    float pulseFrequency=5000000.0f;
    float pulseAmplitude=40.0f;
    int pulseCount=2;
    int numTransmitPulseValues=20;

    iup_t pulse;
    iunpp_t nonParametricPulse;

    for( index = 0 ; index < numTransmitPulses; index++)
    {
        if( index%2 )
        {
            // Parametric
            pulse = (iup_t) iusCreateParametricPulse(pulseFrequency,pulseAmplitude,pulseCount);
            TEST_ASSERT_NOT_EQUAL(pulse, NULL);
        }
        else
        {
            nonParametricPulse = iusCreateNonParametricPulse(numTransmitPulseValues);
            TEST_ASSERT_NOT_EQUAL(nonParametricPulse, NULL);
            status = nonParametricPulseFillAmplitudeTime(nonParametricPulse);
            pulse = (iup_t) nonParametricPulse;
            TEST_ASSERT_EQUAL(IUS_E_OK,status);
        }
        status = iusPulseListSet(transmitPulses,pulse,index);
    }
    return status;

}





int fillTransmitPatternList
(
    iutpal_t transmitPatterns
)
{
    int status=0;
    int index = 0;
    int sourceIndex = 0;
    int pulseIndex = 0;
    int numTransmitPatterns=iusTransmitPatternListGetSize(transmitPatterns);
    float time = 0.0f;

    iutpa_t pattern;

    for( index = 0 ; index < numTransmitPatterns; index++)
    {
        sourceIndex = index/2;
        pulseIndex = index;
        time = index * 0.0004f;
        status = iusTransmitPatternListSet(transmitPatterns,time,sourceIndex,pulseIndex,index);
        TEST_ASSERT_EQUAL(IUS_E_OK,status);
    }
    return status;
}



int fill2DSourceLocationList
    (
        iusl_t list
    )
{
    int status=0;
    int numTransmitSources;
    int index = 0;
    int  pos;
    iu2dp_t _2dpos;
    float x,z;

    numTransmitSources= iusSourceListGetSize( list );
    for( index = 0 ; index < numTransmitSources; index++)
    {
        x = index + 1.0*index;
        z = 2*index + 1.0*index;
        iu2dp_t _2dpos = iusCreate2DPosition(x, z);
        iusSourceListSet2DPosition( list, _2dpos, index );
    }
    return status;
}

int dgDeleteDrivingScheme
(
    iuds_t drivingScheme
)
{
    if( drivingScheme == NULL ) return IUS_ERR_VALUE;
    iusl_t transmitSources = iusDrivingSchemeGetSourceLocationList(drivingScheme);
    int status = iusDeleteSourceList( transmitSources );
    return status;
}


iuds_t dgCreateDrivingScheme
(
    IusDrivingSchemeType type,
    IusShape shape,
    IusSourceType locationType,
    int numTransmitPulses,
    int numTransmitSources,
    int numChannels,
    int numApodizations,
    int numTGCs,
    int numElements
)
{
    iuds_t parametrizedDrivingScheme;
    int status = 0;

    // Create transmit sources
    iusl_t transmitSources ;
    if( locationType == IUS_PARAMETRIC_3D_SOURCE_LOCATION )
    {
        // fill
        iu2dsl_t sources =iusCreate2DSourceList(numTransmitSources);
        status = fill3DSourceLocationList(sources);
        transmitSources = (iusl_t) sources;
    }
    else
    {
        iu3dsl_t sources =iusCreate3DSourceList(numTransmitSources);
        status = fill2DSourceLocationList(transmitSources);
        transmitSources = (iusl_t) sources;
    }

    // Create transmit pulses
    iupl_t transmitPulses = iusCreatePulseList( numTransmitPulses );

    // fill
    status = fillTransmitPulseList(transmitPulses);

    // Create transmit pattern
    iutpal_t transmitPatterns = iusCreateTransmitPatternList( numTransmitPulses );
    status = fillTransmitPatternList(transmitPatterns);

    iual_t  apodizations = iusCreateApodizationList(numApodizations,numElements);

    // fill
    parametrizedDrivingScheme = iusCreateDrivingScheme( type,
                                                          shape,
                                                          transmitSources,
                                                          transmitPulses,
                                                          transmitPatterns,
                                                          apodizations,
                                                          numElements );
    return parametrizedDrivingScheme;
}


#endif
