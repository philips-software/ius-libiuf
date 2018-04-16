//
// Created by Ruijzendaal on 16/04/2018.
//

#include <unity_fixture.h>
#include <include/iusHLDrivingScheme.h>

TEST_GROUP( InputfileTransmitApodization );

TEST_SETUP( InputfileTransmitApodization ) {
}

TEST_TEAR_DOWN( InputfileTransmitApodization ) {
}
//
//TEST( InputfileTransmitApodization, testSchemeGetTransmitApodization ) {
//    int numElements = 32;
//    int numTransmitPulses = 13;
//    int numTransmitSources = 13;
//    int status = 0;
//
//    iuds_t drivingScheme;
//    IusShape shape = IUS_2D_SHAPE;
//    drivingScheme = iusHLCreateDrivingScheme( IUS_DIVERGING_WAVES_PARAMETRIZED,
//                                              shape,
//                                              numTransmitPulses,
//                                              numTransmitSources,
//                                              numElements );
//
//// By default, apodization should be initialized to 1.0f for all elements.
//    float elementApodization;
//    int pulseIndex;
//    int elementIndex;
//    for (pulseIndex = 0; pulseIndex < numTransmitPulses; pulseIndex++) {
//        for (elementIndex = 0; elementIndex < numElements; elementIndex++) {
//            elementApodization = iusDrivingSchemeGetTransmitApodization( drivingScheme, pulseIndex, elementIndex );
//            TEST_ASSERT_EQUAL_FLOAT( 1.0f, elementApodization );
//        }
//    }
//
//// Invalid params
//    elementApodization = iusDrivingSchemeGetTransmitApodization( drivingScheme, numTransmitPulses, numElements );
//    TEST_ASSERT_EQUAL_FLOAT( NAN, elementApodization );
//
//    elementApodization = iusDrivingSchemeGetTransmitApodization( drivingScheme, -1, -10 );
//    TEST_ASSERT_EQUAL_FLOAT( NAN, elementApodization );
//
//
//    status = iusHLDeleteDrivingScheme( drivingScheme );
//    TEST_ASSERT( status == IUS_E_OK );
//}
//
//
//TEST( InputfileTransmitApodization, testSchemeSetTransmitApodization ) {
//    int numElements = 32;
//    int numTransmitPulses = 13;
//    int numTransmitSources = 13;
//    int status = 0;
//
//    iuds_t drivingScheme;
//    IusShape shape = IUS_2D_SHAPE;
//    drivingScheme = iusHLCreateDrivingScheme( IUS_DIVERGING_WAVES_PARAMETRIZED,
//                                              shape,
//                                              numTransmitPulses,
//                                              numTransmitSources,
//                                              numElements );
//
//// By default, apodization should be initialized to 1.0f for all elements.
//    float elementApodization;
//    int pulseIndex;
//    int elementIndex;
//    for (pulseIndex = 0; pulseIndex < numTransmitPulses; pulseIndex++) {
//        for (elementIndex = 0; elementIndex < numElements; elementIndex++) {
//            elementApodization = 1.0f / ((pulseIndex % 10) + 1);
//            status = iusDrivingSchemeSetTransmitApodization( drivingScheme, elementApodization, pulseIndex,
//                                                             elementIndex );
//            TEST_ASSERT( status == IUS_E_OK );
//            TEST_ASSERT_EQUAL_FLOAT( elementApodization,
//                                     iusDrivingSchemeGetTransmitApodization( drivingScheme, pulseIndex, elementIndex ));
//        }
//    }
//
//// Invalid params
//    status = iusDrivingSchemeSetTransmitApodization( drivingScheme, -0.1f, 0, 0 );
//    TEST_ASSERT( status == IUS_ERR_VALUE );
//
//    status = iusDrivingSchemeSetTransmitApodization( drivingScheme, 1.001f, 0, 0 );
//    TEST_ASSERT( status == IUS_ERR_VALUE );
//
//    status = iusDrivingSchemeSetTransmitApodization( drivingScheme, 0.1f, numTransmitPulses, numElements );
//    TEST_ASSERT( status == IUS_ERR_VALUE );
//
//    status = iusDrivingSchemeSetTransmitApodization( drivingScheme, 0.1f, -1, -10 );
//    TEST_ASSERT( status == IUS_ERR_VALUE );
//
//    status = iusHLDeleteDrivingScheme( drivingScheme );
//    TEST_ASSERT( status == IUS_E_OK );
//}


TEST_GROUP_RUNNER(InputfileTransmitApodization)
{
//    RUN_TEST_CASE(InputfileTransmitApodization, testSchemeGetTransmitApodization);
//    RUN_TEST_CASE(InputfileTransmitApodization, testSchemeSetTransmitApodization);
}


