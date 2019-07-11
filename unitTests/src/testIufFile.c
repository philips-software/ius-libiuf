#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <iuf.h>
#include <dg/dataGenerators.h>

static char *pErrorFilename = "IufFile.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(IufFile);

TEST_SETUP(IufFile)
{
    iufErrorLogClear();
    iufErrorLog(IUF_TRUE);
    iufErrorAutoReportSet(IUF_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iufErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IufFile)
{
    iufErrorLogClear();
    iufErrorLog(IUF_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iufErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
}



TEST(IufFile, testIufInputFileHistoryScenario)
{
// As a developer I want to be able get the data history of an iuf file.
// The data history  is organised as a DataHistory Tree.
    int numParents = 0;
    int numFrames = 10;
    int numSamplesPerLine = 10;
    int numChannels = 8;
    char *pFilename = "testIufInputFileHistoryScenario.hdf5";

    // Create Input file.
    iuif_t iufInputFile = dgGenerateInputFile(pFilename, "S5-1", "bmode", numFrames, numSamplesPerLine, numChannels);
    iufInputFileNodeSave(iufInputFile);
    iufInputFileClose(iufInputFile);

    // Open file
    iuf_t iufFile = iufFileLoad(pFilename);
    TEST_ASSERT_NOT_EQUAL(IUFI_INVALID,iufFile);

    // get file history
    iuhn_t rootNode = iufFileGetHistoryTree(iufFile);
    // Input file history should be empty
    numParents = iufHistoryNodeGetNumParents(rootNode);
    TEST_ASSERT_EQUAL(0,numParents);
    // root node type should be equal to file type
    TEST_ASSERT_EQUAL_STRING(iufFileGetType(iufFile),iufHistoryNodeGetType(rootNode));

    // algo params should be empty
    int numAlgoParams = iufHistoryNodeGetNumParams(rootNode);
    TEST_ASSERT_EQUAL(0, numAlgoParams);
    dgDeleteInputFile(iufInputFile);
    iufFileDelete(iufFile);
    iufFileDelete(NULL);

}

TEST(IufFile, testIufCWCFileHistoryScenario)
{
// As a developer I want to be able get the data history of an iuf file.
// The data history  is organised as a DataHistory Tree.
    int numParents = 0;
    int numFrames = 10;
    int numSamplesPerLine = 10;
    int numChannels = 8;
    char *pNodeType = IUF_INPUT_TYPE;
    char *pFilename = "testIufCWCFileHistoryScenario.hdf5";
    char *pFilename2 = "testIufCWCFileHistoryScenario2.hdf5";

    // --- Generate and Save Input file type
    // Create Input file.
    iuif_t iufInputFile = dgGenerateInputFile(pFilename2, "S5-1-1", "bmode", numFrames, numSamplesPerLine, numChannels);
    iufInputFileNodeSave(iufInputFile);
    iufInputFileClose(iufInputFile);

    // --- Generate and Save CWC file type, including Input file history
    // Open file
    iuf_t iufFile = iufFileLoad(pFilename2);
    TEST_ASSERT_NOT_EQUAL(IUFI_INVALID,iufFile);

    // get file history
    iuhn_t rootNode = iufFileGetHistoryTree(iufFile);
    // Input file history should be empty
    numParents = iufHistoryNodeGetNumParents(rootNode);
    TEST_ASSERT_EQUAL(0,numParents);

    // root node type should be equal to file type
    TEST_ASSERT_EQUAL_STRING(iufFileGetType(iufFile),iufHistoryNodeGetType(rootNode));

    iuif_t iufInputFile2 = dgGenerateInputFile(pFilename, "S5-1-0", "bmode", numFrames, numSamplesPerLine, numChannels);
    iuhn_t cwcParents = iufHistoryNodeCreate(pNodeType);
    iuhnl_t parents = iufHistoryNodeListCreate(1);
    iufHistoryNodeListSet(parents,rootNode,0);
    iufHistoryNodeSetParents(cwcParents,parents);
    iufFileSetHistoryTree((iuf_t) iufInputFile2, cwcParents);

    // --- Validate History
    iufInputFileNodeSave(iufInputFile2);
    iufInputFileClose(iufInputFile2);

    // Open file
    iuf_t iufCWCFile = iufFileLoad(pFilename);
    TEST_ASSERT_NOT_EQUAL(IUFI_INVALID,iufCWCFile);

    // get file history
    iuhn_t cwcRootNode = iufFileGetHistoryTree(iufCWCFile);
    // Input file history should be empty
    numParents = iufHistoryNodeGetNumParents(cwcRootNode);
    TEST_ASSERT(numParents == 0);

    dgDeleteInputFile(iufInputFile);
    dgDeleteInputFile(iufInputFile2);
    iufFileDelete(iufFile);
    iufFileDelete(iufCWCFile);

}

TEST_GROUP_RUNNER(IufFile)
{
    // testIufCWCFileHistoryScenario
    RUN_TEST_CASE(IufFile, testIufInputFileHistoryScenario);
//    RUN_TEST_CASE(IufFile, testIufCWCFileHistoryScenario);
}
