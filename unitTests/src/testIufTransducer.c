//
// Created by nlv09165 on 26/04/2018.
//
#include <util.h>
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <iuf.h>
#include <iufTransducerPrivate.h>
#include <iuf2DTransducerPrivate.h>
#include <iuf3DTransducerPrivate.h>



static char *pErrorFilename = "IufTransducer.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(IufTransducer);

TEST_SETUP(IufTransducer)
{
    iufErrorLogClear();
    iufErrorLog(IUF_TRUE);
    iufErrorAutoReportSet(IUF_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iufErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IufTransducer)
{
    iufErrorLogClear();
    iufErrorLog(IUF_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iufErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
}


TEST(IufTransducer, testIufTransducerDelete)
{
    char *pTransducerName = "created in testIufTransducerDelete";   /**< descriptive name of the ultrasound probe */
    float centerFrequency = 8000000;   /**< operating frequency of the transducer */
    int numElements = 1;          /**< number of transducer Elements in the probe */
    iut_t _3dTransducer;
    iut_t _2dTransducer;

    _3dTransducer = (iut_t) iuf3DTransducerCreate(pTransducerName, IUF_PLANE, centerFrequency, numElements);
    int status = iufTransducerDelete(_3dTransducer);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    _2dTransducer = (iut_t) iuf2DTransducerCreate(pTransducerName, IUF_LINE, centerFrequency, numElements);
    status = iufTransducerDelete(_2dTransducer);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    // invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    status = iufTransducerDelete(NULL);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, status);

    TEST_ASSERT_EQUAL(1,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
}

TEST(IufTransducer, testIufTransducerCompare)
{
    char *pTransducerName = "created in testIufTransducerCompare";   /**< descriptive name of the ultrasound probe */
    float centerFrequency = 8000000;   /**< operating frequency of the transducer */
    int numElements = 1;          /**< number of transducer Elements in the probe */
    iut_t _3dTransducer, _3dTransducerDuplicate;
    iut_t _2dTransducer
    ;

    _3dTransducer = (iut_t) iuf3DTransducerCreate(pTransducerName, IUF_PLANE, centerFrequency, numElements);
    TEST_ASSERT_NOT_EQUAL(IUT_INVALID,_3dTransducer);

    _3dTransducerDuplicate = (iut_t) iuf3DTransducerCreate(pTransducerName, IUF_PLANE, centerFrequency, numElements);
    TEST_ASSERT_NOT_EQUAL(IUT_INVALID,_3dTransducerDuplicate);

    _2dTransducer = (iut_t) iuf2DTransducerCreate(pTransducerName, IUF_LINE, centerFrequency, numElements);
    TEST_ASSERT_NOT_EQUAL(IUT_INVALID,_2dTransducer);

    IUF_BOOL isEqual = iufTransducerCompare(_3dTransducer, _3dTransducer);
    TEST_ASSERT(isEqual == IUF_TRUE);

    isEqual = iufTransducerCompare(_3dTransducer, _3dTransducerDuplicate);
    TEST_ASSERT(isEqual == IUF_TRUE);

    isEqual = iufTransducerCompare(_3dTransducer, _2dTransducer);
    TEST_ASSERT(isEqual == IUF_FALSE);

    iuf2DTransducerDelete((iu2dt_t)_2dTransducer);
    iuf3DTransducerDelete((iu3dt_t)_3dTransducer);
    iuf3DTransducerDelete((iu3dt_t)_3dTransducerDuplicate);
}



TEST(IufTransducer, testIufTransducerSetGet)
{
    char
    *p3DTransducerName = "3D created in testIufTransducerSetGet";   /**< descriptive name of the ultrasound probe */
    char
    *p2DTransducerName = "2D created in testIufTransducerSetGet";   /**< descriptive name of the ultrasound probe */
    float _2DCenterFrequency = 8000000;   /**< operating frequency of the transducer */
    float _3DCenterFrequency = 8500000;   /**< operating frequency of the transducer */
    int _3DNumElements = 3;          /**< number of transducer Elements in the probe */
    int _2DNumElements = 2;          /**< number of transducer Elements in the probe */
    iut_t _3dTransducer;
    iut_t _2dTransducer;

    _3dTransducer = (iut_t) iuf3DTransducerCreate(p3DTransducerName, IUF_PLANE, _3DCenterFrequency, _3DNumElements);
    TEST_ASSERT_NOT_EQUAL(IUT_INVALID, _3dTransducer);
    TEST_ASSERT(_3dTransducer != IUT_INVALID);
    TEST_ASSERT_EQUAL_FLOAT(_3DCenterFrequency, iufTransducerGetCenterFrequency(_3dTransducer));
    TEST_ASSERT_EQUAL(_3DNumElements, iufTransducerGetNumElements(_3dTransducer));
    TEST_ASSERT_EQUAL_STRING(p3DTransducerName, iufTransducerGetName(_3dTransducer));
    TEST_ASSERT_EQUAL(IUF_PLANE, iufTransducerGetShape(_3dTransducer));

    _2dTransducer = (iut_t) iuf2DTransducerCreate(p2DTransducerName, IUF_LINE, _2DCenterFrequency, _2DNumElements);
    TEST_ASSERT_NOT_EQUAL(IUT_INVALID, _2dTransducer);
    TEST_ASSERT(_2dTransducer != IUT_INVALID);
    TEST_ASSERT_EQUAL_FLOAT(_2DCenterFrequency, iufTransducerGetCenterFrequency(_2dTransducer));
    TEST_ASSERT_EQUAL(_2DNumElements, iufTransducerGetNumElements(_2dTransducer));
    TEST_ASSERT_EQUAL_STRING(p2DTransducerName, iufTransducerGetName(_2dTransducer));
    TEST_ASSERT_EQUAL(IUF_LINE, iufTransducerGetShape(_2dTransducer));

    iuf2DTransducerDelete((iu2dt_t)_2dTransducer);
    iuf3DTransducerDelete((iu3dt_t)_3dTransducer);
}

TEST(IufTransducer, testIufTransducerSerialization)

{
	int status = 0;
	char *filename = "testIufTransducerSerialization.hdf5";
	char *transducerName = "S5-1";
	float centerfrequency = 2500000.0f;
	const int numTransducerElements = 128;
	int i = 0;

	// create and fill
	const float transducerPitch = 0.000005f;

	iut_t transducer =  (iut_t) iuf2DTransducerCreate(transducerName, IUF_LINE, centerfrequency, numTransducerElements);
	for (i = 0; i < numTransducerElements; i++)
	{
		iu2dp_t elemPos = iuf2DPositionCreate((float)(i - numTransducerElements / 2.0)*transducerPitch, 0.0f);
		iu2ds_t elemSize = iuf2DSizeCreate(0.0001f,0.0001f);
		iu2dte_t element = iuf2DTransducerElementCreate(elemPos, 0.0f, elemSize);
		iuf2DTransducerSetElement( (iu2dt_t) transducer, i, element);
	}
	TEST_ASSERT(transducer != IUT_INVALID);

	// save
	hid_t handle = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
	TEST_ASSERT(handle > 0);
	status = iufTransducerSave(transducer, handle);
	H5Fclose(handle);
	TEST_ASSERT_EQUAL(IUF_E_OK, status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iut_t savedObj = iufTransducerLoad(handle);
    TEST_ASSERT_NOT_EQUAL(NULL, savedObj);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUF_TRUE, iufTransducerCompare(transducer,savedObj));
    for (i = 0; i < numTransducerElements; i++)
    {
        iu2dte_t element = iuf2DTransducerGetElement( (iu2dt_t) transducer, i);
        iuf2DSizeDelete(iuf2DTransducerElementGetSize(element));
        iuf2DPositionDelete(iuf2DTransducerElementGetPosition(element));
        iuf2DTransducerElementDelete(element);
    }
    status = iufTransducerDelete(transducer);
    status |= iufTransducerDelete(savedObj);
	TEST_ASSERT(status == IUF_E_OK);
}



TEST_GROUP_RUNNER(IufTransducer)
{
    RUN_TEST_CASE(IufTransducer, testIufTransducerDelete);
    RUN_TEST_CASE(IufTransducer, testIufTransducerCompare);
    RUN_TEST_CASE(IufTransducer, testIufTransducerSetGet);
	RUN_TEST_CASE(IufTransducer, testIufTransducerSerialization);
}
