//
// Created by Ruijzendaal on 16/04/2018.
//
#include <unity.h>

#include <ius.h>
#include <iusError.h>
#include <iusHLSourceListOld.h>
#include <iusHLPosition.h>
#include <iusHLNonParametricPulse.h>
#include <iusHLPulseList.h>
#include <iusHLParametricPulse.h>
#include <iusHLTransmitPatternList.h>
#include <iusHLTransmitPattern.h>
#include <iusHLDrivingScheme.h>
#include <iusHL2DSourceList.h>
#include <iusHL3DSourceList.h>

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
