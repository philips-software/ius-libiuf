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
#include <include/iusTransducerImp.h>
#include <include/ius2DTransducerImp.h>
#include <include/ius3DTransducerImp.h>

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

    _3dTransducer = (iut_t) ius3DTransducerCreate(pTransducerName, IUS_PLANE, centerFrequency, numElements);
    int status = iusTransducerDelete(_3dTransducer);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    _2dTransducer = (iut_t) ius2DTransducerCreate(pTransducerName, IUS_LINE, centerFrequency, numElements);
    status = iusTransducerDelete(_2dTransducer);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // invalid params
    status = iusTransducerDelete(NULL);
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

    _3dTransducer = (iut_t) ius3DTransducerCreate(pTransducerName, IUS_PLANE, centerFrequency, numElements);
    TEST_ASSERT_NOT_EQUAL(IUT_INVALID,_3dTransducer);

    _3dTransducerDuplicate = (iut_t) ius3DTransducerCreate(pTransducerName, IUS_PLANE, centerFrequency, numElements);
    TEST_ASSERT_NOT_EQUAL(IUT_INVALID,_3dTransducerDuplicate);

    _2dTransducer = (iut_t) ius2DTransducerCreate(pTransducerName, IUS_LINE, centerFrequency, numElements);
    TEST_ASSERT_NOT_EQUAL(IUT_INVALID,_2dTransducer);

    IUS_BOOL isEqual = iusTransducerCompare(_3dTransducer, _3dTransducer);
    TEST_ASSERT(isEqual == IUS_TRUE);

    isEqual = iusTransducerCompare(_3dTransducer, _3dTransducerDuplicate);
    TEST_ASSERT(isEqual == IUS_TRUE);

    isEqual = iusTransducerCompare(_3dTransducer, _2dTransducer);
    TEST_ASSERT(isEqual == IUS_FALSE);

    // invalid params
    int status = iusTransducerDelete(NULL);
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

    _3dTransducer = (iut_t) ius3DTransducerCreate(p3DTransducerName, IUS_PLANE, _3DCenterFrequency, _3DNumElements);
    TEST_ASSERT_NOT_EQUAL(IUT_INVALID, _3dTransducer);
    TEST_ASSERT(_3dTransducer != IUT_INVALID);
    TEST_ASSERT_EQUAL_FLOAT(_3DCenterFrequency, iusTransducerGetCenterFrequency(_3dTransducer));
    TEST_ASSERT_EQUAL(_3DNumElements, iusTransducerGetNumElements(_3dTransducer));
    TEST_ASSERT_EQUAL_STRING(p3DTransducerName, iusTransducerGetName(_3dTransducer));
    TEST_ASSERT_EQUAL(IUS_PLANE, iusTransducerGetShape(_3dTransducer));

    _2dTransducer = (iut_t) ius2DTransducerCreate(p2DTransducerName, IUS_LINE, _2DCenterFrequency, _2DNumElements);
    TEST_ASSERT_NOT_EQUAL(IUT_INVALID, _2dTransducer);
    TEST_ASSERT(_2dTransducer != IUT_INVALID);
    TEST_ASSERT_EQUAL_FLOAT(_2DCenterFrequency, iusTransducerGetCenterFrequency(_2dTransducer));
    TEST_ASSERT_EQUAL(_2DNumElements, iusTransducerGetNumElements(_2dTransducer));
    TEST_ASSERT_EQUAL_STRING(p2DTransducerName, iusTransducerGetName(_2dTransducer));
    TEST_ASSERT_EQUAL(IUS_LINE, iusTransducerGetShape(_2dTransducer));

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

	iut_t transducer =  (iut_t) ius2DTransducerCreate(transducerName, IUS_LINE, centerfrequency, numTransducerElements);
	for (i = 0; i < numTransducerElements; i++)
	{
		iu2dp_t elemPos = ius2DPositionCreate((i - numTransducerElements / 2)*transducerPitch, 0.0f);
		iu2ds_t elemSize = ius2DSizeCreate(0.0001f,0.0001f);
		iu2dte_t element = ius2DTransducerElementCreate(elemPos, 0.0f, elemSize);
		ius2DTransducerSetElement( (iu2dt_t) transducer, i, element);
	}
	TEST_ASSERT(transducer != IUT_INVALID);

	// save
	hid_t handle = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
	TEST_ASSERT(handle > 0);
	status = iusTransducerSave(transducer, path, handle);
	H5Fclose(handle);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iut_t savedObj = iusTransducerLoad(handle, path);
    TEST_ASSERT_NOT_EQUAL(NULL, savedObj);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUS_TRUE, iusTransducerCompare(transducer,savedObj));
    status = iusTransducerDelete(transducer);
    status |= iusTransducerDelete(savedObj);
	TEST_ASSERT(status == IUS_E_OK);
}



TEST_GROUP_RUNNER(IusTransducer)
{
    RUN_TEST_CASE(IusTransducer, testIusTransducerDelete);
    RUN_TEST_CASE(IusTransducer, testIusTransducerCompare);
    RUN_TEST_CASE(IusTransducer, testIusTransducerSetGet);
	RUN_TEST_CASE(IusTransducer, testIusTransducerSerialization);
}
