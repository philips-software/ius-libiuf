#include <math.h>
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <iuf.h>
#include <iuf2DTransducerElementPrivate.h>

static char *pErrorFilename = "Iuf2DTransducerElement.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(Iuf2DTransducerElement);

TEST_SETUP(Iuf2DTransducerElement)
{
    iufErrorLogClear();
    iufErrorLog(IUF_TRUE);
    iufErrorAutoReportSet(IUF_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iufErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(Iuf2DTransducerElement)
{
    iufErrorLogClear();
    iufErrorLog(IUF_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iufErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
}


TEST(Iuf2DTransducerElement, testIuf2DTransducerElementCreate)
{
    const float transducerPitch = 0.000005f;
    const int numTransducerElements = 128;
    iu2dp_t elemPos = iuf2DPositionCreate((float)(10 - numTransducerElements / 2.0)*transducerPitch, 0.0f);
    iu2ds_t elemSize = iuf2DSizeCreate(0.0001f,0.0001f);
    float  theta = 0.3f;
    iu2dte_t element = iuf2DTransducerElementCreate(elemPos, theta, elemSize);
    TEST_ASSERT_NOT_EQUAL(IU2DTE_INVALID,element);

    //invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    iu2dte_t notherElement = iuf2DTransducerElementCreate(elemPos, theta, NULL);
    TEST_ASSERT_EQUAL(IU2DTE_INVALID,notherElement);

    float ap = NAN;
    notherElement = iuf2DTransducerElementCreate(elemPos, ap, elemSize);
    TEST_ASSERT_EQUAL(IU2DTE_INVALID,notherElement);

    notherElement = iuf2DTransducerElementCreate(NULL, theta, elemSize);
    TEST_ASSERT_EQUAL(IU2DTE_INVALID,notherElement);

    TEST_ASSERT_EQUAL(3,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

    int status = iuf2DTransducerElementDelete(element);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);
    iuf2DPositionDelete(elemPos);
    iuf2DSizeDelete(elemSize);
}


TEST(Iuf2DTransducerElement, testIuf2DTransducerElementDelete)
{
    const float transducerPitch = 0.000005f;
    const int numTransducerElements = 128;
    iu2dp_t elemPos = iuf2DPositionCreate((float)(10 - numTransducerElements / 2.0)*transducerPitch, 0.0f);
    iu2ds_t elemSize = iuf2DSizeCreate(0.0001f,0.0001f);
    float  theta = 0.3f;
    iu2dte_t element = iuf2DTransducerElementCreate(elemPos, theta, elemSize);
    TEST_ASSERT_NOT_EQUAL(IU2DTE_INVALID,element);

    int status = iuf2DTransducerElementDelete(element);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    // invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    status = iuf2DTransducerElementDelete(NULL);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE,status);

    TEST_ASSERT_EQUAL(1,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
    iuf2DPositionDelete(elemPos);
    iuf2DSizeDelete(elemSize);
}

TEST(Iuf2DTransducerElement, testIuf2DTransducerElementCompare)
{
    IUF_BOOL equal;
    const float transducerPitch = 0.000005f;
    const int numTransducerElements = 128;
    iu2dp_t elemPos = iuf2DPositionCreate((float)(10 - numTransducerElements / 2.0)*transducerPitch, 0.0f);
    iu2dp_t diffElemPos = iuf2DPositionCreate((float)(19 - numTransducerElements / 2.0)*transducerPitch, 0.0f);

    iu2ds_t elemSize = iuf2DSizeCreate(0.0001f,0.0001f);
    iu2ds_t diffElemSize = iuf2DSizeCreate(0.0002f,0.0001f);

    float  theta = 0.3f;
    float  difftTheta = 0.4f;

    iu2dte_t element = iuf2DTransducerElementCreate(elemPos, theta, elemSize);
    TEST_ASSERT_NOT_EQUAL(IU2DTE_INVALID,element);

    iu2dte_t duplicateElement = iuf2DTransducerElementCreate(elemPos, theta, elemSize);
    TEST_ASSERT_NOT_EQUAL(IU2DTE_INVALID,element);

    equal = iuf2DTransducerElementCompare(element,element);
    TEST_ASSERT_EQUAL(IUF_TRUE,equal);

    equal = iuf2DTransducerElementCompare(element,duplicateElement);
    TEST_ASSERT_EQUAL(IUF_TRUE,equal);

    iu2dte_t diffElement = iuf2DTransducerElementCreate(elemPos, theta, diffElemSize);
    equal = iuf2DTransducerElementCompare(element,diffElement);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);
    iuf2DTransducerElementDelete(diffElement);

    diffElement = iuf2DTransducerElementCreate(elemPos, difftTheta, elemSize);
    equal = iuf2DTransducerElementCompare(element,diffElement);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);
    iuf2DTransducerElementDelete(diffElement);

    diffElement = iuf2DTransducerElementCreate(diffElemPos, theta, elemSize);
    equal = iuf2DTransducerElementCompare(element,diffElement);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);
    iuf2DTransducerElementDelete(diffElement);

    // invalid params
    equal = iuf2DTransducerElementCompare(element,NULL);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);
    equal = iuf2DTransducerElementCompare(NULL,element);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);


    iuf2DTransducerElementDelete(element);
    iuf2DTransducerElementDelete(duplicateElement);
    iuf2DPositionDelete(elemPos);
    iuf2DSizeDelete(elemSize);
    iuf2DPositionDelete(diffElemPos);
    iuf2DSizeDelete(diffElemSize);
}


TEST(Iuf2DTransducerElement, testIuf2DTransducerElementSetGet)
{
    IUF_BOOL equal;
    const float transducerPitch = 0.000005f;
    const int numTransducerElements = 128;
    iu2dp_t elemPos = iuf2DPositionCreate((float)(10 - numTransducerElements / 2.0)*transducerPitch, 0.0f);
    iu2ds_t elemSize = iuf2DSizeCreate(0.0001f,0.0001f);
    float  theta = 0.3f;

    iu2dte_t element = iuf2DTransducerElementCreate(elemPos, theta, elemSize);
    TEST_ASSERT_NOT_EQUAL(IU2DTE_INVALID,element);

    TEST_ASSERT_EQUAL(theta,iuf2DTransducerElementGetAngle(element));

    iu2dp_t position = iuf2DTransducerElementGetPosition(element);
    equal = iuf2DPositionCompare(elemPos,position);
    TEST_ASSERT_EQUAL(IUF_TRUE,equal);

    iu2ds_t size = iuf2DTransducerElementGetSize(element);
    equal = iuf2DSizeCompare(elemSize,size);
    TEST_ASSERT_EQUAL(IUF_TRUE,equal);

    iuf2DPositionDelete(elemPos);
    iuf2DSizeDelete(elemSize);
    iuf2DTransducerElementDelete(element);
}

TEST(Iuf2DTransducerElement, testIuf2DTransducerElementSerialization)
{
    int status = 0;
    char *filename = "testIuf2DTransducerElementSerialization.hdf5";
    //char *path = "/TransducerElement"; Elements path is not variable 
    const float transducerPitch = 0.000005f;
    const int numTransducerElements = 128;
    iu2dp_t elemPos = iuf2DPositionCreate((float)(10 - numTransducerElements / 2.0)*transducerPitch, 0.0f);
    iu2ds_t elemSize = iuf2DSizeCreate(0.0001f,0.0001f);
    float theta = 0.3f;

    iu2dte_t element = iuf2DTransducerElementCreate(elemPos, theta, elemSize);
    TEST_ASSERT_NOT_EQUAL(IU2DTE_INVALID,element);

    // save
    hid_t handle = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    TEST_ASSERT(handle > 0);
    status = iuf2DTransducerElementSave(element, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iu2dte_t savedObj = iuf2DTransducerElementLoad(handle);
    TEST_ASSERT_NOT_EQUAL(NULL, savedObj);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUF_TRUE, iuf2DTransducerElementCompare(element,savedObj));
    status = iuf2DTransducerElementDelete(element);
    status |= iuf2DTransducerElementDelete(savedObj);
    TEST_ASSERT(status == IUF_E_OK);
    iuf2DPositionDelete(elemPos);
    iuf2DSizeDelete(elemSize);
}

TEST_GROUP_RUNNER(Iuf2DTransducerElement)
{
    RUN_TEST_CASE(Iuf2DTransducerElement, testIuf2DTransducerElementCreate);
    RUN_TEST_CASE(Iuf2DTransducerElement, testIuf2DTransducerElementDelete);
    RUN_TEST_CASE(Iuf2DTransducerElement, testIuf2DTransducerElementCompare);
    RUN_TEST_CASE(Iuf2DTransducerElement, testIuf2DTransducerElementSetGet);
    RUN_TEST_CASE(Iuf2DTransducerElement, testIuf2DTransducerElementSerialization);
}
