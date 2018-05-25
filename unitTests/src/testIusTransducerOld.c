//
// Created by Ruijzendaal on 20/03/2018.
//
#include <util.h>
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <ius.h>
#include <iusError.h>
#include <iusUtil.h>
#include <iusTypes.h>
#include <iusHLExperiment.h>
#include <iusHLTransducer.h>

TEST_GROUP(IusTransducer);

TEST_SETUP(IusTransducer)
{
}

TEST_TEAR_DOWN(IusTransducer)
{
}


TEST(IusTransducer,  testIusTransducerElements)
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
    sx = 0.150f / 1000.0f; // meter
    sy = 0.0f;
    sz = 6.0f / 1000.0f;
    iu3dte_t _3dElement = iusUtilCreate3DElement(x, y, z, sx, sy, sz, theta, phi);
    int status = iusHLTransducerSetElement(transducer, elementIndex, _3dElement);
    TEST_ASSERT(status == IUS_E_OK);

    // Should not be unable to set a 2D element in a 3D shaped transducer
    iu2dte_t _2dElement = iusUtilCreate2DElement(x, z, sx, sz, phi);
    status = iusHLTransducerSetElement(transducer, elementIndex, _2dElement);
    TEST_ASSERT(status == IUS_ERR_VALUE);

    iudte_t elementActual = iusHLTransducerGetElement(transducer, elementIndex);
    TEST_ASSERT(elementActual != IUTE_INVALID);
    IusShape elementShape = iusHLTransducerGetElementShape(elementActual);
    TEST_ASSERT(elementShape == IUS_3D_SHAPE);

    IUS_BOOL isEqual = iusHLTransducerCompareElement((iudte_t) _3dElement, (iudte_t) elementActual);
    TEST_ASSERT(isEqual == IUS_TRUE);

}

TEST(IusTransducer,  testIusCreateTransducer)
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
    TEST_ASSERT_EQUAL(shape, iusHLTransducerGetShape(transducer));
}






TEST_GROUP_RUNNER(IusTransducer)
{
    RUN_TEST_CASE(IusTransducer, testIusCreateTransducer);
    RUN_TEST_CASE(IusTransducer, testIusTransducerElements);
    RUN_TEST_CASE(IusTransducer, testIusHLCompare3DTransducer);
    RUN_TEST_CASE(IusTransducer, testIusHLCompare2DTransducer);
}
