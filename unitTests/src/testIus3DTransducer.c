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
#include <include/ius3DTransducerImp.h>

TEST_GROUP(Ius3DTransducer);

TEST_SETUP(Ius3DTransducer)
{
}

TEST_TEAR_DOWN(Ius3DTransducer)
{
}


TEST(Ius3DTransducer, testIus3DTransducerCreate)
{
	char *transducerName = "S5-1";
	float centerFrequency = 2500000.0f;
	const int numTransducerElements = 128;
    IusTransducerShape shape = IUS_SPHERE;
    iu3dt_t transducer;
    transducer = ius3DTransducerCreate(transducerName, shape, centerFrequency, numTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU3DT_INVALID,transducer);
    int status = ius3DTransducerDelete(transducer);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);


    //invalid params
    transducer = ius3DTransducerCreate(NULL, shape, centerFrequency, numTransducerElements);
    TEST_ASSERT_EQUAL(IU3DT_INVALID,transducer);
    transducer = ius3DTransducerCreate(transducerName, IUS_INVALID_TRANSDUCER_SHAPE, centerFrequency, numTransducerElements);
    TEST_ASSERT_EQUAL(IU3DT_INVALID,transducer);
    transducer = ius3DTransducerCreate(transducerName, IUS_LINE, centerFrequency, numTransducerElements);
    TEST_ASSERT_EQUAL(IU3DT_INVALID,transducer);
    transducer = ius3DTransducerCreate(transducerName, IUS_CIRCLE, centerFrequency, numTransducerElements);
    TEST_ASSERT_EQUAL(IU3DT_INVALID,transducer);
    transducer = ius3DTransducerCreate(transducerName, shape, NAN, numTransducerElements);
    TEST_ASSERT_EQUAL(IU3DT_INVALID,transducer);
    transducer = ius3DTransducerCreate(transducerName, shape, centerFrequency, -1);
    TEST_ASSERT_EQUAL(IU3DT_INVALID,transducer);

}

TEST(Ius3DTransducer, testIus3DTransducerDelete)
{
    char *transducerName = "S5-1";
    float centerFrequency = 2500000.0f;
    const int numTransducerElements = 128;
    IusTransducerShape shape = IUS_CYLINDER;
    iu3dt_t transducer;
    transducer = ius3DTransducerCreate(transducerName, shape, centerFrequency, numTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU3DT_INVALID,transducer);
    int status = ius3DTransducerDelete(transducer);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);

    // invalid params
    status = ius3DTransducerDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
}


TEST(Ius3DTransducer,  testIusHL3DTransducerCompare)
{
    char *transducerName = "S5-1";
    char *diffTransducerName = "S5-2";
    float centerFrequency = 2500000.0f;
    float diffCenterFrequency = 2600000.0f;
    const int numTransducerElements = 128;
    const int diffNumTransducerElements = 256;
    const float transducerPitch = 0.000005f;
    iu3dp_t elemPos = ius3DPositionCreate((10 - numTransducerElements / 2)*transducerPitch, 0.0f, 0.0f);
    iu3ds_t elemSize = ius3DSizeCreate(0.0001f,0.0001f,0.0001f);
    iu3da_t elemAngle = ius3DAngleCreate(0.0f,0.3f);

    iu3dte_t element = ius3DTransducerElementCreate(elemPos, elemAngle, elemSize);
    TEST_ASSERT_NOT_EQUAL(IU3DTE_INVALID,element);

    IusTransducerShape shape = IUS_PLANE;
    IusTransducerShape diffShape = IUS_CYLINDER;

    iu3dt_t transducer;
    iu3dt_t identicalTransducer;
    transducer = ius3DTransducerCreate(transducerName, shape, centerFrequency, numTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU3DT_INVALID,transducer);
    identicalTransducer = ius3DTransducerCreate(transducerName, shape, centerFrequency, numTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU3DT_INVALID,identicalTransducer);

    TEST_ASSERT_EQUAL(IUS_TRUE,ius3DTransducerCompare(transducer,transducer));
    TEST_ASSERT_EQUAL(IUS_TRUE,ius3DTransducerCompare(transducer,identicalTransducer));

    iu3dt_t differentTransducer = ius3DTransducerCreate(transducerName, shape, centerFrequency, numTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU3DT_INVALID,identicalTransducer);
    int status = ius3DTransducerSetElement(differentTransducer,0,element);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    TEST_ASSERT_EQUAL(IUS_FALSE,ius3DTransducerCompare(transducer,differentTransducer));
    ius3DTransducerDelete(differentTransducer);

    differentTransducer = ius3DTransducerCreate(diffTransducerName, shape, centerFrequency, numTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU3DT_INVALID,identicalTransducer);
    TEST_ASSERT_EQUAL(IUS_FALSE,ius3DTransducerCompare(transducer,differentTransducer));
    ius3DTransducerDelete(differentTransducer);

    differentTransducer = ius3DTransducerCreate(transducerName, diffShape, centerFrequency, numTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU3DT_INVALID,identicalTransducer);
    TEST_ASSERT_EQUAL(IUS_FALSE,ius3DTransducerCompare(transducer,differentTransducer));
    ius3DTransducerDelete(differentTransducer);

    differentTransducer = ius3DTransducerCreate(transducerName, shape, diffCenterFrequency, numTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU3DT_INVALID,identicalTransducer);
    TEST_ASSERT_EQUAL(IUS_FALSE,ius3DTransducerCompare(transducer,differentTransducer));
    ius3DTransducerDelete(differentTransducer);

    differentTransducer = ius3DTransducerCreate(transducerName, shape, centerFrequency, diffNumTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU3DT_INVALID,identicalTransducer);
    TEST_ASSERT_EQUAL(IUS_FALSE,ius3DTransducerCompare(transducer,differentTransducer));
    ius3DTransducerDelete(differentTransducer);

    // invalid params
    TEST_ASSERT_EQUAL(IU3DT_INVALID,ius3DTransducerCompare(transducer,NULL));
    TEST_ASSERT_EQUAL(IU3DT_INVALID,ius3DTransducerCompare(NULL,transducer));

    status = ius3DTransducerDelete(transducer);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);
    status = ius3DTransducerDelete(identicalTransducer);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);

}

TEST(Ius3DTransducer, testIus3DTransducerSetGet)
{
    char *transducerName = "S5-1";
    float centerFrequency = 2500000.0f;
    const int numTransducerElements = 128;
    const float transducerPitch = 0.000005f;
    iu3dp_t elemPos = ius3DPositionCreate((10 - numTransducerElements / 2)*transducerPitch, 0.0f, 0.0f);
    iu3ds_t elemSize = ius3DSizeCreate(0.0001f,0.0001f,0.0001f);
    iu3da_t elemAngle = ius3DAngleCreate(0.0f,0.3f);


    iu3dt_t transducer;
    IusTransducerShape shape = IUS_PLANE;

    transducer = ius3DTransducerCreate(transducerName, shape, centerFrequency, numTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU3DT_INVALID,transducer);

    iu3dte_t element = ius3DTransducerElementCreate(elemPos, elemAngle, elemSize);
    TEST_ASSERT_NOT_EQUAL(IU3DTE_INVALID,element);

    int status = ius3DTransducerSetElement(transducer,0,element);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    TEST_ASSERT_EQUAL(IUS_TRUE,ius3DTransducerElementCompare(element,ius3DTransducerGetElement(transducer,0)));


    // invalid params
    status = ius3DTransducerSetElement(transducer,numTransducerElements,element);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE,status);
    status = ius3DTransducerSetElement(transducer,-1,element);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE,status);
    status = ius3DTransducerSetElement(NULL,0,element);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE,status);
    status = ius3DTransducerSetElement(transducer,0,NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE,status);

}

TEST(Ius3DTransducer, testIus3DTransducerSerialization)

{
	int status = 0;
	char *filename = "testIus3DTransducerSerialization.hdf5";
	char *path = "/Transducer";
	char *transducerName = "S5-1";
	const int numTransducerElements = 128;
	int i = 0;

	// create and fill
	const float transducerPitch = 0.000005f;

	iu3dt_t transducer = ius3DTransducerCreate(transducerName, IUS_PLANE, 2500000.0f, numTransducerElements);
    TEST_ASSERT(transducer != IU3DT_INVALID);
	for (i = 0; i < numTransducerElements; i++)
	{
        iu3dp_t elemPos = ius3DPositionCreate((10 - numTransducerElements / 2)*transducerPitch, 0.0f, 0.0f);
        iu3ds_t elemSize = ius3DSizeCreate(0.0001f,0.0001f,0.0001f);
        iu3da_t elemAngle = ius3DAngleCreate(0.0f,0.3f);
		iu3dte_t element = ius3DTransducerElementCreate(elemPos, elemAngle, elemSize);
		ius3DTransducerSetElement(transducer, i, element);
	}

	// save
	hid_t handle = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
	TEST_ASSERT(handle > 0);
	status = ius3DTransducerSave(transducer, path, handle);
	H5Fclose(handle);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iu3dt_t savedObj = ius3DTransducerLoad(handle, path);
    TEST_ASSERT_NOT_EQUAL(NULL, savedObj);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUS_TRUE, ius3DTransducerCompare(transducer,savedObj));
    status = ius3DTransducerDelete(transducer);
    status |= ius3DTransducerDelete(savedObj);
	TEST_ASSERT(status == IUS_E_OK);
}



TEST_GROUP_RUNNER(Ius3DTransducer)
{
    RUN_TEST_CASE(Ius3DTransducer, testIus3DTransducerCreate);
    RUN_TEST_CASE(Ius3DTransducer, testIus3DTransducerDelete);
    RUN_TEST_CASE(Ius3DTransducer, testIusHL3DTransducerCompare);
    RUN_TEST_CASE(Ius3DTransducer, testIus3DTransducerSetGet);
	RUN_TEST_CASE(Ius3DTransducer, testIus3DTransducerSerialization);
}
