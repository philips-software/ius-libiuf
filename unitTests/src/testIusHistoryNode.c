

//
// Created by nlv09165 on 24/08/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <include/ius.h>
#include <include/iusError.h>
#include <include/iusTypes.h>
#include <include/iusHistoryNode.h>

TEST_GROUP(IusHistoryNode);

TEST_SETUP(IusHistoryNode)
{
}

TEST_TEAR_DOWN(IusHistoryNode)
{
}


TEST(IusHistoryNode, testIusHistoryNodeCreate)
{
//    char *type =  IUS_INPUT_TYPE;
//    int numParents = 0;
//    iuhtn_t node = iusHistoryTreeNodeCreate(type,numParents);
//    TEST_ASSERT_NOT_EQUAL(IUHTN_INVALID,node);
//    iusHistoryTreeNodeDelete(node);
//
//    // invalid params
//    node = iusHistoryTreeNodeCreate(NULL,numParents);
//    TEST_ASSERT_EQUAL(IUHTN_INVALID,node);
//
//    node = iusHistoryTreeNodeCreate(type,-1);
//    TEST_ASSERT_EQUAL(IUHTN_INVALID,node);
//    iusHistoryTreeNodeDelete(node);
}

TEST(IusHistoryNode, testIusHistoryNodeDelete)
{

}



TEST(IusHistoryNode, testIusHistoryNodeCompare)
{

}


TEST(IusHistoryNode, testIusHistoryTreeNodeSetGet)
{

}



TEST_GROUP_RUNNER(IusHistoryNode)
{
    RUN_TEST_CASE(IusHistoryNode, testIusHistoryNodeCreate);
    RUN_TEST_CASE(IusHistoryNode, testIusHistoryNodeDelete);
    RUN_TEST_CASE(IusHistoryNode, testIusHistoryNodeCompare);
    RUN_TEST_CASE(IusHistoryNode, testIusHistoryNodeSetGet);
    RUN_TEST_CASE(IusHistoryNode, testIusHistoryNodeSerialize);
}
