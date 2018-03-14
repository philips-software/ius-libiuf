//
// Created by Ruijzendaal on 07/03/2018.
//

#include <stdio.h>
#include <include/iusHLInput.h>
#include <include/iusHLInputFile.h>
#include <include/iusUtil.h>
#include "uniTests.h"
#include "iusTypes.h"
#include "iusHLNode.h"
#include "ius.h"



void testVersion(void)
{
    TEST_ASSERT_EQUAL(3, iusGetVersionMajor());
}


//
// ADT Node has
// Unique id
// Type, uniquely specifying file contents
// 0-n Parent nodes.
//
void testNode(void)
{
    int numParents = 0;
    char pNodeType[] = "IUSINPUTFILETYPE_V3";

    iun_t hSimpleNode = iusHLCreateNode( pNodeType, numParents );
    TEST_ASSERT(hSimpleNode != IUN_INVALID);
    // Check type
    char *pActualNodeType = iusHLGetNodeType( hSimpleNode );
    TEST_ASSERT_EQUAL_STRING( pNodeType, pActualNodeType );
    // Check numParents
    TEST_ASSERT( iusHLNodeGetNumParents( hSimpleNode ) == 0 );
    // Check unique
    TEST_ASSERT( iusHLGetNodeId( hSimpleNode ) != IUN_INVALID );
}

void testIusHLCreateExperiment()
{
    float  speedOfSound = 1498.0;
    int    date = 20160123;
    char * pDescription = "My important experiment notes";
    iue_t ex;
    ex = iusHLCreateExperiment(speedOfSound,date,pDescription);
    TEST_ASSERT_EQUAL_FLOAT(speedOfSound,iusHLExperimentGetSpeedOfSound(ex));
    TEST_ASSERT_EQUAL(date,iusHLExperimentGetDate(ex));
    TEST_ASSERT_EQUAL_STRING(pDescription,iusHLExperimentGetDescription(ex));
}



void testIusHLCreateInputHeader()
{
    // Goal test
/*
 * Header consists of experiment
 * transducer
 * receive settings
 */
    iuh_t ih;
    iue_t ex;
    int status = 0;
    float  speedOfSound = 1498.0;
    int    date = 20160123;
    char * pDescription = "My important experiment notes";

    ih = iusHLCreateInputHeader();
    TEST_ASSERT(ih != IUH_INVALID);

//    IusExperiment
//    IusTransducer      * pTransducer;      /**< transducer that has been used */
//    IusReceiveSettings * pReceiveSettings; /**< data receive settings */
//    IusDrivingScheme   * pDrivingScheme;   /**< data transmit settings */

    ex = iusHLCreateExperiment(speedOfSound,date,pDescription);
    status = iusHLHeaderSetExperiment(ih, ex);


}


void testIusHLCreateFile()
{
    const char *pFilename="testIusHLCreateFile.input";
    const char *pEmptyFilename="";
    const char *pSpecialCharsFilename="*&/";

    iuf_t ifh = iusHLCreateFile(pFilename);
    TEST_ASSERT(ifh != IUF_INVALID);

    // Create file that is already open should result in error
    iuf_t ifh2 = iusHLCreateFile(pFilename);
    TEST_ASSERT(ifh2 == IUF_INVALID);

    int status = iusHLCloseFile(ifh);
    TEST_ASSERT(status == IUS_E_OK);

    // Closing file that has already been closed results in error
    status = iusHLCloseFile(ifh);
    TEST_ASSERT(status != IUS_E_OK);

    // Invalid argument should result in error.
    ifh = iusHLCreateFile(pEmptyFilename);
    TEST_ASSERT(ifh == IUF_INVALID);
    ifh = iusHLCreateFile(pSpecialCharsFilename);
    TEST_ASSERT(ifh == IUF_INVALID);
}

void testInputFileHeader(void)
{
    // Goal: validate experiment hdf5 file io.
    //
    // fill ius generic input file header fields
    //  Fill ID
    //  Fill type
    //  Fill numberOfParents
    iuh_t reference_header;
    iuh_t actual_header;
    iuf_t ifh;
    int status;
    IUS_BOOL equal;
    const char *filename="myfirst.input";
    reference_header = iusHLCreateInputHeader();
    TEST_ASSERT(reference_header != IUH_INVALID);
    TEST_ASSERT(iusHLGetNumFrames(reference_header) == IUS_DEFAULT_NUM_FRAMES );

    // save to file
    ifh = iusHLCreateFile(filename);
    TEST_ASSERT(ifh != IUF_INVALID);

    // set header
    status = iusHLFileSetHeader(ifh,reference_header);
    TEST_ASSERT(status == IUS_E_OK);

    status = iusHLFileSave(ifh);
    TEST_ASSERT(status == IUS_E_OK);

    status = iusHLCloseFile(ifh);
    TEST_ASSERT(status == IUS_E_OK);

    // read from file
    ifh = iusHLOpenFile(filename);
    TEST_ASSERT(ifh != IUF_INVALID);
    actual_header = iusHLFileGetHeader(ifh);
    TEST_ASSERT(actual_header != IUH_INVALID);
    status = iusHLCloseFile(ifh);
    TEST_ASSERT(status == IUS_E_OK);

    // validate / compare reference with actual header
    equal = iusHLCompareHeader(reference_header, actual_header);
    TEST_ASSERT(equal == IUS_TRUE);
}


int main(void)
{
    // Disable diagnostic error messages
    // During testruns we expect errors to happen.
    iusHLDiagDisable();
    UNITY_BEGIN();
    RUN_TEST(testNode);
    RUN_TEST(testVersion);
    RUN_TEST(testIusHLCreateExperiment);
    RUN_TEST(testIusHLCreateFile);
    RUN_TEST(testIusHLCreateInputHeader);
    RUN_TEST(testInputFileHeader);
    return UNITY_END();
}