//
// Created by nlv09165 on 24/04/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusDrivingScheme.h>

#include <testDataGenerators.h>

TEST_GROUP(IusDrivingScheme);

TEST_SETUP(IusDrivingScheme)
{
}

TEST_TEAR_DOWN(IusDrivingScheme)
{
}

int createCompareAndDeleteScheme
(
    IusDrivingSchemeType type,
    IusShape shape,
    IusSourceLocationType locationType,
    int numTransmitPulses,
    int numTransmitSources,
    int numChannels,
    int numApodizations,
    int numTGCs,
    int numElements
)
{
    iuds_t drivingScheme;

    int status = 0;
    drivingScheme = dgCreateDrivingScheme(type,shape,locationType,numTransmitPulses,
                                          numTransmitSources,numChannels,numApodizations,numTGCs,
                                          numElements);
    TEST_ASSERT(drivingScheme != IUDS_INVALID);
    status = iusDeleteDrivingScheme(drivingScheme);
    TEST_ASSERT(status == (IUS_E_OK));
    return status;
}


int createTestAndDeleteScheme
(
    IusDrivingSchemeType type,
    IusShape shape,
    IusSourceLocationType locationType,
    int numTransmitPulses,
    int numTransmitSources,
    int numChannels,
    int numApodizations,
    int numTGCs,
    int numElements
)
{
    iuds_t drivingScheme;

    int status = 0;
    drivingScheme = dgCreateDrivingScheme(type,shape,locationType,numTransmitPulses,
                                          numTransmitSources,numChannels,numApodizations,numTGCs,
                                          numElements);
    status |= (drivingScheme == IUDS_INVALID);
    status |= ((shape) != iusDrivingSchemeGetShape(drivingScheme));
    status |= ((type) != iusDrivingSchemeGetType(drivingScheme));
    status |= ((numTransmitPulses) != iusDrivingSchemeGetNumTransmitPulses(drivingScheme));
    status |= ((numChannels) != iusDrivingSchemeGetNumChannels(drivingScheme));
    status |= ((numTransmitSources) != iusDrivingSchemeGetNumTransmitSources(drivingScheme));
    status |= ((numElements) != iusDrivingSchemeGetNumElements(drivingScheme));
    status |= iusDeleteDrivingScheme(drivingScheme);
    return status;
}

void createTestAndDeleteSchemeForLocation
(
    IusSourceLocationType locationType,
    int numTransmitPulses,
    int numTransmitSources,
    int numChannels,
    int numApodizations,
    int numTGCs,
    int numElements

)
{
    int status = 0;
    status = createTestAndDeleteScheme( IUS_DIVERGING_WAVES_PARAMETRIZED, IUS_2D_SHAPE, locationType,
                                         numTransmitSources, numTransmitPulses, numChannels, numApodizations,
                                         numTGCs,numElements );
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    status = createTestAndDeleteScheme( IUS_DIVERGING_WAVES_PARAMETRIZED, IUS_3D_SHAPE, locationType,
                                         numTransmitSources, numTransmitPulses, numChannels, numApodizations,
                                         numTGCs,numElements );
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    status = createTestAndDeleteScheme( IUS_DIVERGING_WAVES, IUS_2D_SHAPE, locationType,
                                         numTransmitSources, numTransmitPulses, numChannels, numApodizations,
                                         numTGCs,numElements );
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    status = createTestAndDeleteScheme( IUS_DIVERGING_WAVES, IUS_3D_SHAPE, locationType,
                                         numTransmitSources, numTransmitPulses, numChannels, numApodizations,
                                         numTGCs,numElements );
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    status = createTestAndDeleteScheme( IUS_FOCUSED_WAVES_PARAMETRIZED, IUS_2D_SHAPE, locationType,
                                         numTransmitSources, numTransmitPulses, numChannels, numApodizations,
                                         numTGCs,numElements );
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    status = createTestAndDeleteScheme( IUS_FOCUSED_WAVES_PARAMETRIZED, IUS_3D_SHAPE, locationType,
                                         numTransmitSources, numTransmitPulses, numChannels, numApodizations,
                                         numTGCs,numElements );
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    status = createTestAndDeleteScheme( IUS_FOCUSED_WAVES, IUS_2D_SHAPE, locationType,
                                         numTransmitSources, numTransmitPulses, numChannels, numApodizations,
                                         numTGCs,numElements );
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    status = createTestAndDeleteScheme( IUS_FOCUSED_WAVES, IUS_3D_SHAPE, locationType,
                                         numTransmitSources, numTransmitPulses, numChannels, numApodizations,
                                         numTGCs,numElements );
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    status = createTestAndDeleteScheme( IUS_SINGLE_ELEMENT, IUS_2D_SHAPE, locationType,
                                         numTransmitSources, numTransmitPulses, numChannels, numApodizations,
                                         numTGCs,numElements );
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    status = createTestAndDeleteScheme( IUS_SINGLE_ELEMENT, IUS_3D_SHAPE, locationType,
                                         numTransmitSources, numTransmitPulses, numChannels, numApodizations,
                                         numTGCs,numElements );
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    status = createTestAndDeleteScheme( IUS_CUSTOM_WAVES, IUS_2D_SHAPE, locationType,
                                         numTransmitSources, numTransmitPulses, numChannels, numApodizations,
                                         numTGCs,numElements );
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    status = createTestAndDeleteScheme( IUS_CUSTOM_WAVES, IUS_3D_SHAPE, locationType,
                                         numTransmitSources, numTransmitPulses, numChannels, numApodizations,
                                         numTGCs,numElements );
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    status = createTestAndDeleteScheme( IUS_PLANE_WAVES, IUS_2D_SHAPE, locationType,
                                         numTransmitSources, numTransmitPulses, numChannels, numApodizations,
                                         numTGCs,numElements );
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    status = createTestAndDeleteScheme( IUS_PLANE_WAVES, IUS_3D_SHAPE, locationType,
                                         numTransmitSources, numTransmitPulses, numChannels, numApodizations,
                                         numTGCs,numElements );
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    return;
}

TEST(IusDrivingScheme, testIusSetGetDrivingScheme)
{
    int numElements = 32;
    int numTransmitPulses = 13;
    int numTransmitSources = 13;
    int numChannels = 4;
    int numApodizations = numElements;
    int numTGCs = numChannels;

    createTestAndDeleteSchemeForLocation( IUS_PARAMETRIC_3D_SOURCE_LOCATION, numTransmitPulses, numTransmitSources,
                                          numChannels, numApodizations, numTGCs, numElements);
    createTestAndDeleteSchemeForLocation( IUS_PARAMETRIC_2D_SOURCE_LOCATION, numTransmitPulses, numTransmitSources,
                                          numChannels, numApodizations, numTGCs, numElements);
    createTestAndDeleteSchemeForLocation( IUS_3D_SOURCE_LOCATION, numTransmitPulses, numTransmitSources,
                                          numChannels, numApodizations, numTGCs, numElements);
    createTestAndDeleteSchemeForLocation( IUS_2D_SOURCE_LOCATION, numTransmitPulses, numTransmitSources,
                                          numChannels, numApodizations, numTGCs, numElements);

}

TEST(IusDrivingScheme, testIusDeleteDrivingScheme)
{
    IUS_BOOL equal;
    int numElements = 32;
    int numTransmitPulses = 13;
    int numTransmitSources = 13;
    int numChannels = 6;
    int numApodizations = numElements;
    int numTGCs = 10;

    iuds_t obj = dgCreateDrivingScheme(IUS_DIVERGING_WAVES_PARAMETRIZED,
                                       IUS_3D_SHAPE,IUS_PARAMETRIC_3D_SOURCE_LOCATION,
                                       numTransmitPulses,
                                       numTransmitSources,
                                       numChannels,
                                       numApodizations,
                                       numTGCs,
                                       numElements);
    TEST_ASSERT(obj != IUDS_INVALID);
    int status = iusDeleteDrivingScheme(obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // invalid params
    status = iusDeleteDrivingScheme(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
}



TEST(IusDrivingScheme, testIusCompareDrivingScheme)
{
    IUS_BOOL equal;
    int numElements = 32;
    int numTransmitPulses = 13;
    int numTransmitSources = 13;
    int numChannels = 6;
    int numApodizations = numTransmitPulses;
    int numTGCs = 10;

    iuds_t ynotherObj;
    iuds_t obj = dgCreateDrivingScheme(IUS_DIVERGING_WAVES_PARAMETRIZED,
                                       IUS_3D_SHAPE,IUS_PARAMETRIC_3D_SOURCE_LOCATION,
                                       numTransmitPulses,
                                       numTransmitSources,
                                       numChannels,
                                       numApodizations,
                                       numTGCs,
                                       numElements);

    iuds_t notherObj = dgCreateDrivingScheme(IUS_DIVERGING_WAVES_PARAMETRIZED,
                                             IUS_3D_SHAPE,IUS_PARAMETRIC_3D_SOURCE_LOCATION,
                                             numTransmitPulses,
                                             numTransmitSources,
                                             numChannels,
                                             numApodizations,
                                             numTGCs,
                                             numElements);
    TEST_ASSERT(obj != IUDS_INVALID);
    TEST_ASSERT(notherObj != IUDS_INVALID);
    equal = iusCompareDrivingScheme(obj,obj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = iusCompareDrivingScheme(obj,notherObj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);

    // Changing a single parameter should change the compoare outcome
    // for now, not changing contents, only number of items in sub-components
    ynotherObj = dgCreateDrivingScheme(IUS_DIVERGING_WAVES,
                                       IUS_3D_SHAPE,IUS_PARAMETRIC_3D_SOURCE_LOCATION,
                                       numTransmitPulses,
                                       numTransmitSources,
                                       numChannels,
                                       numApodizations,
                                       numTGCs,
                                       numElements);
    equal = iusCompareDrivingScheme(obj,ynotherObj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    ynotherObj = dgCreateDrivingScheme(IUS_DIVERGING_WAVES_PARAMETRIZED,
                                       IUS_2D_SHAPE,IUS_PARAMETRIC_3D_SOURCE_LOCATION,
                                       numTransmitPulses,
                                       numTransmitSources,
                                       numChannels,
                                       numApodizations,
                                       numTGCs,
                                       numElements);
    equal = iusCompareDrivingScheme(obj,ynotherObj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    ynotherObj = dgCreateDrivingScheme(IUS_DIVERGING_WAVES_PARAMETRIZED,
                                       IUS_3D_SHAPE,IUS_PARAMETRIC_3D_SOURCE_LOCATION,
                                       0,
                                       numTransmitSources,
                                       numChannels,
                                       numApodizations,
                                       numTGCs,
                                       numElements);
    equal = iusCompareDrivingScheme(obj,ynotherObj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    ynotherObj = dgCreateDrivingScheme(IUS_DIVERGING_WAVES_PARAMETRIZED,
                                       IUS_3D_SHAPE,IUS_PARAMETRIC_3D_SOURCE_LOCATION,
                                       numTransmitPulses,
                                       0,
                                       numChannels,
                                       numApodizations,
                                       numTGCs,
                                       numElements);
    equal = iusCompareDrivingScheme(obj,ynotherObj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    ynotherObj = dgCreateDrivingScheme(IUS_DIVERGING_WAVES_PARAMETRIZED,
                                       IUS_3D_SHAPE,IUS_PARAMETRIC_3D_SOURCE_LOCATION,
                                       numTransmitPulses,
                                       numTransmitSources,
                                       0,
                                       numApodizations,
                                       numTGCs,
                                       numElements);
    equal = iusCompareDrivingScheme(obj,ynotherObj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    ynotherObj = dgCreateDrivingScheme(IUS_DIVERGING_WAVES_PARAMETRIZED,
                                       IUS_3D_SHAPE,IUS_PARAMETRIC_3D_SOURCE_LOCATION,
                                       numTransmitPulses,
                                       numTransmitSources,
                                       numChannels,
                                       0,
                                       numTGCs,
                                       numElements);
    equal = iusCompareDrivingScheme(obj,ynotherObj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    ynotherObj = dgCreateDrivingScheme(IUS_DIVERGING_WAVES_PARAMETRIZED,
                                       IUS_3D_SHAPE,IUS_PARAMETRIC_3D_SOURCE_LOCATION,
                                       numTransmitPulses,
                                       numTransmitSources,
                                       numChannels,
                                       numApodizations,
                                       0,
                                       numElements
                                       );
    equal = iusCompareDrivingScheme(obj,ynotherObj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    ynotherObj = dgCreateDrivingScheme(IUS_DIVERGING_WAVES_PARAMETRIZED,
                                       IUS_3D_SHAPE,IUS_PARAMETRIC_3D_SOURCE_LOCATION,
                                       numTransmitPulses,
                                       numTransmitSources,
                                       numChannels,
                                       numApodizations,
                                       numTGCs,
                                       0);
    equal = iusCompareDrivingScheme(obj,ynotherObj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);


    // invalid
    equal = iusCompareDrivingScheme(obj,NULL);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    equal = iusCompareDrivingScheme(NULL,obj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

    iusDeleteDrivingScheme(obj);
    iusDeleteDrivingScheme(notherObj);
}

TEST(IusDrivingScheme, testIusCreateDrivingScheme)
{
    int numElements = 32;
    int numTransmitPulses = 13;
    int numTransmitSources = 13;
    int numChannels = 6;
    int numApodizations = numTransmitPulses;
    int numTGCs = 10;
    iuds_t obj = dgCreateDrivingScheme(IUS_DIVERGING_WAVES_PARAMETRIZED,
                                       IUS_3D_SHAPE,IUS_PARAMETRIC_3D_SOURCE_LOCATION,
                                       numTransmitPulses,
                                       numTransmitSources,
                                       numChannels,
                                       numApodizations,
                                       numTGCs,
                                       numElements);
    TEST_ASSERT(obj != IUDS_INVALID);
    dgDeleteDrivingScheme(obj);
//
//    // invalid
//    obj = dgCreateDrivingScheme(IUS_INVALID_DRIVING_SCHEME, IUS_3D_SHAPE,IUS_PARAMETRIC_3D_SOURCE_LOCATION, numTransmitPulses, numTransmitSources, numElements);
//    TEST_ASSERT(obj == IUDS_INVALID);
//    obj = dgCreateDrivingScheme(IUS_DIVERGING_WAVES_PARAMETRIZED, IUS_INVALID_SHAPE,IUS_PARAMETRIC_3D_SOURCE_LOCATION, numTransmitPulses, numTransmitSources, numElements);
//    TEST_ASSERT(obj == IUDS_INVALID);
//    obj = dgCreateDrivingScheme(IUS_DIVERGING_WAVES_PARAMETRIZED, IUS_3D_SHAPE,IUS_INVALID_SOURCE_LOCATION, numTransmitPulses, numTransmitSources, numElements);
//    TEST_ASSERT(obj == IUDS_INVALID);
}



TEST_GROUP_RUNNER(IusDrivingScheme)
{
    RUN_TEST_CASE(IusDrivingScheme, testIusCreateDrivingScheme);
//    RUN_TEST_CASE(IusDrivingScheme, testIusDeleteDrivingScheme);
//    RUN_TEST_CASE(IusDrivingScheme, testIusCompareDrivingScheme);
//    RUN_TEST_CASE(IusDrivingScheme, testIusSetGetDrivingScheme);
}
