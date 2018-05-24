
//
// Created by nlv09165 on 24/04/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <include/ius.h>
#include <include/iusError.h>
#include <include/iusTypes.h>
#include "include/iusHLApodizationList.h"
#include "../../external/Unity/src/unity.h"

TEST_GROUP(IusApodizationList);

TEST_SETUP(IusApodizationList)
{
}

TEST_TEAR_DOWN(IusApodizationList)
{
}


TEST(IusApodizationList, testIusCreateApodizationList)
{
    IUS_BOOL equal;
    iual_t obj = iusHLCreateApodizationList(1,2.0f);
    iual_t notherObj = iusHLCreateApodizationList(1,2.0f);
    TEST_ASSERT(obj != IUAL_INVALID);
    TEST_ASSERT(notherObj != IUAL_INVALID);
    iusHLDeleteApodizationList(obj);
    iusHLDeleteApodizationList(notherObj);

    // invalid params
    obj = iusHLCreateApodizationList(-1,2.0f);
    TEST_ASSERT(obj == IUAL_INVALID);
    obj = iusHLCreateApodizationList(1,-2.0f);
    TEST_ASSERT(obj == IUAL_INVALID);
}

TEST(IusApodizationList, testIusDeleteApodizationList)
{
    IUS_BOOL equal;
    iual_t obj = iusHLCreateApodizationList(1,2.0f);
    TEST_ASSERT(obj != IUAL_INVALID);
    int status = iusHLDeleteApodizationList(obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // invalid params
    status = iusHLDeleteApodizationList(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
}



TEST(IusApodizationList, testIusCompareApodizationList)
{
    IUS_BOOL equal;
    iual_t obj = iusHLCreateApodizationList(1,2.0f);
    iual_t notherObj = iusHLCreateApodizationList(1,2.0f);
    TEST_ASSERT(obj != IUAL_INVALID);
    TEST_ASSERT(notherObj != IUAL_INVALID);
    equal = iusHLCompareApodizationList(obj,obj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = iusHLCompareApodizationList(obj,notherObj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    iusHLDeleteApodizationList(obj);
    iusHLDeleteApodizationList(notherObj);
}



TEST( IusApodizationList, testIusGetApodizationList ) {
    int numElements = 32;
    int numApodizations = 2;
    int status = 0;

    iual_t transmitApodization = iusHLCreateApodizationList(numApodizations,numElements);

    // By default, apodization should be initialized to 1.0f for all elements.
    float elementApodization;
    int apodizationIndex;
    int elementIndex;
    for (apodizationIndex = 0; apodizationIndex < numApodizations; apodizationIndex++) {
        for (elementIndex = 0; elementIndex < numElements; elementIndex++) {
            elementApodization = iusHLApodizationListGetApodization( transmitApodization, apodizationIndex, elementIndex );
            TEST_ASSERT_EQUAL_FLOAT( 1.0f, elementApodization );
        }
    }

    status = iusHLDeleteApodizationList( transmitApodization );
    TEST_ASSERT( status == IUS_E_OK );

    // Invalid params
    elementApodization = iusHLApodizationListGetApodization( transmitApodization, apodizationIndex, numElements );
    TEST_ASSERT_EQUAL_FLOAT( NAN, elementApodization );

    elementApodization = iusHLApodizationListGetApodization( transmitApodization, -1, -10 );
    TEST_ASSERT_EQUAL_FLOAT( NAN, elementApodization );



}


TEST( IusApodizationList, testIusSetApodizationList ) {
    int numElements = 32;
    int numApodizations = 2;
    int status = 0;

    iual_t transmitApodization = iusHLCreateApodizationList(numApodizations,numElements);

// By default, apodization should be initialized to 1.0f for all elements.
    float elementApodization;
    int apodizationIndex;
    int elementIndex;
    for (apodizationIndex = 0; apodizationIndex < numApodizations; apodizationIndex++) {
        for (elementIndex = 0; elementIndex < numElements; elementIndex++) {
            elementApodization = 1.0f / ((apodizationIndex % 10) + 1);
            status = iusHLApodizationListSetApodization( transmitApodization, apodizationIndex,
                                                             elementIndex, elementApodization );
            TEST_ASSERT( status == IUS_E_OK );
            TEST_ASSERT_EQUAL_FLOAT( elementApodization,
                                     iusHLApodizationListGetApodization( transmitApodization, apodizationIndex, elementIndex ));
        }
    }

    status = iusHLDeleteApodizationList( transmitApodization );
    TEST_ASSERT( status == IUS_E_OK );

    // Invalid params
    // - negative apo's
    // - apo's > 1.0f
    // - to large apo indices
    // - negative indices
    status = iusHLApodizationListSetApodization( transmitApodization, 0, 0, -0.1f );
    TEST_ASSERT( status == IUS_ERR_VALUE );

    status = iusHLApodizationListSetApodization( transmitApodization, 0, 0, 1.001f );
    TEST_ASSERT( status == IUS_ERR_VALUE );

    status = iusHLApodizationListSetApodization( transmitApodization, 0, numElements , 0.1f);
    TEST_ASSERT( status == IUS_ERR_VALUE );
    status = iusHLApodizationListSetApodization( transmitApodization, numApodizations, 0 , 0.1f);
    TEST_ASSERT( status == IUS_ERR_VALUE );

    status = iusHLApodizationListSetApodization( transmitApodization, -1,   0, 0.1f );
    TEST_ASSERT( status == IUS_ERR_VALUE );
    status = iusHLApodizationListSetApodization( transmitApodization,  0, -10, 0.1f );
    TEST_ASSERT( status == IUS_ERR_VALUE );
}


TEST_GROUP_RUNNER(IusApodizationList)
{
    RUN_TEST_CASE(IusApodizationList, testIusCreateApodizationList);
    RUN_TEST_CASE(IusApodizationList, testIusDeleteApodizationList);
    RUN_TEST_CASE(IusApodizationList, testIusCompareApodizationList);
    RUN_TEST_CASE(IusApodizationList, testIusSetApodizationList);
    RUN_TEST_CASE(IusApodizationList, testIusGetApodizationList);
}
