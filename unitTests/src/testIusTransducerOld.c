//
// Created by nlv09165 on 26/04/2018.
//
#include <util.h>
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <hdf5.h>
#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusTransducerFactory.h>
#include <include/iusTransducerImp.h>

TEST_GROUP(IusTransducer);

TEST_SETUP(IusTransducer)
{
}

TEST_TEAR_DOWN(IusTransducer)
{
}


TEST(IusTransducer, testIusCreateTransducer)
{
    char *pTransducerName = "created in testIusHLCreateTransducer";   /**< descriptive name of the ultrasound probe */
    float centerFrequency = 8000000;   /**< operating frequency of the transducer */
    int numElements = 1;          /**< number of transducer Elements in the probe */
    IusTransducerShape shape = IUS_PLANE;
    iut_t transducer;

    transducer = iusTransducerFactoryCreate(pTransducerName, IUS_PLANE, centerFrequency, numElements);
    TEST_ASSERT(transducer != IUT_INVALID);
    TEST_ASSERT_EQUAL_FLOAT(centerFrequency, iusTransducerGetCenterFrequency(transducer));
    TEST_ASSERT_EQUAL(numElements, iusTransducerGetNumElements(transducer));
    TEST_ASSERT_EQUAL_STRING(pTransducerName, iusTransducerGetName(transducer));
    TEST_ASSERT_EQUAL(shape, iusTransducerGetShape(transducer));
}

TEST(IusTransducer, testIusDeleteTransducer)
{
    char *pTransducerName = "created in testIusHLCreateTransducer";   /**< descriptive name of the ultrasound probe */
    float centerFrequency = 8000000;   /**< operating frequency of the transducer */
    int numElements = 1;          /**< number of transducer Elements in the probe */
    iut_t obj;

    obj = iusTransducerFactoryCreate(pTransducerName, IUS_PLANE, centerFrequency, numElements);
    TEST_ASSERT(obj != IUT_INVALID);
    int status = iusTransducerDelete(obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // invalid params
    status = iusTransducerDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
}



TEST(IusTransducer,  testIusHLCompare2DTransducer)
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
    sx = 0.150f / 1000.0f; // meter
    sz = 6.0f / 1000.0f;

    iu2dte_t _2dElement = iusUtilCreate2DElement(x, z, sx, sz, phi);

    transducer = iusTransducerFactoryCreate(pTransducerName, shape, centerFrequency, numElements);
    transducerDuplicate = iusTransducerFactoryCreate(pTransducerName, shape, centerFrequency, numElements);
    isEqual = iusTransducerCompare(transducer, transducerDuplicate);
    TEST_ASSERT(isEqual == IUS_TRUE);


    //
    // TESTS Related to different transducers
    //
#define CREATE_2DTRANSDUCER_AND_TEST_EQUALITY(reference,actual,nm,shp,freq,num,bool)   actual = iusTransducerFactoryCreate( \
                                                                                        nm, \
                                                                                        shp, \
                                                                                        freq, \
                                                                                        num); \
                                                                                    isEqual = iusTransducerCompare( \
                                                                                        reference, \
                                                                                        actual \
                                                                                        ); \
                                                                                    TEST_ASSERT(isEqual == (bool)); \
                                                                                    iusTransducerDelete(actual)

    CREATE_2DTRANSDUCER_AND_TEST_EQUALITY(transducer, differentTransducer, pTransducerName, IUS_LINE,
                                          centerFrequency + 1,
                                          numElements, IUS_FALSE);
    CREATE_2DTRANSDUCER_AND_TEST_EQUALITY(transducer, differentTransducer, pTransducerName, IUS_LINE, centerFrequency,
                                          numElements + 1, IUS_FALSE);
    CREATE_2DTRANSDUCER_AND_TEST_EQUALITY(transducer, differentTransducer, pTransducerName, IUS_PLANE, centerFrequency,
                                          numElements, IUS_FALSE);
    CREATE_2DTRANSDUCER_AND_TEST_EQUALITY(transducer, differentTransducer, "", IUS_LINE, centerFrequency,
                                          numElements, IUS_FALSE);
    CREATE_2DTRANSDUCER_AND_TEST_EQUALITY(transducer, differentTransducer, pDifferentTransducerName, IUS_LINE,
                                          centerFrequency,
                                          numElements, IUS_FALSE);


    status = iusTransducerSetElement(transducer, 0, (iute_t) _2dElement);
    TEST_ASSERT(status == IUS_E_OK);
    status = iusTransducerSetElement(transducerDuplicate, 0, (iute_t) _2dElement);
    TEST_ASSERT(status == IUS_E_OK);
    isEqual = iusTransducerCompare(transducer, transducerDuplicate);
    TEST_ASSERT(isEqual == IUS_TRUE);

    differentTransducer = iusTransducerFactoryCreate(pTransducerName, IUS_LINE, centerFrequency, numElements);
    status = iusTransducerSetElement(differentTransducer, 0, (iute_t) _2dElement);
    TEST_ASSERT(status == IUS_E_OK);
    isEqual = iusTransducerCompare(transducer, differentTransducer);
    TEST_ASSERT(isEqual == IUS_TRUE);

#define CREATE_AND_SET_2DELEMENT_TEST_EQUALITY(reference,actual,_elem,_x,_z,_sx,_sz,_phi,bool) \
    _elem = iusUtilCreate2DElement(_x,_z,_sx,_sz,_phi); \
    status = iusTransducerSetElement(actual, 0, (iute_t) (_elem)); \
    TEST_ASSERT(status == IUS_E_OK); \
    isEqual = iusTransducerCompare(reference, actual); \
    TEST_ASSERT(isEqual == (bool))

    CREATE_AND_SET_2DELEMENT_TEST_EQUALITY(transducer, differentTransducer, _2dElement, x + 1, z, sx, sz, phi,
                                           IUS_FALSE);
    CREATE_AND_SET_2DELEMENT_TEST_EQUALITY(transducer, differentTransducer, _2dElement, x, z + 1, sx, sz, phi,
                                           IUS_FALSE);
    CREATE_AND_SET_2DELEMENT_TEST_EQUALITY(transducer, differentTransducer, _2dElement, x, z, sx + 1, sz, phi,
                                           IUS_FALSE);
    CREATE_AND_SET_2DELEMENT_TEST_EQUALITY(transducer, differentTransducer, _2dElement, x, z, sx, sz + 1, phi,
                                           IUS_FALSE);
    CREATE_AND_SET_2DELEMENT_TEST_EQUALITY(transducer, differentTransducer, _2dElement, x, z, sx, sz, phi + 1,
                                           IUS_FALSE);

    iusTransducerDelete(differentTransducer);
}


TEST(IusTransducer,  testIusHLCompare3DTransducer)
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
    sx = 0.150f / 1000.0f; // meter
    sy = 0.0f;
    sz = 6.0f / 1000.0f;

    iu3dte_t _3dElement = iusUtilCreate3DElement(x, y, z, sx, sy, sz, theta, phi);

    transducer = iusTransducerFactoryCreate(pTransducerName, IUS_PLANE, centerFrequency, numElements);
    transducerDuplicate = iusTransducerFactoryCreate(pTransducerName, IUS_PLANE, centerFrequency, numElements);
    isEqual = iusTransducerCompare(transducer, transducerDuplicate);
    TEST_ASSERT(isEqual == IUS_TRUE);


    //
    // TESTS Related to different transducers
    //
    differentTransducer = iusTransducerFactoryCreate(pTransducerName, IUS_PLANE, centerFrequency, numElements+1);
    isEqual = iusTransducerCompare( transducer, differentTransducer );
    TEST_ASSERT(isEqual == IUS_FALSE);
    iusTransducerDelete(differentTransducer);

#define CREATE_TRANSDUCER_AND_TEST_EQUALITY(reference,actual,nm,shp,freq,num,bool)   actual = iusTransducerFactoryCreate( \
                                                                                        nm, \
                                                                                        shp, \
                                                                                        freq, \
                                                                                        num); \
                                                                                    isEqual = iusTransducerCompare( \
                                                                                        reference, \
                                                                                        actual \
                                                                                        ); \
                                                                                    TEST_ASSERT(isEqual == bool); \
                                                                                    iusTransducerDelete(actual)

    CREATE_TRANSDUCER_AND_TEST_EQUALITY(transducer, differentTransducer, pTransducerName, IUS_PLANE,
                                        centerFrequency + 1,
                                        numElements, IUS_FALSE);
    CREATE_TRANSDUCER_AND_TEST_EQUALITY(transducer, differentTransducer, pTransducerName, IUS_PLANE, centerFrequency,
                                        numElements + 1, IUS_FALSE);
    CREATE_TRANSDUCER_AND_TEST_EQUALITY(transducer, differentTransducer, pTransducerName, IUS_LINE, centerFrequency,
                                        numElements, IUS_FALSE);
    CREATE_TRANSDUCER_AND_TEST_EQUALITY(transducer, differentTransducer, "", IUS_PLANE, centerFrequency,
                                        numElements, IUS_FALSE);
    CREATE_TRANSDUCER_AND_TEST_EQUALITY(transducer, differentTransducer, pDifferentTransducerName, IUS_PLANE,
                                        centerFrequency,
                                        numElements, IUS_FALSE);


    status = iusTransducerSetElement(transducer, 0, (iute_t) _3dElement);
    TEST_ASSERT(status == IUS_E_OK);
    status = iusTransducerSetElement(transducerDuplicate, 0, (iute_t) _3dElement);
    TEST_ASSERT(status == IUS_E_OK);
    isEqual = iusTransducerCompare(transducer, transducerDuplicate);
    TEST_ASSERT(isEqual == IUS_TRUE);

    differentTransducer = iusTransducerFactoryCreate(pTransducerName, IUS_PLANE, centerFrequency, numElements);
    status = iusTransducerSetElement(differentTransducer, 0, (iute_t) _3dElement);
    TEST_ASSERT(status == IUS_E_OK);
    isEqual = iusTransducerCompare(transducer, differentTransducer);
    TEST_ASSERT(isEqual == IUS_TRUE);

#define CREATE_AND_SET_ELEMENT_TEST_EQUALITY(reference,actual,_elem,_x,_y,_z,_sx,_sy,_sz,_theta,_phi,bool) \
    _elem = iusUtilCreate3DElement(_x,_y,_z,_sx,_sy,_sz,_theta,_phi); \
    status = iusTransducerSetElement(actual, 0, (iute_t) (_elem)); \
    TEST_ASSERT(status == IUS_E_OK); \
    isEqual = iusTransducerCompare(reference, actual); \
    TEST_ASSERT(isEqual == bool)


    CREATE_AND_SET_ELEMENT_TEST_EQUALITY(transducer, differentTransducer, _3dElement, x + 1, y, z, sx, sy, sz, theta,
                                         phi,
                                         IUS_FALSE);
    CREATE_AND_SET_ELEMENT_TEST_EQUALITY(transducer, differentTransducer, _3dElement, x, y + 1, z, sx, sy, sz, theta,
                                         phi,
                                         IUS_FALSE);
    CREATE_AND_SET_ELEMENT_TEST_EQUALITY(transducer, differentTransducer, _3dElement, x, y, z + 1, sx, sy, sz, theta,
                                         phi,
                                         IUS_FALSE);
    CREATE_AND_SET_ELEMENT_TEST_EQUALITY(transducer, differentTransducer, _3dElement, x, y, z, sx + 1, sy, sz, theta,
                                         phi,
                                         IUS_FALSE);
    CREATE_AND_SET_ELEMENT_TEST_EQUALITY(transducer, differentTransducer, _3dElement, x, y, z, sx, sy + 1, sz, theta,
                                         phi,
                                         IUS_FALSE);
    CREATE_AND_SET_ELEMENT_TEST_EQUALITY(transducer, differentTransducer, _3dElement, x, y, z, sx, sy, sz + 1, theta,
                                         phi,
                                         IUS_FALSE);
    CREATE_AND_SET_ELEMENT_TEST_EQUALITY(transducer, differentTransducer, _3dElement, x, y, z, sx, sy, sz, theta + 1,
                                         phi,
                                         IUS_FALSE);
    CREATE_AND_SET_ELEMENT_TEST_EQUALITY(transducer, differentTransducer, _3dElement, x, y, z, sx, sy, sz, theta,
                                         phi + 1,
                                         IUS_FALSE);

    iusTransducerDelete(differentTransducer);
}

TEST(IusTransducer, testIusSetGetTransducer)
{
    char *pTransducerName = "created in testIusHLCompareTransducer";
    float centerFrequency = 8000000;   /**< operating frequency of the transducer */

    int numElements = 1;          /**< number of transducer Elements in the probe */
    IusTransducerShape shape = IUS_PLANE;
    iut_t transducer;
    IUS_BOOL isEqual;
    int status=0;

    float x, y, z;
    float sx, sy, sz;
    float theta, phi;

    x = y = z = 0.0f;
    theta = phi = 0.0f;
    sx = 0.150f / 1000.0f; // meter
    sy = 0.0f;
    sz = 6.0f / 1000.0f;

    transducer = iusTransducerFactoryCreate(pTransducerName, IUS_PLANE, centerFrequency, numElements);
    TEST_ASSERT(transducer != IUT_INVALID);
    TEST_ASSERT_EQUAL_FLOAT(centerFrequency, iusTransducerGetCenterFrequency(transducer));
    TEST_ASSERT_EQUAL(numElements, iusTransducerGetNumElements(transducer));
    TEST_ASSERT_EQUAL_STRING(pTransducerName, iusTransducerGetName(transducer));
    TEST_ASSERT_EQUAL(shape, iusTransducerGetShape(transducer));

    iu3dte_t _3dElement = iusUtilCreate3DElement( x, y, z, sx, sy, sz, theta, phi);
    status = iusTransducerSetElement( transducer, 0, (iute_t) _3dElement );
    TEST_ASSERT(status == IUS_E_OK);

    iu3dte_t _nother3dElement = (iu3dte_t) iusTransducerGetElement(transducer,0);
    isEqual = iusTransducerElementCompare((iute_t) _3dElement, (iute_t) _nother3dElement);
    TEST_ASSERT_EQUAL(IUS_TRUE,isEqual);
}

TEST(IusTransducer, testIusTransducerSerialization)

{
    int status = 0;
    char *filename = "testIusTransducerSerialization.hdf5";
    char *path = "/Transducer";
    char *transducerName = "S5-1";
    const int numTransducerElements = 128;
    int i = 0;

    // create and fill
    const float transducerPitch = 0.000005f;

    iut_t transducer = iusTransducerFactoryCreate(transducerName, IUS_LINE, 2500000.0f, numTransducerElements);
    for (i = 0; i < numTransducerElements; i++)
    {
        iu2dp_t elemPos = ius2DPositionCreate((i - numTransducerElements / 2)*transducerPitch, 0.0f);
        iu2ds_t elemSize = ius2DSizeCreate(0.0001f,0.0001f);
        iu2dte_t element = ius2DTransducerElementCreate(elemPos, 0.0f, elemSize);
        iusTransducerSetElement(transducer, i, (iute_t)element);
    }
    TEST_ASSERT(transducer != IUT_INVALID);

    // save
    hid_t handle = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    TEST_ASSERT(handle > 0);
    status = iusTransducerSave(transducer, path, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);


    TEST_ASSERT(status == IUS_E_OK);
}



TEST_GROUP_RUNNER(IusTransducer)
{
    RUN_TEST_CASE(IusTransducer, testIusCreateTransducer);
    RUN_TEST_CASE(IusTransducer, testIusDeleteTransducer);
    RUN_TEST_CASE(IusTransducer, testIusHLCompare3DTransducer);
    RUN_TEST_CASE(IusTransducer, testIusHLCompare2DTransducer);
    RUN_TEST_CASE(IusTransducer, testIusSetGetTransducer);
    RUN_TEST_CASE(IusTransducer, testIusTransducerSerialization);
}
