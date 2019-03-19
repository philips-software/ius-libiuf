
//
// Created by nlv09165 on 22/08/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <iuf.h>
#include <iuf2DTransducerElementListPrivate.h>

static char *pErrorFilename = "Iuf2DTransducerElementList.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(Iuf2DTransducerElementList);

TEST_SETUP(Iuf2DTransducerElementList)
{
    iufErrorLogClear();
    iufErrorLog(IUF_TRUE);
    iufErrorAutoReportSet(IUF_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iufErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(Iuf2DTransducerElementList)
{
    iufErrorLogClear();
    iufErrorLog(IUF_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iufErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
}

TEST(Iuf2DTransducerElementList, testIuf2DTransducerElementListCreate)
{
    // Put your test code here
    int num2DTransducerElements = 100;
    iu2dtel_t _2dTransducerElementList = iuf2DTransducerElementListCreate(num2DTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU2DTEL_INVALID, _2dTransducerElementList);
    TEST_ASSERT_EQUAL(num2DTransducerElements, iuf2DTransducerElementListGetSize(_2dTransducerElementList));
    iuf2DTransducerElementListDelete(_2dTransducerElementList);

    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    _2dTransducerElementList = iuf2DTransducerElementListCreate(-1);
    TEST_ASSERT_EQUAL(IU2DTEL_INVALID, _2dTransducerElementList);

    TEST_ASSERT_EQUAL(1,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
}


TEST(Iuf2DTransducerElementList, testIuf2DTransducerElementListDelete)
{
    // Put your test code here
    int num2DTransducerElements = 100;
    iu2dtel_t _2dTransducerElementList = iuf2DTransducerElementListCreate(num2DTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU2DTEL_INVALID, _2dTransducerElementList);
    int status = iuf2DTransducerElementListDelete(_2dTransducerElementList);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    status = iuf2DTransducerElementListDelete(NULL);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE,status);

    TEST_ASSERT_EQUAL(1,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

}

TEST(Iuf2DTransducerElementList, testIuf2DTransducerElementListCompare)
{
    // Put your test code here
    int num2DTransducerElements = 100;
    int status;
    IUF_BOOL equal;
    const int numTransducerElements = 128;
    const float transducerPitch = 0.000005f;
    iu2dp_t elemPos = iuf2DPositionCreate((float)(10 - numTransducerElements / 2.0)*transducerPitch, 0.0f);
    iu2ds_t elemSize = iuf2DSizeCreate(0.0001f,0.0001f);
    float elemAngle = 0.3f;

    // Empty lists should be equal
    iu2dtel_t _2dTransducerElementList = iuf2DTransducerElementListCreate(num2DTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU2DTEL_INVALID, _2dTransducerElementList);
    iu2dtel_t nother2DTransducerElementList = iuf2DTransducerElementListCreate(num2DTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU2DTEL_INVALID, nother2DTransducerElementList);
    equal = iuf2DTransducerElementListCompare(_2dTransducerElementList, nother2DTransducerElementList);
    TEST_ASSERT_EQUAL(IUF_TRUE, equal);


    iu2dte_t element = iuf2DTransducerElementCreate(elemPos, elemAngle, elemSize);
    TEST_ASSERT_NOT_EQUAL(IU2DTE_INVALID,element);

    // Change one list..add bmode
    status = iuf2DTransducerElementListSet(_2dTransducerElementList, element, 0);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);
    equal = iuf2DTransducerElementListCompare(_2dTransducerElementList, nother2DTransducerElementList);
    TEST_ASSERT_EQUAL(IUF_FALSE, equal);

    // Change other
    status = iuf2DTransducerElementListSet(nother2DTransducerElementList, element, 0);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);
    equal = iuf2DTransducerElementListCompare(_2dTransducerElementList, nother2DTransducerElementList);
    TEST_ASSERT_EQUAL(IUF_TRUE, equal);


    iuf2DTransducerElementListDelete(_2dTransducerElementList);
    iuf2DTransducerElementListDelete(nother2DTransducerElementList);
    iuf2DTransducerElementDelete(element);
    iuf2DPositionDelete(elemPos);
    iuf2DSizeDelete(elemSize);
}

TEST(Iuf2DTransducerElementList, testIuf2DTransducerElementListSetGet)
{
    int num2DTransducerElements = 2;
    int status;

    const int numTransducerElements = 128;
    const float transducerPitch = 0.000005f;
    iu2dp_t elemPos = iuf2DPositionCreate((float)(10 - numTransducerElements / 2.0)*transducerPitch,0.0f);
    iu2ds_t elemSize = iuf2DSizeCreate(0.0001f,0.0001f);
    float elemAngle = 0.3f;
    iu2dte_t element = iuf2DTransducerElementCreate(elemPos, elemAngle, elemSize);

    iu2dtel_t _2dTransducerElementList = iuf2DTransducerElementListCreate(num2DTransducerElements);
    status = iuf2DTransducerElementListSet(_2dTransducerElementList,element,0);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    // invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, iuf2DTransducerElementListSet(NULL,element,0));
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, iuf2DTransducerElementListSet(_2dTransducerElementList,element,-1));
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, iuf2DTransducerElementListSet(_2dTransducerElementList,element,num2DTransducerElements));

    TEST_ASSERT_EQUAL(3,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

    iuf2DTransducerElementListDelete(_2dTransducerElementList);
    iuf2DTransducerElementDelete(element);
    iuf2DPositionDelete(elemPos);
    iuf2DSizeDelete(elemSize);
}

TEST(Iuf2DTransducerElementList, testIuf2DTransducerElementListSerialization)
{
    int i, num2DTransducerElements = 2;
    int status;
    IUF_BOOL equal;
    char *pFilename = "testIuf2DTransducerElementListSerialization.hdf5";
    //char *p2DTransducerElementListPath = "/2DTransducerElementList";
    const float transducerPitch = 0.000005f;

    // Non-happy flow
    // for lists it should not be possible to save, if not all elements have been
    // filled.
    iu2dtel_t _2dTransducerElementList = iuf2DTransducerElementListCreate(num2DTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU2DTEL_INVALID, _2dTransducerElementList);
    hid_t handle = H5Fcreate(pFilename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    TEST_ASSERT(handle > 0);
    status = iuf2DTransducerElementListSave(_2dTransducerElementList, handle);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, status);
    status = H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);

    // Happy Flow
    //
    // fill list
    for (i = 0; i < num2DTransducerElements; i++)
    {
        iu2dp_t elemPos = iuf2DPositionCreate((float)(10 - num2DTransducerElements / 2.0)*transducerPitch, 0.0f);
        iu2ds_t elemSize = iuf2DSizeCreate(0.0001f,0.0001f);
        float elemAngle = 0.4f;
        iu2dte_t element = iuf2DTransducerElementCreate(elemPos, elemAngle, elemSize);
        iuf2DTransducerElementListSet(_2dTransducerElementList, element, i);
    }

    // save
    handle = H5Fcreate(pFilename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    TEST_ASSERT(handle > 0);
    status = iuf2DTransducerElementListSave(_2dTransducerElementList, handle);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);
    status = H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);

    // read back
    handle = H5Fopen(pFilename, H5F_ACC_RDONLY, H5P_DEFAULT);
    iu2dtel_t saved2DTransducerElementList = iuf2DTransducerElementListLoad(handle);
    TEST_ASSERT_NOT_EQUAL(IU2DTEL_INVALID, saved2DTransducerElementList);
    status |= H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);

    // compare
    equal = iuf2DTransducerElementListCompare(_2dTransducerElementList, saved2DTransducerElementList);
    TEST_ASSERT_EQUAL(IUF_TRUE, equal);


    // cleanup
    for (i = 0; i < num2DTransducerElements; i++)
    {
        iu2dte_t element = iuf2DTransducerElementListGet(_2dTransducerElementList, i);
        iu2dp_t elemPos = iuf2DTransducerElementGetPosition(element);
        iu2ds_t elemSize = iuf2DTransducerElementGetSize(element);
        iuf2DTransducerElementDelete(element);
        iuf2DPositionDelete(elemPos);
        iuf2DSizeDelete(elemSize);
    }
    iuf2DTransducerElementListDelete(_2dTransducerElementList);
    iuf2DTransducerElementListDelete(saved2DTransducerElementList);

}

TEST_GROUP_RUNNER(Iuf2DTransducerElementList)
{
    RUN_TEST_CASE(Iuf2DTransducerElementList, testIuf2DTransducerElementListDelete);
    RUN_TEST_CASE(Iuf2DTransducerElementList, testIuf2DTransducerElementListCreate);
    RUN_TEST_CASE(Iuf2DTransducerElementList, testIuf2DTransducerElementListSetGet);
    RUN_TEST_CASE(Iuf2DTransducerElementList, testIuf2DTransducerElementListCompare);
    RUN_TEST_CASE(Iuf2DTransducerElementList, testIuf2DTransducerElementListSerialization);
}
