//
// Created by Ruijzendaal on 27/03/2018.
//
#include <unity_fixture.h>
#include <include/iusHLDrivingScheme.h>
#include <include/ius.h>
#include <include/iusTypes.h>
#include <include/iusHLTransducer.h>
#include <include/iusError.h>
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

    // Create 3D Transducer
    //
    IusShape shape;
    IusDrivingSchemeType type;

    int numElements = 32;
    int numTransmitPulses = 13;
    int numTransmitSources = 13;
    int status = 0;

#define CREATE_TEST_AND_DELETE_SCHEME(typ,shpe,nt,ns,ne,stat) drivingScheme = iusHLCreateDrivingScheme(typ,shpe,nt,ns,ne); \
                                                            TEST_ASSERT(drivingScheme != IUDS_INVALID); \
                                                            TEST_ASSERT(shpe == iusHLDrivingSchemeGetShape(drivingScheme)); \
                                                            TEST_ASSERT(typ == iusHLDrivingSchemeGetType(drivingScheme)); \
                                                            TEST_ASSERT(nt == iusHLDrivingSchemeGetNumTransmitPulses(drivingScheme)); \
                                                            TEST_ASSERT(ns == iusHLDrivingSchemeGetNumTransmitSources(drivingScheme)); \
                                                            TEST_ASSERT(ne == iusHLDrivingSchemeGetNumElements(drivingScheme)); \
                                                            status = iusHLDeleteDrivingScheme(drivingScheme); \
                                                            TEST_ASSERT(status == stat)

    CREATE_TEST_AND_DELETE_SCHEME(IUS_DIVERGING_WAVES_PARAMETRIZED, IUS_2D_SHAPE, numTransmitPulses,numTransmitSources,numElements,IUS_E_OK);
    CREATE_TEST_AND_DELETE_SCHEME(IUS_DIVERGING_WAVES_PARAMETRIZED, IUS_3D_SHAPE, numTransmitPulses,numTransmitSources,numElements,IUS_E_OK);
    CREATE_TEST_AND_DELETE_SCHEME(IUS_DIVERGING_WAVES, IUS_2D_SHAPE, numTransmitPulses,numTransmitSources,numElements,IUS_E_OK);
    CREATE_TEST_AND_DELETE_SCHEME(IUS_DIVERGING_WAVES, IUS_3D_SHAPE, numTransmitPulses,numTransmitSources,numElements,IUS_E_OK);
    CREATE_TEST_AND_DELETE_SCHEME(IUS_FOCUSED_WAVES_PARAMETRIZED, IUS_2D_SHAPE, numTransmitPulses,numTransmitSources,numElements,IUS_E_OK);
    CREATE_TEST_AND_DELETE_SCHEME(IUS_FOCUSED_WAVES_PARAMETRIZED, IUS_3D_SHAPE, numTransmitPulses,numTransmitSources,numElements,IUS_E_OK);
    CREATE_TEST_AND_DELETE_SCHEME(IUS_FOCUSED_WAVES, IUS_2D_SHAPE, numTransmitPulses,numTransmitSources,numElements,IUS_E_OK);
    CREATE_TEST_AND_DELETE_SCHEME(IUS_FOCUSED_WAVES, IUS_3D_SHAPE, numTransmitPulses,numTransmitSources,numElements,IUS_E_OK);
    CREATE_TEST_AND_DELETE_SCHEME(IUS_SINGLE_ELEMENT, IUS_2D_SHAPE, numTransmitPulses,numTransmitSources,numElements,IUS_E_OK);
    CREATE_TEST_AND_DELETE_SCHEME(IUS_SINGLE_ELEMENT, IUS_3D_SHAPE, numTransmitPulses,numTransmitSources,numElements,IUS_E_OK);
    CREATE_TEST_AND_DELETE_SCHEME(IUS_CUSTOM_WAVES, IUS_2D_SHAPE, numTransmitPulses,numTransmitSources,numElements,IUS_E_OK);
    CREATE_TEST_AND_DELETE_SCHEME(IUS_CUSTOM_WAVES, IUS_3D_SHAPE, numTransmitPulses,numTransmitSources,numElements,IUS_E_OK);
    CREATE_TEST_AND_DELETE_SCHEME(IUS_PLANE_WAVES, IUS_2D_SHAPE, numTransmitPulses,numTransmitSources,numElements,IUS_E_OK);
    CREATE_TEST_AND_DELETE_SCHEME(IUS_PLANE_WAVES, IUS_3D_SHAPE, numTransmitPulses,numTransmitSources,numElements,IUS_E_OK);

    // test invalid arguments
    drivingScheme = iusHLCreateDrivingScheme(0, IUS_3D_SHAPE, numTransmitPulses,numTransmitSources,numElements);
    TEST_ASSERT(drivingScheme == IUDS_INVALID);
    drivingScheme = iusHLCreateDrivingScheme(IUS_CUSTOM_WAVES, 0, numTransmitPulses,numTransmitSources,numElements);
    TEST_ASSERT(drivingScheme == IUDS_INVALID);
    drivingScheme = iusHLCreateDrivingScheme(IUS_CUSTOM_WAVES, IUS_3D_SHAPE, -1,numTransmitSources,numElements);
    TEST_ASSERT(drivingScheme == IUDS_INVALID);
}

TEST(InputfileDrivingScheme, testSchemeGetTransmitApodization)
{
    int numElements = 32;
    int numTransmitPulses = 13;
    int numTransmitSources = 13;
    int status = 0;

    iuds_t drivingScheme;
    IusShape shape = IUS_2D_SHAPE;
    drivingScheme = iusHLCreateDrivingScheme(IUS_DIVERGING_WAVES_PARAMETRIZED, shape, numTransmitPulses, numTransmitSources,numElements);

    // By default, apodization should be initialized to 1.0f for all elements.
    float elementApodization = NAN;
    int index;
    for(index=0;index < numElements ; index++)
    {
        elementApodization = iusDrivingSchemeGetTransmitApodization(drivingScheme,index);
        TEST_ASSERT_EQUAL_FLOAT(1.0f,elementApodization);
    }

    // Invalid params
    elementApodization=iusDrivingSchemeGetTransmitApodization(drivingScheme,numElements);
    TEST_ASSERT_EQUAL_FLOAT(NAN,elementApodization);

    elementApodization=iusDrivingSchemeGetTransmitApodization(drivingScheme,-10);
    TEST_ASSERT_EQUAL_FLOAT(NAN,elementApodization);

    status = iusHLDeleteDrivingScheme(drivingScheme);
    TEST_ASSERT(status == IUS_E_OK);
}


TEST(InputfileDrivingScheme, testSchemeSetTransmitApodization)
{
    int numElements = 32;
    int numTransmitPulses = 13;
    int numTransmitSources = 13;
    int status = 0;

    iuds_t drivingScheme;
    IusShape shape = IUS_2D_SHAPE;
    drivingScheme = iusHLCreateDrivingScheme(IUS_DIVERGING_WAVES_PARAMETRIZED, shape, numTransmitPulses, numTransmitSources,numElements);

    // By default, apodization should be initialized to 1.0f for all elements.
    float elementApodization = NAN;
    int index;
    for(index=0 ;index < numElements ; index++)
    {
        elementApodization = 1.0/(index%10);
        status = iusDrivingSchemeSetTransmitApodization(drivingScheme,elementApodization,index);
        TEST_ASSERT(status == IUS_E_OK);
        TEST_ASSERT_EQUAL_FLOAT(elementApodization,iusDrivingSchemeGetTransmitApodization(drivingScheme,index));
    }

    // Invalid params
    status=iusDrivingSchemeSetTransmitApodization(drivingScheme,0.1f,numElements);
    TEST_ASSERT(status == IUS_ERR_VALUE);

    status=iusDrivingSchemeSetTransmitApodization(drivingScheme,0.1f,-10);
    TEST_ASSERT(status == IUS_ERR_VALUE);

    status = iusHLDeleteDrivingScheme(drivingScheme);
    TEST_ASSERT(status == IUS_E_OK);
}

TEST(InputfileDrivingScheme, testIUS_DIVERGING_WAVES)
{
    iuds_t drivingScheme;
    int numElements = 32;
    int numTransmitPulses = 13;
    int numTransmitSources = 13;
    int status = 0;

    drivingScheme = iusHLCreateDrivingScheme(IUS_DIVERGING_WAVES, IUS_3D_SHAPE, numTransmitPulses, numTransmitSources,numElements);

    TEST_ASSERT(numTransmitSources == iusHLDrivingSchemeGetNumTransmitSources(drivingScheme));
    float x,y,z;
    int index;
    for(index = 0; index < numTransmitSources; index++)
    {
        x=1.0/(index%10);
        y=1.0/(index%10);
        z=1.0/(index%10);
        iu3dp_t pos = iusHLCreate3DPosition(
            x,
            y,
            z
        );
        status=iusHLDrivingSchemeSet3DSourceLocation(drivingScheme,pos,index);
        TEST_ASSERT(status == IUS_E_OK);
        iu3dp_t actual = iusHLDrivingSchemeGet3DSourceLocation(drivingScheme,index);
        TEST_ASSERT(iusCompare3DPosition(pos,actual) == IUS_TRUE);
    }

    // invalid params
    // Driving scheme specific params
    float angularDelta = 0.13f;
    float FNumber = -0.955f;
    float startAngle = 3.14f;

    status |= iusDrivingSchemeSetSourceAngularDelta(drivingScheme,angularDelta);
    TEST_ASSERT(status == IUS_ERR_VALUE);
    status |= iusDrivingSchemeSetSourceFNumber(drivingScheme,FNumber);
    TEST_ASSERT(status == IUS_ERR_VALUE);
    status |= iusDrivingSchemeSetSourceStartAngle(drivingScheme,startAngle);
    TEST_ASSERT(status == IUS_ERR_VALUE);
    status = iusHLDeleteDrivingScheme(drivingScheme);
    TEST_ASSERT(status == IUS_E_OK);
}

TEST(InputfileDrivingScheme, testIUS_DIVERGING_WAVES_PARAMETRIZED)
{
    iuds_t parametrizedDrivingScheme;
    int numElements = 32;
    int numTransmitPulses = 13;
    int numTransmitSources = 13;
    int status = 0;

    IusShape shape = IUS_2D_SHAPE;
    parametrizedDrivingScheme = iusHLCreateDrivingScheme(IUS_DIVERGING_WAVES_PARAMETRIZED, shape, numTransmitPulses, numTransmitSources,numElements);

    // Driving scheme specific params
    float angularDelta = 0.13f;
    float FNumber = -0.955f;
    float startAngle = 3.14f;

    status |= iusDrivingSchemeSetSourceAngularDelta(parametrizedDrivingScheme,angularDelta);
    TEST_ASSERT(status == IUS_E_OK);
    TEST_ASSERT_EQUAL_FLOAT(angularDelta,iusDrivingSchemeGetSourceAngularDelta(parametrizedDrivingScheme));
    status |= iusDrivingSchemeSetSourceFNumber(parametrizedDrivingScheme,FNumber);
    TEST_ASSERT(status == IUS_E_OK);
    TEST_ASSERT_EQUAL_FLOAT(FNumber,iusDrivingSchemeGetSourceFNumber(parametrizedDrivingScheme));
    status |= iusDrivingSchemeSetSourceStartAngle(parametrizedDrivingScheme,startAngle);
    TEST_ASSERT(status == IUS_E_OK);
    TEST_ASSERT_EQUAL_FLOAT(startAngle,iusDrivingSchemeGetSourceStartAngle(parametrizedDrivingScheme));
    status = iusHLDeleteDrivingScheme(parametrizedDrivingScheme);
    TEST_ASSERT(status == IUS_E_OK);



    // invalid params
    parametrizedDrivingScheme = iusHLCreateDrivingScheme(IUS_DIVERGING_WAVES_PARAMETRIZED, IUS_3D_SHAPE, numTransmitPulses, numTransmitSources,numElements);
    status |= iusDrivingSchemeSetSourceAngularDelta(parametrizedDrivingScheme,angularDelta);
    TEST_ASSERT(status == IUS_ERR_VALUE);
    status |= iusDrivingSchemeSetSourceFNumber(parametrizedDrivingScheme,FNumber);
    TEST_ASSERT(status == IUS_ERR_VALUE);
    status |= iusDrivingSchemeSetSourceStartAngle(parametrizedDrivingScheme,startAngle);
    TEST_ASSERT(status == IUS_ERR_VALUE);
    status = iusHLDeleteDrivingScheme(parametrizedDrivingScheme);
    TEST_ASSERT(status == IUS_E_OK);





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
    RUN_TEST_CASE(InputfileDrivingScheme, testIusHLCreateDrivingScheme);
    RUN_TEST_CASE(InputfileDrivingScheme, testIUS_DIVERGING_WAVES_PARAMETRIZED);
    RUN_TEST_CASE(InputfileDrivingScheme, testIUS_DIVERGING_WAVES);
    RUN_TEST_CASE(InputfileDrivingScheme, testSchemeGetTransmitApodization);
    RUN_TEST_CASE(InputfileDrivingScheme, testSchemeSetTransmitApodization);

}

