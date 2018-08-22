//
// Created by nlv09165 on 22/08/2018.
//
#include <util.h>
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <hdf5.h>
#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <include/iusHL2DTransducerImp.h>

TEST_GROUP(Ius2DTransducer);

TEST_SETUP(Ius2DTransducer)
{
}

TEST_TEAR_DOWN(Ius2DTransducer)
{
}


TEST(Ius2DTransducer, testIus2DTransducerCreate)
{
	char *transducerName = "S5-1";
	float centerFrequency = 2500000.0f;
	const int numTransducerElements = 128;
    IusTransducerShape shape = IUS_CIRCLE;
    iu2dt_t transducer;
    transducer = iusHL2DTransducerCreate(transducerName, shape, centerFrequency, numTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU2DT_INVALID,transducer);
    int status = iusHL2DTransducerDelete(transducer);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);


    //invalid params
    transducer = iusHL2DTransducerCreate(NULL, shape, centerFrequency, numTransducerElements);
    TEST_ASSERT_EQUAL(IU2DT_INVALID,transducer);
    transducer = iusHL2DTransducerCreate(transducerName, IUS_INVALID_TRANSDUCER_SHAPE, centerFrequency, numTransducerElements);
    TEST_ASSERT_EQUAL(IU2DT_INVALID,transducer);
    transducer = iusHL2DTransducerCreate(transducerName, IUS_CYLINDER, centerFrequency, numTransducerElements);
    TEST_ASSERT_EQUAL(IU2DT_INVALID,transducer);
    transducer = iusHL2DTransducerCreate(transducerName, IUS_PLANE, centerFrequency, numTransducerElements);
    TEST_ASSERT_EQUAL(IU2DT_INVALID,transducer);
    transducer = iusHL2DTransducerCreate(transducerName, IUS_SPHERE, centerFrequency, numTransducerElements);
    TEST_ASSERT_EQUAL(IU2DT_INVALID,transducer);
    transducer = iusHL2DTransducerCreate(transducerName, shape, NAN, numTransducerElements);
    TEST_ASSERT_EQUAL(IU2DT_INVALID,transducer);
    transducer = iusHL2DTransducerCreate(transducerName, shape, centerFrequency, -1);
    TEST_ASSERT_EQUAL(IU2DT_INVALID,transducer);

}

TEST(Ius2DTransducer, testIus2DTransducerDelete)
{
    char *transducerName = "S5-1";
    float centerFrequency = 2500000.0f;
    const int numTransducerElements = 128;
    IusTransducerShape shape = IUS_CIRCLE;
    iu2dt_t transducer;
    transducer = iusHL2DTransducerCreate(transducerName, shape, centerFrequency, numTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU2DT_INVALID,transducer);
    int status = iusHL2DTransducerDelete(transducer);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);

    // invalid params
    status = iusHL2DTransducerDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
}


TEST(Ius2DTransducer,  testIusHL2DTransducerCompare)
{
    char *transducerName = "S5-1";
    char *diffTransducerName = "S5-2";
    float centerFrequency = 2500000.0f;
    float diffCenterFrequency = 2600000.0f;
    const int numTransducerElements = 128;
    const int diffNumTransducerElements = 256;
    const float transducerPitch = 0.000005f;
    iu2dp_t elemPos = iusHL2DPositionCreate((10 - numTransducerElements / 2)*transducerPitch, 0.0f);
    iu2ds_t elemSize = iusHL2DSizeCreate(0.0001f,0.0001f);
    float elemAngle = 0.5f;

    iu2dte_t element = iusHL2DTransducerElementCreate(elemPos, elemAngle, elemSize);
    TEST_ASSERT_NOT_EQUAL(IU2DTE_INVALID,element);

    IusTransducerShape shape = IUS_LINE;
    IusTransducerShape diffShape = IUS_CIRCLE;

    iu2dt_t transducer;
    iu2dt_t identicalTransducer;
    transducer = iusHL2DTransducerCreate(transducerName, shape, centerFrequency, numTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU2DT_INVALID,transducer);
    identicalTransducer = iusHL2DTransducerCreate(transducerName, shape, centerFrequency, numTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU2DT_INVALID,identicalTransducer);

    TEST_ASSERT_EQUAL(IUS_TRUE,iusHL2DTransducerCompare(transducer,transducer));
    TEST_ASSERT_EQUAL(IUS_TRUE,iusHL2DTransducerCompare(transducer,identicalTransducer));

    iu2dt_t differentTransducer = iusHL2DTransducerCreate(transducerName, shape, centerFrequency, numTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU2DT_INVALID,identicalTransducer);
    int status = iusHL2DTransducerSetElement(differentTransducer,0,element);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    TEST_ASSERT_EQUAL(IUS_FALSE,iusHL2DTransducerCompare(transducer,differentTransducer));
    iusHL2DTransducerDelete(differentTransducer);

    differentTransducer = iusHL2DTransducerCreate(diffTransducerName, shape, centerFrequency, numTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU2DT_INVALID,identicalTransducer);
    TEST_ASSERT_EQUAL(IUS_FALSE,iusHL2DTransducerCompare(transducer,differentTransducer));
    iusHL2DTransducerDelete(differentTransducer);

    differentTransducer = iusHL2DTransducerCreate(transducerName, diffShape, centerFrequency, numTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU2DT_INVALID,identicalTransducer);
    TEST_ASSERT_EQUAL(IUS_FALSE,iusHL2DTransducerCompare(transducer,differentTransducer));
    iusHL2DTransducerDelete(differentTransducer);

    differentTransducer = iusHL2DTransducerCreate(transducerName, shape, diffCenterFrequency, numTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU2DT_INVALID,identicalTransducer);
    TEST_ASSERT_EQUAL(IUS_FALSE,iusHL2DTransducerCompare(transducer,differentTransducer));
    iusHL2DTransducerDelete(differentTransducer);

    differentTransducer = iusHL2DTransducerCreate(transducerName, shape, centerFrequency, diffNumTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU2DT_INVALID,identicalTransducer);
    TEST_ASSERT_EQUAL(IUS_FALSE,iusHL2DTransducerCompare(transducer,differentTransducer));
    iusHL2DTransducerDelete(differentTransducer);

    // invalid params
    TEST_ASSERT_EQUAL(IU2DT_INVALID,iusHL2DTransducerCompare(transducer,NULL));
    TEST_ASSERT_EQUAL(IU2DT_INVALID,iusHL2DTransducerCompare(NULL,transducer));

    status = iusHL2DTransducerDelete(transducer);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);
    status = iusHL2DTransducerDelete(identicalTransducer);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);

}

TEST(Ius2DTransducer, testIus2DTransducerSetGet)
{
    char *transducerName = "S5-1";
    float centerFrequency = 2500000.0f;
    const int numTransducerElements = 128;
    const float transducerPitch = 0.000005f;
    iu2dp_t elemPos = iusHL2DPositionCreate((10 - numTransducerElements / 2)*transducerPitch, 0.0f);
    iu2ds_t elemSize = iusHL2DSizeCreate(0.0001f,0.0001f);
    float elemAngle = 0.5f;


    iu2dt_t transducer;
    IusTransducerShape shape = IUS_LINE;

    transducer = iusHL2DTransducerCreate(transducerName, shape, centerFrequency, numTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU2DT_INVALID,transducer);

    iu2dte_t element = iusHL2DTransducerElementCreate(elemPos, elemAngle, elemSize);
    TEST_ASSERT_NOT_EQUAL(IU2DTE_INVALID,element);

    int status = iusHL2DTransducerSetElement(transducer,0,element);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    TEST_ASSERT_EQUAL(IUS_TRUE,iusHL2DTransducerElementCompare(element,iusHL2DTransducerGetElement(transducer,0)));


    // invalid params
    status = iusHL2DTransducerSetElement(transducer,numTransducerElements,element);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE,status);
    status = iusHL2DTransducerSetElement(transducer,-1,element);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE,status);
    status = iusHL2DTransducerSetElement(NULL,0,element);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE,status);
    status = iusHL2DTransducerSetElement(transducer,0,NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE,status);

}

TEST(Ius2DTransducer, testIus2DTransducerSerialization)

{
	int status = 0;
	char *filename = "testIus2DTransducerSerialization.hdf5";
	char *path = "/Transducer";
	char *transducerName = "S5-1";
	const int numTransducerElements = 128;
	int i = 0;

	// create and fill
	const float transducerPitch = 0.000005f;

	iu2dt_t transducer = iusHL2DTransducerCreate(transducerName, IUS_LINE, 2500000.0f, numTransducerElements);
    TEST_ASSERT(transducer != IU2DT_INVALID);
	for (i = 0; i < numTransducerElements; i++)
	{
        iu2dp_t elemPos = iusHL2DPositionCreate((10 - numTransducerElements / 2)*transducerPitch, 0.0f);
        iu2ds_t elemSize = iusHL2DSizeCreate(0.0001f,0.0001f);
        float elemAngle = 0.5f;
		iu2dte_t element = iusHL2DTransducerElementCreate(elemPos, elemAngle, elemSize);
		iusHL2DTransducerSetElement(transducer, i, element);
	}

	// save
	hid_t handle = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
	TEST_ASSERT(handle > 0);
	status = iusHL2DTransducerSave(transducer, path, handle);
	H5Fclose(handle);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iu2dt_t savedObj = iusHL2DTransducerLoad(handle, path);
    TEST_ASSERT_NOT_EQUAL(NULL, savedObj);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUS_TRUE, iusHL2DTransducerCompare(transducer,savedObj));
    status = iusHL2DTransducerDelete(transducer);
    status |= iusHL2DTransducerDelete(savedObj);
	TEST_ASSERT(status == IUS_E_OK);
}



TEST_GROUP_RUNNER(Ius2DTransducer)
{
    RUN_TEST_CASE(Ius2DTransducer, testIus2DTransducerCreate);
    RUN_TEST_CASE(Ius2DTransducer, testIus2DTransducerDelete);
    RUN_TEST_CASE(Ius2DTransducer, testIusHL2DTransducerCompare);
    RUN_TEST_CASE(Ius2DTransducer, testIus2DTransducerSetGet);
	RUN_TEST_CASE(Ius2DTransducer, testIus2DTransducerSerialization);
}
