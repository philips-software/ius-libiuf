

//
// Created by nlv09165 on 24/08/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <ius.h>
#include <testDataGenerators.h>
#include <iusHistoryNodePrivate.h>

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
    iuhn_t node = iusHistoryNodeCreate(type);
    TEST_ASSERT_NOT_EQUAL(IUHN_INVALID,node);
    iusHistoryNodeDelete(node);

    // invalid params
    node = iusHistoryNodeCreate(NULL);
    TEST_ASSERT_EQUAL(IUHN_INVALID,node);
}

TEST(IusHistoryNode, testIusHistoryNodeDelete)
{
    char *type =  IUS_INPUT_TYPE;
    iuhn_t node = iusHistoryNodeCreate(type);
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


TEST(IusHistoryNode, testIusHistoryNodeSerialize)
{

    // create and fill
    iuhn_t historyNode = dgGenerateHistoryNode();
    char *filename = "testIusHistoryNodeSerialization.hdf5";

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
    RUN_TEST_CASE(IusHistoryNode, testIusHistoryNodeCreate);
    RUN_TEST_CASE(IusHistoryNode, testIusHistoryNodeDelete);
    RUN_TEST_CASE(IusHistoryNode, testIusHistoryNodeCompare);
    RUN_TEST_CASE(IusHistoryNode, testIusHistoryNodeSetGet);
    RUN_TEST_CASE(IusHistoryNode, testIusHistoryNodeSerialize);
}
