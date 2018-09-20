

//
// Created by nlv09165 on 20/08/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <include/ius.h>
#include <include/iusError.h>
#include <include/iusTypes.h>
#include "include/iusHL3DTransducerElementImp.h"

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
    iu3dp_t elemPos = iusHL3DPositionCreate((10 - numTransducerElements / 2)*transducerPitch, 0.0f, 0.0f);
    iu3ds_t elemSize = iusHL3DSizeCreate(0.0001f,0.0001f,0.0001f);
    iu3da_t elemAngle = iusHL3DAngleCreate(0.0f,0.3f);
    iu3dte_t element = iusHL3DTransducerElementCreate(elemPos, elemAngle, elemSize);
    TEST_ASSERT_NOT_EQUAL(IU3DTE_INVALID,element);

    //invalid params
    iu3dte_t nothereElement = iusHL3DTransducerElementCreate(elemPos, elemAngle, NULL);
    TEST_ASSERT_EQUAL(IU3DTE_INVALID,nothereElement);
    nothereElement = iusHL3DTransducerElementCreate(elemPos, NULL, elemSize);
    TEST_ASSERT_EQUAL(IU3DTE_INVALID,nothereElement);
    nothereElement = iusHL3DTransducerElementCreate(NULL, elemAngle, elemSize);
    TEST_ASSERT_EQUAL(IU3DTE_INVALID,nothereElement);

    int status = iusHL3DTransducerElementDelete(element);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
}


TEST(Ius3DTransducerElement, testIus3DTransducerElementDelete)
{
    const float transducerPitch = 0.000005f;
    const int numTransducerElements = 128;
    iu3dp_t elemPos = iusHL3DPositionCreate((10 - numTransducerElements / 2)*transducerPitch, 0.0f, 0.0f);
    iu3ds_t elemSize = iusHL3DSizeCreate(0.0001f,0.0001f,0.0001f);
    iu3da_t elemAngle = iusHL3DAngleCreate(0.0f,0.3f);
    iu3dte_t element = iusHL3DTransducerElementCreate(elemPos, elemAngle, elemSize);
    TEST_ASSERT_NOT_EQUAL(IU3DTE_INVALID,element);

    int status = iusHL3DTransducerElementDelete(element);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // invalid params
    status = iusHL3DTransducerElementDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE,status);
    iusHL3DPositionDelete(elemPos);
    iusHL3DAngleDelete(elemAngle);
    iusHL3DSizeDelete(elemSize);
}

TEST(Ius3DTransducerElement, testIus3DTransducerElementCompare)
{
    IUS_BOOL equal;
    const float transducerPitch = 0.000005f;
    const int numTransducerElements = 128;
    iu3dp_t elemPos = iusHL3DPositionCreate((10 - numTransducerElements / 2)*transducerPitch, 0.0f, 0.0f);
    iu3dp_t diffElemPos = iusHL3DPositionCreate((19 - numTransducerElements / 2)*transducerPitch, 0.1f, 0.0f);

    iu3ds_t elemSize = iusHL3DSizeCreate(0.0001f,0.0001f,0.0001f);
    iu3ds_t diffElemSize = iusHL3DSizeCreate(0.0002f,0.0001f,0.0001f);

    iu3da_t elemAngle = iusHL3DAngleCreate(0.0f,0.3f);
    iu3da_t diffElemAngle = iusHL3DAngleCreate(0.1f,0.3f);

    iu3dte_t element = iusHL3DTransducerElementCreate(elemPos, elemAngle, elemSize);
    TEST_ASSERT_NOT_EQUAL(IU3DTE_INVALID,element);

    iu3dte_t duplicateElement = iusHL3DTransducerElementCreate(elemPos, elemAngle, elemSize);
    TEST_ASSERT_NOT_EQUAL(IU3DTE_INVALID,element);

    equal = iusHL3DTransducerElementCompare(element,element);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);

    equal = iusHL3DTransducerElementCompare(element,duplicateElement);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);

    iu3dte_t diffElement = iusHL3DTransducerElementCreate(elemPos, elemAngle, diffElemSize);
    equal = iusHL3DTransducerElementCompare(element,diffElement);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    iusHL3DTransducerElementDelete(diffElement);

    diffElement = iusHL3DTransducerElementCreate(elemPos, diffElemAngle, elemSize);
    equal = iusHL3DTransducerElementCompare(element,diffElement);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    iusHL3DTransducerElementDelete(diffElement);

    diffElement = iusHL3DTransducerElementCreate(diffElemPos, elemAngle, elemSize);
    equal = iusHL3DTransducerElementCompare(element,diffElement);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    iusHL3DTransducerElementDelete(diffElement);

    // invalid params
    equal = iusHL3DTransducerElementCompare(element,NULL);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    equal = iusHL3DTransducerElementCompare(NULL,element);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);


    iusHL3DTransducerElementDelete(element);
    iusHL3DTransducerElementDelete(duplicateElement);
    iusHL3DPositionDelete(elemPos);
    iusHL3DAngleDelete(elemAngle);
    iusHL3DSizeDelete(elemSize);
    iusHL3DPositionDelete(diffElemPos);
    iusHL3DAngleDelete(diffElemAngle);
    iusHL3DSizeDelete(diffElemSize);
}


TEST(Ius3DTransducerElement, testIus3DTransducerElementSetGet)
{
    IUS_BOOL equal;
    const float transducerPitch = 0.000005f;
    const int numTransducerElements = 128;
    iu3dp_t elemPos = iusHL3DPositionCreate((10 - numTransducerElements / 2)*transducerPitch, 0.0f, 0.0f);
    iu3ds_t elemSize = iusHL3DSizeCreate(0.0001f,0.0001f,0.0001f);
    iu3da_t elemAngle = iusHL3DAngleCreate(0.0f,0.3f);

    iu3dte_t element = iusHL3DTransducerElementCreate(elemPos, elemAngle, elemSize);
    TEST_ASSERT_NOT_EQUAL(IU3DTE_INVALID,element);

    iu3da_t angle = iusHL3DTransducerElementGetAngle(element);
    equal = iusHL3DAngleCompare(elemAngle,angle);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);

    iu3dp_t position = iusHL3DTransducerElementGetPosition(element);
    equal = iusHL3DPositionCompare(elemPos,position);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);

    iu3ds_t size = iusHL3DTransducerElementGetSize(element);
    equal = iusHL3DSizeCompare(elemSize,size);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);

    iusHL3DPositionDelete(elemPos);
    iusHL3DAngleDelete(elemAngle);
    iusHL3DSizeDelete(elemSize);
    iusHL3DTransducerElementDelete(element);

}

TEST(Ius3DTransducerElement, testIus3DTransducerElementSerialization)
{
    int status = 0;
    char *filename = "testIus3DTransducerElementSerialization.hdf5";
    //char *path = "/TransducerElement";
    const float transducerPitch = 0.000005f;
    const int numTransducerElements = 128;
    iu3dp_t elemPos = iusHL3DPositionCreate((10 - numTransducerElements / 2)*transducerPitch, 0.0f, 0.0f);
    iu3ds_t elemSize = iusHL3DSizeCreate(0.0001f,0.0001f,0.0001f);
    iu3da_t elemAngle = iusHL3DAngleCreate(0.0f,0.3f);

    iu3dte_t element = iusHL3DTransducerElementCreate(elemPos, elemAngle, elemSize);
    TEST_ASSERT_NOT_EQUAL(IU3DTE_INVALID,element);

    // save
    hid_t handle = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    TEST_ASSERT(handle > 0);
    status = iusHL3DTransducerElementSave(element, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iu3dte_t savedObj = iusHL3DTransducerElementLoad(handle);
    TEST_ASSERT_NOT_EQUAL(NULL, savedObj);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUS_TRUE, iusHL3DTransducerElementCompare(element,savedObj));
    status = iusHL3DTransducerElementDelete(element);
    status |= iusHL3DTransducerElementDelete(savedObj);
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
