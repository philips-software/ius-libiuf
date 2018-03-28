//
// Created by Ruijzendaal on 27/03/2018.
//
#include <unity_fixture.h>
#include <include/iusHLDrivingScheme.h>
#include <include/ius.h>
#include <include/iusTypes.h>
#include <include/iusHLTransducer.h>
#include "testDrivingScheme.h"

TEST_GROUP(InputfileDrivingScheme);

TEST_SETUP(InputfileDrivingScheme)
{
}

TEST_TEAR_DOWN(InputfileDrivingScheme)
{
}

TEST(InputfileDrivingScheme, testIusHLCreateDrivingScheme)
{
    iuds_t drivingScheme;

    // 2 types of driving schemes
    // - Parametrized
    // - non-parametrized
    //

//    DIVERGING_WAVES_RADIAL, FOCUSED_WAVES, PLANE_WAVES, SINGLE_ELEMENT

    // Create 3D Transducer
    //
    int numElements = 32;
    int numTransmitPulses = 13;
    int numTransmitSources = 13;

    iut_t transducer = iusHLCreateTransducer("", IUS_LINE, 8000000, numElements);
    IusTransducerShape shape = iusHLGetTransducerShape(transducer);
    
    drivingScheme = iusHLCreateDrivingScheme(IUS_DIVERGING_WAVES_PARAMETRIZED, shape,  numElements, numTransmitPulses);
    TEST_ASSERT(drivingScheme != IUDS_INVALID);
    int status = iusHLDeleteDrivingScheme(drivingScheme);
    TEST_ASSERT(status == IUS_E_OK);
}

TEST(InputfileDrivingScheme, testFirst)
{
    iuds_t drivingScheme;

    // 2 types of driving schemes
    // - Parametrized
    // - non-parametrized
    //

//    DIVERGING_WAVES_RADIAL, FOCUSED_WAVES, PLANE_WAVES, SINGLE_ELEMENT
    
    // Create 3D Transducer
    //
    int numElements = 32;
    int numTransmitPulses = 13;
    int numTransmitSources = 13;

    iut_t transducer = iusHLCreateTransducer("", IUS_LINE, 8000000, numElements);
    IusTransducerShape shape = iusHLGetTransducerShape(transducer);
    
    drivingScheme = iusHLCreateDrivingScheme(IUS_DIVERGING_WAVES_PARAMETRIZED, shape,  numElements, numTransmitPulses);

    // Driving scheme specific params
//    iusDrivingSchemeSetSourceAngularDelta(drivingScheme,0.13f);
//    iusDrivingSchemeSetSourceFNumber(drivingScheme,-0.955f);
//    iusDrivingSchemeSetSourceStartAngle(drivingScheme,3.14f);
//    // Only for diverging and focussed:
//    iusDrivingSchemeSetNumTransmitSources(drivingScheme,numTransmitSources);
//
//    // TransmitPulse can be Parametric and non parametric
//    // currently identified by checking the numPulseValues
//    //
//     iusDrivingSchemeCreatePulse( IUS_TRANSMIT_PULSE_PARAMETRIC  );
//     pls = iusDrivingSchemeCreatePulse( IUS_TRANSMIT_PULSE_NON_PARAMETRIC  );
//     iusDrivingSchemeSetPulse( drivingScheme, pls );

//    iusDrivingSchemeSetNumSamplesPerLine(drivingScheme,9);
//    iusDrivingSchemeSetNumTransmitSources(drivingScheme,1); // Determines length of pSourceLocations
//    iusDrivingSchemeSetNumTransmitPulses(drivingScheme,1); // this * receiveChannelCoding.numChannels = pChannelMap length
//
//    pSourceLocations = (IusPosition *)calloc(1, sizeof(IusPosition));
//    pSourceLocations[0].x = 1.1f;
//    pSourceLocations[0].y = 2.2f;
//    pSourceLocations[0].z = 3.3f;
//    sourceFNumber = 0.1f;
//    sourceAngularDelta = 0.2f;
//    sourceStartAngle = 0.3f;
//    transmitPatternDelay = 0.4f;
//    pTransmitPattern = (IusTransmitPattern *)calloc(1, sizeof(IusTransmitPattern));
//    pTransmitPattern[0].index = 6;
//    pTransmitPattern[0].time = 0.5;
//    transmitPulse.numPulseValues = 1;
//    transmitPulse.pRawPulseAmplitudes = (float *)calloc(1, sizeof(float));
//    transmitPulse.pRawPulseAmplitudes[0] = 9.9f;
//    transmitPulse.pRawPulseTimes = (float *)calloc(1, sizeof(float));
//    transmitPulse.pRawPulseTimes[0] = 8.8f;
//    transmitPulse.pulseFrequency = 4.4f;
//    transmitPulse.pulseAmplitude = 5.5f;
//    transmitPulse.pulseCount = 3;
//    pTransmitApodization = (float *)calloc(1, sizeof(float));
//    pTransmitApodization[0] = 0.6f;
//    transmitChannelCoding.numChannels = 1;
//    transmitChannelCoding.pChannelMap = (int *)calloc(1, sizeof(int));
//    transmitChannelCoding.pChannelMap[0] = 2;
    TEST_ASSERT(1);
}

TEST_GROUP_RUNNER(InputfileDrivingScheme)
{
    RUN_TEST_CASE(InputfileDrivingScheme, testFirst);
    RUN_TEST_CASE(InputfileDrivingScheme, testIusHLCreateDrivingScheme);
}

