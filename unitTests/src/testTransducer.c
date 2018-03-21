//
// Created by Ruijzendaal on 20/03/2018.
//

#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>
#include <include/ius.h>
#include <include/iusTypes.h>
#include <include/iusHLExperiment.h>
#include <include/iusHLTransducer.h>
#include <include/iusError.h>
#include <util.h>
#include <include/iusUtil.h>

TEST_GROUP(InputfileTransducer);

TEST_SETUP(InputfileTransducer)
{
}

TEST_TEAR_DOWN(InputfileTransducer)
{
}

TEST(InputfileTransducer, testIusHLCreateExperiment)
{
    float speedOfSound = 1498.0;
    int date = 20160123;
    char *pDescription = "My important experiment notes";
    int status = 0;

    IusExperiment *ex;
    ex = iusHLCreateExperiment(speedOfSound, date, pDescription);
    TEST_ASSERT(ex != IUEX_INVALID);
    TEST_ASSERT_EQUAL_FLOAT(speedOfSound, iusHLExperimentGetSpeedOfSound(ex));
    TEST_ASSERT_EQUAL(date, iusHLExperimentGetDate(ex));
    TEST_ASSERT_EQUAL_STRING(pDescription, iusHLExperimentGetDescription(ex));
    status = iusHLDeleteExperiment(ex);
    TEST_ASSERT(status == IUS_E_OK);
}


TEST(InputfileTransducer,  testIusCompareExperiment)
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
    exAltered = iusHLCreateExperiment(speedOfSound*(1+FLOAT_PRECISION), date, pDescription);
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

TEST(InputfileTransducer,  testIusTransducerElements)
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

TEST(InputfileTransducer,  testIusHLCreateTransducer)
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
    TEST_ASSERT_EQUAL(shape, iusHLGetTransducerShape(transducer));
}

TEST(InputfileTransducer,  testIusHLCompareTransducer)
{
    char *pTransducerName = "created in testIusHLCompareTransducer";
    char *pDifferentTransducerName = "different transducer, created in testIusHLCompareTransducer";
    float centerFrequency = 8000000;   /**< operating frequency of the transducer */
    int numElements = 1;          /**< number of transducer Elements in the probe */
    IusTransducerShape shape = IUS_PLANE;
    iut_t transducer, transducerDuplicate, differentTransducer;
    IUS_BOOL isEqual;

    transducer = iusHLCreateTransducer(pTransducerName, IUS_PLANE, centerFrequency, numElements);
    TEST_ASSERT(transducer != IUT_INVALID);
    TEST_ASSERT_EQUAL_FLOAT(centerFrequency, iusHLTransducerGetCenterFrequency(transducer));
    TEST_ASSERT_EQUAL(numElements, iusHLTransducerGetNumElements(transducer));
    TEST_ASSERT_EQUAL_STRING(pTransducerName, iusHLTransducerGetName(transducer));
    TEST_ASSERT_EQUAL(shape, iusHLGetTransducerShape(transducer));

    transducerDuplicate = iusHLCreateTransducer(pTransducerName, IUS_PLANE, centerFrequency, numElements);
    TEST_ASSERT_EQUAL_FLOAT(centerFrequency, iusHLTransducerGetCenterFrequency(transducerDuplicate));
    TEST_ASSERT_EQUAL(numElements, iusHLTransducerGetNumElements(transducerDuplicate));
    TEST_ASSERT_EQUAL_STRING(pTransducerName, iusHLTransducerGetName(transducerDuplicate));
    TEST_ASSERT_EQUAL(shape, iusHLGetTransducerShape(transducerDuplicate));
    isEqual = iusHLCompareTransducer(transducer, transducerDuplicate);
    TEST_ASSERT(isEqual == IUS_TRUE);

    //
    // TESTS Related to different transducers
    //
    differentTransducer = iusHLCreateTransducer(pTransducerName, IUS_PLANE, centerFrequency+1, numElements);
    isEqual = iusHLCompareTransducer(transducer, differentTransducer);
    TEST_ASSERT(isEqual == IUS_FALSE);
    iusHLDeleteTransducer(differentTransducer);

    differentTransducer = iusHLCreateTransducer(pTransducerName, IUS_PLANE, centerFrequency, numElements+1);
    isEqual = iusHLCompareTransducer(transducer, differentTransducer);
    TEST_ASSERT(isEqual == IUS_FALSE);
    iusHLDeleteTransducer(differentTransducer);

    differentTransducer = iusHLCreateTransducer(pTransducerName, IUS_LINE, centerFrequency, numElements);
    isEqual = iusHLCompareTransducer(transducer, differentTransducer);
    TEST_ASSERT(isEqual == IUS_FALSE);
    iusHLDeleteTransducer(differentTransducer);

    differentTransducer = iusHLCreateTransducer("", IUS_PLANE, centerFrequency, numElements);
    isEqual = iusHLCompareTransducer(transducer, differentTransducer);
    TEST_ASSERT(isEqual == IUS_FALSE);
    iusHLDeleteTransducer(differentTransducer);

    differentTransducer = iusHLCreateTransducer(pDifferentTransducerName, IUS_PLANE, centerFrequency, numElements);
    isEqual = iusHLCompareTransducer(transducer, differentTransducer);
    TEST_ASSERT(isEqual == IUS_FALSE);
    iusHLDeleteTransducer(differentTransducer);
}


TEST_GROUP_RUNNER(InputfileTransducer)
{
    RUN_TEST_CASE(InputfileTransducer, testIusHLCreateExperiment);
    RUN_TEST_CASE(InputfileTransducer, testIusCompareExperiment);
    RUN_TEST_CASE(InputfileTransducer, testIusHLCreateTransducer);
    RUN_TEST_CASE(InputfileTransducer, testIusTransducerElements);
    RUN_TEST_CASE(InputfileTransducer, testIusHLCompareTransducer);
}
