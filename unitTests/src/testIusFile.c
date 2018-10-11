//
// Created by Ruijzendaal on 20/03/2018.
//

#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <include/iusInputFile.h>
#include <include/ius.h>
#include <testDataGenerators.h>
#include <include/iusFile.h>
#include <include/iusHistoryNodeList.h>

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
    iusInputFileSave(iusInputFile);
    iusInputFileClose(iusInputFile);

    // Open file
    iufi_t iusFile = iusFileLoad(pFilename);
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
}

TEST(IusFile, testIusCWCFileHistoryScenario)
{
// As a developer I want to be able get the data history of an ius file.
// The data history  is organised as a DataHistory Tree.
    int numParents = 0;
    char pNodeType[] = IUS_INPUT_TYPE;
    char *pFilename = "testIusCWCFileHistoryScenario.hdf5";
    char *pFilename2 = "testIusCWCFileHistoryScenario2.hdf5";

    // --- Generate and Save Input file type
    // Create Input file.
    iuif_t iusInputFile = dgGenerateInputFile(pFilename2, "S5-1-1", "bmode", 10);
    iusInputFileSave(iusInputFile);
    iusInputFileClose(iusInputFile);

    // --- Generate and Save CWC file type, including Input file history
    // Open file
    iufi_t iusFile = iusFileLoad(pFilename2);
    TEST_ASSERT_NOT_EQUAL(IUFI_INVALID,iusFile);

    // get file history
    iuhn_t rootNode = iusFileGetHistoryTree(iusFile);
    // Input file history should be empty
    numParents = iusHistoryNodeGetNumParents(rootNode);
    TEST_ASSERT_EQUAL(0,numParents);

    // root node type should be equal to file type
    TEST_ASSERT_EQUAL_STRING(iusFileGetType(iusFile),iusHistoryNodeGetType(rootNode));

    iuif_t iusInputFile2 = dgGenerateInputFile(pFilename, "S5-1-0", "bmode", 10);
    iuhn_t cwcParents = iusHistoryNodeCreate(pNodeType,1);
    iuhnl_t parents = iusHistoryNodeGetParents(cwcParents);
    iusHistoryNodeListSet(parents,rootNode,0);
    iusHistoryNodeSetParents(cwcParents,parents);
    iusFileSetHistoryTree((iufi_t) iusInputFile2, cwcParents);


    // --- Validate History
    iusInputFileSave(iusInputFile2);
    iusInputFileClose(iusInputFile2);

    // Open file
    iufi_t iusCWCFile = iusFileLoad(pFilename);
    TEST_ASSERT_NOT_EQUAL(IUFI_INVALID,iusCWCFile);

    // get file history
    iuhn_t cwcRootNode = iusFileGetHistoryTree(iusCWCFile);
    // Input file history should be empty
    numParents = iusHistoryNodeGetNumParents(cwcRootNode);
    TEST_ASSERT(numParents == 0);

}

//// How to get the Input file header structure
//iuf_t file = iusFileLoad(“existing_cwc.hdf5”);
//iuhn_t history = iusFileGetNodeHistory(file);
//iuhn_t inputNode = iusHistoryNodeGetParent(history,0); (parent0 is INPUT_FILE_V3);
//iuif_t inputFile  = (iuif_t) inputNode;
//iut_t transducer = iusInputfileGetTransducer(inputFile);
//float centerFrequency = iusTransducerGetCenterFrequency(transducer);


//TEST(IusFile, testIusSaveParametersScenario)
//{
//// goal: test node history with processing paraneters by
//// creating input file
//// create a dummy processing step that creates
//// a new file, based i this input file and
//// a parameter set.
////
//
//    // Create Input file.
//    iuif_t iusInputFile = dgGenerateInputFile(fileName);
//    iusInputFileSave(iusInputFile);
//    iusInputFileClose(iusInputFile);
//
//    iuf_t iusFile = iusFileLoad(fileName);
//
//    // get file history
//    iuhtn_t inputHistoryNode = iusFileGetHistoryTree(iusFile);
//    iucf_t iusCWCFile = iusCWCFileCreate(cwcFilename);
//
//
//    // create dummy processing parameters
//    iupd_t iusParameterDict = iusParameterDictCreate();
//    iusParameterDictSet(iusParameterDict, "Name", "Dummy Processing");
//    iusParameterDictSet(iusParameterDict, "Version", "0.1");
//    iusParameterDictSet(iusParameterDict, "MyFloatParam", "0.3");
//    iusParameterDictSet(iusParameterDict, "MyIntParam", "11");
//
//    iusCWCFileHistoryAddParent(inputHistoryNode);
//    iusCWCFileHistorySetParameterDict(iusParameterDict);
//    iusCWCFileFileSave(iusCWCFile);
//    iusCWCFileFileClose(iusCWCFile);
//    iusFileClose(iusFile);
//
//    // check params
//    iusFile = iusFileLoad(cwcFilename);
//    iupd_t iusSavedParameterDict = iusFileGetParameterDict(iusFile);
//    IUS_BOOL eqial = iusParameterDictCompare(iusParameterDict,iusSavedParameterDict);
//    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
//
//    // check parent structure
//    // check transducer center frequency
//}
//
//TEST(IusFile, testIusHistoryTreeTraversalScenario)
//{
////    if1 -> cwc -> bmode -+
////                         |
////                         +-> reg
////                         |
////    if1 -> cwc -> bmode -+
////
//// Goal:
//// implement history tree above
//// demonstrate how to traverse a history like this
//}



// Developer (User) Scenario's
//
// As a developer I want to be able get the data history of an ius file.
// The data history  is organised as a DataHistory Tree.
//
// When creating a new ius file N that is a result of performing processing
// on an existing ius file E. The file N should contain the data history tree of
// E. Additionally, file E should contain parameters
//
// There are use cases where the Data History is needed In order to process an ius based
// file. This means that the History should be easily accessible.
// For example: The Transducer parameters (centerFrequency, number of elements)
// are needed in a processing step.
//
// In order to be able to reproduce a file that was the result of a processing step.
// the ius file format allows for storing the processing parameters in the data
// history tree. HistoryNode contains a member processing parameters, which is
// a dictionary of key value pairs. Both keys and values are strings.
//
// ADTs
// HistoryTree
//   HistoryNode
//       pId
//       pType
//       numberOfParents
//       numberOfParameters
//       pParents
//       .
//       .
//       pParameterDict
//
//       Type specific history
//       for example, the
//       For the IUS input type would have IusInputFile struct contents here.
//
//

//
//TEST(IusFile, testIusFileScenarios2)
//{
//    int numParents = 0;
//    char pNodeType[] = IUS_INPUT_TYPE;
//    char *fileName = "aap";
//
//    // Test scenario
//    // create cwc file, based on input file
//    // validate  history tree contains inputfile.
//    iuf_t if = iuFileOpen("inputFile");
//    iucwcf_t  cwcFile  = iuCWCFileCreate("cwcFile");
//    iusHistoryTreeSetParent(cwcFile, if);
//    iusCWCFileSave(cwcFile);
//    iusCWCFileClose(cwcFile);
//
//    iuf_t if = iuFileOpen("cwcFile");
//    iuhtn_t rootNode = iusFileGetHistoryTree(iusFile);
//    int numParents = iusHistoryTreeGetNumParents(rootNode);
//    TEST_ASSERT_EQUAL(1,numParents);
//
//    iuhnap_t historyNodeAlgoParams = iusHistoryTreeGetParams(rootNode);
//    int size = iusNodeAlgoParamsGetSize(historyNodeAlgoParams);
//    TEST_ASSERT(size>0);
//
//    iuhtn_t parent = iusHistoryTreeGetParent(rootNode,0);
//    TEST_ASSERT_EQUAL_STRING(IUS_INPUT_TYPE,iusHistoryTreeNodeGetType(parent));
//
//}

//Voorbeeld code gebruik met node structuur:
//
//iuif_t inputFile = iusInputFileOpen(“test”);
//iuapd_t algoParams ;
//iuf_t cwcFile = iusFileCreate(“cw1”, “c:\autoexec.bat”, file, algoParams );
//
//
//
//iuf_t iusFile = iusFileLoad( “c:\autoexec.bat” );
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
// generic input file
// should have 0 parents
//
// cw1 file, could have 1 parent (inputfile)
// in that case get transducer should resolve into something when input type
// is in the parent tree.

TEST_GROUP_RUNNER(IusFile)
{
    // testIusCWCFileHistoryScenario
    RUN_TEST_CASE(IusFile, testIusInputFileHistoryScenario);
    RUN_TEST_CASE(IusFile, testIusCWCFileHistoryScenario);
}
