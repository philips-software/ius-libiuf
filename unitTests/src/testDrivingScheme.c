//
// Created by Ruijzendaal on 27/03/2018.
//
#include <unity_fixture.h>
#include <include/iusHLDrivingScheme.h>
#include <include/ius.h>
#include <include/iusTypes.h>
#include <include/iusHLTransducer.h>
#include <include/iusError.h>
#include <include/iusHLTransmitPattern.h>
#include <include/iusHLTransmitPatternList.h>
#include <util.h>
#include <include/iusHLSourceLocationList.h>
#include <include/iusHLTransmitPulseList.h>
#include <include/iusHLParametricTransmitPulse.h>
#include <include/iusHLNonParametricTransmitPulse.h>
#include <testDataGenerators.h>
#include "testDrivingScheme.h"

TEST_GROUP(InputfileDrivingScheme);

TEST_SETUP(InputfileDrivingScheme)
{
}

TEST_TEAR_DOWN(InputfileDrivingScheme)
{
}



//TEST(InputfileDrivingScheme, testIusHLTransmitPulseList)
//{
//    iuds_t drivingScheme;
//
//    int numElements = 32;
//    int numTransmitPulses = 13;
//    int numTransmitSources = 13;
//    int status = 0;
//    int i;
//    iutpul_t pulseList;
//
//    drivingScheme = iusHLCreateDrivingScheme( IUS_DIVERGING_WAVES_PARAMETRIZED, IUS_3D_SHAPE, numTransmitPulses,numTransmitSources,numElements);
//    pulseList = iusHLDrivingSchemeGetTransmitPulseList(drivingScheme);
//    TEST_ASSERT( pulseList != IUTPUL_INVALID );
//    fillTransmitPulseList(pulseList);
//}


TEST(InputfileDrivingScheme, testIusHLCreateDrivingScheme)
{
    iuds_t drivingScheme;

    int numElements = 32;
    int numTransmitPulses = 13;
    int numTransmitSources = 13;
    int status = 0;

    IusDrivingSchemeType typ = IUS_DIVERGING_WAVES_PARAMETRIZED;
    IusShape shpe = IUS_3D_SHAPE;
    IusSourceLocationType locty = IUS_PARAMETRIC_3D_SOURCE_LOCATION;
    int nts = numTransmitSources;
    int ntpu = numTransmitPulses;
    int ne =  numElements;

    drivingScheme = dgCreateDrivingScheme(typ,shpe,locty,ntpu,nts,ne); \
                                            TEST_ASSERT(drivingScheme != IUDS_INVALID); \
                                            TEST_ASSERT((shpe) == iusHLDrivingSchemeGetShape(drivingScheme)); \
                                            TEST_ASSERT((typ) == iusHLDrivingSchemeGetType(drivingScheme)); \
                                            TEST_ASSERT((ntpu) == iusHLDrivingSchemeGetNumTransmitPulses(drivingScheme)); \
                                            TEST_ASSERT((nts) == iusHLDrivingSchemeGetNumTransmitSources(drivingScheme)); \
                                            TEST_ASSERT((ne) == iusHLDrivingSchemeGetNumElements(drivingScheme)); \
                                            status = iusHLDeleteDrivingScheme(drivingScheme); \
                                            TEST_ASSERT(status == (IUS_E_OK))
    #if 0
#define CREATE_TEST_AND_DELETE_SCHEME(typ,shpe,nt,ns,ne,stat) drivingScheme = iusHLCreateDrivingScheme(typ,shpe,nt,ns,ne); \
                                            TEST_ASSERT(drivingScheme != IUDS_INVALID); \
                                            TEST_ASSERT((shpe) == iusHLDrivingSchemeGetShape(drivingScheme)); \
                                            TEST_ASSERT((typ) == iusHLDrivingSchemeGetType(drivingScheme)); \
                                            TEST_ASSERT((nt) == iusHLDrivingSchemeGetNumTransmitPulses(drivingScheme)); \
                                            TEST_ASSERT((ns) == iusHLDrivingSchemeGetNumTransmitSources(drivingScheme)); \
                                            TEST_ASSERT((ne) == iusHLDrivingSchemeGetNumElements(drivingScheme)); \
                                            status = iusHLDeleteDrivingScheme(drivingScheme); \
                                            TEST_ASSERT(status == (stat))

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
    #endif
//    // test invalid arguments
//    drivingScheme = iusHLCreateDrivingScheme( (IusDrivingSchemeType) 0, IUS_3D_SHAPE, IUS_PARAMETRIC_3D_SOURCE_LOCATION, numTransmitPulses,numTransmitSources,numElements);
//    TEST_ASSERT(drivingScheme == IUDS_INVALID);
//    drivingScheme = iusHLCreateDrivingScheme(IUS_CUSTOM_WAVES, (IusShape) 0, IUS_PARAMETRIC_3D_SOURCE_LOCATION, numTransmitPulses,numTransmitSources,numElements);
//    TEST_ASSERT(drivingScheme == IUDS_INVALID);
//    drivingScheme = iusHLCreateDrivingScheme(IUS_CUSTOM_WAVES, IUS_3D_SHAPE, -1,numTransmitSources,numElements);
//    TEST_ASSERT(drivingScheme == IUDS_INVALID);
}

//TEST(InputfileDrivingScheme, testDivergingWaves3DShape)
//{
//    iuds_t drivingScheme;
//    int numElements = 32;
//    int numTransmitPulses = 13;
//    int numTransmitSources = 13;
//    int status = 0;
//    int index;
//    float x = 0;
//    float y = 0;
//    float z = 0;
//    float angularDelta = 0.13f;
//    float FNumber = -0.955f;
//    float startAngle = 3.14f;
//
//
//    // Diverging waves type is not parametrized
//    // but specified by source locations.
//    drivingScheme = iusHLCreateDrivingScheme( IUS_DIVERGING_WAVES, IUS_3D_SHAPE,
//                                              numTransmitPulses,
//                                              numTransmitSources,
//                                              numElements );
//
//    TEST_ASSERT(numTransmitSources == iusHLDrivingSchemeGetNumTransmitSources(drivingScheme));
//    for(index = 0; index < numTransmitSources; index++)
//    {
//        x=1.0f/((index%10)+1.0f);
//        y=1.0f/((index%10)+1.0f);
//        z=1.0f/((index%10)+1.0f);
//        iu3dp_t pos = iusHLCreate3DPosition(
//            x,
//            y,
//            z
//        );
//        status=iusHLDrivingSchemeSet3DSourceLocation(drivingScheme,pos,index);
//        TEST_ASSERT(status == IUS_E_OK);
//        iu3dp_t actual = iusHLDrivingSchemeGet3DSourceLocation(drivingScheme,index);
//        TEST_ASSERT(iusCompare3DPosition(pos,actual) == IUS_TRUE);
//    }
//
//
//    // invalid params/operations
//
//    // 3D shape transducer, should not alow 2d source positions
//    iu2dp_t _2dpos = iusHLCreate2DPosition(x, z);
//    status=iusHLDrivingSchemeSet2DSourceLocation(drivingScheme,_2dpos,index);
//    TEST_ASSERT(status == IUS_ERR_VALUE);
//
//
//    // Driving scheme specific params
//    // Setting Parametric values should result in errors for
//    // a non parametric driver scheme
//    status |= iusDrivingSchemeSetSourceDeltaTheta(drivingScheme,angularDelta);
//    TEST_ASSERT(status == IUS_ERR_VALUE);
//    status |= iusDrivingSchemeSetSourceFNumber(drivingScheme,FNumber);
//    TEST_ASSERT(status == IUS_ERR_VALUE);
//    status |= iusDrivingSchemeSetSourceStartTheta(drivingScheme,startAngle);
//    TEST_ASSERT(status == IUS_ERR_VALUE);
//    status = iusHLDeleteDrivingScheme(drivingScheme);
//    TEST_ASSERT(status == IUS_E_OK);
//}
//
//
//
//
//TEST(InputfileDrivingScheme, testDivergingWavesParametrized2DShape)
//{
//    iuds_t parametrizedDrivingScheme;
//    int numElements = 32;
//    int numTransmitPulses = 13;
//    int numTransmitSources = 13;
//    int status = 0;
//    float angularDelta = 0.13f;
//    float FNumber = -0.955f;
//    float startAngle = 3.14f;
//    float startPhi = startAngle;
//    float deltaPhi = angularDelta;
//
//
//    IusShape shape = IUS_2D_SHAPE;
//    parametrizedDrivingScheme = iusHLCreateDrivingScheme( IUS_DIVERGING_WAVES_PARAMETRIZED,
//                                                          shape,
//                                                          numTransmitPulses,
//                                                          numTransmitSources,
//                                                          numElements );
//    // Driving scheme specific params
//    status |= iusDrivingSchemeSetSourceDeltaTheta(parametrizedDrivingScheme,angularDelta);
//    TEST_ASSERT(status == IUS_E_OK);
//    TEST_ASSERT_EQUAL_FLOAT(angularDelta,iusDrivingSchemeGetSourceDeltaTheta(parametrizedDrivingScheme));
//    status |= iusDrivingSchemeSetSourceFNumber(parametrizedDrivingScheme,FNumber);
//    TEST_ASSERT(status == IUS_E_OK);
//    TEST_ASSERT_EQUAL_FLOAT(FNumber,iusDrivingSchemeGetSourceFNumber(parametrizedDrivingScheme));
//    status |= iusDrivingSchemeSetSourceStartTheta(parametrizedDrivingScheme,startAngle);
//    TEST_ASSERT(status == IUS_E_OK);
//    TEST_ASSERT_EQUAL_FLOAT(startAngle,iusDrivingSchemeGetSourceStartTheta(parametrizedDrivingScheme));
//
//
//    status = iusHLDeleteDrivingScheme(parametrizedDrivingScheme);
//    TEST_ASSERT(status == IUS_E_OK);
//
//    // invalid params/operations
//    // 2D driving scheme does not support setting 3D parameters
//    status |= iusDrivingSchemeSetSourceStartPhi(parametrizedDrivingScheme,startPhi);
//    TEST_ASSERT(status == IUS_ERR_VALUE);
//    status |= iusDrivingSchemeSetSourceDeltaPhi(parametrizedDrivingScheme,deltaPhi);
//    TEST_ASSERT(status == IUS_ERR_VALUE);
//
//
//
//
//
////    // Only for diverging and focussed:
////    iusDrivingSchemeSetNumTransmitSources(drivingScheme,numTransmitSources);
////
////    // TransmitPulse can be Parametric and non parametric
////    // currently identified by checking the numPulseValues
////    //
////     iusDrivingSchemeCreatePulse( IUS_TRANSMIT_PULSE_PARAMETRIC  );
////     pls = iusDrivingSchemeCreatePulse( IUS_TRANSMIT_PULSE_NON_PARAMETRIC  );
////     iusDrivingSchemeSetPulse( drivingScheme, pls );
//
////    iusDrivingSchemeSetNumSamplesPerLine(drivingScheme,9);
////    iusDrivingSchemeSetNumTransmitSources(drivingScheme,1); // Determines length of pSourceLocations
////    iusDrivingSchemeSetNumTransmitPulses(drivingScheme,1); // this * receiveChannelCoding.numChannels = pChannelMap length
////
////    pSourceLocations = (IusPosition *)calloc(1, sizeof(IusPosition));
////    pSourceLocations[0].x = 1.1f;
////    pSourceLocations[0].y = 2.2f;
////    pSourceLocations[0].z = 3.3f;
////    sourceFNumber = 0.1f;
////    sourceAngularDelta = 0.2f;
////    sourceStartAngle = 0.3f;
////    transmitPatternDelay = 0.4f;
////    pTransmitPattern = (IusTransmitPattern *)calloc(1, sizeof(IusTransmitPattern));
////    pTransmitPattern[0].index = 6;
////    pTransmitPattern[0].time = 0.5;
////    transmitPulse.numPulseValues = 1;
////    transmitPulse.pRawPulseAmplitudes = (float *)calloc(1, sizeof(float));
////    transmitPulse.pRawPulseAmplitudes[0] = 9.9f;
////    transmitPulse.pRawPulseTimes = (float *)calloc(1, sizeof(float));
////    transmitPulse.pRawPulseTimes[0] = 8.8f;
////    transmitPulse.pulseFrequency = 4.4f;
////    transmitPulse.pulseAmplitude = 5.5f;
////    transmitPulse.pulseCount = 3;
////    pTransmitApodization = (float *)calloc(1, sizeof(float));
////    pTransmitApodization[0] = 0.6f;
////    transmitChannelCoding.numChannels = 1;
////    transmitChannelCoding.pChannelMap = (int *)calloc(1, sizeof(int));
////    transmitChannelCoding.pChannelMap[0] = 2;
//    TEST_ASSERT(1);
//}
//





TEST(InputfileDrivingScheme, testIusHLCreate2DParametricDrivingScheme)
{
    iuds_t parametrizedDrivingScheme;
    int numElements = 32;
    int numTransmitPulses = 13;
    int numTransmitSources = 13;

    parametrizedDrivingScheme = dgCreateDrivingScheme(
        IUS_DIVERGING_WAVES_PARAMETRIZED,
        IUS_2D_SHAPE,
        IUS_PARAMETRIC_2D_SOURCE_LOCATION,
        numTransmitPulses,
        numTransmitSources,
        numElements);

}


//TEST(InputfileDrivingScheme, testIusHLCreateMultipulse2DNonParametricDrivingScheme)
//{
//}
//
//TEST(InputfileDrivingScheme, testIusHLCreateMultipulse3DNonParametricDrivingScheme)
//{
//}
//
//TEST(InputfileDrivingScheme, testIusHLCreateMultipulse2DParametricDrivingScheme)
//{
//}

TEST(InputfileDrivingScheme, testIusHLCreateMultipulse3DParametricDrivingScheme)
{
    iuds_t parametrizedDrivingScheme;
    int numElements = 32;
    int numTransmitPulses = 26;
    int numTransmitSources = 13;
    int status = 0;
    parametrizedDrivingScheme = dgCreateDrivingScheme(
        IUS_DIVERGING_WAVES_PARAMETRIZED,
        IUS_3D_SHAPE,
        IUS_PARAMETRIC_3D_SOURCE_LOCATION,
        numTransmitPulses,
        numTransmitSources,
        numElements);
}

TEST_GROUP_RUNNER(InputfileDrivingScheme)
{
    RUN_TEST_CASE(InputfileDrivingScheme, testIusHLCreateDrivingScheme);
//    RUN_TEST_CASE(InputfileDrivingScheme, testDivergingWavesParametrized2DShape);
//    RUN_TEST_CASE(InputfileDrivingScheme, testDivergingWaves3DShape);
}

