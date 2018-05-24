//
// Created by Ruijzendaal on 20/03/2018.
//

#include <include/iusHLInputInstance.h>
#include <include/iusHLNode.h>
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>


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

    IusNode *hSimpleNode = iusHLCreateNode(pNodeType, numParents);
    TEST_ASSERT(hSimpleNode != IUN_INVALID);
    // Check type
    char *pActualNodeType = iusHLGetNodeType(hSimpleNode);
    TEST_ASSERT_EQUAL_STRING(pNodeType, pActualNodeType);
    // Check numParents
    TEST_ASSERT(iusHLNodeGetNumParents(hSimpleNode) == 0);
    // Check unique
    TEST_ASSERT(iusHLGetNodeId(hSimpleNode) != IUN_INVALID);
}



TEST_GROUP_RUNNER(InputfileNode)
{
    RUN_TEST_CASE(InputfileNode, testNode);
}