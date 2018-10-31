//
// Created by Ruijzendaal on 20/03/2018.
//

#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <ius.h>
#include <testDataGenerators.h>

TEST_GROUP(IusFile);

TEST_SETUP(IusFile)
{
}

TEST_TEAR_DOWN(IusFile)
{
}


TEST(IusFile, testIusInputFileHistoryScenario)
{
// As a developer I want to be able get the data history of an ius file.
// The data history  is organised as a DataHistory Tree.
    int numParents = 0;
    char *pFilename = "testIusInputFileHistoryScenario.hdf5";

    // Create Input file.
    iuif_t iusInputFile = dgGenerateInputFile(pFilename, "S5-1", "bmode", 10);
    iusInputFileNodeSave(iusInputFile);
    iusInputFileClose(iusInputFile);

    // Open file
    iuf_t iusFile = iusFileLoad(pFilename);
    TEST_ASSERT_NOT_EQUAL(IUFI_INVALID,iusFile);

    // get file history
    iuhn_t rootNode = iusFileGetHistoryTree(iusFile);
    // Input file history should be empty
    numParents = iusHistoryNodeGetNumParents(rootNode);
    TEST_ASSERT_EQUAL(0,numParents);
    // root node type should be equal to file type
    TEST_ASSERT_EQUAL_STRING(iusFileGetType(iusFile),iusHistoryNodeGetType(rootNode));

    // algo params should be empty
    int numAlgoParams = iusHistoryNodeGetNumParams(rootNode);
    TEST_ASSERT_EQUAL(0, numAlgoParams);
    dgDeleteInputFile(iusInputFile);
    iusFileDelete(iusFile);
}

TEST(IusFile, testIusCWCFileHistoryScenario)
{
// As a developer I want to be able get the data history of an ius file.
// The data history  is organised as a DataHistory Tree.
    int numParents = 0;
    char *pNodeType = IUS_INPUT_TYPE;
    char *pFilename = "testIusCWCFileHistoryScenario.hdf5";
    char *pFilename2 = "testIusCWCFileHistoryScenario2.hdf5";

    // --- Generate and Save Input file type
    // Create Input file.
    iuif_t iusInputFile = dgGenerateInputFile(pFilename2, "S5-1-1", "bmode", 10);
    iusInputFileNodeSave(iusInputFile);
    iusInputFileClose(iusInputFile);

    // --- Generate and Save CWC file type, including Input file history
    // Open file
    iuf_t iusFile = iusFileLoad(pFilename2);
    TEST_ASSERT_NOT_EQUAL(IUFI_INVALID,iusFile);

    // get file history
    iuhn_t rootNode = iusFileGetHistoryTree(iusFile);
    // Input file history should be empty
    numParents = iusHistoryNodeGetNumParents(rootNode);
    TEST_ASSERT_EQUAL(0,numParents);

    // root node type should be equal to file type
    TEST_ASSERT_EQUAL_STRING(iusFileGetType(iusFile),iusHistoryNodeGetType(rootNode));

    iuif_t iusInputFile2 = dgGenerateInputFile(pFilename, "S5-1-0", "bmode", 10);
    iuhn_t cwcParents = iusHistoryNodeCreate(pNodeType);
    iuhnl_t parents = iusHistoryNodeListCreate(1);
    iusHistoryNodeListSet(parents,rootNode,0);
    iusHistoryNodeSetParents(cwcParents,parents);
    iusFileSetHistoryTree((iuf_t) iusInputFile2, cwcParents);

    // --- Validate History
    iusInputFileNodeSave(iusInputFile2);
    iusInputFileClose(iusInputFile2);

    // Open file
    iuf_t iusCWCFile = iusFileLoad(pFilename);
    TEST_ASSERT_NOT_EQUAL(IUFI_INVALID,iusCWCFile);

    // get file history
    iuhn_t cwcRootNode = iusFileGetHistoryTree(iusCWCFile);
    // Input file history should be empty
    numParents = iusHistoryNodeGetNumParents(cwcRootNode);
    TEST_ASSERT(numParents == 0);

    dgDeleteInputFile(iusInputFile);
    dgDeleteInputFile(iusInputFile2);
    iusFileDelete(iusFile);
    iusFileDelete(iusCWCFile);

}

TEST_GROUP_RUNNER(IusFile)
{
    // testIusCWCFileHistoryScenario
    RUN_TEST_CASE(IusFile, testIusInputFileHistoryScenario);
    RUN_TEST_CASE(IusFile, testIusCWCFileHistoryScenario);
}
