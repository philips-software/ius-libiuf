

//
// Created by nlv09165 on 20/08/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <ius.h>
#include <ius3DTransducerElementPrivate.h>

TEST_GROUP(Ius3DTransducerElement);

TEST_SETUP(Ius3DTransducerElement)
{
}

TEST_TEAR_DOWN(Ius3DTransducerElement)
{
}


TEST(Ius3DTransducerElement, testIus3DTransducerElementCreate)
{
    const float transducerPitch = 0.000005f;
    const int numTransducerElements = 128;
    iu3dp_t elemPos = ius3DPositionCreate((float)(10 - numTransducerElements / 2.0)*transducerPitch, 0.0f, 0.0f);
    iu3ds_t elemSize = ius3DSizeCreate(0.0001f,0.0001f,0.0001f);
    iu3da_t elemAngle = ius3DAngleCreate(0.0f,0.3f);
    iu3dte_t element = ius3DTransducerElementCreate(elemPos, elemAngle, elemSize);
    TEST_ASSERT_NOT_EQUAL(IU3DTE_INVALID,element);

    //invalid params
    iu3dte_t nothereElement = ius3DTransducerElementCreate(elemPos, elemAngle, NULL);
    TEST_ASSERT_EQUAL(IU3DTE_INVALID,nothereElement);
    nothereElement = ius3DTransducerElementCreate(elemPos, NULL, elemSize);
    TEST_ASSERT_EQUAL(IU3DTE_INVALID,nothereElement);
    nothereElement = ius3DTransducerElementCreate(NULL, elemAngle, elemSize);
    TEST_ASSERT_EQUAL(IU3DTE_INVALID,nothereElement);

    int status = ius3DTransducerElementDelete(element);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
}


TEST(Ius3DTransducerElement, testIus3DTransducerElementDelete)
{
    const float transducerPitch = 0.000005f;
    const int numTransducerElements = 128;
    iu3dp_t elemPos = ius3DPositionCreate((float)(10 - numTransducerElements / 2.0)*transducerPitch, 0.0f, 0.0f);
    iu3ds_t elemSize = ius3DSizeCreate(0.0001f,0.0001f,0.0001f);
    iu3da_t elemAngle = ius3DAngleCreate(0.0f,0.3f);
    iu3dte_t element = ius3DTransducerElementCreate(elemPos, elemAngle, elemSize);
    TEST_ASSERT_NOT_EQUAL(IU3DTE_INVALID,element);

    int status = ius3DTransducerElementDelete(element);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // invalid params
    status = ius3DTransducerElementDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE,status);
    ius3DPositionDelete(elemPos);
    ius3DAngleDelete(elemAngle);
    ius3DSizeDelete(elemSize);
}

TEST(Ius3DTransducerElement, testIus3DTransducerElementCompare)
{
    IUS_BOOL equal;
    const float transducerPitch = 0.000005f;
    const int numTransducerElements = 128;
    iu3dp_t elemPos = ius3DPositionCreate((float)(10 - numTransducerElements / 2.0)*transducerPitch, 0.0f, 0.0f);
    iu3dp_t diffElemPos = ius3DPositionCreate((float)(19 - numTransducerElements / 2.0)*transducerPitch, 0.1f, 0.0f);

    iu3ds_t elemSize = ius3DSizeCreate(0.0001f,0.0001f,0.0001f);
    iu3ds_t diffElemSize = ius3DSizeCreate(0.0002f,0.0001f,0.0001f);

    iu3da_t elemAngle = ius3DAngleCreate(0.0f,0.3f);
    iu3da_t diffElemAngle = ius3DAngleCreate(0.1f,0.3f);

    iu3dte_t element = ius3DTransducerElementCreate(elemPos, elemAngle, elemSize);
    TEST_ASSERT_NOT_EQUAL(IU3DTE_INVALID,element);

    iu3dte_t duplicateElement = ius3DTransducerElementCreate(elemPos, elemAngle, elemSize);
    TEST_ASSERT_NOT_EQUAL(IU3DTE_INVALID,element);

    equal = ius3DTransducerElementCompare(element,element);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);

    equal = ius3DTransducerElementCompare(element,duplicateElement);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);

    iu3dte_t diffElement = ius3DTransducerElementCreate(elemPos, elemAngle, diffElemSize);
    equal = ius3DTransducerElementCompare(element,diffElement);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    ius3DTransducerElementDelete(diffElement);

    diffElement = ius3DTransducerElementCreate(elemPos, diffElemAngle, elemSize);
    equal = ius3DTransducerElementCompare(element,diffElement);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    ius3DTransducerElementDelete(diffElement);

    diffElement = ius3DTransducerElementCreate(diffElemPos, elemAngle, elemSize);
    equal = ius3DTransducerElementCompare(element,diffElement);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    ius3DTransducerElementDelete(diffElement);

    // invalid params
    equal = ius3DTransducerElementCompare(element,NULL);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    equal = ius3DTransducerElementCompare(NULL,element);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);


    ius3DTransducerElementDelete(element);
    ius3DTransducerElementDelete(duplicateElement);
    ius3DPositionDelete(elemPos);
    ius3DAngleDelete(elemAngle);
    ius3DSizeDelete(elemSize);
    ius3DPositionDelete(diffElemPos);
    ius3DAngleDelete(diffElemAngle);
    ius3DSizeDelete(diffElemSize);
}


TEST(Ius3DTransducerElement, testIus3DTransducerElementSetGet)
{
    IUS_BOOL equal;
    const float transducerPitch = 0.000005f;
    const int numTransducerElements = 128;
    iu3dp_t elemPos = ius3DPositionCreate((float)(10 - numTransducerElements / 2.0)*transducerPitch, 0.0f, 0.0f);
    iu3ds_t elemSize = ius3DSizeCreate(0.0001f,0.0001f,0.0001f);
    iu3da_t elemAngle = ius3DAngleCreate(0.0f,0.3f);

    iu3dte_t element = ius3DTransducerElementCreate(elemPos, elemAngle, elemSize);
    TEST_ASSERT_NOT_EQUAL(IU3DTE_INVALID,element);

    iu3da_t angle = ius3DTransducerElementGetAngle(element);
    equal = ius3DAngleCompare(elemAngle,angle);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);

    iu3dp_t position = ius3DTransducerElementGetPosition(element);
    equal = ius3DPositionCompare(elemPos,position);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);

    iu3ds_t size = ius3DTransducerElementGetSize(element);
    equal = ius3DSizeCompare(elemSize,size);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);

    ius3DPositionDelete(elemPos);
    ius3DAngleDelete(elemAngle);
    ius3DSizeDelete(elemSize);
    ius3DTransducerElementDelete(element);

}

TEST(Ius3DTransducerElement, testIus3DTransducerElementSerialization)
{
    int status = 0;
    char *filename = "testIus3DTransducerElementSerialization.hdf5";
    //char *path = "/TransducerElement";
    const float transducerPitch = 0.000005f;
    const int numTransducerElements = 128;
    iu3dp_t elemPos = ius3DPositionCreate((float)(10 - numTransducerElements / 2.0)*transducerPitch, 0.0f, 0.0f);
    iu3ds_t elemSize = ius3DSizeCreate(0.0001f,0.0001f,0.0001f);
    iu3da_t elemAngle = ius3DAngleCreate(0.0f,0.3f);

    iu3dte_t element = ius3DTransducerElementCreate(elemPos, elemAngle, elemSize);
    TEST_ASSERT_NOT_EQUAL(IU3DTE_INVALID,element);

    // save
    hid_t handle = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    TEST_ASSERT(handle > 0);
    status = ius3DTransducerElementSave(element, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iu3dte_t savedObj = ius3DTransducerElementLoad(handle);
    TEST_ASSERT_NOT_EQUAL(NULL, savedObj);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUS_TRUE, ius3DTransducerElementCompare(element,savedObj));
    status = ius3DTransducerElementDelete(element);
    status |= ius3DTransducerElementDelete(savedObj);
    TEST_ASSERT(status == IUS_E_OK);
}

TEST_GROUP_RUNNER(Ius3DTransducerElement)
{
    RUN_TEST_CASE(Ius3DTransducerElement, testIus3DTransducerElementCreate);
    RUN_TEST_CASE(Ius3DTransducerElement, testIus3DTransducerElementDelete);
    RUN_TEST_CASE(Ius3DTransducerElement, testIus3DTransducerElementCompare);
    RUN_TEST_CASE(Ius3DTransducerElement, testIus3DTransducerElementSetGet);
    RUN_TEST_CASE(Ius3DTransducerElement, testIus3DTransducerElementSerialization);
}
