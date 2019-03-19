//
// Created by nlv09165 on 26/04/2018.
//
#include <util.h>
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <iuf.h>
#include <iuf3DTransducerPrivate.h>

static char *pErrorFilename = "Iuf3DTransducer.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(Iuf3DTransducer);

TEST_SETUP(Iuf3DTransducer)
{
    iufErrorLogClear();
    iufErrorLog(IUF_TRUE);
    iufErrorAutoReportSet(IUF_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iufErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(Iuf3DTransducer)
{
    iufErrorLogClear();
    iufErrorLog(IUF_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iufErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
}


TEST(Iuf3DTransducer, testIuf3DTransducerCreate)
{
	char *transducerName = "S5-1";
	float centerFrequency = 2500000.0f;
	const int numTransducerElements = 128;
    IufTransducerShape shape = IUF_SPHERE;
    iu3dt_t transducer;
    transducer = iuf3DTransducerCreate(transducerName, shape, centerFrequency, numTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU3DT_INVALID,transducer);
    int status = iuf3DTransducerDelete(transducer);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);


    //invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    transducer = iuf3DTransducerCreate(NULL, shape, centerFrequency, numTransducerElements);
    TEST_ASSERT_EQUAL(IU3DT_INVALID,transducer);
    transducer = iuf3DTransducerCreate(transducerName, IUF_INVALID_TRANSDUCER_SHAPE, centerFrequency, numTransducerElements);
    TEST_ASSERT_EQUAL(IU3DT_INVALID,transducer);
    transducer = iuf3DTransducerCreate(transducerName, IUF_LINE, centerFrequency, numTransducerElements);
    TEST_ASSERT_EQUAL(IU3DT_INVALID,transducer);
    transducer = iuf3DTransducerCreate(transducerName, IUF_CIRCLE, centerFrequency, numTransducerElements);
    TEST_ASSERT_EQUAL(IU3DT_INVALID,transducer);
    transducer = iuf3DTransducerCreate(transducerName, shape, NAN, numTransducerElements);
    TEST_ASSERT_EQUAL(IU3DT_INVALID,transducer);
    transducer = iuf3DTransducerCreate(transducerName, shape, centerFrequency, -1);
    TEST_ASSERT_EQUAL(IU3DT_INVALID,transducer);

    TEST_ASSERT_EQUAL(6,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));


}

TEST(Iuf3DTransducer, testIuf3DTransducerDelete)
{
    char *transducerName = "S5-1";
    float centerFrequency = 2500000.0f;
    const int numTransducerElements = 128;
    IufTransducerShape shape = IUF_CYLINDER;
    iu3dt_t transducer;
    transducer = iuf3DTransducerCreate(transducerName, shape, centerFrequency, numTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU3DT_INVALID,transducer);
    int status = iuf3DTransducerDelete(transducer);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);

    // invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    status = iuf3DTransducerDelete(NULL);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, status);

    TEST_ASSERT_EQUAL(1,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
}


TEST(Iuf3DTransducer,  testIufHL3DTransducerCompare)
{
    char *transducerName = "S5-1";
    char *diffTransducerName = "S5-2";
    float centerFrequency = 2500000.0f;
    float diffCenterFrequency = 2600000.0f;
    const int numTransducerElements = 128;
    const int diffNumTransducerElements = 256;
    const float transducerPitch = 0.000005f;
    iu3dp_t elemPos = iuf3DPositionCreate((float)(10 - numTransducerElements / 2.0)*transducerPitch, 0.0f, 0.0f);
    iu3ds_t elemSize = iuf3DSizeCreate(0.0001f,0.0001f,0.0001f);
    iu3da_t elemAngle = iuf3DAngleCreate(0.0f,0.3f);

    iu3dte_t element = iuf3DTransducerElementCreate(elemPos, elemAngle, elemSize);
    TEST_ASSERT_NOT_EQUAL(IU3DTE_INVALID,element);

    IufTransducerShape shape = IUF_PLANE;
    IufTransducerShape diffShape = IUF_CYLINDER;

    iu3dt_t transducer;
    iu3dt_t identicalTransducer;
    transducer = iuf3DTransducerCreate(transducerName, shape, centerFrequency, numTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU3DT_INVALID,transducer);
    identicalTransducer = iuf3DTransducerCreate(transducerName, shape, centerFrequency, numTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU3DT_INVALID,identicalTransducer);

    TEST_ASSERT_EQUAL(IUF_TRUE,iuf3DTransducerCompare(transducer,transducer));
    TEST_ASSERT_EQUAL(IUF_TRUE,iuf3DTransducerCompare(transducer,identicalTransducer));

    iu3dt_t differentTransducer = iuf3DTransducerCreate(transducerName, shape, centerFrequency, numTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU3DT_INVALID,identicalTransducer);
    int status = iuf3DTransducerSetElement(differentTransducer,0,element);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);
    TEST_ASSERT_EQUAL(IUF_FALSE,iuf3DTransducerCompare(transducer,differentTransducer));
    iuf3DTransducerDelete(differentTransducer);

    differentTransducer = iuf3DTransducerCreate(diffTransducerName, shape, centerFrequency, numTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU3DT_INVALID,identicalTransducer);
    TEST_ASSERT_EQUAL(IUF_FALSE,iuf3DTransducerCompare(transducer,differentTransducer));
    iuf3DTransducerDelete(differentTransducer);

    differentTransducer = iuf3DTransducerCreate(transducerName, diffShape, centerFrequency, numTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU3DT_INVALID,identicalTransducer);
    TEST_ASSERT_EQUAL(IUF_FALSE,iuf3DTransducerCompare(transducer,differentTransducer));
    iuf3DTransducerDelete(differentTransducer);

    differentTransducer = iuf3DTransducerCreate(transducerName, shape, diffCenterFrequency, numTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU3DT_INVALID,identicalTransducer);
    TEST_ASSERT_EQUAL(IUF_FALSE,iuf3DTransducerCompare(transducer,differentTransducer));
    iuf3DTransducerDelete(differentTransducer);

    differentTransducer = iuf3DTransducerCreate(transducerName, shape, centerFrequency, diffNumTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU3DT_INVALID,identicalTransducer);
    TEST_ASSERT_EQUAL(IUF_FALSE,iuf3DTransducerCompare(transducer,differentTransducer));
    iuf3DTransducerDelete(differentTransducer);

    // invalid params
    TEST_ASSERT_EQUAL(IU3DT_INVALID,iuf3DTransducerCompare(transducer,NULL));
    TEST_ASSERT_EQUAL(IU3DT_INVALID,iuf3DTransducerCompare(NULL,transducer));

    status = iuf3DTransducerDelete(transducer);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);
    status = iuf3DTransducerDelete(identicalTransducer);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);
    iuf3DTransducerElementDelete(element);
    iuf3DPositionDelete(elemPos);
    iuf3DSizeDelete(elemSize);
    iuf3DAngleDelete(elemAngle);
}

TEST(Iuf3DTransducer, testIuf3DTransducerSetGet)
{
    char *transducerName = "S5-1";
    float centerFrequency = 2500000.0f;
    const int numTransducerElements = 128;
    const float transducerPitch = 0.000005f;
    iu3dp_t elemPos = iuf3DPositionCreate((float)(10 - numTransducerElements / 2.0)*transducerPitch, 0.0f, 0.0f);
    iu3ds_t elemSize = iuf3DSizeCreate(0.0001f,0.0001f,0.0001f);
    iu3da_t elemAngle = iuf3DAngleCreate(0.0f,0.3f);


    iu3dt_t transducer;
    IufTransducerShape shape = IUF_PLANE;

    transducer = iuf3DTransducerCreate(transducerName, shape, centerFrequency, numTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU3DT_INVALID,transducer);

    iu3dte_t element = iuf3DTransducerElementCreate(elemPos, elemAngle, elemSize);
    TEST_ASSERT_NOT_EQUAL(IU3DTE_INVALID,element);

    int status = iuf3DTransducerSetElement(transducer,0,element);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);
    TEST_ASSERT_EQUAL(IUF_TRUE,iuf3DTransducerElementCompare(element,iuf3DTransducerGetElement(transducer,0)));


    // invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    status = iuf3DTransducerSetElement(transducer,numTransducerElements,element);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE,status);
    status = iuf3DTransducerSetElement(transducer,-1,element);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE,status);
    status = iuf3DTransducerSetElement(NULL,0,element);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE,status);
    status = iuf3DTransducerSetElement(transducer,0,NULL);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE,status);

    TEST_ASSERT_EQUAL(4,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));


    iuf3DTransducerDelete(transducer);
    iuf3DTransducerElementDelete(element);
    iuf3DPositionDelete(elemPos);
    iuf3DSizeDelete(elemSize);
    iuf3DAngleDelete(elemAngle);
}

TEST(Iuf3DTransducer, testIuf3DTransducerSerialization)

{
	int status = 0;
	char *filename = "testIuf3DTransducerSerialization.hdf5";
	char *transducerName = "S5-1";
	const int numTransducerElements = 128;
	int i = 0;

	// create and fill
	const float transducerPitch = 0.000005f;

	iu3dt_t transducer = iuf3DTransducerCreate(transducerName, IUF_PLANE, 2500000.0f, numTransducerElements);
    TEST_ASSERT(transducer != IU3DT_INVALID);
	for (i = 0; i < numTransducerElements; i++)
	{
        iu3dp_t elemPos = iuf3DPositionCreate((float)(10 - numTransducerElements / 2.0)*transducerPitch, 0.0f, 0.0f);
        iu3ds_t elemSize = iuf3DSizeCreate(0.0001f,0.0001f,0.0001f);
        iu3da_t elemAngle = iuf3DAngleCreate(0.0f,0.3f);
		iu3dte_t element = iuf3DTransducerElementCreate(elemPos, elemAngle, elemSize);
		iuf3DTransducerSetElement(transducer, i, element);
	}

	// save
	hid_t handle = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
	TEST_ASSERT(handle > 0);
	status = iuf3DTransducerSave(transducer, handle);
	H5Fclose(handle);
	TEST_ASSERT_EQUAL(IUF_E_OK, status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iu3dt_t savedObj = iuf3DTransducerLoad(handle);
    TEST_ASSERT_NOT_EQUAL(NULL, savedObj);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUF_TRUE, iuf3DTransducerCompare(transducer,savedObj));

	// cleanup
    for (i = 0; i < numTransducerElements; i++)
    {
        iu3dte_t element = iuf3DTransducerGetElement(transducer, i);
        iu3dp_t elemPos = iuf3DTransducerElementGetPosition(element);
        iu3ds_t elemSize = iuf3DTransducerElementGetSize(element);
        iu3da_t elemAngle = iuf3DTransducerElementGetAngle(element);
        iuf3DTransducerElementDelete(element);
        iuf3DPositionDelete(elemPos);
        iuf3DSizeDelete(elemSize);
        iuf3DAngleDelete(elemAngle);
    }
    status = iuf3DTransducerDelete(transducer);
    status |= iuf3DTransducerDelete(savedObj);
    TEST_ASSERT(status == IUF_E_OK);

}



TEST_GROUP_RUNNER(Iuf3DTransducer)
{
    RUN_TEST_CASE(Iuf3DTransducer, testIuf3DTransducerCreate);
    RUN_TEST_CASE(Iuf3DTransducer, testIuf3DTransducerDelete);
    RUN_TEST_CASE(Iuf3DTransducer, testIufHL3DTransducerCompare);
    RUN_TEST_CASE(Iuf3DTransducer, testIuf3DTransducerSetGet);
	RUN_TEST_CASE(Iuf3DTransducer, testIuf3DTransducerSerialization);
}
