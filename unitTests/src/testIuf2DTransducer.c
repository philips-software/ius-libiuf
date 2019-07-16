#include <util.h>
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <iuf.h>
#include <iuf2DTransducerPrivate.h>

static char *pErrorFilename = "Iuf2DTransducer.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(Iuf2DTransducer);

TEST_SETUP(Iuf2DTransducer)
{
    iufErrorLogClear();
    iufErrorLog(IUF_TRUE);
    iufErrorAutoReportSet(IUF_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iufErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(Iuf2DTransducer)
{
    iufErrorLogClear();
    iufErrorLog(IUF_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iufErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
}


TEST(Iuf2DTransducer, testIuf2DTransducerCreate)
{
	char *transducerName = "S5-1";
	float centerFrequency = 2500000.0f;
	const int numTransducerElements = 128;
    IufTransducerShape shape = IUF_CIRCLE;
    iu2dt_t transducer;
    transducer = iuf2DTransducerCreate(transducerName, shape, centerFrequency, numTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU2DT_INVALID,transducer);
    int status = iuf2DTransducerDelete(transducer);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);


    //invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    transducer = iuf2DTransducerCreate(NULL, shape, centerFrequency, numTransducerElements);
    TEST_ASSERT_EQUAL(IU2DT_INVALID,transducer);
    transducer = iuf2DTransducerCreate(transducerName, IUF_INVALID_TRANSDUCER_SHAPE, centerFrequency, numTransducerElements);
    TEST_ASSERT_EQUAL(IU2DT_INVALID,transducer);
    transducer = iuf2DTransducerCreate(transducerName, IUF_CYLINDER, centerFrequency, numTransducerElements);
    TEST_ASSERT_EQUAL(IU2DT_INVALID,transducer);
    transducer = iuf2DTransducerCreate(transducerName, IUF_PLANE, centerFrequency, numTransducerElements);
    TEST_ASSERT_EQUAL(IU2DT_INVALID,transducer);
    transducer = iuf2DTransducerCreate(transducerName, IUF_SPHERE, centerFrequency, numTransducerElements);
    TEST_ASSERT_EQUAL(IU2DT_INVALID,transducer);
    transducer = iuf2DTransducerCreate(transducerName, shape, NAN, numTransducerElements);
    TEST_ASSERT_EQUAL(IU2DT_INVALID,transducer);
    transducer = iuf2DTransducerCreate(transducerName, shape, centerFrequency, -1);
    TEST_ASSERT_EQUAL(IU2DT_INVALID,transducer);

    TEST_ASSERT_EQUAL(7,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));


}

TEST(Iuf2DTransducer, testIuf2DTransducerDelete)
{
    char *transducerName = "S5-1";
    float centerFrequency = 2500000.0f;
    const int numTransducerElements = 128;
    IufTransducerShape shape = IUF_CIRCLE;
    iu2dt_t transducer;
    transducer = iuf2DTransducerCreate(transducerName, shape, centerFrequency, numTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU2DT_INVALID,transducer);
    int status = iuf2DTransducerDelete(transducer);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);

    // invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    status = iuf2DTransducerDelete(NULL);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, status);

    TEST_ASSERT_EQUAL(1,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
}


TEST(Iuf2DTransducer,  testIufHL2DTransducerCompare)
{
    char *transducerName = "S5-1";
    char *diffTransducerName = "S5-2";
    float centerFrequency = 2500000.0f;
    float diffCenterFrequency = 2600000.0f;
    const int numTransducerElements = 128;
    const int diffNumTransducerElements = 256;
    const float transducerPitch = 0.000005f;
    iu2dp_t elemPos = iuf2DPositionCreate((float)(10 - numTransducerElements/2.0)*transducerPitch, 0.0f);
    iu2ds_t elemSize = iuf2DSizeCreate(0.0001f,0.0001f);
    float elemAngle = 0.5f;

    iu2dte_t element = iuf2DTransducerElementCreate(elemPos, elemAngle, elemSize);
    TEST_ASSERT_NOT_EQUAL(IU2DTE_INVALID,element);

    IufTransducerShape shape = IUF_LINE;
    IufTransducerShape diffShape = IUF_CIRCLE;

    iu2dt_t transducer;
    iu2dt_t identicalTransducer;
    transducer = iuf2DTransducerCreate(transducerName, shape, centerFrequency, numTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU2DT_INVALID,transducer);
    identicalTransducer = iuf2DTransducerCreate(transducerName, shape, centerFrequency, numTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU2DT_INVALID,identicalTransducer);

    TEST_ASSERT_EQUAL(IUF_TRUE,iuf2DTransducerCompare(transducer,transducer));
    TEST_ASSERT_EQUAL(IUF_TRUE,iuf2DTransducerCompare(transducer,identicalTransducer));

    iu2dt_t differentTransducer = iuf2DTransducerCreate(transducerName, shape, centerFrequency, numTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU2DT_INVALID,identicalTransducer);
    int status = iuf2DTransducerSetElement(differentTransducer,0,element);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);
    TEST_ASSERT_EQUAL(IUF_FALSE,iuf2DTransducerCompare(transducer,differentTransducer));
    iuf2DTransducerDelete(differentTransducer);

    differentTransducer = iuf2DTransducerCreate(diffTransducerName, shape, centerFrequency, numTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU2DT_INVALID,identicalTransducer);
    TEST_ASSERT_EQUAL(IUF_FALSE,iuf2DTransducerCompare(transducer,differentTransducer));
    iuf2DTransducerDelete(differentTransducer);

    differentTransducer = iuf2DTransducerCreate(transducerName, diffShape, centerFrequency, numTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU2DT_INVALID,identicalTransducer);
    TEST_ASSERT_EQUAL(IUF_FALSE,iuf2DTransducerCompare(transducer,differentTransducer));
    iuf2DTransducerDelete(differentTransducer);

    differentTransducer = iuf2DTransducerCreate(transducerName, shape, diffCenterFrequency, numTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU2DT_INVALID,identicalTransducer);
    TEST_ASSERT_EQUAL(IUF_FALSE,iuf2DTransducerCompare(transducer,differentTransducer));
    iuf2DTransducerDelete(differentTransducer);

    differentTransducer = iuf2DTransducerCreate(transducerName, shape, centerFrequency, diffNumTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU2DT_INVALID,identicalTransducer);
    TEST_ASSERT_EQUAL(IUF_FALSE,iuf2DTransducerCompare(transducer,differentTransducer));
    iuf2DTransducerDelete(differentTransducer);

    // invalid params
    TEST_ASSERT_EQUAL(IU2DT_INVALID,iuf2DTransducerCompare(transducer,NULL));
    TEST_ASSERT_EQUAL(IU2DT_INVALID,iuf2DTransducerCompare(NULL,transducer));

    status = iuf2DTransducerDelete(transducer);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);
    status = iuf2DTransducerDelete(identicalTransducer);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);

    iuf2DTransducerElementDelete(element);
    iuf2DPositionDelete(elemPos);
    iuf2DSizeDelete(elemSize);
}

TEST(Iuf2DTransducer, testIuf2DTransducerSetGet)
{
    char *transducerName = "S5-1";
    float centerFrequency = 2500000.0f;
    const int numTransducerElements = 128;
    const float transducerPitch = 0.000005f;
    iu2dp_t elemPos = iuf2DPositionCreate((float)(10 - numTransducerElements / 2.0)*transducerPitch, 0.0f);
    iu2ds_t elemSize = iuf2DSizeCreate(0.0001f,0.0001f);
    float elemAngle = 0.5f;


    iu2dt_t transducer;
    IufTransducerShape shape = IUF_LINE;

    transducer = iuf2DTransducerCreate(transducerName, shape, centerFrequency, numTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU2DT_INVALID,transducer);

    iu2dte_t element = iuf2DTransducerElementCreate(elemPos, elemAngle, elemSize);
    TEST_ASSERT_NOT_EQUAL(IU2DTE_INVALID,element);

    int status = iuf2DTransducerSetElement(transducer,0,element);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);
    TEST_ASSERT_EQUAL(IUF_TRUE,iuf2DTransducerElementCompare(element,iuf2DTransducerGetElement(transducer,0)));


    // invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    status = iuf2DTransducerSetElement(transducer,numTransducerElements,element);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE,status);
    status = iuf2DTransducerSetElement(transducer,-1,element);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE,status);
    status = iuf2DTransducerSetElement(NULL,0,element);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE,status);
    status = iuf2DTransducerSetElement(transducer,0,NULL);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE,status);

    TEST_ASSERT_EQUAL(4,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

    iuf2DTransducerDelete(transducer);
    iuf2DTransducerElementDelete(element);
    iuf2DPositionDelete(elemPos);
    iuf2DSizeDelete(elemSize);
}

TEST(Iuf2DTransducer, testIuf2DTransducerSerialization)

{
	int status = 0;
	char *filename = "testIuf2DTransducerSerialization.hdf5";
	char *transducerName = "S5-1";
	const int numTransducerElements = 128;
	int i = 0;

	// create and fill
	const float transducerPitch = 0.000005f;

	iu2dt_t transducer = iuf2DTransducerCreate(transducerName, IUF_LINE, 2500000.0f, numTransducerElements);
    TEST_ASSERT(transducer != IU2DT_INVALID);
	for (i = 0; i < numTransducerElements; i++)
	{
        iu2dp_t elemPos = iuf2DPositionCreate((float)(10 - numTransducerElements / 2.0)*transducerPitch, 0.0f);
        iu2ds_t elemSize = iuf2DSizeCreate(0.0001f,0.0001f);
        float elemAngle = 0.5f;
		iu2dte_t element = iuf2DTransducerElementCreate(elemPos, elemAngle, elemSize);
		iuf2DTransducerSetElement(transducer, i, element);
	}

	// save
	hid_t handle = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
	TEST_ASSERT(handle > 0);
	status = iuf2DTransducerSave(transducer, handle);
	H5Fclose(handle);
	TEST_ASSERT_EQUAL(IUF_E_OK, status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iu2dt_t savedObj = iuf2DTransducerLoad(handle);
    TEST_ASSERT_NOT_EQUAL(NULL, savedObj);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUF_TRUE, iuf2DTransducerCompare(transducer,savedObj));
	// cleanup
    for (i = 0; i < numTransducerElements; i++)
    {
        iu2dte_t element = iuf2DTransducerGetElement(transducer, i);
        iu2dp_t elemPos = iuf2DTransducerElementGetPosition(element);
        iu2ds_t elemSize = iuf2DTransducerElementGetSize(element);
        iuf2DTransducerElementDelete(element);
        iuf2DPositionDelete(elemPos);
        iuf2DSizeDelete(elemSize);
    }

    status = iuf2DTransducerDelete(transducer);
    status |= iuf2DTransducerDelete(savedObj);
    TEST_ASSERT(status == IUF_E_OK);
}



TEST_GROUP_RUNNER(Iuf2DTransducer)
{
    RUN_TEST_CASE(Iuf2DTransducer, testIuf2DTransducerCreate);
    RUN_TEST_CASE(Iuf2DTransducer, testIuf2DTransducerDelete);
    RUN_TEST_CASE(Iuf2DTransducer, testIufHL2DTransducerCompare);
    RUN_TEST_CASE(Iuf2DTransducer, testIuf2DTransducerSetGet);
	RUN_TEST_CASE(Iuf2DTransducer, testIuf2DTransducerSerialization);
}
