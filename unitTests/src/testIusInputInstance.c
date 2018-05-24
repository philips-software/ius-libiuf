//
// Created by nlv09165 on 25/04/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <include/ius.h>
#include <include/iusError.h>
#include <include/iusTypes.h>
#include <include/iusHLNode.h>
#include "include/iusHLInputInstance.h"

TEST_GROUP(IusInputInstance);

TEST_SETUP(IusInputInstance)
{
}

TEST_TEAR_DOWN(IusInputInstance)
{
}


TEST(IusInputInstance, testIusCreateInputInstance)
{
    // TODO constructor with arguments..
    IUS_BOOL equal;
    iuii_t obj = iusHLCreateInputInstance();

    iuii_t notherObj = iusHLCreateInputInstance();
    TEST_ASSERT(obj != IUII_INVALID);
    TEST_ASSERT(notherObj != IUII_INVALID);
    iusHLDeleteInputInstance(obj);
    iusHLDeleteInputInstance(notherObj);

    // TODO: invalid params
}

TEST(IusInputInstance, testIusDeleteInputInstance)
{
    IUS_BOOL equal;
    iuii_t obj = iusHLCreateInputInstance();
    TEST_ASSERT(obj != IUII_INVALID);
    int status = iusHLDeleteInputInstance(obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // invalid params
    status = iusHLDeleteInputInstance(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
}



TEST(IusInputInstance, testIusCompareInputInstance)
{
    IUS_BOOL equal;
    int status;

    iuii_t obj = iusHLCreateInputInstance();
    iuii_t notherObj = iusHLCreateInputInstance();
    iun_t node = iusHLInputInstanceGetNode(obj);
    iun_t nothernNode = iusHLInputInstanceGetNode(notherObj);

    TEST_ASSERT(obj != IUII_INVALID);
    TEST_ASSERT(notherObj != IUII_INVALID);

    equal = iusHLCompareInputInstance(obj,obj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);

    // since every node that is being created is unique
    // every InputInstance is unique.
    equal = iusHLCompareInputInstance(obj,notherObj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

    // so, thats why the Node of obj is put into notherObj
    status = iusHLInputInstanceSetNode(notherObj, node);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    equal = iusHLCompareInputInstance(obj,notherObj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);

    iusHLDeleteInputInstance(obj);
    status = iusHLInputInstanceSetNode(notherObj, nothernNode);
    iusHLDeleteInputInstance(notherObj);
}


TEST(IusInputInstance, testIusSetGetInputInstance)
{
    IUS_BOOL equal;
    iuii_t obj = iusHLCreateInputInstance();
    TEST_ASSERT(obj != IUII_INVALID);
    
    // invalid param
    TEST_ASSERT_EQUAL(IUS_DEFAULT_NUM_FRAMES,iusHLInputInstanceGetNumFrames(obj));
    iusHLDeleteInputInstance(obj);
}


TEST_GROUP_RUNNER(IusInputInstance)
{
    RUN_TEST_CASE(IusInputInstance, testIusCreateInputInstance);
    RUN_TEST_CASE(IusInputInstance, testIusDeleteInputInstance);
    RUN_TEST_CASE(IusInputInstance, testIusCompareInputInstance);
    RUN_TEST_CASE(IusInputInstance, testIusSetGetInputInstance);
}
