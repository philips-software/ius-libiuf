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



TEST(InputfileTransducer,  testIusHLCompare2DTransducer)
{
    char *pTransducerName = "created in testIusHLCompareTransducer";
    char *pDifferentTransducerName = "different transducer, created in testIusHLCompareTransducer";
    float centerFrequency = 8000000;   /**< operating frequency of the transducer */
    float x, z;
    float sx, sz;
    float phi;
    int numElements = 1;          /**< number of transducer Elements in the probe */
    IusTransducerShape shape = IUS_LINE;
    iut_t transducer, transducerDuplicate, differentTransducer;
    IUS_BOOL isEqual;
    int status=0;

    x = z = 0.0f;
    phi = 0.0f;
    sx = 0.150 / 1000.0; // meter
    sz = 6.0 / 1000.0;

    iu2dte_t _2dElement = iusUtilCreate2DElement(x, z, sx, sz, phi);

    transducer = iusHLCreateTransducer(pTransducerName, shape, centerFrequency, numElements);
    TEST_ASSERT(transducer != IUT_INVALID);
    TEST_ASSERT_EQUAL_FLOAT(centerFrequency, iusHLTransducerGetCenterFrequency(transducer));
    TEST_ASSERT_EQUAL(numElements, iusHLTransducerGetNumElements(transducer));
    TEST_ASSERT_EQUAL_STRING(pTransducerName, iusHLTransducerGetName(transducer));
    TEST_ASSERT_EQUAL(shape, iusHLGetTransducerShape(transducer));

    transducerDuplicate = iusHLCreateTransducer(pTransducerName, shape, centerFrequency, numElements);
    TEST_ASSERT_EQUAL_FLOAT(centerFrequency, iusHLTransducerGetCenterFrequency(transducerDuplicate));
    TEST_ASSERT_EQUAL(numElements, iusHLTransducerGetNumElements(transducerDuplicate));
    TEST_ASSERT_EQUAL_STRING(pTransducerName, iusHLTransducerGetName(transducerDuplicate));
    TEST_ASSERT_EQUAL(shape, iusHLGetTransducerShape(transducerDuplicate));
    isEqual = iusHLCompareTransducer(transducer, transducerDuplicate);
    TEST_ASSERT(isEqual == IUS_TRUE);


    //
    // TESTS Related to different transducers
    //
#define CREATE_2DTRANSDUCER_AND_TEST_EQUALITY(reference,actual,nm,shp,freq,num,bool)   actual = iusHLCreateTransducer( \
                                                                                        nm, \
                                                                                        shp, \
                                                                                        freq, \
                                                                                        num); \
                                                                                    isEqual = iusHLCompareTransducer( \
                                                                                        reference, \
                                                                                        actual \
                                                                                        ); \
                                                                                    TEST_ASSERT(isEqual == bool); \
                                                                                    iusHLDeleteTransducer(actual)

    CREATE_2DTRANSDUCER_AND_TEST_EQUALITY( transducer,differentTransducer,pTransducerName, IUS_LINE, centerFrequency+1,
                                         numElements, IUS_FALSE);
    CREATE_2DTRANSDUCER_AND_TEST_EQUALITY( transducer,differentTransducer,pTransducerName, IUS_LINE, centerFrequency,
                                         numElements+1, IUS_FALSE);
    CREATE_2DTRANSDUCER_AND_TEST_EQUALITY( transducer,differentTransducer,pTransducerName, IUS_PLANE, centerFrequency,
                                         numElements, IUS_FALSE);
    CREATE_2DTRANSDUCER_AND_TEST_EQUALITY( transducer,differentTransducer,"", IUS_LINE, centerFrequency,
                                         numElements, IUS_FALSE);
    CREATE_2DTRANSDUCER_AND_TEST_EQUALITY( transducer,differentTransducer,pDifferentTransducerName, IUS_LINE, centerFrequency,
                                         numElements, IUS_FALSE);


    status = iusTransducerSetElement(transducer, 0, _2dElement);
    TEST_ASSERT(status == IUS_E_OK);
    status = iusTransducerSetElement(transducerDuplicate, 0, _2dElement);
    TEST_ASSERT(status == IUS_E_OK);
    isEqual = iusHLCompareTransducer(transducer, transducerDuplicate);
    TEST_ASSERT(isEqual == IUS_TRUE);

    differentTransducer = iusHLCreateTransducer(pTransducerName, IUS_LINE, centerFrequency, numElements);
    status = iusTransducerSetElement(differentTransducer, 0, _2dElement);
    TEST_ASSERT(status == IUS_E_OK);
    isEqual = iusHLCompareTransducer(transducer, differentTransducer);
    TEST_ASSERT(isEqual == IUS_TRUE);

#define CREATE_AND_SET_2DELEMENT_TEST_EQUALITY(reference,actual,_elem,_x,_z,_sx,_sz,_phi,bool) \
    _elem = iusUtilCreate2DElement(_x,_z,_sx,_sz,_phi); \
    status = iusTransducerSetElement(actual, 0, _elem); \
    TEST_ASSERT(status == IUS_E_OK); \
    isEqual = iusHLCompareTransducer(reference, actual); \
    TEST_ASSERT(isEqual == bool)
 
    CREATE_AND_SET_2DELEMENT_TEST_EQUALITY(transducer,differentTransducer,_2dElement,x+1, z, sx, sz, phi,
                                         IUS_FALSE);
    CREATE_AND_SET_2DELEMENT_TEST_EQUALITY(transducer,differentTransducer,_2dElement,x, z+1, sx, sz, phi,
                                         IUS_FALSE);
    CREATE_AND_SET_2DELEMENT_TEST_EQUALITY(transducer,differentTransducer,_2dElement,x, z, sx+1, sz, phi,
                                         IUS_FALSE);
    CREATE_AND_SET_2DELEMENT_TEST_EQUALITY(transducer,differentTransducer,_2dElement,x, z, sx, sz+1, phi,
                                         IUS_FALSE);
    CREATE_AND_SET_2DELEMENT_TEST_EQUALITY(transducer,differentTransducer,_2dElement,x, z, sx, sz, phi+1,
                                         IUS_FALSE);

    iusHLDeleteTransducer(differentTransducer);
}


TEST(InputfileTransducer,  testIusHLCompare3DTransducer)
{
    char *pTransducerName = "created in testIusHLCompareTransducer";
    char *pDifferentTransducerName = "different transducer, created in testIusHLCompareTransducer";
    float centerFrequency = 8000000;   /**< operating frequency of the transducer */
    float x, y, z;
    float sx, sy, sz;
    float theta, phi;
    int numElements = 1;          /**< number of transducer Elements in the probe */
    IusTransducerShape shape = IUS_PLANE;
    iut_t transducer, transducerDuplicate, differentTransducer;
    IUS_BOOL isEqual;
    int status=0;

    x = y = z = 0.0f;
    theta = phi = 0.0f;
    sx = 0.150 / 1000.0; // meter
    sy = 0.0f;
    sz = 6.0 / 1000.0;

    iu3dte_t _3dElement = iusUtilCreate3DElement(x, y, z, sx, sy, sz, theta, phi);

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
#define CREATE_TRANSDUCER_AND_TEST_EQUALITY(reference,actual,nm,shp,freq,num,bool)   actual = iusHLCreateTransducer( \
                                                                                        nm, \
                                                                                        shp, \
                                                                                        freq, \
                                                                                        num); \
                                                                                    isEqual = iusHLCompareTransducer( \
                                                                                        reference, \
                                                                                        actual \
                                                                                        ); \
                                                                                    TEST_ASSERT(isEqual == bool); \
                                                                                    iusHLDeleteTransducer(actual)

    CREATE_TRANSDUCER_AND_TEST_EQUALITY( transducer,differentTransducer,pTransducerName, IUS_PLANE, centerFrequency+1,
                                         numElements, IUS_FALSE);
    CREATE_TRANSDUCER_AND_TEST_EQUALITY( transducer,differentTransducer,pTransducerName, IUS_PLANE, centerFrequency,
                                         numElements+1, IUS_FALSE);
    CREATE_TRANSDUCER_AND_TEST_EQUALITY( transducer,differentTransducer,pTransducerName, IUS_LINE, centerFrequency,
                                         numElements, IUS_FALSE);
    CREATE_TRANSDUCER_AND_TEST_EQUALITY( transducer,differentTransducer,"", IUS_PLANE, centerFrequency,
                                         numElements, IUS_FALSE);
    CREATE_TRANSDUCER_AND_TEST_EQUALITY( transducer,differentTransducer,pDifferentTransducerName, IUS_PLANE, centerFrequency,
                                         numElements, IUS_FALSE);


    status = iusTransducerSetElement(transducer, 0, _3dElement);
    TEST_ASSERT(status == IUS_E_OK);
    status = iusTransducerSetElement(transducerDuplicate, 0, _3dElement);
    TEST_ASSERT(status == IUS_E_OK);
    isEqual = iusHLCompareTransducer(transducer, transducerDuplicate);
    TEST_ASSERT(isEqual == IUS_TRUE);

    differentTransducer = iusHLCreateTransducer(pTransducerName, IUS_PLANE, centerFrequency, numElements);
    status = iusTransducerSetElement(differentTransducer, 0, _3dElement);
    TEST_ASSERT(status == IUS_E_OK);
    isEqual = iusHLCompareTransducer(transducer, differentTransducer);
    TEST_ASSERT(isEqual == IUS_TRUE);

#define CREATE_AND_SET_ELEMENT_TEST_EQUALITY(reference,actual,_elem,_x,_y,_z,_sx,_sy,_sz,_theta,_phi,bool) \
    _elem = iusUtilCreate3DElement(_x,_y,_z,_sx,_sy,_sz,_theta,_phi); \
    status = iusTransducerSetElement(actual, 0, _elem); \
    TEST_ASSERT(status == IUS_E_OK); \
    isEqual = iusHLCompareTransducer(reference, actual); \
    TEST_ASSERT(isEqual == bool)


    CREATE_AND_SET_ELEMENT_TEST_EQUALITY(transducer,differentTransducer,_3dElement,x+1, y, z, sx, sy, sz, theta, phi,
                                         IUS_FALSE);
    CREATE_AND_SET_ELEMENT_TEST_EQUALITY(transducer,differentTransducer,_3dElement,x, y+1, z, sx, sy, sz, theta, phi,
                                         IUS_FALSE);
    CREATE_AND_SET_ELEMENT_TEST_EQUALITY(transducer,differentTransducer,_3dElement,x, y, z+1, sx, sy, sz, theta, phi,
                                         IUS_FALSE);
    CREATE_AND_SET_ELEMENT_TEST_EQUALITY(transducer,differentTransducer,_3dElement,x, y, z, sx+1, sy, sz, theta, phi,
                                         IUS_FALSE);
    CREATE_AND_SET_ELEMENT_TEST_EQUALITY(transducer,differentTransducer,_3dElement,x, y, z, sx, sy+1, sz, theta, phi,
                                         IUS_FALSE);
    CREATE_AND_SET_ELEMENT_TEST_EQUALITY(transducer,differentTransducer,_3dElement,x, y, z, sx, sy, sz+1, theta, phi,
                                         IUS_FALSE);
    CREATE_AND_SET_ELEMENT_TEST_EQUALITY(transducer,differentTransducer,_3dElement,x, y, z, sx, sy, sz, theta+1, phi,
                                         IUS_FALSE);
    CREATE_AND_SET_ELEMENT_TEST_EQUALITY(transducer,differentTransducer,_3dElement,x, y, z, sx, sy, sz, theta, phi+1,
                                         IUS_FALSE);

    iusHLDeleteTransducer(differentTransducer);
}




TEST_GROUP_RUNNER(InputfileTransducer)
{
    RUN_TEST_CASE(InputfileTransducer, testIusHLCreateExperiment);
    RUN_TEST_CASE(InputfileTransducer, testIusCompareExperiment);
    RUN_TEST_CASE(InputfileTransducer, testIusHLCreateTransducer);
    RUN_TEST_CASE(InputfileTransducer, testIusTransducerElements);
    RUN_TEST_CASE(InputfileTransducer, testIusHLCompare3DTransducer);
    RUN_TEST_CASE(InputfileTransducer, testIusHLCompare2DTransducer);
}
