

//
// Created by nlv09165 on 20/08/2018.
//
#include <math.h>
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <include/ius.h>
#include "include/ius2DTransducerElementPrivate.h"

static char *pErrorFilename = "Ius2DTransducerElement.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(Ius2DTransducerElement);

TEST_SETUP(Ius2DTransducerElement)
{
    iusErrorLogClear();
    iusErrorLog(IUS_TRUE);
    iusErrorAutoReport(IUS_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iusErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(Ius2DTransducerElement)
{
    iusErrorLogClear();
    iusErrorLog(IUS_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iusErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
}


TEST(Ius2DTransducerElement, testIus2DTransducerElementCreate)
{
    const float transducerPitch = 0.000005f;
    const int numTransducerElements = 128;
    iu2dp_t elemPos = ius2DPositionCreate((float)(10 - numTransducerElements / 2.0)*transducerPitch, 0.0f);
    iu2ds_t elemSize = ius2DSizeCreate(0.0001f,0.0001f);
    float  theta = 0.3f;
    iu2dte_t element = ius2DTransducerElementCreate(elemPos, theta, elemSize);
    TEST_ASSERT_NOT_EQUAL(IU2DTE_INVALID,element);

    //invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());

    iu2dte_t notherElement = ius2DTransducerElementCreate(elemPos, theta, NULL);
    TEST_ASSERT_EQUAL(IU2DTE_INVALID,notherElement);

    float ap = NAN;
    notherElement = ius2DTransducerElementCreate(elemPos, ap, elemSize);
    TEST_ASSERT_EQUAL(IU2DTE_INVALID,notherElement);

    notherElement = ius2DTransducerElementCreate(NULL, theta, elemSize);
    TEST_ASSERT_EQUAL(IU2DTE_INVALID,notherElement);

    TEST_ASSERT_EQUAL(3,iusErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

    int status = ius2DTransducerElementDelete(element);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    ius2DPositionDelete(elemPos);
    ius2DSizeDelete(elemSize);
}


TEST(Ius2DTransducerElement, testIus2DTransducerElementDelete)
{
    const float transducerPitch = 0.000005f;
    const int numTransducerElements = 128;
    iu2dp_t elemPos = ius2DPositionCreate((float)(10 - numTransducerElements / 2.0)*transducerPitch, 0.0f);
    iu2ds_t elemSize = ius2DSizeCreate(0.0001f,0.0001f);
    float  theta = 0.3f;
    iu2dte_t element = ius2DTransducerElementCreate(elemPos, theta, elemSize);
    TEST_ASSERT_NOT_EQUAL(IU2DTE_INVALID,element);

    int status = ius2DTransducerElementDelete(element);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());

    status = ius2DTransducerElementDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE,status);

    TEST_ASSERT_EQUAL(1,iusErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
    ius2DPositionDelete(elemPos);
    ius2DSizeDelete(elemSize);
}

TEST(Ius2DTransducerElement, testIus2DTransducerElementCompare)
{
    IUS_BOOL equal;
    const float transducerPitch = 0.000005f;
    const int numTransducerElements = 128;
    iu2dp_t elemPos = ius2DPositionCreate((float)(10 - numTransducerElements / 2.0)*transducerPitch, 0.0f);
    iu2dp_t diffElemPos = ius2DPositionCreate((float)(19 - numTransducerElements / 2.0)*transducerPitch, 0.0f);

    iu2ds_t elemSize = ius2DSizeCreate(0.0001f,0.0001f);
    iu2ds_t diffElemSize = ius2DSizeCreate(0.0002f,0.0001f);

    float  theta = 0.3f;
    float  difftTheta = 0.4f;

    iu2dte_t element = ius2DTransducerElementCreate(elemPos, theta, elemSize);
    TEST_ASSERT_NOT_EQUAL(IU2DTE_INVALID,element);

    iu2dte_t duplicateElement = ius2DTransducerElementCreate(elemPos, theta, elemSize);
    TEST_ASSERT_NOT_EQUAL(IU2DTE_INVALID,element);

    equal = ius2DTransducerElementCompare(element,element);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);

    equal = ius2DTransducerElementCompare(element,duplicateElement);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);

    iu2dte_t diffElement = ius2DTransducerElementCreate(elemPos, theta, diffElemSize);
    equal = ius2DTransducerElementCompare(element,diffElement);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    ius2DTransducerElementDelete(diffElement);

    diffElement = ius2DTransducerElementCreate(elemPos, difftTheta, elemSize);
    equal = ius2DTransducerElementCompare(element,diffElement);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    ius2DTransducerElementDelete(diffElement);

    diffElement = ius2DTransducerElementCreate(diffElemPos, theta, elemSize);
    equal = ius2DTransducerElementCompare(element,diffElement);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    ius2DTransducerElementDelete(diffElement);

    // invalid params
    equal = ius2DTransducerElementCompare(element,NULL);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    equal = ius2DTransducerElementCompare(NULL,element);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);


    ius2DTransducerElementDelete(element);
    ius2DTransducerElementDelete(duplicateElement);
    ius2DPositionDelete(elemPos);
    ius2DSizeDelete(elemSize);
    ius2DPositionDelete(diffElemPos);
    ius2DSizeDelete(diffElemSize);
}


TEST(Ius2DTransducerElement, testIus2DTransducerElementSetGet)
{
    IUS_BOOL equal;
    const float transducerPitch = 0.000005f;
    const int numTransducerElements = 128;
    iu2dp_t elemPos = ius2DPositionCreate((float)(10 - numTransducerElements / 2.0)*transducerPitch, 0.0f);
    iu2ds_t elemSize = ius2DSizeCreate(0.0001f,0.0001f);
    float  theta = 0.3f;

    iu2dte_t element = ius2DTransducerElementCreate(elemPos, theta, elemSize);
    TEST_ASSERT_NOT_EQUAL(IU2DTE_INVALID,element);

    TEST_ASSERT_EQUAL(theta,ius2DTransducerElementGetAngle(element));

    iu2dp_t position = ius2DTransducerElementGetPosition(element);
    equal = ius2DPositionCompare(elemPos,position);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);

    iu2ds_t size = ius2DTransducerElementGetSize(element);
    equal = ius2DSizeCompare(elemSize,size);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);

    ius2DPositionDelete(elemPos);
    ius2DSizeDelete(elemSize);
    ius2DTransducerElementDelete(element);
}

TEST(Ius2DTransducerElement, testIus2DTransducerElementSerialization)
{
    int status = 0;
    char *filename = "testIus2DTransducerElementSerialization.hdf5";
    //char *path = "/TransducerElement"; Elements path is not variable 
    const float transducerPitch = 0.000005f;
    const int numTransducerElements = 128;
    iu2dp_t elemPos = ius2DPositionCreate((float)(10 - numTransducerElements / 2.0)*transducerPitch, 0.0f);
    iu2ds_t elemSize = ius2DSizeCreate(0.0001f,0.0001f);
    float theta = 0.3f;

    iu2dte_t element = ius2DTransducerElementCreate(elemPos, theta, elemSize);
    TEST_ASSERT_NOT_EQUAL(IU2DTE_INVALID,element);

    // save
    hid_t handle = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    TEST_ASSERT(handle > 0);
    status = ius2DTransducerElementSave(element, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iu2dte_t savedObj = ius2DTransducerElementLoad(handle);
    TEST_ASSERT_NOT_EQUAL(NULL, savedObj);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUS_TRUE, ius2DTransducerElementCompare(element,savedObj));
    status = ius2DTransducerElementDelete(element);
    status |= ius2DTransducerElementDelete(savedObj);
    TEST_ASSERT(status == IUS_E_OK);
    ius2DPositionDelete(elemPos);
    ius2DSizeDelete(elemSize);
}

TEST_GROUP_RUNNER(Ius2DTransducerElement)
{
    RUN_TEST_CASE(Ius2DTransducerElement, testIus2DTransducerElementCreate);
    RUN_TEST_CASE(Ius2DTransducerElement, testIus2DTransducerElementDelete);
    RUN_TEST_CASE(Ius2DTransducerElement, testIus2DTransducerElementCompare);
    RUN_TEST_CASE(Ius2DTransducerElement, testIus2DTransducerElementSetGet);
    RUN_TEST_CASE(Ius2DTransducerElement, testIus2DTransducerElementSerialization);
}
