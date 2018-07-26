

//
// Created by nlv09165 on 26/07/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <include/ius.h>
#include <include/iusError.h>
#include <include/iusTypes.h>
#include "include/iusHL3DParametricSource.h"

TEST_GROUP(Ius3DParametricSource);

TEST_SETUP(Ius3DParametricSource)
{
}

TEST_TEAR_DOWN(Ius3DParametricSource)
{
}


TEST(Ius3DParametricSource, testIus3DParametricSourceCreate)
{
    iu3dps_t obj = iusHL3DParametricSourceCreate(1,2.0f);
    iu3dps_t notherObj = iusHL3DParametricSourceCreate(1,2.0f);
    TEST_ASSERT(obj != IU3DPS_INVALID);
    TEST_ASSERT(notherObj != IU3DPS_INVALID);
    iusHL3DParametricSourceDelete(obj);
    iusHL3DParametricSourceDelete(notherObj);

    // invalid params
    obj = iusHL3DParametricSourceCreate(-1,2.0f);
    TEST_ASSERT(obj == IU3DPS_INVALID);
    obj = iusHL3DParametricSourceCreate(1,-2.0f);
    TEST_ASSERT(obj == IU3DPS_INVALID);
}

#if 0
TEST(Ius3DParametricSource, testIus3DParametricSourceDelete)
{
    IUS_BOOL equal;
    iu3dps_t obj = iusHL3DParametricSourceCreate(1,2.0f);
    TEST_ASSERT(obj != IU3DPS_INVALID);
    int status = iusHL3DParametricSourceDelete(obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // invalid params
    status = iusHL3DParametricSourceDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
}



TEST(Ius3DParametricSource, testIus3DParametricSourceCompare)
{
    IUS_BOOL equal;
    iu3dps_t obj = iusHL3DParametricSourceCreate(1,2.0f);
    iu3dps_t notherObj = iusHL3DParametricSourceCreate(1,2.0f);
    TEST_ASSERT(obj != IU3DPS_INVALID);
    TEST_ASSERT(notherObj != IU3DPS_INVALID);
    equal = iusHL3DParametricSourceCompare(obj,obj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = iusHL3DParametricSourceCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    
    // invalid params
    equal = iusHL3DParametricSourceCompare(obj,NULL);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    equal = iusHL3DParametricSourceCompare(NULL,obj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    
    iusHL3DParametricSourceDelete(obj);
    iusHL3DParametricSourceDelete(notherObj);
}


TEST(Ius3DParametricSource, testIus3DParametricSourceSetGet)
{
    IUS_BOOL equal;
    iu3dps_t obj = iusHL3DParametricSourceCreate(1,2.0f);
    TEST_ASSERT(obj != IU3DPS_INVALID);
    
    // int param
    TEST_ASSERT_EQUAL(1,iusHL3DParametricSourceGetIntParam(obj));
    iusHL3DParametricSourceSetIntParam(obj,2);
    TEST_ASSERT_EQUAL(2,iusHL3DParametricSourceGetIntParam(obj));
    
    // float param
    TEST_ASSERT_EQUAL_FLOAT(2.0f,iusHL3DParametricSourceGetFloatParam(obj));
    iusHL3DParametricSourceSetFloatParam(obj,3.0f);
    TEST_ASSERT_EQUAL(3.0f,iusHL3DParametricSourceGetFloatParam(obj));

    // invalid param
    TEST_ASSERT_EQUAL_FLOAT(NAN,iusHL3DParametricSourceGetFloatParam(NULL));
    TEST_ASSERT_EQUAL(-1,iusHL3DParametricSourceGetIntParam(NULL));
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE,iusHL3DParametricSourceSetFloatParam(NULL,2.0f));
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE,iusHL3DParametricSourceSetIntParam(NULL,2));
   
    iusHL3DParametricSourceDelete(obj);
}

#endif

TEST_GROUP_RUNNER(Ius3DParametricSource)
{
    RUN_TEST_CASE(Ius3DParametricSource, testIus3DParametricSourceCreate);
//    RUN_TEST_CASE(Ius3DParametricSource, testIus3DParametricSourceDelete);
//    RUN_TEST_CASE(Ius3DParametricSource, testIus3DParametricSourceCompare);
//    RUN_TEST_CASE(Ius3DParametricSource, testIus3DParametricSourceSetGet);
}
