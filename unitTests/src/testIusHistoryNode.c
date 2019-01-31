

//
// Created by nlv09165 on 24/08/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <ius.h>
#include <testDataGenerators.h>
#include <iusHistoryNodePrivate.h>


static char *pErrorFilename = "IusHistoryNode.errlog";
static FILE *fpErrorLogging = NULL;
static char *hierFilename = "testIusHistoryNodeHierarchy.hdf5";
static char *filename = "testIusHistoryNodeSerialization.hdf5";

TEST_GROUP(IusHistoryNode);

TEST_SETUP(IusHistoryNode)
{
    iusErrorLogClear();
    iusErrorLog(IUS_FALSE);
    iusErrorAutoReport(IUS_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iusErrorSetStream(fpErrorLogging);
    iusErrorSetStream(stderr);
}

TEST_TEAR_DOWN(IusHistoryNode)
{
    iusErrorLogClear();
    iusErrorLog(IUS_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iusErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
    remove(filename);
    remove(hierFilename);
}

TEST(IusHistoryNode, testIusHistoryNodeCreate)
{
    char *type =  IUS_INPUT_TYPE;
    iuhn_t node = iusHistoryNodeCreate(type);
    TEST_ASSERT_NOT_EQUAL(IUHN_INVALID,node);
    iusHistoryNodeDelete(node);

    // invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());

    node = iusHistoryNodeCreate(NULL);
    TEST_ASSERT_EQUAL(IUHN_INVALID,node);

    TEST_ASSERT_EQUAL(1,iusErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
}

TEST(IusHistoryNode, testIusHistoryNodeDelete)
{
    char *type =  IUS_INPUT_TYPE;
    iuhn_t node = iusHistoryNodeCreate(type);
    TEST_ASSERT_NOT_EQUAL(IUHN_INVALID,node);
    int status = iusHistoryNodeDelete(node);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);

    // invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());

    status = iusHistoryNodeDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);

    TEST_ASSERT_EQUAL(1,iusErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
}



TEST(IusHistoryNode, testIusHistoryNodeCompare)
{
    char *type =  IUS_INPUT_TYPE;
    iuhn_t node = iusHistoryNodeCreate(type);
    TEST_ASSERT_NOT_EQUAL(IUHN_INVALID,node);
    iuhn_t notherNode = iusHistoryNodeCreate(type);
    TEST_ASSERT_NOT_EQUAL(IUHN_INVALID,node);
    iuhnl_t parents = iusHistoryNodeListCreate(1);
    iuhnl_t notherParents = iusHistoryNodeListCreate(1);
    iusHistoryNodeSetParents(node,parents);
    iusHistoryNodeSetParents(notherNode,notherParents);

    // History nodes will always have unique Id
    IUS_BOOL equal = iusHistoryNodeCompareWithId(node,notherNode);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    equal = iusHistoryNodeCompare(node,notherNode);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);

    // change node, compare
    // change parents
    iuhn_t parentNode = iusHistoryNodeCreate(type);
    iuhnl_t parentList = iusHistoryNodeGetParents(node);
    int status = iusHistoryNodeListSet(parentList,parentNode,0);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);
    status = iusHistoryNodeSetParents(node,parentList);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);
    equal = iusHistoryNodeCompare(node,notherNode);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

    // same change to notherNode, repeat comparison
    parentList = iusHistoryNodeGetParents(notherNode);
    status = iusHistoryNodeListSet(parentList,parentNode,0);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);
    status = iusHistoryNodeSetParents(notherNode,parentList);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);
    equal = iusHistoryNodeCompare(node,notherNode);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);

    // change paraneters
    iupad_t params = dgGenerateParameterDict(20);
    status = iusHistoryNodeSetParameters(node, params);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);
    equal = iusHistoryNodeCompare(node,notherNode);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

    status = iusHistoryNodeSetParameters(notherNode, params);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);
    equal = iusHistoryNodeCompare(node,notherNode);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);

    iusHistoryNodeDelete(node);
    iusHistoryNodeDelete(notherNode);
    iusHistoryNodeDelete(parentNode);
    iusHistoryNodeListDelete(parents);
    iusHistoryNodeListDelete(notherParents);
    iusParameterDictDelete(params);

}


TEST(IusHistoryNode, testIusHistoryNodeSetGet)
{
    char *type =  IUS_INPUT_TYPE;
    int numParents = 0;
    iuhn_t node;

    node = iusHistoryNodeCreate(type);
    TEST_ASSERT_NOT_EQUAL(IUHN_INVALID,node);
    TEST_ASSERT_EQUAL_STRING(IUS_INPUT_TYPE, iusHistoryNodeGetType(node));
    TEST_ASSERT_EQUAL(numParents, iusHistoryNodeGetNumParents(node));
    iuhnl_t parents = iusHistoryNodeGetParents(node);
    TEST_ASSERT_EQUAL(parents, IUHNL_INVALID);
    iusHistoryNodeDelete(node);

    for (numParents=1 ; numParents < 100 ; numParents++)
    {
        node = iusHistoryNodeCreate(type);
        TEST_ASSERT_NOT_EQUAL(IUHN_INVALID,node);
        TEST_ASSERT_EQUAL_STRING(IUS_INPUT_TYPE, iusHistoryNodeGetType(node));
        parents = iusHistoryNodeListCreate(numParents);
        TEST_ASSERT_EQUAL(IUS_E_OK,iusHistoryNodeSetParents(node,parents));
        TEST_ASSERT_EQUAL(numParents, iusHistoryNodeGetNumParents(node));
        parents = iusHistoryNodeGetParents(node);
        TEST_ASSERT_EQUAL(numParents, iusHistoryNodeListGetSize(parents));
        iusHistoryNodeDelete(node);
        iusHistoryNodeListDelete(parents);
    }

    iupad_t parameterDict = dgGenerateParameterDict(10);
    node = iusHistoryNodeCreate(type);
    iusHistoryNodeSetParameters(node, parameterDict);
    iupad_t otherPd = iusHistoryNodeGetParameters(node);
    IUS_BOOL equal = iusParameterDictCompare(parameterDict,otherPd);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);
    iusHistoryNodeDelete(node);
    iusParameterDictDelete(parameterDict);
}

iuhn_t dgGenerateNodeHierarchy
(
    char *topLevel,
    iuhn_t node_h
)
{
    iuhn_t node_a = iusHistoryNodeCreate(topLevel);
    iuhn_t node_b = iusHistoryNodeCreate("B");
    iuhn_t node_c = iusHistoryNodeCreate("C");
    iuhnl_t nodeList_b = iusHistoryNodeListCreate(1);
    iuhnl_t nodeList_c = iusHistoryNodeListCreate(1);
    iupad_t paramDict_a = iusParameterDictCreate();
    iupad_t paramDict_b = iusParameterDictCreate();

    iusParameterDictSet(paramDict_a,"Param for A", "Value for A");
    iusParameterDictSet(paramDict_b,"Param for B", "Value for B");

    iusHistoryNodeListSet(nodeList_b, node_b, 0);
    iusHistoryNodeSetParents(node_a, nodeList_b);
    iusHistoryNodeSetParameters(node_a, paramDict_a);

    iusHistoryNodeListSet(nodeList_c, node_c, 0);
    iusHistoryNodeSetParents(node_b, nodeList_c);
    iusHistoryNodeSetParameters(node_b, paramDict_b);

    if (node_h == IUHN_INVALID)
    {
        return node_a;
    }
    iuhnl_t nodeList_h = iusHistoryNodeListCreate(1);
    iusHistoryNodeListSet(nodeList_h, node_a, 0);
    iusHistoryNodeSetParents(node_h, nodeList_h);
    return node_h;
}

int dgGenerateNodeHierarchyFile
(
        char *filename
)
{
    iuhn_t node = dgGenerateNodeHierarchy("A", IUHN_INVALID);
    int status = 0;
    // save
    hid_t handle = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    if (handle > 0)
    {
        status = iusHistoryNodeSave(node, handle);
        status |= H5Fclose(handle);
    }
    return status;
}

TEST(IusHistoryNode, testIusHistoryNodeHierarchyFromFile)
{
    // test scenario:
    // generate node hierarchy file HF
    int status = dgGenerateNodeHierarchyFile(filename);
    TEST_ASSERT_EQUAL(0,status);

    // Create new node base on node history from file.
    iuhn_t newNode = iusHistoryNodeCreateFromFile("NewFileType", filename);

    // Construct node hierarchy from file, as you would expect it from
    // iusHistoryNodeCreateFromFile and compare the results.
    hid_t handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT);
    TEST_ASSERT(handle > 0);

    iuhn_t savedObj = iusHistoryNodeLoad(handle);
    TEST_ASSERT(savedObj != NULL);
    H5Fclose(handle);

    // create new node NN
    iuhn_t node = iusHistoryNodeCreate("NewFileType");
    iuhnl_t nodeList_nt = iusHistoryNodeListCreate(1);
    status = iusHistoryNodeListSet(nodeList_nt,savedObj,0);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    iusHistoryNodeSetParents(node, nodeList_nt);

    TEST_ASSERT_EQUAL(IUS_TRUE, iusHistoryNodeCompare(node,newNode));
}

TEST(IusHistoryNode, testIusHistoryNodeHierarchyFromMemory)
{
    // test scenario:
    //
    // generate node hierarchy from memory
    // save it to file
    // load from file
    // compare with original
    iuhn_t node = dgGenerateNodeHierarchy("A", IUHN_INVALID);

    // save
    hid_t handle = H5Fcreate(hierFilename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    TEST_ASSERT(handle > 0);

    int status = iusHistoryNodeSave(node, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // read back
    handle = H5Fopen(hierFilename, H5F_ACC_RDONLY, H5P_DEFAULT);
    TEST_ASSERT(handle > 0);

    iuhn_t savedObj = iusHistoryNodeLoad(handle);
    TEST_ASSERT(savedObj != NULL);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUS_TRUE, iusHistoryNodeCompareWithId(node,savedObj));
}


TEST(IusHistoryNode, testIusHistoryNodeSerialize)
{

    // create and fill
    iuhn_t historyNode = dgGenerateHistoryNode();

    // save
    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);

    int status = iusHistoryNodeSave(historyNode, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);

    iuhn_t savedObj = iusHistoryNodeLoad(handle);
    TEST_ASSERT(savedObj != NULL);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUS_TRUE, iusHistoryNodeCompareWithId(historyNode,savedObj));

    if( iusHistoryNodeGetNumParams(historyNode) != 0 )
        iusParameterDictDelete(iusHistoryNodeGetParameters(historyNode));
    iusHistoryNodeDelete(historyNode);
    iusHistoryNodeDelete(savedObj);
}


TEST_GROUP_RUNNER(IusHistoryNode)
{
//    RUN_TEST_CASE(IusHistoryNode, testIusHistoryNodeCreate);
//    RUN_TEST_CASE(IusHistoryNode, testIusHistoryNodeDelete);
//    RUN_TEST_CASE(IusHistoryNode, testIusHistoryNodeCompare);
//    RUN_TEST_CASE(IusHistoryNode, testIusHistoryNodeSetGet);
    RUN_TEST_CASE(IusHistoryNode, testIusHistoryNodeHierarchyFromMemory);
    RUN_TEST_CASE(IusHistoryNode, testIusHistoryNodeHierarchyFromFile);
//    RUN_TEST_CASE(IusHistoryNode, testIusHistoryNodeCreate);
//    RUN_TEST_CASE(IusHistoryNode, testIusHistoryNodeSerialize);
}
