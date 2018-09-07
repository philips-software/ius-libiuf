

//
// Created by nlv09165 on 24/08/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <include/ius.h>
#include <include/iusError.h>
#include <include/iusTypes.h>
#include <include/iusHistoryNodeList.h>

TEST_GROUP(IusHistoryNode);

TEST_SETUP(IusHistoryNode)
{
}

TEST_TEAR_DOWN(IusHistoryNode)
{
}


TEST(IusHistoryNode, testIusHistoryNodeCreate)
{
    char *type =  IUS_INPUT_TYPE;
    int numParents = 0;
    iuhn_t node = iusHistoryNodeCreate(type,numParents);
    TEST_ASSERT_NOT_EQUAL(IUHN_INVALID,node);
    iusHistoryNodeDelete(node);

    // invalid params
    node = iusHistoryNodeCreate(NULL,numParents);
    TEST_ASSERT_EQUAL(IUHN_INVALID,node);

    node = iusHistoryNodeCreate(type,-1);
    TEST_ASSERT_EQUAL(IUHN_INVALID,node);
    iusHistoryNodeDelete(node);
}

TEST(IusHistoryNode, testIusHistoryNodeDelete)
{
    char *type =  IUS_INPUT_TYPE;
    int numParents = 0;
    iuhn_t node = iusHistoryNodeCreate(type,numParents);
    TEST_ASSERT_NOT_EQUAL(IUHN_INVALID,node);
    int status = iusHistoryNodeDelete(node);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);

    // invalid params
    status = iusHistoryNodeDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
}



TEST(IusHistoryNode, testIusHistoryNodeCompare)
{
    char *type =  IUS_INPUT_TYPE;
    int numParents = 1;
    iuhn_t node = iusHistoryNodeCreate(type,numParents);
    TEST_ASSERT_NOT_EQUAL(IUHN_INVALID,node);
    iuhn_t notherNode = iusHistoryNodeCreate(type,numParents);
    TEST_ASSERT_NOT_EQUAL(IUHN_INVALID,node);

    // History nodes will always have unique Id
    IUS_BOOL equal = iusHistoryNodeCompareWithId(node,notherNode);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    equal = iusHistoryNodeCompare(node,notherNode);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);

    // change node, compare
    // change parents
    iuhn_t parentNode = iusHistoryNodeCreate(type,0);
    iuhnl_t parentList = iusHistoryNodeGetParents(node);
    int status = iusHistoryNodeListSet(parentList,parentNode,0);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);
    status = iusHistoryNodeSetParents(node,parentList);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);
    equal = iusHistoryNodeCompare(node,notherNode);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

    // change paraneters

    // same change to notherNode, repeat comparison
    parentList = iusHistoryNodeGetParents(notherNode);
    status = iusHistoryNodeListSet(parentList,parentNode,0);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);
    status = iusHistoryNodeSetParents(node,parentList);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);
    equal = iusHistoryNodeCompare(node,notherNode);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);

    iusHistoryNodeDelete(node);
}


TEST(IusHistoryNode, testIusHistoryTreeNodeSetGet)
{

}



TEST_GROUP_RUNNER(IusHistoryNode)
{
    RUN_TEST_CASE(IusHistoryNode, testIusHistoryNodeCreate);
    RUN_TEST_CASE(IusHistoryNode, testIusHistoryNodeDelete);
    RUN_TEST_CASE(IusHistoryNode, testIusHistoryNodeCompare);
//    RUN_TEST_CASE(IusHistoryNode, testIusHistoryNodeSetGet);
//    RUN_TEST_CASE(IusHistoryNode, testIusHistoryNodeSerialize);
}
