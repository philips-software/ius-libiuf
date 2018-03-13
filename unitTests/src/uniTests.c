//
// Created by Ruijzendaal on 07/03/2018.
//

#include <stdio.h>
#include <include/iusHLInput.h>
#include <include/iusHLInputFile.h>
#include "uniTests.h"
#include "library.h"
#include "iusTypes.h"
#include "iusHLNode.h"
#include "ius.h"


void testHello(void)
{
    TEST_ASSERT_EQUAL(0,hello("aap"));
}

void testVersion(void)
{
    TEST_ASSERT_EQUAL(3, iusGetVersionMajor());
}

void testCreateFile(void)
{
    #define FILE "dset.h5"
    TEST_ASSERT_EQUAL(0,createDataset(FILE));
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
    ifh = iusHLCreateFile(filename, reference_header);
    TEST_ASSERT(ifh != IUF_INVALID);

    status = iusHLCloseFile(ifh);
    TEST_ASSERT(status == IUS_E_OK);

    // read from file
    ifh = iusHLOpenFile(filename);
    TEST_ASSERT(ifh != IUF_INVALID);
    actual_header = iusHLGetHeader(ifh);
    TEST_ASSERT(actual_header != IUH_INVALID);
    status = iusHLCloseFile(ifh);
    TEST_ASSERT(status == IUS_E_OK);

    // validate / compare reference with actual header
    equal = iusHLCompareHeader(reference_header, actual_header);
    TEST_ASSERT(equal == IUS_TRUE);
}


int main(void)
{
    UNITY_BEGIN();
//    RUN_TEST(testHello);
    RUN_TEST(testNode);
//    RUN_TEST(testVersion);
    RUN_TEST(testCreateFile);
    RUN_TEST(testInputFileHeader);
    return UNITY_END();
}