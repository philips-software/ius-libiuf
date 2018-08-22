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
#include <include/iusHL3DTransducerImp.h>

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
    transducer = iusHL3DTransducerCreate(transducerName, shape, centerFrequency, numTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU3DT_INVALID,transducer);
    int status = iusHL3DTransducerDelete(transducer);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);


    //invalid params
    transducer = iusHL3DTransducerCreate(NULL, shape, centerFrequency, numTransducerElements);
    TEST_ASSERT_EQUAL(IU3DT_INVALID,transducer);
    transducer = iusHL3DTransducerCreate(transducerName, IUS_INVALID_TRANSDUCER_SHAPE, centerFrequency, numTransducerElements);
    TEST_ASSERT_EQUAL(IU3DT_INVALID,transducer);
    transducer = iusHL3DTransducerCreate(transducerName, IUS_LINE, centerFrequency, numTransducerElements);
    TEST_ASSERT_EQUAL(IU3DT_INVALID,transducer);
    transducer = iusHL3DTransducerCreate(transducerName, IUS_CIRCLE, centerFrequency, numTransducerElements);
    TEST_ASSERT_EQUAL(IU3DT_INVALID,transducer);
    transducer = iusHL3DTransducerCreate(transducerName, shape, NAN, numTransducerElements);
    TEST_ASSERT_EQUAL(IU3DT_INVALID,transducer);
    transducer = iusHL3DTransducerCreate(transducerName, shape, centerFrequency, -1);
    TEST_ASSERT_EQUAL(IU3DT_INVALID,transducer);

}

TEST(Ius3DTransducer, testIus3DTransducerDelete)
{
    char *transducerName = "S5-1";
    float centerFrequency = 2500000.0f;
    const int numTransducerElements = 128;
    IusTransducerShape shape = IUS_CYLINDER;
    iu3dt_t transducer;
    transducer = iusHL3DTransducerCreate(transducerName, shape, centerFrequency, numTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU3DT_INVALID,transducer);
    int status = iusHL3DTransducerDelete(transducer);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);

    // invalid params
    status = iusHL3DTransducerDelete(NULL);
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
    iu3dp_t elemPos = iusHL3DPositionCreate((10 - numTransducerElements / 2)*transducerPitch, 0.0f, 0.0f);
    iu3ds_t elemSize = iusHL3DSizeCreate(0.0001f,0.0001f,0.0001f);
    iu3da_t elemAngle = iusHL3DAngleCreate(0.0f,0.3f);

    iu3dte_t element = iusHL3DTransducerElementCreate(elemPos, elemAngle, elemSize);
    TEST_ASSERT_NOT_EQUAL(IU3DTE_INVALID,element);

    IusTransducerShape shape = IUS_PLANE;
    IusTransducerShape diffShape = IUS_CYLINDER;

    iu3dt_t transducer;
    iu3dt_t identicalTransducer;
    transducer = iusHL3DTransducerCreate(transducerName, shape, centerFrequency, numTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU3DT_INVALID,transducer);
    identicalTransducer = iusHL3DTransducerCreate(transducerName, shape, centerFrequency, numTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU3DT_INVALID,identicalTransducer);

    TEST_ASSERT_EQUAL(IUS_TRUE,iusHL3DTransducerCompare(transducer,transducer));
    TEST_ASSERT_EQUAL(IUS_TRUE,iusHL3DTransducerCompare(transducer,identicalTransducer));

    iu3dt_t differentTransducer = iusHL3DTransducerCreate(transducerName, shape, centerFrequency, numTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU3DT_INVALID,identicalTransducer);
    int status = iusHL3DTransducerSetElement(differentTransducer,0,element);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    TEST_ASSERT_EQUAL(IUS_FALSE,iusHL3DTransducerCompare(transducer,differentTransducer));
    iusHL3DTransducerDelete(differentTransducer);

    differentTransducer = iusHL3DTransducerCreate(diffTransducerName, shape, centerFrequency, numTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU3DT_INVALID,identicalTransducer);
    TEST_ASSERT_EQUAL(IUS_FALSE,iusHL3DTransducerCompare(transducer,differentTransducer));
    iusHL3DTransducerDelete(differentTransducer);

    differentTransducer = iusHL3DTransducerCreate(transducerName, diffShape, centerFrequency, numTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU3DT_INVALID,identicalTransducer);
    TEST_ASSERT_EQUAL(IUS_FALSE,iusHL3DTransducerCompare(transducer,differentTransducer));
    iusHL3DTransducerDelete(differentTransducer);

    differentTransducer = iusHL3DTransducerCreate(transducerName, shape, diffCenterFrequency, numTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU3DT_INVALID,identicalTransducer);
    TEST_ASSERT_EQUAL(IUS_FALSE,iusHL3DTransducerCompare(transducer,differentTransducer));
    iusHL3DTransducerDelete(differentTransducer);

    differentTransducer = iusHL3DTransducerCreate(transducerName, shape, centerFrequency, diffNumTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU3DT_INVALID,identicalTransducer);
    TEST_ASSERT_EQUAL(IUS_FALSE,iusHL3DTransducerCompare(transducer,differentTransducer));
    iusHL3DTransducerDelete(differentTransducer);

    // invalid params
    TEST_ASSERT_EQUAL(IU3DT_INVALID,iusHL3DTransducerCompare(transducer,NULL));
    TEST_ASSERT_EQUAL(IU3DT_INVALID,iusHL3DTransducerCompare(NULL,transducer));

    status = iusHL3DTransducerDelete(transducer);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);
    status = iusHL3DTransducerDelete(identicalTransducer);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);

}

TEST(Ius3DTransducer, testIus3DTransducerSetGet)
{
    char *transducerName = "S5-1";
    float centerFrequency = 2500000.0f;
    const int numTransducerElements = 128;
    const float transducerPitch = 0.000005f;
    iu3dp_t elemPos = iusHL3DPositionCreate((10 - numTransducerElements / 2)*transducerPitch, 0.0f, 0.0f);
    iu3ds_t elemSize = iusHL3DSizeCreate(0.0001f,0.0001f,0.0001f);
    iu3da_t elemAngle = iusHL3DAngleCreate(0.0f,0.3f);


    iu3dt_t transducer;
    IusTransducerShape shape = IUS_PLANE;

    transducer = iusHL3DTransducerCreate(transducerName, shape, centerFrequency, numTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU3DT_INVALID,transducer);

    iu3dte_t element = iusHL3DTransducerElementCreate(elemPos, elemAngle, elemSize);
    TEST_ASSERT_NOT_EQUAL(IU3DTE_INVALID,element);

    int status = iusHL3DTransducerSetElement(transducer,0,element);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    TEST_ASSERT_EQUAL(IUS_TRUE,iusHL3DTransducerElementCompare(element,iusHL3DTransducerGetElement(transducer,0)));


    // invalid params
    status = iusHL3DTransducerSetElement(transducer,numTransducerElements,element);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE,status);
    status = iusHL3DTransducerSetElement(transducer,-1,element);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE,status);
    status = iusHL3DTransducerSetElement(NULL,0,element);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE,status);
    status = iusHL3DTransducerSetElement(transducer,0,NULL);
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

	iu3dt_t transducer = iusHL3DTransducerCreate(transducerName, IUS_PLANE, 2500000.0f, numTransducerElements);
    TEST_ASSERT(transducer != IU3DT_INVALID);
	for (i = 0; i < numTransducerElements; i++)
	{
        iu3dp_t elemPos = iusHL3DPositionCreate((10 - numTransducerElements / 2)*transducerPitch, 0.0f, 0.0f);
        iu3ds_t elemSize = iusHL3DSizeCreate(0.0001f,0.0001f,0.0001f);
        iu3da_t elemAngle = iusHL3DAngleCreate(0.0f,0.3f);
		iu3dte_t element = iusHL3DTransducerElementCreate(elemPos, elemAngle, elemSize);
		iusHL3DTransducerSetElement(transducer, i, element);
	}

	// save
	hid_t handle = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
	TEST_ASSERT(handle > 0);
	status = iusHL3DTransducerSave(transducer, path, handle);
	H5Fclose(handle);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iu3dt_t savedObj = iusHL3DTransducerLoad(handle, path);
    TEST_ASSERT_NOT_EQUAL(NULL, savedObj);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUS_TRUE, iusHL3DTransducerCompare(transducer,savedObj));
    status = iusHL3DTransducerDelete(transducer);
    status |= iusHL3DTransducerDelete(savedObj);
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
