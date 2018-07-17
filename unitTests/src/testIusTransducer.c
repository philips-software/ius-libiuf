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
#include <iusHLTransducerFactory.h>

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

    transducer = iusHLTransducerFactoryCreate(pTransducerName, IUS_PLANE, centerFrequency, numElements);
    TEST_ASSERT(transducer != IUT_INVALID);
    TEST_ASSERT_EQUAL_FLOAT(centerFrequency, iusHLTransducerGetCenterFrequency(transducer));
    TEST_ASSERT_EQUAL(numElements, iusHLTransducerGetNumElements(transducer));
    TEST_ASSERT_EQUAL_STRING(pTransducerName, iusHLTransducerGetName(transducer));
    TEST_ASSERT_EQUAL(shape, iusHLTransducerGetShape(transducer));
}

TEST(IusTransducer, testIusDeleteTransducer)
{
    char *pTransducerName = "created in testIusHLCreateTransducer";   /**< descriptive name of the ultrasound probe */
    float centerFrequency = 8000000;   /**< operating frequency of the transducer */
    int numElements = 1;          /**< number of transducer Elements in the probe */
    iut_t obj;

    obj = iusHLTransducerFactoryCreate(pTransducerName, IUS_PLANE, centerFrequency, numElements);
    TEST_ASSERT(obj != IUT_INVALID);
    int status = iusHLTransducerDelete(obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // invalid params
    status = iusHLTransducerDelete(NULL);
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

    transducer = iusHLTransducerFactoryCreate(pTransducerName, shape, centerFrequency, numElements);
    transducerDuplicate = iusHLTransducerFactoryCreate(pTransducerName, shape, centerFrequency, numElements);
    isEqual = iusHLTransducerCompare(transducer, transducerDuplicate);
    TEST_ASSERT(isEqual == IUS_TRUE);


    //
    // TESTS Related to different transducers
    //
#define CREATE_2DTRANSDUCER_AND_TEST_EQUALITY(reference,actual,nm,shp,freq,num,bool)   actual = iusHLTransducerFactoryCreate( \
                                                                                        nm, \
                                                                                        shp, \
                                                                                        freq, \
                                                                                        num); \
                                                                                    isEqual = iusHLTransducerCompare( \
                                                                                        reference, \
                                                                                        actual \
                                                                                        ); \
                                                                                    TEST_ASSERT(isEqual == (bool)); \
                                                                                    iusHLTransducerDelete(actual)

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


    status = iusHLTransducerSetElement(transducer, 0, (iute_t) _2dElement);
    TEST_ASSERT(status == IUS_E_OK);
    status = iusHLTransducerSetElement(transducerDuplicate, 0, (iute_t) _2dElement);
    TEST_ASSERT(status == IUS_E_OK);
    isEqual = iusHLTransducerCompare(transducer, transducerDuplicate);
    TEST_ASSERT(isEqual == IUS_TRUE);

    differentTransducer = iusHLTransducerFactoryCreate(pTransducerName, IUS_LINE, centerFrequency, numElements);
    status = iusHLTransducerSetElement(differentTransducer, 0, (iute_t) _2dElement);
    TEST_ASSERT(status == IUS_E_OK);
    isEqual = iusHLTransducerCompare(transducer, differentTransducer);
    TEST_ASSERT(isEqual == IUS_TRUE);

#define CREATE_AND_SET_2DELEMENT_TEST_EQUALITY(reference,actual,_elem,_x,_z,_sx,_sz,_phi,bool) \
    _elem = iusUtilCreate2DElement(_x,_z,_sx,_sz,_phi); \
    status = iusHLTransducerSetElement(actual, 0, (iute_t) (_elem)); \
    TEST_ASSERT(status == IUS_E_OK); \
    isEqual = iusHLTransducerCompare(reference, actual); \
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

  iusHLTransducerDelete(differentTransducer);
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

    transducer = iusHLTransducerFactoryCreate(pTransducerName, IUS_PLANE, centerFrequency, numElements);
    transducerDuplicate = iusHLTransducerFactoryCreate(pTransducerName, IUS_PLANE, centerFrequency, numElements);
    isEqual = iusHLTransducerCompare(transducer, transducerDuplicate);
    TEST_ASSERT(isEqual == IUS_TRUE);


    //
    // TESTS Related to different transducers
    //
  differentTransducer = iusHLTransducerFactoryCreate(pTransducerName, IUS_PLANE, centerFrequency, numElements+1);
  isEqual = iusHLTransducerCompare( transducer, differentTransducer );
  TEST_ASSERT(isEqual == IUS_FALSE);
  iusHLTransducerDelete(differentTransducer);

#define CREATE_TRANSDUCER_AND_TEST_EQUALITY(reference,actual,nm,shp,freq,num,bool)   actual = iusHLTransducerFactoryCreate( \
                                                                                        nm, \
                                                                                        shp, \
                                                                                        freq, \
                                                                                        num); \
                                                                                    isEqual = iusHLTransducerCompare( \
                                                                                        reference, \
                                                                                        actual \
                                                                                        ); \
                                                                                    TEST_ASSERT(isEqual == bool); \
                                                                                    iusHLTransducerDelete(actual)

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


    status = iusHLTransducerSetElement(transducer, 0, (iute_t) _3dElement);
    TEST_ASSERT(status == IUS_E_OK);
    status = iusHLTransducerSetElement(transducerDuplicate, 0, (iute_t) _3dElement);
    TEST_ASSERT(status == IUS_E_OK);
    isEqual = iusHLTransducerCompare(transducer, transducerDuplicate);
    TEST_ASSERT(isEqual == IUS_TRUE);

    differentTransducer = iusHLTransducerFactoryCreate(pTransducerName, IUS_PLANE, centerFrequency, numElements);
    status = iusHLTransducerSetElement(differentTransducer, 0, (iute_t) _3dElement);
    TEST_ASSERT(status == IUS_E_OK);
    isEqual = iusHLTransducerCompare(transducer, differentTransducer);
    TEST_ASSERT(isEqual == IUS_TRUE);

#define CREATE_AND_SET_ELEMENT_TEST_EQUALITY(reference,actual,_elem,_x,_y,_z,_sx,_sy,_sz,_theta,_phi,bool) \
    _elem = iusUtilCreate3DElement(_x,_y,_z,_sx,_sy,_sz,_theta,_phi); \
    status = iusHLTransducerSetElement(actual, 0, (iute_t) (_elem)); \
    TEST_ASSERT(status == IUS_E_OK); \
    isEqual = iusHLTransducerCompare(reference, actual); \
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

  iusHLTransducerDelete(differentTransducer);
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

    transducer = iusHLTransducerFactoryCreate(pTransducerName, IUS_PLANE, centerFrequency, numElements);
    TEST_ASSERT(transducer != IUT_INVALID);
    TEST_ASSERT_EQUAL_FLOAT(centerFrequency, iusHLTransducerGetCenterFrequency(transducer));
    TEST_ASSERT_EQUAL(numElements, iusHLTransducerGetNumElements(transducer));
    TEST_ASSERT_EQUAL_STRING(pTransducerName, iusHLTransducerGetName(transducer));
    TEST_ASSERT_EQUAL(shape, iusHLTransducerGetShape(transducer));

    iu3dte_t _3dElement = iusUtilCreate3DElement( x, y, z, sx, sy, sz, theta, phi);
    status = iusHLTransducerSetElement( transducer, 0, (iute_t) _3dElement );
    TEST_ASSERT(status == IUS_E_OK);

    iu3dte_t _nother3dElement = (iu3dte_t) iusHLTransducerGetElement(transducer,0);
    isEqual = iusHLTransducerElementCompare((iute_t) _3dElement, (iute_t) _nother3dElement);
    TEST_ASSERT_EQUAL(IUS_TRUE,isEqual);
}

TEST(IusTransducer, testIusTransducerSerialization)

{
	int status = 0;
	char *filename = "testIusTransducerSerialization.hdf5";
	char *path = "/Transducer";
	char *transducerName = "S5-1";
	const int numTransducerElements = 128;
	
	// create and fill
	const float transducerPitch = 0.000005;

	iut_t transducer = iusHLTransducerFactoryCreate(transducerName, IUS_LINE, 2500000.0f, numTransducerElements);
	for (int i = 0; i < numTransducerElements; i++)
	{
		iu2dp_t elemPos = iusHL2DPositionCreate((i - numTransducerElements / 2)*transducerPitch, 0.0f);	
		iu2ds_t elemSize = iusHL2DSizeCreate(0.0001,0.0001);
		iu2dte_t element = iusHL2DTransducerElementCreate(elemPos, 0.0f, elemSize);
		iusHLTransducerSetElement(transducer, i, element);
	}
	TEST_ASSERT(transducer != IUT_INVALID);

	// save
	hid_t handle = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
	TEST_ASSERT(handle > 0);
	status = iusHLTransducerWrite(transducer, path, handle);
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
