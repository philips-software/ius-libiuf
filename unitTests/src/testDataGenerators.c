//
// Created by Ruijzendaal on 16/04/2018.
//
#include <unity.h>

#include <include/ius.h>
#include <include/iusHLExperiment.h>
#include <include/iusHLNonParametricPulse.h>
#include <include/iusHLPulseDict.h>
#include <include/iusHLParametricPulse.h>
#include <testDataGenerators.h>


static const char *pBmodePatternLabel = "bmode";
static const char *pDopplerPatternLabel = "doppler";
static const char *pPulseLabel = "pulseLabel";
static const char *pSourceLabel = "sourceLabel";
static const char *pChannelMapLabel = "channelMapLabel";
static const char *pApodizationLabel = "apodizationLabel";
static const char *pReceivesettingsLabel = "receivesettingsLabel";


iupal_t dgGeneratePatternList
(
  void
)
{
  int numPatterns = 2;
  int status;

  // fill list
  iupal_t patternList = iusHLPatternListCreate(numPatterns);
  TEST_ASSERT_NOT_EQUAL(IUPAL_INVALID, patternList);

  iupa_t bmodePattern = iusHLPatternCreate(pBmodePatternLabel,
                                           0.01f,
                                           pPulseLabel,
                                           pSourceLabel,
                                           pChannelMapLabel,
                                           pApodizationLabel,
                                           pReceivesettingsLabel);

  iupa_t dopplerPattern = iusHLPatternCreate(pDopplerPatternLabel,
                                             0.02f,
                                             pPulseLabel,
                                             pSourceLabel,
                                             pChannelMapLabel,
                                             pApodizationLabel,
                                             pReceivesettingsLabel);
  status = iusHLPatternListSet(patternList, bmodePattern, 0);
  TEST_ASSERT_EQUAL(IUS_E_OK, status);
  status = iusHLPatternListSet(patternList, dopplerPattern, 1);
  TEST_ASSERT_EQUAL(IUS_E_OK, status);
  return patternList;
}


iupd_t dgGeneratePulseDict
(
  void
)
{
  int numPulseValues=10;
  float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
  float   pulseAmplitude=800.0f;       /**< (max) amplitude of the pulse in Volts */
  int     pulseCount=10;               /**< number of cycles that the pulse represents */
  int     status=10;

  // create
  iupd_t dict = iusHLPulseDictCreate();
  TEST_ASSERT(dict != IUPD_INVALID);

  // fill
  char *parametricLabel = "parametricPulseLabel";
  char *nonParametricLabel = "nonParametricPulseLabel";
  iupp_t parametricPulse = iusHLParametricPulseCreate(parametricLabel, pulseFrequency, pulseAmplitude, pulseCount);
  iunpp_t nonParametricPulse = iusHLNonParametricPulseCreate(nonParametricLabel,numPulseValues);
  iusHLNonParametricPulseSetValue(nonParametricPulse,0,10.0f,10.0f);
  iusHLNonParametricPulseSetValue(nonParametricPulse,1,20.0f,10.0f);
  status = iusHLPulseDictSet(dict,nonParametricLabel, (iup_t) nonParametricPulse);
  TEST_ASSERT(status == IUS_E_OK);
  status = iusHLPulseDictSet(dict,parametricLabel, (iup_t) parametricPulse);
  TEST_ASSERT(status == IUS_E_OK);
  return dict;
}

iue_t dgGenerateExperiment()
{
	int date = 20180416;
	char *pDescription = "a nice experiment that almost won me the nobel prize";
	float speedOfSound = 1540.0f;

	iue_t experiment = iusHLExperimentCreate(speedOfSound, date, pDescription);
	TEST_ASSERT(experiment != IUE_INVALID);

	return experiment;
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

    transducer = iusHLCreateTransducer(pTransducerName, shape, centerFrequency, numElements);
    TEST_ASSERT(transducer != IUT_INVALID);

    x = y = z = 0.0f;
    theta = phi = 0.0f;
    sx = 0.150f / 1000.0f; // meter
    sy = 0.0f;
    sz = 6.0f / 1000.0f;
    iu3dte_t _3dElement = iusUtilCreate3DElement(x, y, z, sx, sy, sz, theta, phi);
    int status = iusHLTransducerSetElement(transducer, elementIndex, _3dElement);
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

    transducer = iusHLCreateTransducer(pTransducerName, shape, centerFrequency, numElements);
    TEST_ASSERT(transducer != IUT_INVALID);

    x = z = 0.0f;
    phi = 0.0f;
    sx = 0.150f / 1000.0f; // meter
    sz = 6.0f / 1000.0f;
    iu2dte_t _2dElement = iusUtilCreate2DElement(x, z, sx, sz, phi);
    int status = iusHLTransducerSetElement(transducer, elementIndex, _2dElement);
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
        status |= iusHLTransmitPatternListSet(transmitPatternList,time,sourceIndex,pulseIndex,i);
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
    iusll_t transmitSources = iusHLCreateSourceLocationList( locationType, numTransmitSources );
    // fill
    status = fill3DSourceLocationList(transmitSources);

    // Create transmit pulses
    iutpl_t transmitPulses = iusHLCreateTransmitPulseList( numTransmitPulses );

    // fill
    status = fillTransmitPulseList(transmitPulses);

    // Create transmit pattern
    iutpal_t transmitPatterns = iusHLCreateTransmitPatternList( numTransmitPulses );
    status = fillTransmitPatternList(transmitPatterns);

    dri = iusHLCreateDrivingScheme( IUS_DIVERGING_WAVES_PARAMETRIZED,
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

    iuhHeader = iusHLCreateInputHeader();
    TEST_ASSERT(iuhHeader != IUH_INVALID);

//    IusReceiveSettings * pReceiveSettings; /**< data receive settings */
//    IusDrivingScheme   * pDrivingScheme;   /**< data transmit settings */

    ex = iusHLCreateExperiment(speedOfSound, date, pDescription);
    status = iusHLHeaderSetExperiment(iuhHeader, ex);
    if (status != IUS_E_OK) return IUH_INVALID;

    tra = create2DTransducer();
    status = iusHLHeaderSetTransducer(iuhHeader, tra);
    if (status != IUS_E_OK) return IUH_INVALID;


    dri = createDrivingScheme(IUS_2D_SHAPE);
    status = iusHLHeaderSetDrivingScheme(iuhHeader, dri);
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


    iuhHeader = iusHLCreateInputHeader();
    TEST_ASSERT(iuhHeader != IUH_INVALID);

//   Todo:
// IusReceiveSettings * pReceiveSettings; /**< data receive settings */

    ex = iusHLCreateExperiment(speedOfSound, date, pDescription);
    status = iusHLHeaderSetExperiment(iuhHeader, ex);
    if (status != IUS_E_OK) return IUH_INVALID;


    tra = create3DTransducer();
    status = iusHLHeaderSetTransducer(iuhHeader, tra);
    if (status != IUS_E_OK) return IUH_INVALID;

    dri = createDrivingScheme(IUS_3D_SHAPE);
    status = iusHLHeaderSetDrivingScheme(iuhHeader, dri);
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

    numTransmitSources= iusHLSourceListGetSize( sourceLocationList );
    for( index = 0 ; index < numTransmitSources; index++)
    {
        x = 0.05f*index;
        y = 0.01f*index;
        z = -0.05f;
        iu3dp_t _3dpos = iusHLCreate3DPosition(x, y, z);
        TEST_ASSERT_NOT_EQUAL(_3dpos,NULL);
        status = iusHLSourceListSet3DPosition( sourceLocationList, _3dpos, index );
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
    int numPulseValues = iusHLNonParametricPulseGetNumValues(transmitPulse);
    for(i=0; i< numPulseValues; i++)
    {
        pulseAmplitude = offset / ((i % 10) + 1.0f);
        pulseTimes = offset / ((i % 10) + 1.0f);
        status = iusHLNonParametricPulseSetAmplitudeTime(transmitPulse,pulseTimes,pulseAmplitude,i);
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
    int numPulseValues = iusHLNonParametricPulseGetNumValues(transmitPulse);
    for(i=0; i< numPulseValues; i++)
    {
        pulseAmplitude = ((i%3)-1)*40.0f;
        pulseTimes = i*0.00001;
        status = iusHLNonParametricPulseSetAmplitudeTime(transmitPulse,pulseTimes,pulseAmplitude,i);
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
    int numTransmitPulses=iusHLPulseListGetSize(transmitPulses);

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
            pulse = (iup_t) iusHLCreateParametricPulse(pulseFrequency,pulseAmplitude,pulseCount);
            TEST_ASSERT_NOT_EQUAL(pulse, NULL);
        }
        else
        {
            nonParametricPulse = iusHLCreateNonParametricPulse(numTransmitPulseValues);
            TEST_ASSERT_NOT_EQUAL(nonParametricPulse, NULL);
            status = nonParametricPulseFillAmplitudeTime(nonParametricPulse);
            pulse = (iup_t) nonParametricPulse;
            TEST_ASSERT_EQUAL(IUS_E_OK,status);
        }
        status = iusHLPulseListSet(transmitPulses,pulse,index);
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
    int numTransmitPatterns=iusHLTransmitPatternListGetSize(transmitPatterns);
    float time = 0.0f;

    iutpa_t pattern;

    for( index = 0 ; index < numTransmitPatterns; index++)
    {
        sourceIndex = index/2;
        pulseIndex = index;
        time = index * 0.0004f;
        status = iusHLTransmitPatternListSet(transmitPatterns,time,sourceIndex,pulseIndex,index);
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

    numTransmitSources= iusHLSourceListGetSize( list );
    for( index = 0 ; index < numTransmitSources; index++)
    {
        x = index + 1.0*index;
        z = 2*index + 1.0*index;
        iu2dp_t _2dpos = iusHLCreate2DPosition(x, z);
        iusHLSourceListSet2DPosition( list, _2dpos, index );
    }
    return status;
}

int dgDeleteDrivingScheme
(
    iuds_t drivingScheme
)
{
    if( drivingScheme == NULL ) return IUS_ERR_VALUE;
    iusl_t transmitSources = iusHLDrivingSchemeGetSourceLocationList(drivingScheme);
    int status = iusHLDeleteSourceList( transmitSources );
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
        iu2dsl_t sources =iusHLCreate2DSourceList(numTransmitSources);
        status = fill3DSourceLocationList(sources);
        transmitSources = (iusl_t) sources;
    }
    else
    {
        iu3dsl_t sources =iusHLCreate3DSourceList(numTransmitSources);
        status = fill2DSourceLocationList(transmitSources);
        transmitSources = (iusl_t) sources;
    }

    // Create transmit pulses
    iupl_t transmitPulses = iusHLCreatePulseList( numTransmitPulses );

    // fill
    status = fillTransmitPulseList(transmitPulses);

    // Create transmit pattern
    iutpal_t transmitPatterns = iusHLCreateTransmitPatternList( numTransmitPulses );
    status = fillTransmitPatternList(transmitPatterns);

    iual_t  apodizations = iusHLCreateApodizationList(numApodizations,numElements);

    // fill
    parametrizedDrivingScheme = iusHLCreateDrivingScheme( type,
                                                          shape,
                                                          transmitSources,
                                                          transmitPulses,
                                                          transmitPatterns,
                                                          apodizations,
                                                          numElements );
    return parametrizedDrivingScheme;
}
#endif