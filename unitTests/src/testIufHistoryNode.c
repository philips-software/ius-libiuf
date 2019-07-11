#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <iuf.h>
#include <dg/dataGenerators.h>
#include <iufHistoryNodePrivate.h>


static char *pErrorFilename = "IufHistoryNode.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(IufHistoryNode);

TEST_SETUP(IufHistoryNode)
{
    iufErrorLogClear();
    iufErrorLog(IUF_TRUE);
    iufErrorAutoReportSet(IUF_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iufErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IufHistoryNode)
{
    iufErrorLogClear();
    iufErrorLog(IUF_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iufErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
}

TEST(IufHistoryNode, testIufHistoryNodeCreate)
{
    char *type =  IUF_INPUT_TYPE;
    iuhn_t node = iufHistoryNodeCreate(type);
    TEST_ASSERT_NOT_EQUAL(IUHN_INVALID,node);
    iufHistoryNodeDelete(node);

    // invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    node = iufHistoryNodeCreate(NULL);
    TEST_ASSERT_EQUAL(IUHN_INVALID,node);

    TEST_ASSERT_EQUAL(1,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
}

TEST(IufHistoryNode, testIufHistoryNodeDelete)
{
    char *type =  IUF_INPUT_TYPE;
    iuhn_t node = iufHistoryNodeCreate(type);
    TEST_ASSERT_NOT_EQUAL(IUHN_INVALID,node);
    int status = iufHistoryNodeDelete(node);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);

    // invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    status = iufHistoryNodeDelete(NULL);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, status);

    TEST_ASSERT_EQUAL(1,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
}



TEST(IufHistoryNode, testIufHistoryNodeCompare)
{
    char *type =  IUF_INPUT_TYPE;
    iuhn_t node = iufHistoryNodeCreate(type);
    TEST_ASSERT_NOT_EQUAL(IUHN_INVALID,node);
    iuhn_t notherNode = iufHistoryNodeCreate(type);
    TEST_ASSERT_NOT_EQUAL(IUHN_INVALID,node);
    iuhnl_t parents = iufHistoryNodeListCreate(1);
    iuhnl_t notherParents = iufHistoryNodeListCreate(1);
    iufHistoryNodeSetParents(node,parents);
    iufHistoryNodeSetParents(notherNode,notherParents);

    // History nodes will always have unique Id
    IUF_BOOL equal = iufHistoryNodeCompareWithId(node,notherNode);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);
    equal = iufHistoryNodeCompare(node,notherNode);
    TEST_ASSERT_EQUAL(IUF_TRUE,equal);

    // change node, compare
    // change parents
    iuhn_t parentNode = iufHistoryNodeCreate(type);
    iuhnl_t parentList = iufHistoryNodeGetParents(node);
    int status = iufHistoryNodeListSet(parentList,parentNode,0);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);
    status = iufHistoryNodeSetParents(node,parentList);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);
    equal = iufHistoryNodeCompare(node,notherNode);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);

    // same change to notherNode, repeat comparison
    parentList = iufHistoryNodeGetParents(notherNode);
    status = iufHistoryNodeListSet(parentList,parentNode,0);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);
    status = iufHistoryNodeSetParents(notherNode,parentList);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);
    equal = iufHistoryNodeCompare(node,notherNode);
    TEST_ASSERT_EQUAL(IUF_TRUE,equal);

    // change paraneters
    iupad_t params = dgGenerateParameterDict(20);
    status = iufHistoryNodeSetParameters(node, params);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);
    equal = iufHistoryNodeCompare(node,notherNode);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);

    status = iufHistoryNodeSetParameters(notherNode, params);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);
    equal = iufHistoryNodeCompare(node,notherNode);
    TEST_ASSERT_EQUAL(IUF_TRUE,equal);

    iufHistoryNodeDelete(node);
    iufHistoryNodeDelete(notherNode);
    iufHistoryNodeDelete(parentNode);
    iufHistoryNodeListDelete(parents);
    iufHistoryNodeListDelete(notherParents);
    iufParameterDictDelete(params);

}


TEST(IufHistoryNode, testIufHistoryNodeSetGet)
{
    char *type =  IUF_INPUT_TYPE;
    int numParents = 0;
    iuhn_t node;

    node = iufHistoryNodeCreate(type);
    TEST_ASSERT_NOT_EQUAL(IUHN_INVALID,node);
    TEST_ASSERT_EQUAL_STRING(IUF_INPUT_TYPE, iufHistoryNodeGetType(node));
    TEST_ASSERT_EQUAL(numParents, iufHistoryNodeGetNumParents(node));
    iuhnl_t parents = iufHistoryNodeGetParents(node);
    TEST_ASSERT_EQUAL(parents, IUHNL_INVALID);
    iufHistoryNodeDelete(node);

    for (numParents=1 ; numParents < 100 ; numParents++)
    {
        node = iufHistoryNodeCreate(type);
        TEST_ASSERT_NOT_EQUAL(IUHN_INVALID,node);
        TEST_ASSERT_EQUAL_STRING(IUF_INPUT_TYPE, iufHistoryNodeGetType(node));
        parents = iufHistoryNodeListCreate(numParents);
        TEST_ASSERT_EQUAL(IUF_E_OK,iufHistoryNodeSetParents(node,parents));
        TEST_ASSERT_EQUAL(numParents, iufHistoryNodeGetNumParents(node));
        parents = iufHistoryNodeGetParents(node);
        TEST_ASSERT_EQUAL(numParents, iufHistoryNodeListGetSize(parents));
        iufHistoryNodeDelete(node);
        iufHistoryNodeListDelete(parents);
    }

    iupad_t parameterDict = dgGenerateParameterDict(10);
    node = iufHistoryNodeCreate(type);
    iufHistoryNodeSetParameters(node, parameterDict);
    iupad_t otherPd = iufHistoryNodeGetParameters(node);
    IUF_BOOL equal = iufParameterDictCompare(parameterDict,otherPd);
    TEST_ASSERT_EQUAL(IUF_TRUE, equal);
    iufHistoryNodeDelete(node);
    iufParameterDictDelete(parameterDict);
}


TEST(IufHistoryNode, testIufHistoryNodeSerialize)
{

    // create and fill
    iuhn_t historyNode = dgGenerateHistoryNode();
    char *filename = "testIufHistoryNodeSerialization.hdf5";

    // save
    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);

    int status = iufHistoryNodeSave(historyNode, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);

    iuhn_t savedObj = iufHistoryNodeLoad(handle);
    TEST_ASSERT(savedObj != NULL);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUF_TRUE, iufHistoryNodeCompareWithId(historyNode,savedObj));

    if( iufHistoryNodeGetNumParams(historyNode) != 0 )
        iufParameterDictDelete(iufHistoryNodeGetParameters(historyNode));
    iufHistoryNodeDelete(historyNode);
    iufHistoryNodeDelete(savedObj);
}


TEST_GROUP_RUNNER(IufHistoryNode)
{
    RUN_TEST_CASE(IufHistoryNode, testIufHistoryNodeCreate);
    RUN_TEST_CASE(IufHistoryNode, testIufHistoryNodeDelete);
    RUN_TEST_CASE(IufHistoryNode, testIufHistoryNodeCompare);
    RUN_TEST_CASE(IufHistoryNode, testIufHistoryNodeSetGet);
    RUN_TEST_CASE(IufHistoryNode, testIufHistoryNodeSerialize);
}
