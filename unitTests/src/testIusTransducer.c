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
#include <include/iusHLTransducerImp.h>
#include <include/iusHL2DTransducerImp.h>
#include <include/iusHL3DTransducerImp.h>

TEST_GROUP(IusTransducer);

TEST_SETUP(IusTransducer)
{
}

TEST_TEAR_DOWN(IusTransducer)
{
}


TEST(IusTransducer, testIusTransducerDelete)
{
    char *pTransducerName = "created in testIusTransducerDelete";   /**< descriptive name of the ultrasound probe */
    float centerFrequency = 8000000;   /**< operating frequency of the transducer */
    int numElements = 1;          /**< number of transducer Elements in the probe */
    IusTransducerShape shape = IUS_PLANE;
    iut_t _3dTransducer;
    iut_t _2dTransducer;

    _3dTransducer = (iut_t) iusHL3DTransducerCreate(pTransducerName, IUS_PLANE, centerFrequency, numElements);
    int status = iusHLTransducerDelete(_3dTransducer);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    _2dTransducer = (iut_t) iusHL2DTransducerCreate(pTransducerName, IUS_LINE, centerFrequency, numElements);
    status = iusHLTransducerDelete(_2dTransducer);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // invalid params
    status = iusHLTransducerDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
}

TEST(IusTransducer, testIusTransducerCompare)
{
    char *pTransducerName = "created in testIusTransducerCompare";   /**< descriptive name of the ultrasound probe */
    float centerFrequency = 8000000;   /**< operating frequency of the transducer */
    int numElements = 1;          /**< number of transducer Elements in the probe */
    IusTransducerShape shape = IUS_PLANE;
    iut_t _3dTransducer, _3dTransducerDuplicate;
    iut_t _2dTransducer
    ;

    _3dTransducer = (iut_t) iusHL3DTransducerCreate(pTransducerName, IUS_PLANE, centerFrequency, numElements);
    TEST_ASSERT_NOT_EQUAL(IUT_INVALID,_3dTransducer);

    _3dTransducerDuplicate = (iut_t) iusHL3DTransducerCreate(pTransducerName, IUS_PLANE, centerFrequency, numElements);
    TEST_ASSERT_NOT_EQUAL(IUT_INVALID,_3dTransducerDuplicate);

    _2dTransducer = (iut_t) iusHL2DTransducerCreate(pTransducerName, IUS_LINE, centerFrequency, numElements);
    TEST_ASSERT_NOT_EQUAL(IUT_INVALID,_2dTransducer);

    IUS_BOOL isEqual = iusHLTransducerCompare(_3dTransducer, _3dTransducer);
    TEST_ASSERT(isEqual == IUS_TRUE);

    isEqual = iusHLTransducerCompare(_3dTransducer, _3dTransducerDuplicate);
    TEST_ASSERT(isEqual == IUS_TRUE);

    isEqual = iusHLTransducerCompare(_3dTransducer, _2dTransducer);
    TEST_ASSERT(isEqual == IUS_FALSE);

    // invalid params
    int status = iusHLTransducerDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
}



TEST(IusTransducer, testIusTransducerSetGet)
{
    char
    *p3DTransducerName = "3D created in testIusTransducerSetGet";   /**< descriptive name of the ultrasound probe */
    char
    *p2DTransducerName = "2D created in testIusTransducerSetGet";   /**< descriptive name of the ultrasound probe */
    float _2DCenterFrequency = 8000000;   /**< operating frequency of the transducer */
    float _3DCenterFrequency = 8500000;   /**< operating frequency of the transducer */
    int _3DNumElements = 3;          /**< number of transducer Elements in the probe */
    int _2DNumElements = 2;          /**< number of transducer Elements in the probe */
    iut_t _3dTransducer;
    iut_t _2dTransducer;

    _3dTransducer = (iut_t) iusHL3DTransducerCreate(p3DTransducerName, IUS_PLANE, _3DCenterFrequency, _3DNumElements);
    TEST_ASSERT_NOT_EQUAL(IUT_INVALID, _3dTransducer);
    TEST_ASSERT(_3dTransducer != IUT_INVALID);
    TEST_ASSERT_EQUAL_FLOAT(_3DCenterFrequency, iusHLTransducerGetCenterFrequency(_3dTransducer));
    TEST_ASSERT_EQUAL(_3DNumElements, iusHLTransducerGetNumElements(_3dTransducer));
    TEST_ASSERT_EQUAL_STRING(p3DTransducerName, iusHLTransducerGetName(_3dTransducer));
    TEST_ASSERT_EQUAL(IUS_PLANE, iusHLTransducerGetShape(_3dTransducer));

    _2dTransducer = (iut_t) iusHL2DTransducerCreate(p2DTransducerName, IUS_LINE, _2DCenterFrequency, _2DNumElements);
    TEST_ASSERT_NOT_EQUAL(IUT_INVALID, _2dTransducer);
    TEST_ASSERT(_2dTransducer != IUT_INVALID);
    TEST_ASSERT_EQUAL_FLOAT(_2DCenterFrequency, iusHLTransducerGetCenterFrequency(_2dTransducer));
    TEST_ASSERT_EQUAL(_2DNumElements, iusHLTransducerGetNumElements(_2dTransducer));
    TEST_ASSERT_EQUAL_STRING(p2DTransducerName, iusHLTransducerGetName(_2dTransducer));
    TEST_ASSERT_EQUAL(IUS_LINE, iusHLTransducerGetShape(_2dTransducer));

}

TEST(IusTransducer, testIusTransducerSerialization)

{
	int status = 0;
	char *filename = "testIusTransducerSerialization.hdf5";
	char *path = "/Transducer";
	char *transducerName = "S5-1";
	float centerfrequency = 2500000.0f;
	const int numTransducerElements = 128;
	int i = 0;

	// create and fill
	const float transducerPitch = 0.000005f;

	iut_t transducer =  (iut_t) iusHL2DTransducerCreate(transducerName, IUS_LINE, centerfrequency, numTransducerElements);
	for (i = 0; i < numTransducerElements; i++)
	{
		iu2dp_t elemPos = iusHL2DPositionCreate((i - numTransducerElements / 2)*transducerPitch, 0.0f);	
		iu2ds_t elemSize = iusHL2DSizeCreate(0.0001f,0.0001f);
		iu2dte_t element = iusHL2DTransducerElementCreate(elemPos, 0.0f, elemSize);
		iusHL2DTransducerSetElement( (iu2dt_t) transducer, i, element);
	}
	TEST_ASSERT(transducer != IUT_INVALID);

	// save
	hid_t handle = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
	TEST_ASSERT(handle > 0);
	status = iusHLTransducerSave(transducer, path, handle);
	H5Fclose(handle);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iut_t savedObj = iusHLTransducerLoad(handle, path);
    TEST_ASSERT_NOT_EQUAL(NULL, savedObj);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUS_TRUE, iusHLTransducerCompare(transducer,savedObj));
    status = iusHLTransducerDelete(transducer);
    status |= iusHLTransducerDelete(savedObj);
	TEST_ASSERT(status == IUS_E_OK);
}



TEST_GROUP_RUNNER(IusTransducer)
{
    RUN_TEST_CASE(IusTransducer, testIusTransducerDelete);
    RUN_TEST_CASE(IusTransducer, testIusTransducerCompare);
    RUN_TEST_CASE(IusTransducer, testIusTransducerSetGet);
	RUN_TEST_CASE(IusTransducer, testIusTransducerSerialization);
}
