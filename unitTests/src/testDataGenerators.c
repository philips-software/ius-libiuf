//
// Created by Ruijzendaal on 16/04/2018.
//


#include <include/iusHLSourceLocationList.h>
#include <include/iusHLPosition.h>
#include <unity.h>
#include <include/iusHLNonParametricTransmitPulse.h>
#include <include/ius.h>
#include <include/iusHLTransmitPulseList.h>
#include <include/iusHLParametricTransmitPulse.h>
#include <include/iusHLTransmitPatternList.h>
#include <include/iusHLTransmitPattern.h>
#include <include/iusHLDrivingScheme.h>

int fill3DSourceLocationList
(
iusll_t sourceLocationList
)
{
    int status=0;
    int numTransmitSources;
    int index = 0;
    iu3dp_t _3dpos;
    float x,y,z;

    numTransmitSources=iusHLSourceLocationListGetSize(sourceLocationList);
    for( index = 0 ; index < numTransmitSources; index++)
    {
        x = 0.05f*index;
        y = 0.01f*index;
        z = -0.05f;
        iu3dp_t _3dpos = iusHLCreate3DPosition(x, y, z);
        TEST_ASSERT_NOT_EQUAL(_3dpos,NULL);
        status = iusHLSourceLocationListSet3DPosition(sourceLocationList, _3dpos, index);
        TEST_ASSERT_EQUAL(IUS_E_OK,status);
    }
    return status;

}


int nonParametricPulseFillAmplitudeTime
(
iunptp_t transmitPulse
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
iutpl_t transmitPulses
)
{
    int status=0;
    int index = 0;
    int numTransmitPulses=iusHLTransmitPulseListGetSize(transmitPulses);

    float pulseFrequency=5000000.0f;
    float pulseAmplitude=40.0f;
    int pulseCount=2;
    int numTransmitPulseValues=20;

    iutp_t pulse;
    iunptp_t nonParametricPulse;

    for( index = 0 ; index < numTransmitPulses; index++)
    {
        if( index%2 )
        {
            // Parametric
            pulse = (iutp_t) iusHLCreateParametricPulse(pulseFrequency,pulseAmplitude,pulseCount);
            TEST_ASSERT_NOT_EQUAL(pulse, NULL);
        }
        else
        {
            nonParametricPulse = iusHLCreateNonParametricPulse(numTransmitPulseValues);
            TEST_ASSERT_NOT_EQUAL(nonParametricPulse, NULL);
            status = nonParametricPulseFillAmplitudeTime(nonParametricPulse);
            pulse = (iutp_t) nonParametricPulse;
            TEST_ASSERT_EQUAL(IUS_E_OK,status);
        }
        status = iusHLTransmitPulseListSet(transmitPulses,pulse,index);
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
        iusll_t list
    )
{
    int status=0;
    int numTransmitSources;
    int index = 0;
    int  pos;
    iu2dp_t _2dpos;
    float x,z;

    numTransmitSources=iusHLSourceLocationListGetSize(list);
    for( index = 0 ; index < numTransmitSources; index++)
    {
        x = index + 1.0*index;
        z = 2*index + 1.0*index;
        iu2dp_t _2dpos = iusHLCreate2DPosition(x, z);
        iusHLSourceLocationListSet2DPosition(list,_2dpos,index);
    }
    return status;
}



iuds_t dgCreateDrivingScheme
(
    IusDrivingSchemeType type,
    IusShape shape,
    IusSourceLocationType locationType,
    int numTransmitPulses,
    int numTransmitSources,
    int numElements
)
{
    iuds_t parametrizedDrivingScheme;
    int status = 0;

    // Create transmit sources
    iusll_t transmitSources = iusHLCreateSourceLocationList( locationType, numTransmitSources );
    if( locationType == IUS_PARAMETRIC_3D_SOURCE_LOCATION )
    {
        // fill
        status = fill3DSourceLocationList(transmitSources);
    }
    else
    {
        status = fill2DSourceLocationList(transmitSources);
    }

    // Create transmit pulses
    iutpl_t transmitPulses = iusHLCreateTransmitPulseList( numTransmitPulses );
    // fill
    status = fillTransmitPulseList(transmitPulses);

    // Create transmit pattern
    iutpal_t transmitPatterns = iusHLCreateTransmitPatternList( numTransmitPulses );
    status = fillTransmitPatternList(transmitPatterns);


    // fill
    parametrizedDrivingScheme = iusHLCreateDrivingScheme( type,
                                                          shape,
                                                          transmitSources,
                                                          transmitPulses,
                                                          transmitPatterns,
                                                          numElements );
    return parametrizedDrivingScheme;
}
