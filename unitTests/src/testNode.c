//
// Created by Ruijzendaal on 20/03/2018.
//

#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <iusNode.h>
#include <include/iusInputFile.h>
#include <include/ius.h>

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
    char pNodeType[] = IUS_INPUT_TYPE;

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

//Voorbeeld code gebruik met node structuur:
//
//iuif_t inputFile = iusInputFileOpen(“test”);
//iuapd_t algoParams ;
//iuf_t cwcFile = iusFileCreate(“cw1”, “c:\autoexec.bat”, file, algoParams );
//
//
//
//iuf_t iusFile = iusFileOpen( “c:\autoexec.bat” );
//type = iusFileGetType( iusFile );
//// cwc file
//iucf_t cwcFile = (iucf_t)iusFile;
//// voor algoparams, zie workbench
//// zie /Users/nlv09165/proj/PPF/ius/Algorithms/CWC4DWD
//iuapd_t algoParams = iusFileGetAlgoparams( iusFile ); // can be NULL
//
//iupl_t parents = iusFileGetParents(iusFile);
//for( i = 0 ; i < iusParentsSize(parents); i++)
//{
//iuf_t parent = iusParentsGet(i);
//type = iusFileGetType(parent); 
//}

// test scenario
// create inpu file

TEST_GROUP_RUNNER(InputfileNode)
{
    RUN_TEST_CASE(InputfileNode, testNode);
}