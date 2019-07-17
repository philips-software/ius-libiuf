#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <iuf.h>
#include <iuf3DTransducerElementPrivate.h>

static char *pErrorFilename = "Iuf3DTransducerElement.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(Iuf3DTransducerElement);

TEST_SETUP(Iuf3DTransducerElement)
{
    iufErrorLogClear();
    iufErrorLog(IUF_TRUE);
    iufErrorAutoReportSet(IUF_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iufErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(Iuf3DTransducerElement)
{
    iufErrorLogClear();
    iufErrorLog(IUF_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iufErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
}


TEST(Iuf3DTransducerElement, testIuf3DTransducerElementCreate)
{
    const float transducerPitch = 0.000005f;
    const int numTransducerElements = 128;
    iu3dp_t elemPos = iuf3DPositionCreate((float)(10 - numTransducerElements / 2.0)*transducerPitch, 0.0f, 0.0f);
    iu3ds_t elemSize = iuf3DSizeCreate(0.0001f,0.0001f,0.0001f);
    iu3da_t elemAngle = iuf3DAngleCreate(0.0f,0.3f);
    iu3dte_t element = iuf3DTransducerElementCreate(elemPos, elemAngle, elemSize);
    TEST_ASSERT_NOT_EQUAL(IU3DTE_INVALID,element);

    //invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    iu3dte_t nothereElement = iuf3DTransducerElementCreate(elemPos, elemAngle, NULL);
    TEST_ASSERT_EQUAL(IU3DTE_INVALID,nothereElement);
    nothereElement = iuf3DTransducerElementCreate(elemPos, NULL, elemSize);
    TEST_ASSERT_EQUAL(IU3DTE_INVALID,nothereElement);
    nothereElement = iuf3DTransducerElementCreate(NULL, elemAngle, elemSize);
    TEST_ASSERT_EQUAL(IU3DTE_INVALID,nothereElement);

    TEST_ASSERT_EQUAL(3,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));


    int status = iuf3DTransducerElementDelete(element);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);
    iuf3DAngleDelete(elemAngle);
    iuf3DSizeDelete(elemSize);
    iuf3DPositionDelete(elemPos);
}


TEST(Iuf3DTransducerElement, testIuf3DTransducerElementDelete)
{
    const float transducerPitch = 0.000005f;
    const int numTransducerElements = 128;
    iu3dp_t elemPos = iuf3DPositionCreate((float)(10 - numTransducerElements / 2.0)*transducerPitch, 0.0f, 0.0f);
    iu3ds_t elemSize = iuf3DSizeCreate(0.0001f,0.0001f,0.0001f);
    iu3da_t elemAngle = iuf3DAngleCreate(0.0f,0.3f);
    iu3dte_t element = iuf3DTransducerElementCreate(elemPos, elemAngle, elemSize);
    TEST_ASSERT_NOT_EQUAL(IU3DTE_INVALID,element);

    int status = iuf3DTransducerElementDelete(element);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    // invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    status = iuf3DTransducerElementDelete(NULL);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE,status);

    TEST_ASSERT_EQUAL(1,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

    iuf3DPositionDelete(elemPos);
    iuf3DAngleDelete(elemAngle);
    iuf3DSizeDelete(elemSize);
}

TEST(Iuf3DTransducerElement, testIuf3DTransducerElementCompare)
{
    IUF_BOOL equal;
    const float transducerPitch = 0.000005f;
    const int numTransducerElements = 128;
    iu3dp_t elemPos = iuf3DPositionCreate((float)(10 - numTransducerElements / 2.0)*transducerPitch, 0.0f, 0.0f);
    iu3dp_t diffElemPos = iuf3DPositionCreate((float)(19 - numTransducerElements / 2.0)*transducerPitch, 0.1f, 0.0f);

    iu3ds_t elemSize = iuf3DSizeCreate(0.0001f,0.0001f,0.0001f);
    iu3ds_t diffElemSize = iuf3DSizeCreate(0.0002f,0.0001f,0.0001f);

    iu3da_t elemAngle = iuf3DAngleCreate(0.0f,0.3f);
    iu3da_t diffElemAngle = iuf3DAngleCreate(0.1f,0.3f);

    iu3dte_t element = iuf3DTransducerElementCreate(elemPos, elemAngle, elemSize);
    TEST_ASSERT_NOT_EQUAL(IU3DTE_INVALID,element);

    iu3dte_t duplicateElement = iuf3DTransducerElementCreate(elemPos, elemAngle, elemSize);
    TEST_ASSERT_NOT_EQUAL(IU3DTE_INVALID,element);

    equal = iuf3DTransducerElementCompare(element,element);
    TEST_ASSERT_EQUAL(IUF_TRUE,equal);

    equal = iuf3DTransducerElementCompare(element,duplicateElement);
    TEST_ASSERT_EQUAL(IUF_TRUE,equal);

    iu3dte_t diffElement = iuf3DTransducerElementCreate(elemPos, elemAngle, diffElemSize);
    equal = iuf3DTransducerElementCompare(element,diffElement);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);
    iuf3DTransducerElementDelete(diffElement);

    diffElement = iuf3DTransducerElementCreate(elemPos, diffElemAngle, elemSize);
    equal = iuf3DTransducerElementCompare(element,diffElement);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);
    iuf3DTransducerElementDelete(diffElement);

    diffElement = iuf3DTransducerElementCreate(diffElemPos, elemAngle, elemSize);
    equal = iuf3DTransducerElementCompare(element,diffElement);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);
    iuf3DTransducerElementDelete(diffElement);

    // invalid params
    equal = iuf3DTransducerElementCompare(element,NULL);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);
    equal = iuf3DTransducerElementCompare(NULL,element);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);


    iuf3DTransducerElementDelete(element);
    iuf3DTransducerElementDelete(duplicateElement);
    iuf3DPositionDelete(elemPos);
    iuf3DAngleDelete(elemAngle);
    iuf3DSizeDelete(elemSize);
    iuf3DPositionDelete(diffElemPos);
    iuf3DAngleDelete(diffElemAngle);
    iuf3DSizeDelete(diffElemSize);
}


TEST(Iuf3DTransducerElement, testIuf3DTransducerElementSetGet)
{
    IUF_BOOL equal;
    const float transducerPitch = 0.000005f;
    const int numTransducerElements = 128;
    iu3dp_t elemPos = iuf3DPositionCreate((float)(10 - numTransducerElements / 2.0)*transducerPitch, 0.0f, 0.0f);
    iu3ds_t elemSize = iuf3DSizeCreate(0.0001f,0.0001f,0.0001f);
    iu3da_t elemAngle = iuf3DAngleCreate(0.0f,0.3f);

    iu3dte_t element = iuf3DTransducerElementCreate(elemPos, elemAngle, elemSize);
    TEST_ASSERT_NOT_EQUAL(IU3DTE_INVALID,element);

    iu3da_t angle = iuf3DTransducerElementGetAngle(element);
    equal = iuf3DAngleCompare(elemAngle,angle);
    TEST_ASSERT_EQUAL(IUF_TRUE,equal);

    iu3dp_t position = iuf3DTransducerElementGetPosition(element);
    equal = iuf3DPositionCompare(elemPos,position);
    TEST_ASSERT_EQUAL(IUF_TRUE,equal);

    iu3ds_t size = iuf3DTransducerElementGetSize(element);
    equal = iuf3DSizeCompare(elemSize,size);
    TEST_ASSERT_EQUAL(IUF_TRUE,equal);

    iuf3DPositionDelete(elemPos);
    iuf3DAngleDelete(elemAngle);
    iuf3DSizeDelete(elemSize);
    iuf3DTransducerElementDelete(element);

}

TEST(Iuf3DTransducerElement, testIuf3DTransducerElementSerialization)
{
    int status = 0;
    char *filename = "testIuf3DTransducerElementSerialization.hdf5";
    //char *path = "/TransducerElement";
    const float transducerPitch = 0.000005f;
    const int numTransducerElements = 128;
    iu3dp_t elemPos = iuf3DPositionCreate((float)(10 - numTransducerElements / 2.0)*transducerPitch, 0.0f, 0.0f);
    iu3ds_t elemSize = iuf3DSizeCreate(0.0001f,0.0001f,0.0001f);
    iu3da_t elemAngle = iuf3DAngleCreate(0.0f,0.3f);

    iu3dte_t element = iuf3DTransducerElementCreate(elemPos, elemAngle, elemSize);
    TEST_ASSERT_NOT_EQUAL(IU3DTE_INVALID,element);

    // save
    hid_t handle = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    TEST_ASSERT(handle > 0);
    status = iuf3DTransducerElementSave(element, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iu3dte_t savedObj = iuf3DTransducerElementLoad(handle);
    TEST_ASSERT_NOT_EQUAL(NULL, savedObj);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUF_TRUE, iuf3DTransducerElementCompare(element,savedObj));
    status = iuf3DTransducerElementDelete(element);
    status |= iuf3DTransducerElementDelete(savedObj);
    TEST_ASSERT(status == IUF_E_OK);
    iuf3DAngleDelete(elemAngle);
    iuf3DSizeDelete(elemSize);
    iuf3DPositionDelete(elemPos);
}

TEST_GROUP_RUNNER(Iuf3DTransducerElement)
{
    RUN_TEST_CASE(Iuf3DTransducerElement, testIuf3DTransducerElementCreate);
    RUN_TEST_CASE(Iuf3DTransducerElement, testIuf3DTransducerElementDelete);
    RUN_TEST_CASE(Iuf3DTransducerElement, testIuf3DTransducerElementCompare);
    RUN_TEST_CASE(Iuf3DTransducerElement, testIuf3DTransducerElementSetGet);
    RUN_TEST_CASE(Iuf3DTransducerElement, testIuf3DTransducerElementSerialization);
}
