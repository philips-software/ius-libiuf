

//
// Created by nlv09165 on 20/08/2018.
//
#include <math.h>
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <include/ius.h>
#include <include/iusError.h>
#include <include/iusTypes.h>
#include "include/iusHL2DTransducerElementImp.h"

TEST_GROUP(Ius2DTransducerElement);

TEST_SETUP(Ius2DTransducerElement)
{
}

TEST_TEAR_DOWN(Ius2DTransducerElement)
{
}


TEST(Ius2DTransducerElement, testIus2DTransducerElementCreate)
{
    const float transducerPitch = 0.000005f;
    const int numTransducerElements = 128;
    iu2dp_t elemPos = iusHL2DPositionCreate((10 - numTransducerElements / 2)*transducerPitch, 0.0f);
    iu2ds_t elemSize = iusHL2DSizeCreate(0.0001f,0.0001f);
    float  theta = 0.3f;
    iu2dte_t element = iusHL2DTransducerElementCreate(elemPos, theta, elemSize);
    TEST_ASSERT_NOT_EQUAL(IU2DTE_INVALID,element);

    //invalid params
    iu2dte_t notherElement = iusHL2DTransducerElementCreate(elemPos, theta, NULL);
    TEST_ASSERT_EQUAL(IU2DTE_INVALID,notherElement);
    int status = iusHL2DTransducerElementDelete(notherElement);

    float ap = NAN;
    notherElement = iusHL2DTransducerElementCreate(elemPos, ap, elemSize);
    TEST_ASSERT_EQUAL(IU2DTE_INVALID,notherElement);

    notherElement = iusHL2DTransducerElementCreate(NULL, theta, elemSize);
    TEST_ASSERT_EQUAL(IU2DTE_INVALID,notherElement);

    status = iusHL2DTransducerElementDelete(element);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
}


TEST(Ius2DTransducerElement, testIus2DTransducerElementDelete)
{
    const float transducerPitch = 0.000005f;
    const int numTransducerElements = 128;
    iu2dp_t elemPos = iusHL2DPositionCreate((10 - numTransducerElements / 2)*transducerPitch, 0.0f);
    iu2ds_t elemSize = iusHL2DSizeCreate(0.0001f,0.0001f);
    float  theta = 0.3f;
    iu2dte_t element = iusHL2DTransducerElementCreate(elemPos, theta, elemSize);
    TEST_ASSERT_NOT_EQUAL(IU2DTE_INVALID,element);

    int status = iusHL2DTransducerElementDelete(element);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // invalid params
    status = iusHL2DTransducerElementDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE,status);
    iusHL2DPositionDelete(elemPos);
    iusHL2DSizeDelete(elemSize);
}

TEST(Ius2DTransducerElement, testIus2DTransducerElementCompare)
{
    IUS_BOOL equal;
    const float transducerPitch = 0.000005f;
    const int numTransducerElements = 128;
    iu2dp_t elemPos = iusHL2DPositionCreate((10 - numTransducerElements / 2)*transducerPitch, 0.0f);
    iu2dp_t diffElemPos = iusHL2DPositionCreate((19 - numTransducerElements / 2)*transducerPitch, 0.0f);

    iu2ds_t elemSize = iusHL2DSizeCreate(0.0001f,0.0001f);
    iu2ds_t diffElemSize = iusHL2DSizeCreate(0.0002f,0.0001f);

    float  theta = 0.3f;
    float  difftTheta = 0.4f;

    iu2dte_t element = iusHL2DTransducerElementCreate(elemPos, theta, elemSize);
    TEST_ASSERT_NOT_EQUAL(IU2DTE_INVALID,element);

    iu2dte_t duplicateElement = iusHL2DTransducerElementCreate(elemPos, theta, elemSize);
    TEST_ASSERT_NOT_EQUAL(IU2DTE_INVALID,element);

    equal = iusHL2DTransducerElementCompare(element,element);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);

    equal = iusHL2DTransducerElementCompare(element,duplicateElement);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);

    iu2dte_t diffElement = iusHL2DTransducerElementCreate(elemPos, theta, diffElemSize);
    equal = iusHL2DTransducerElementCompare(element,diffElement);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    iusHL2DTransducerElementDelete(diffElement);

    diffElement = iusHL2DTransducerElementCreate(elemPos, difftTheta, elemSize);
    equal = iusHL2DTransducerElementCompare(element,diffElement);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    iusHL2DTransducerElementDelete(diffElement);

    diffElement = iusHL2DTransducerElementCreate(diffElemPos, theta, elemSize);
    equal = iusHL2DTransducerElementCompare(element,diffElement);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    iusHL2DTransducerElementDelete(diffElement);

    // invalid params
    equal = iusHL2DTransducerElementCompare(element,NULL);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    equal = iusHL2DTransducerElementCompare(NULL,element);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);


    iusHL2DTransducerElementDelete(element);
    iusHL2DTransducerElementDelete(duplicateElement);
    iusHL2DPositionDelete(elemPos);
    iusHL2DSizeDelete(elemSize);
    iusHL2DPositionDelete(diffElemPos);
    iusHL2DSizeDelete(diffElemSize);
}


TEST(Ius2DTransducerElement, testIus2DTransducerElementSetGet)
{
    IUS_BOOL equal;
    const float transducerPitch = 0.000005f;
    const int numTransducerElements = 128;
    iu2dp_t elemPos = iusHL2DPositionCreate((10 - numTransducerElements / 2)*transducerPitch, 0.0f);
    iu2ds_t elemSize = iusHL2DSizeCreate(0.0001f,0.0001f);
    float  theta = 0.3f;

    iu2dte_t element = iusHL2DTransducerElementCreate(elemPos, theta, elemSize);
    TEST_ASSERT_NOT_EQUAL(IU2DTE_INVALID,element);

    TEST_ASSERT_EQUAL(theta,iusHL2DTransducerElementGetAngle(element));

    iu2dp_t position = iusHL2DTransducerElementGetPosition(element);
    equal = iusHL2DPositionCompare(elemPos,position);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);

    iu2ds_t size = iusHL2DTransducerElementGetSize(element);
    equal = iusHL2DSizeCompare(elemSize,size);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);

    iusHL2DPositionDelete(elemPos);
    iusHL2DSizeDelete(elemSize);
    iusHL2DTransducerElementDelete(element);
}

TEST(Ius2DTransducerElement, testIus2DTransducerElementSerialization)
{
    int status = 0;
    char *filename = "testIus2DTransducerElementSerialization.hdf5";
    char *path = "/TransducerElement";
    const float transducerPitch = 0.000005f;
    const int numTransducerElements = 128;
    iu2dp_t elemPos = iusHL2DPositionCreate((10 - numTransducerElements / 2)*transducerPitch, 0.0f);
    iu2ds_t elemSize = iusHL2DSizeCreate(0.0001f,0.0001f);
    float theta = 0.3f;

    iu2dte_t element = iusHL2DTransducerElementCreate(elemPos, theta, elemSize);
    TEST_ASSERT_NOT_EQUAL(IU2DTE_INVALID,element);

    // save
    hid_t handle = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    TEST_ASSERT(handle > 0);
    status = iusHL2DTransducerElementSave(element, path, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iu2dte_t savedObj = iusHL2DTransducerElementLoad(handle, path);
    TEST_ASSERT_NOT_EQUAL(NULL, savedObj);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUS_TRUE, iusHL2DTransducerElementCompare(element,savedObj));
    status = iusHL2DTransducerElementDelete(element);
    status |= iusHL2DTransducerElementDelete(savedObj);
    TEST_ASSERT(status == IUS_E_OK);
}

TEST_GROUP_RUNNER(Ius2DTransducerElement)
{
    RUN_TEST_CASE(Ius2DTransducerElement, testIus2DTransducerElementCreate);
    RUN_TEST_CASE(Ius2DTransducerElement, testIus2DTransducerElementDelete);
    RUN_TEST_CASE(Ius2DTransducerElement, testIus2DTransducerElementCompare);
    RUN_TEST_CASE(Ius2DTransducerElement, testIus2DTransducerElementSetGet);
    RUN_TEST_CASE(Ius2DTransducerElement, testIus2DTransducerElementSerialization);
}
