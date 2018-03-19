//
// Created by Ruijzendaal on 07/03/2018.
//

#include <stdio.h>
#include "ius.h"
#include <include/iusHLInput.h>
#include <include/iusHLInputFile.h>
#include <include/iusUtil.h>
#include <include/iusHLExperiment.h>
#include "uniTests.h"
#include "iusTypes.h"
#include "iusHLNode.h"
#include "iusError.h"


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

    iun_t hSimpleNode = iusHLCreateNode(pNodeType, numParents);
    TEST_ASSERT(hSimpleNode != IUN_INVALID);
    // Check type
    char *pActualNodeType = iusHLGetNodeType(hSimpleNode);
    TEST_ASSERT_EQUAL_STRING(pNodeType, pActualNodeType);
    // Check numParents
    TEST_ASSERT(iusHLNodeGetNumParents(hSimpleNode) == 0);
    // Check unique
    TEST_ASSERT(iusHLGetNodeId(hSimpleNode) != IUN_INVALID);
}


void testIusCompareExperiment()
{
    float speedOfSound = 1498.0;
    int date = 20160123;
    char *pDescription = "My important experiment notes";
    int status = 0;
    IUS_BOOL isEqual;

    iue_t ex, exAltered;
    iue_t exDuplicate;

    // Test duplicate
    ex = iusHLCreateExperiment(speedOfSound, date, pDescription);
    TEST_ASSERT(ex != IUEX_INVALID);

    exDuplicate = iusHLCreateExperiment(speedOfSound, date, pDescription);
    TEST_ASSERT(exDuplicate != IUEX_INVALID);
    isEqual = iusCompareExperiment(ex, exDuplicate);
    TEST_ASSERT(isEqual == IUS_TRUE);
    isEqual = iusCompareExperiment(ex, ex);
    TEST_ASSERT(isEqual == IUS_TRUE);
    iusHLDeleteExperiment(ex);
    iusHLDeleteExperiment(exDuplicate);

    ex = iusHLCreateExperiment(speedOfSound, date, "");
    TEST_ASSERT(ex != IUEX_INVALID);
    exDuplicate = iusHLCreateExperiment(speedOfSound, date, NULL);
    TEST_ASSERT(exDuplicate != IUEX_INVALID);
    isEqual = iusCompareExperiment(ex, exDuplicate);
    TEST_ASSERT(isEqual == IUS_TRUE);

    // Test alterations
    exAltered = iusHLCreateExperiment(speedOfSound + 0.00007, date, pDescription);
    isEqual = iusCompareExperiment(ex, exAltered);
    TEST_ASSERT(isEqual == IUS_FALSE);
    iusHLDeleteExperiment(exAltered);

    exAltered = iusHLCreateExperiment(speedOfSound, date + 1, pDescription);
    isEqual = iusCompareExperiment(ex, exAltered);
    TEST_ASSERT(isEqual == IUS_FALSE);
    iusHLDeleteExperiment(exAltered);

    // floating point errors
    exAltered = iusHLCreateExperiment(speedOfSound + 0.00006, date, pDescription);
    isEqual = iusCompareExperiment(ex, exAltered);
    TEST_ASSERT(isEqual == IUS_TRUE);
    iusHLDeleteExperiment(exAltered);

}


iu3dte_t iusUtilCreate3DElement
    (
        float x,
        float y,
        float z,
        float sx,
        float sy,
        float sz,
        float theta,
        float phi
    )
{
    iu3dp_t pos = iusHLCreate3DElementPosition(
        x,
        y,
        z
    );
    iu3da_t ang = iusHLCreate3DElementAngle(
        theta,
        phi
    );
    iu3ds_t siz = iusHLCreate3DElementSize(
        sx,
        sy,
        sz
    );
    iu3dte_t ele = iusHLCreate3DElement(
        pos,
        ang,
        siz
    );
    return ele;
}


iu2dte_t iusUtilCreate2DElement
    (
        float x,
        float z,
        float sx,
        float sz,
        float phi
    )
{
    iu2dp_t pos = iusHLCreate2DElementPosition(x,z);
    iu2ds_t siz = iusHLCreate2DElementSize(sx, sz);
    iu2dte_t ele = iusHLCreate2DElement(pos, phi, siz);
    return ele;
}

void testIusTransducerElements()
{
    char *pTransducerName = "created in testIusHLCreateTransducer";   /**< descriptive name of the ultrasound probe */
    float centerFrequency = 8000000;   /**< operating frequency of the transducer */
    int numElements = 32;          /**< number of transducer Elements in the probe */
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

    // Should not be unable to set a 2D element in a 3D shaped transducer
    iu2dte_t _2dElement = iusUtilCreate2DElement(x, z, sx, sz, phi);
    status = iusTransducerSetElement(transducer, elementIndex, _2dElement);
    TEST_ASSERT(status == IUS_ERR_VALUE);

    iudte_t elementActual = iusTransducerGetElement(transducer, elementIndex);
    TEST_ASSERT(elementActual != IUTE_INVALID);
    IusTransducerElementType elementType = iusHLTransducerGetElementType(elementActual);
    TEST_ASSERT(elementType == IUS_3D_TRANSDUCER_ELEMENT);

    IUS_BOOL isEqual = iusTransducerCompareElement((iudte_t)_3dElement, (iudte_t) elementActual);
    TEST_ASSERT(isEqual == IUS_TRUE);

}

void testIusHLCreateTransducer()
{
    char *pTransducerName = "created in testIusHLCreateTransducer";   /**< descriptive name of the ultrasound probe */
    float centerFrequency = 8000000;   /**< operating frequency of the transducer */
    int numElements = 1;          /**< number of transducer Elements in the probe */
    IusTransducerShape shape = IUS_PLANE;
    iut_t transducer;

    transducer = iusHLCreateTransducer(pTransducerName, IUS_PLANE, centerFrequency, numElements);
    TEST_ASSERT(transducer != IUT_INVALID);
    TEST_ASSERT_EQUAL_FLOAT(centerFrequency, iusHLTransducerGetCenterFrequency(transducer));
    TEST_ASSERT_EQUAL(numElements, iusHLTransducerGetNumElements(transducer));
    TEST_ASSERT_EQUAL_STRING(pTransducerName, iusHLTransducerGetName(transducer));
    TEST_ASSERT_EQUAL(shape, iusHLTransducerShape(transducer));
}

void testIusHLCreateExperiment()
{
    float speedOfSound = 1498.0;
    int date = 20160123;
    char *pDescription = "My important experiment notes";
    int status = 0;

    iue_t ex;
    ex = iusHLCreateExperiment(speedOfSound, date, pDescription);
    TEST_ASSERT(ex != IUEX_INVALID);
    TEST_ASSERT_EQUAL_FLOAT(speedOfSound, iusHLExperimentGetSpeedOfSound(ex));
    TEST_ASSERT_EQUAL(date, iusHLExperimentGetDate(ex));
    TEST_ASSERT_EQUAL_STRING(pDescription, iusHLExperimentGetDescription(ex));
    status = iusHLDeleteExperiment(ex);
    TEST_ASSERT(status == IUS_E_OK);
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


void testIusHLCreateFile()
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


iuh_t createHeader()
{
    float speedOfSound = 1498.1;
    int date = 20160124;
    int status;
    char *pDescription = "My important experiment notes, by createHeader()";
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

//    pTransducer->pTransducerName = AllocString( "transducerName" );
//    pTransducer->centerFrequency = 1.2f;
//    pTransducer->numElements = 1;
//    pTransducer->pElements = (IusTransducerElement *)calloc(1, sizeof(IusTransducerElement));
//    pTransducer->pElements[0].position.x = 1.1f;
//    pTransducer->pElements[0].position.y = 2.2f;
//    pTransducer->pElements[0].position.z = 3.3f;
//    pTransducer->pElements[0].angle.theta = 4.4f;
//    pTransducer->pElements[0].angle.phi = 5.5f;
//    pTransducer->pElements[0].size.x = 6.6f;
//    pTransducer->pElements[0].size.y = 7.7f;
//    pTransducer->pElements[0].size.z = 8.8f;
//    tra = iusHLCreateTransducer("transducerName", centerFrequency, nummElements);
    tra = iusHLCreateTransducer(NULL, IUS_LINE, 0, 0);

    status = iusHLHeaderSetTransducer(iuhHeader, tra);
    if (status != IUS_E_OK) return IUH_INVALID;

    return iuhHeader;
}

void testInputFileHeader(void)
{
    // Goal: Construct meta data for input file type
    //       Store data, Read back and validate
    // fill ius generic input file header fields
    iuh_t iuhReferenceHeader;
    iuh_t iuhActualHeader;
    iuf_t ifh;
    int status;
    IUS_BOOL equal;
    const char *filename = "myfirst.input";

    iuhReferenceHeader = createHeader();
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


int main(void)
{
    // Disable diagnostic error messages
    // During testruns we expect errors to happen.
    iusHLDiagDisable();
    UNITY_BEGIN();
    RUN_TEST(testNode);
    RUN_TEST(testVersion);
    RUN_TEST(testIusHLCreateExperiment);
    RUN_TEST(testIusCompareExperiment);
    RUN_TEST(testIusHLCreateFile);
    RUN_TEST(testIusHLCreateInputHeader);
    RUN_TEST(testInputFileHeader);
    RUN_TEST(testIusHLCreateTransducer);
    RUN_TEST(testIusTransducerElements);
    return UNITY_END();
}