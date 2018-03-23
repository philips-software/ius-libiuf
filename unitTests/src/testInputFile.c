//
// Created by Ruijzendaal on 20/03/2018.
//

#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>
#include <include/iusHLInput.h>
#include <include/iusHLInputFile.h>
#include <include/ius.h>
#include <util.h>


TEST_GROUP(Inputfile);

TEST_SETUP(Inputfile)
{
}

TEST_TEAR_DOWN(Inputfile)
{
}


iut_t create3DTransducer()
{
    char *pTransducerName = "created in testIusHLCreateTransducer";   /**< descriptive name of the ultrasound probe */
    float centerFrequency = 8000000;   /**< operating frequency of the transducer */
    int numElements = 1;          /**< number of transducer Elements in the probe */
    int elementIndex = 0;
    IusTransducerShape shape = IUS_PLANE;
    iut_t transducer;
    float x, y, z;
    float sx, sy, sz;
    float theta, phi;

    transducer = iusHLCreateTransducer(pTransducerName, shape, centerFrequency, numElements);
    TEST_ASSERT(transducer != IUT_INVALID);

    x = y = z = 0.0f;
    theta = phi = 0.0f;
    sx = 0.150 / 1000.0; // meter
    sy = 0.0f;
    sz = 6.0 / 1000.0;
    iu3dte_t _3dElement = iusUtilCreate3DElement(x, y, z, sx, sy, sz, theta, phi);
    int status = iusTransducerSetElement(transducer, elementIndex, _3dElement);
    TEST_ASSERT(status == IUS_E_OK);

    return transducer;
}

iut_t create2DTransducer()
{
    char *pTransducerName = "created in testIusHLCreate2DTransducer";   /**< descriptive name of the ultrasound probe */
    float centerFrequency = 8000000;   /**< operating frequency of the transducer */
    int numElements = 1;          /**< number of transducer Elements in the probe */
    int elementIndex = 0;
    IusTransducerShape shape = IUS_LINE;
    iut_t transducer;
    float x, z;
    float sx,  sz;
    float phi;

    transducer = iusHLCreateTransducer(pTransducerName, shape, centerFrequency, numElements);
    TEST_ASSERT(transducer != IUT_INVALID);

    x = z = 0.0f;
    phi = 0.0f;
    sx = 0.150 / 1000.0; // meter
    sz = 6.0 / 1000.0;
    iu2dte_t _2dElement = iusUtilCreate2DElement(x, z, sx, sz, phi);
    int status = iusTransducerSetElement(transducer, elementIndex, _2dElement);
    TEST_ASSERT(status == IUS_E_OK);

    return transducer;
}



iuh_t create2DTransducerHeader()
{
    // Generic settings
    float speedOfSound = 1498.1;
    int date = 20160124;
    int status;
    char *pDescription = "My important experiment notes, by create3DTransducerHeader()";

    iuh_t iuhHeader;
    iut_t tra;
    iue_t ex;

    iuhHeader = iusHLCreateInputHeader();
    TEST_ASSERT(iuhHeader != IUH_INVALID);

//    IusExperiment
//    IusTransducer      * pTransducer;      /**< transducer that has been used */
//    IusReceiveSettings * pReceiveSettings; /**< data receive settings */
//    IusDrivingScheme   * pDrivingScheme;   /**< data transmit settings */

    ex = iusHLCreateExperiment(speedOfSound, date, pDescription);
    status = iusHLHeaderSetExperiment(iuhHeader, ex);
    if (status != IUS_E_OK) return IUH_INVALID;


    tra = create2DTransducer();
    status = iusHLHeaderSetTransducer(iuhHeader, tra);
    if (status != IUS_E_OK) return IUH_INVALID;

    return iuhHeader;
}

iuh_t create3DTransducerHeader()
{
    // Generic settings
    float speedOfSound = 1498.1;
    int date = 20160124;
    int status;
    char *pDescription = "My important experiment notes, by create3DTransducerHeader()";

    iuh_t iuhHeader;
    iut_t tra;
    iue_t ex;

    iuhHeader = iusHLCreateInputHeader();
    TEST_ASSERT(iuhHeader != IUH_INVALID);

//    IusExperiment
//    IusTransducer      * pTransducer;      /**< transducer that has been used */
//    IusReceiveSettings * pReceiveSettings; /**< data receive settings */
//    IusDrivingScheme   * pDrivingScheme;   /**< data transmit settings */

    ex = iusHLCreateExperiment(speedOfSound, date, pDescription);
    status = iusHLHeaderSetExperiment(iuhHeader, ex);
    if (status != IUS_E_OK) return IUH_INVALID;


    tra = create3DTransducer();
    status = iusHLHeaderSetTransducer(iuhHeader, tra);
    if (status != IUS_E_OK) return IUH_INVALID;

    return iuhHeader;
}

TEST(Inputfile, test3DInputFileHeader)
{
    // Goal: Construct meta data for input file type
    //       Store data, Read back and validate
    // fill ius generic input file header fields
    iuh_t iuhReferenceHeader;
    iuh_t iuhActualHeader;
    iuf_t ifh;
    int status;
    IUS_BOOL equal;
    const char *filename = "myfirst3d.input";

    iuhReferenceHeader = create3DTransducerHeader();
    TEST_ASSERT(iusHLGetNumFrames(iuhReferenceHeader) == IUS_DEFAULT_NUM_FRAMES);

    // save to file
    ifh = iusHLCreateFile(filename);
    TEST_ASSERT(ifh != IUF_INVALID);

    // set header
    status = iusHLFileSetHeader(ifh, iuhReferenceHeader);
    TEST_ASSERT(status == IUS_E_OK);

    status = iusHLFileSave(ifh);
    TEST_ASSERT(status == IUS_E_OK);

    status = iusHLCloseFile(ifh);
    TEST_ASSERT(status == IUS_E_OK);

    // read from file
    ifh = iusHLOpenFile(filename);
    TEST_ASSERT(ifh != IUF_INVALID);
    iuhActualHeader = iusHLFileGetHeader(ifh);
    TEST_ASSERT(iuhActualHeader != IUH_INVALID);
    status = iusHLCloseFile(ifh);
    TEST_ASSERT(status == IUS_E_OK);

    // validate / compare reference with actual header
    equal = iusHLCompareHeader(iuhReferenceHeader, iuhActualHeader);
    TEST_ASSERT(equal == IUS_TRUE);
}



TEST(Inputfile, test2DInputFileHeader)
{
    // Goal: Construct meta data for input file type
    //       Store data, Read back and validate
    // fill ius generic input file header fields
    iuh_t iuhReferenceHeader;
    iuh_t iuhActualHeader;
    iuf_t ifh;
    int status;
    IUS_BOOL equal;
    const char *filename = "myfirst2d.input";

    iuhReferenceHeader = create2DTransducerHeader();
    TEST_ASSERT(iusHLGetNumFrames(iuhReferenceHeader) == IUS_DEFAULT_NUM_FRAMES);

    // save to file
    ifh = iusHLCreateFile(filename);
    TEST_ASSERT(ifh != IUF_INVALID);

    // set header
    status = iusHLFileSetHeader(ifh, iuhReferenceHeader);
    TEST_ASSERT(status == IUS_E_OK);

    status = iusHLFileSave(ifh);
    TEST_ASSERT(status == IUS_E_OK);

    status = iusHLCloseFile(ifh);
    TEST_ASSERT(status == IUS_E_OK);

    // read from file
    ifh = iusHLOpenFile(filename);
    TEST_ASSERT(ifh != IUF_INVALID);
    iuhActualHeader = iusHLFileGetHeader(ifh);
    TEST_ASSERT(iuhActualHeader != IUH_INVALID);
    status = iusHLCloseFile(ifh);
    TEST_ASSERT(status == IUS_E_OK);

    // validate / compare reference with actual header
    equal = iusHLCompareHeader(iuhReferenceHeader, iuhActualHeader);
    TEST_ASSERT(equal == IUS_TRUE);
}


TEST(Inputfile, testIusHLCreateInputHeader)
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
    float speedOfSound = 1498.0;
    int date = 20160123;
    char *pDescription = "My important experiment notes";

    ih = iusHLCreateInputHeader();
    TEST_ASSERT(ih != IUH_INVALID);

//    IusExperiment
//    IusTransducer      * pTransducer;      /**< transducer that has been used */
//    IusReceiveSettings * pReceiveSettings; /**< data receive settings */
//    IusDrivingScheme   * pDrivingScheme;   /**< data transmit settings */

    ex = iusHLCreateExperiment(speedOfSound, date, pDescription);
    status = iusHLHeaderSetExperiment(ih, ex);
    TEST_ASSERT(status == IUS_E_OK);

}


TEST(Inputfile,  testIusHLCreateFile)
{
    const char *pFilename = "testIusHLCreateFile.input";
    const char *pEmptyFilename = "";
    const char *pSpecialCharsFilename = "*&/";

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




TEST_GROUP_RUNNER(Inputfile)
{
    RUN_TEST_CASE(Inputfile, testIusHLCreateFile);
    RUN_TEST_CASE(Inputfile, testIusHLCreateInputHeader);
    RUN_TEST_CASE(Inputfile, test3DInputFileHeader);
    RUN_TEST_CASE(Inputfile, test2DInputFileHeader);
}