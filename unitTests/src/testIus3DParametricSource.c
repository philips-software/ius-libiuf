

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
    float angularDelta = 0.13f;
    float FNumber = -0.955f;
    float startAngle = 3.14f;
    float startPhi = startAngle;
    float deltaPhi = angularDelta;
    char *pLabel = "label for 3d parametric source";

    iu3dps_t obj = iusHL3DParametricSourceCreate(pLabel, FNumber, angularDelta, startAngle, deltaPhi, startPhi);
    iu3dps_t notherObj = iusHL3DParametricSourceCreate(pLabel, FNumber, angularDelta, startAngle, deltaPhi, startPhi);
    TEST_ASSERT(obj != IU3DPS_INVALID);
    TEST_ASSERT(notherObj != IU3DPS_INVALID);
    iusHL3DParametricSourceDelete(obj);
    iusHL3DParametricSourceDelete(notherObj);

    // invalid params
    obj = iusHL3DParametricSourceCreate(NULL, -1, 2.0f, 0, 0, 0);
    TEST_ASSERT(obj == IU3DPS_INVALID);
    obj = iusHL3DParametricSourceCreate(NULL, 1, -2.0f, 0, 0, 0);
    TEST_ASSERT(obj == IU3DPS_INVALID);
}

TEST(Ius3DParametricSource, testIus3DParametricSourceDelete)
{
    float angularDelta = 0.13f;
    float FNumber = -0.955f;
    float startAngle = 3.14f;
    float startPhi = startAngle;
    float deltaPhi = angularDelta;
    char *pLabel = "label for 3d parametric source";
    iu3dps_t obj = iusHL3DParametricSourceCreate(pLabel, FNumber, angularDelta, startAngle, deltaPhi, startPhi);
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
    float angularDelta = 0.13f;
    float FNumber = -0.955f;
    float startAngle = 3.14f;
    float startPhi = startAngle;
    float deltaPhi = angularDelta;
    char *pLabel = "label for 3d parametric source";

    iu3dps_t obj = iusHL3DParametricSourceCreate(pLabel, FNumber, angularDelta, startAngle, deltaPhi, startPhi);
    iu3dps_t notherObj = iusHL3DParametricSourceCreate(pLabel, FNumber, angularDelta, startAngle, deltaPhi, startPhi);
    iu3dps_t differentObj = iusHL3DParametricSourceCreate(pLabel, FNumber+0.01f, angularDelta, startAngle, deltaPhi, startPhi);
    TEST_ASSERT(obj != IU3DPS_INVALID);
    TEST_ASSERT(notherObj != IU3DPS_INVALID);
    equal = iusHL3DParametricSourceCompare(obj,obj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = iusHL3DParametricSourceCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = iusHL3DParametricSourceCompare(obj,differentObj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

    // invalid params
    equal = iusHL3DParametricSourceCompare(obj,NULL);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    equal = iusHL3DParametricSourceCompare(NULL,obj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    
    iusHL3DParametricSourceDelete(obj);
    iusHL3DParametricSourceDelete(notherObj);
    iusHL3DParametricSourceDelete(differentObj);
}

#if 0
TEST(Ius3DParametricSource, testIus3DParametricSourceSetGet)
{
    IUS_BOOL equal;
    float angularDelta = 0.13f;
    float FNumber = -0.955f;
    float startAngle = 3.14f;
    float startPhi = startAngle;
    float deltaPhi = angularDelta;
    char *pLabel = "label for 3d parametric source";

    iu3dps_t obj = iusHL3DParametricSourceCreate(pLabel, FNumber, angularDelta, startAngle, deltaPhi, startPhi);

    TEST_ASSERT_EQUAL_FLOAT(FNumber,iusHL3DParametricSourceGetFNumber(obj));
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
    RUN_TEST_CASE(Ius3DParametricSource, testIus3DParametricSourceDelete);
    RUN_TEST_CASE(Ius3DParametricSource, testIus3DParametricSourceCompare);
//    RUN_TEST_CASE(Ius3DParametricSource, testIus3DParametricSourceSetGet);
}
