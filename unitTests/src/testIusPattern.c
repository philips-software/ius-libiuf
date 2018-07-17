

//
// Created by nlv09165 on 13/07/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <include/ius.h>
#include <include/iusError.h>
#include <include/iusTypes.h>
#include "include/iusHLPattern.h"

TEST_GROUP(IusPattern);

TEST_SETUP(IusPattern)
{
}

TEST_TEAR_DOWN(IusPattern)
{
}


TEST(IusPattern, testIusPatternCreate)
{
    IUS_BOOL equal;
    iup_t obj = iusHLPatternCreate("bmode",0.01f);
    iup_t notherObj = iusHLPatternCreate("bmode",0.01f);
    TEST_ASSERT(obj != IUP_INVALID);
    TEST_ASSERT(notherObj != IUP_INVALID);
    iusHLPatternDelete(obj);
    iusHLPatternDelete(notherObj);

    // invalid params
    obj = iusHLPatternCreate(NULL,2.0f);
    TEST_ASSERT(obj == IUP_INVALID);
    obj = iusHLPatternCreate("",2.0f);
    TEST_ASSERT(obj == IUP_INVALID);
    obj = iusHLPatternCreate("bmode",-2.0f);
    TEST_ASSERT(obj == IUP_INVALID);
}

TEST(IusPattern, testIusPatternDelete)
{
    IUS_BOOL equal;
    iup_t obj = iusHLPatternCreate(1,2.0f);
    TEST_ASSERT(obj != IUP_INVALID);
    int status = iusHLPatternDelete(obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // invalid params
    status = iusHLPatternDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
}



TEST(IusPattern, testIusPatternCompare)
{
    IUS_BOOL equal;
    iup_t obj = iusHLPatternCreate(1,2.0f);
    iup_t notherObj = iusHLPatternCreate(1,2.0f);
    TEST_ASSERT(obj != IUP_INVALID);
    TEST_ASSERT(notherObj != IUP_INVALID);
    equal = iusHLPatternCompare(obj,obj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = iusHLPatternCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    
    // invalid params
    equal = iusHLPatternCompare(obj,NULL);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    equal = iusHLPatternCompare(NULL,obj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    
    iusHLPatternDelete(obj);
    iusHLPatternDelete(notherObj);
}


TEST(IusPattern, testIusPatternSetGet)
{
    IUS_BOOL equal;
    iup_t obj = iusHLPatternCreate(1,2.0f);
    TEST_ASSERT(obj != IUP_INVALID);
    
    // int param
    TEST_ASSERT_EQUAL(1,iusHLPatternGetIntParam(obj));
    iusHLPatternSetIntParam(obj,2);
    TEST_ASSERT_EQUAL(2,iusHLPatternGetIntParam(obj));
    
    // float param
    TEST_ASSERT_EQUAL_FLOAT(2.0f,iusHLPatternGetFloatParam(obj));
    iusHLPatternSetFloatParam(obj,3.0f);
    TEST_ASSERT_EQUAL(3.0f,iusHLPatternGetFloatParam(obj));

    // invalid param
    TEST_ASSERT_EQUAL_FLOAT(NAN,iusHLPatternGetFloatParam(NULL));
    TEST_ASSERT_EQUAL(-1,iusHLPatternGetIntParam(NULL));
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE,iusHLPatternSetFloatParam(NULL,2.0f));
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE,iusHLPatternSetIntParam(NULL,2));
   
    iusHLPatternDelete(obj);
}



TEST_GROUP_RUNNER(IusPattern)
{
    RUN_TEST_CASE(IusPattern, testIusPatternCreate);
//    RUN_TEST_CASE(IusPattern, testIusPatternDelete);
//    RUN_TEST_CASE(IusPattern, testIusPatternCompare);
//    RUN_TEST_CASE(IusPattern, testIusPatternSetGet);
}
