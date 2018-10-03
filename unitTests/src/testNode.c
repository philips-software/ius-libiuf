//
// Created by Ruijzendaal on 20/03/2018.
//

#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <iusNode.h>
#include <iusInputInstance.h>

TEST_GROUP(InputfileNode);

TEST_SETUP(InputfileNode)
{
}

TEST_TEAR_DOWN(InputfileNode)
{
}

//
// ADT Node has
// Unique id
// Type, uniquely specifying file contents
// 0-n Parent nodes.
//
TEST(InputfileNode, testNode)
{
    int numParents = 0;
    char pNodeType[] = "IUSINPUTFILETYPE_V3";

    IusNode *hSimpleNode = iusCreateNode(pNodeType, numParents);
    TEST_ASSERT(hSimpleNode != IUN_INVALID);
    // Check type
    char *pActualNodeType = iusGetNodeType(hSimpleNode);
    TEST_ASSERT_EQUAL_STRING(pNodeType, pActualNodeType);
    // Check numParents
    TEST_ASSERT(iusNodeGetNumParents(hSimpleNode) == 0);
    // Check unique
    TEST_ASSERT(iusGetNodeId(hSimpleNode) != IUN_INVALID);
}



TEST_GROUP_RUNNER(InputfileNode)
{
    RUN_TEST_CASE(InputfileNode, testNode);
}