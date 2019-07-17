#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <iuf.h>
#include <iuf3DTransducerElementListPrivate.h>

static char *pErrorFilename = "Iuf3DTransducerElementList.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(Iuf3DTransducerElementList);

TEST_SETUP(Iuf3DTransducerElementList)
{
    iufErrorLogClear();
    iufErrorLog(IUF_TRUE);
    iufErrorAutoReportSet(IUF_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iufErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(Iuf3DTransducerElementList)
{
    iufErrorLogClear();
    iufErrorLog(IUF_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iufErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
}

TEST(Iuf3DTransducerElementList, testIuf3DTransducerElementListCreate)
{
    // Put your test code here
    int num3DTransducerElements = 100;
    iu3dtel_t _3dTransducerElementList = iuf3DTransducerElementListCreate(num3DTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU3DTEL_INVALID, _3dTransducerElementList);
    TEST_ASSERT_EQUAL(num3DTransducerElements, iuf3DTransducerElementListGetSize(_3dTransducerElementList));
    iuf3DTransducerElementListDelete(_3dTransducerElementList);

    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    _3dTransducerElementList = iuf3DTransducerElementListCreate(-1);
    TEST_ASSERT_EQUAL(IU3DTEL_INVALID, _3dTransducerElementList);
    _3dTransducerElementList = iuf3DTransducerElementListCreate(0);
    TEST_ASSERT_EQUAL(IU3DTEL_INVALID, _3dTransducerElementList);

    TEST_ASSERT_EQUAL(2,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
}


TEST(Iuf3DTransducerElementList, testIuf3DTransducerElementListDelete)
{
    // Put your test code here
    int num3DTransducerElements = 100;
    iu3dtel_t _3dTransducerElementList = iuf3DTransducerElementListCreate(num3DTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU3DTEL_INVALID, _3dTransducerElementList);
    int status = iuf3DTransducerElementListDelete(_3dTransducerElementList);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0, iufErrorGetCount());

    status = iuf3DTransducerElementListDelete(NULL);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE,status);

    TEST_ASSERT_EQUAL(1, iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

}

TEST(Iuf3DTransducerElementList, testIuf3DTransducerElementListCompare)
{
    // Put your test code here
    int num3DTransducerElements = 100;
    int status;
    IUF_BOOL equal;
    const int numTransducerElements = 128;
    const float transducerPitch = 0.000005f;
    iu3dp_t elemPos = iuf3DPositionCreate((float)(10 - numTransducerElements / 2.0)*transducerPitch, 0.0f, 0.0f);
    iu3ds_t elemSize = iuf3DSizeCreate(0.0001f,0.0001f,0.0001f);
    iu3da_t elemAngle = iuf3DAngleCreate(0.0f,0.3f);

    // Empty lists should be equal
    iu3dtel_t _3dTransducerElementList = iuf3DTransducerElementListCreate(num3DTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU3DTEL_INVALID, _3dTransducerElementList);
    iu3dtel_t nother3DTransducerElementList = iuf3DTransducerElementListCreate(num3DTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU3DTEL_INVALID, nother3DTransducerElementList);
    equal = iuf3DTransducerElementListCompare(_3dTransducerElementList, nother3DTransducerElementList);
    TEST_ASSERT_EQUAL(IUF_TRUE, equal);


    iu3dte_t element = iuf3DTransducerElementCreate(elemPos, elemAngle, elemSize);
    TEST_ASSERT_NOT_EQUAL(IU3DTE_INVALID,element);

    // Change one list..add bmode
    status = iuf3DTransducerElementListSet(_3dTransducerElementList, element, 0);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);
    equal = iuf3DTransducerElementListCompare(_3dTransducerElementList, nother3DTransducerElementList);
    TEST_ASSERT_EQUAL(IUF_FALSE, equal);

    // Change other
    status = iuf3DTransducerElementListSet(nother3DTransducerElementList, element, 0);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);
    equal = iuf3DTransducerElementListCompare(_3dTransducerElementList, nother3DTransducerElementList);
    TEST_ASSERT_EQUAL(IUF_TRUE, equal);

    iuf3DTransducerElementListDelete(_3dTransducerElementList);
    iuf3DTransducerElementListDelete(nother3DTransducerElementList);
    iuf3DTransducerElementDelete(element);
    iuf3DPositionDelete(elemPos);
    iuf3DSizeDelete(elemSize);
    iuf3DAngleDelete(elemAngle);
}

TEST(Iuf3DTransducerElementList, testIuf3DTransducerElementListSetGet)
{
    int num3DTransducerElements = 2;
    int status;
    //IUF_BOOL equal;

    const int numTransducerElements = 128;
    const float transducerPitch = 0.000005f;
    iu3dp_t elemPos = iuf3DPositionCreate((float)(10 - numTransducerElements / 2.0)*transducerPitch, 0.0f, 0.0f);
    iu3ds_t elemSize = iuf3DSizeCreate(0.0001f,0.0001f,0.0001f);
    iu3da_t elemAngle = iuf3DAngleCreate(0.0f,0.3f);
    iu3dte_t element = iuf3DTransducerElementCreate(elemPos, elemAngle, elemSize);

    iu3dtel_t _3dTransducerElementList = iuf3DTransducerElementListCreate(num3DTransducerElements);
    status = iuf3DTransducerElementListSet(_3dTransducerElementList,element,0);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    // invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0, iufErrorGetCount());

    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, iuf3DTransducerElementListSet(NULL,element,0));
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, iuf3DTransducerElementListSet(_3dTransducerElementList,element,-1));
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, iuf3DTransducerElementListSet(_3dTransducerElementList,element,num3DTransducerElements));

    TEST_ASSERT_EQUAL(3, iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

    iuf3DTransducerElementListDelete(_3dTransducerElementList);
    iuf3DTransducerElementDelete(element);
    iuf3DPositionDelete(elemPos);
    iuf3DSizeDelete(elemSize);
    iuf3DAngleDelete(elemAngle);
}

TEST(Iuf3DTransducerElementList, testIuf3DTransducerElementListSerialization)
{
    int i, num3DTransducerElements = 2;
    int status;
    IUF_BOOL equal;
    char *pFilename = "testIuf3DTransducerElementListSerialization.hdf5";
    //char *p3DTransducerElementListPath = "/3DTransducerElementList";
    const float transducerPitch = 0.000005f;

    // Non-happy flow
    // for lists it should not be possible to save, if not all elements have been
    // filled.
    iu3dtel_t _3dTransducerElementList = iuf3DTransducerElementListCreate(num3DTransducerElements);
    TEST_ASSERT_NOT_EQUAL(IU3DTEL_INVALID, _3dTransducerElementList);
    hid_t handle = H5Fcreate(pFilename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    TEST_ASSERT(handle > 0);
    status = iuf3DTransducerElementListSave(_3dTransducerElementList, handle);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, status);
    status = H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);

    // Happy Flow
    //
    // fill list
    for (i = 0; i < num3DTransducerElements; i++)
    {
        iu3dp_t elemPos = iuf3DPositionCreate((float)(10 - num3DTransducerElements / 2.0)*transducerPitch, 0.0f, 0.0f);
        iu3ds_t elemSize = iuf3DSizeCreate(0.0001f,0.0001f,0.0001f);
        iu3da_t elemAngle = iuf3DAngleCreate(0.0f,0.3f);
        iu3dte_t element = iuf3DTransducerElementCreate(elemPos, elemAngle, elemSize);
        iuf3DTransducerElementListSet(_3dTransducerElementList, element, i);
    }

    // save
    handle = H5Fcreate(pFilename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    TEST_ASSERT(handle > 0);
    status = iuf3DTransducerElementListSave(_3dTransducerElementList, handle);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);
    status = H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);

    // read back
    handle = H5Fopen(pFilename, H5F_ACC_RDONLY, H5P_DEFAULT);
    iu3dtel_t saved3DTransducerElementList = iuf3DTransducerElementListLoad(handle);
    TEST_ASSERT_NOT_EQUAL(IU3DTEL_INVALID, saved3DTransducerElementList);
    status |= H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);

    // compare
    equal = iuf3DTransducerElementListCompare(_3dTransducerElementList, saved3DTransducerElementList);
    TEST_ASSERT_EQUAL(IUF_TRUE, equal);

    // cleanup
    for (i = 0; i < num3DTransducerElements; i++)
    {
        iu3dte_t element = iuf3DTransducerElementListGet(_3dTransducerElementList, i);
        iu3dp_t elemPos = iuf3DTransducerElementGetPosition(element);
        iu3ds_t elemSize = iuf3DTransducerElementGetSize(element);
        iu3da_t elemAngle = iuf3DTransducerElementGetAngle(element);
        iuf3DAngleDelete(elemAngle);
        iuf3DSizeDelete(elemSize);
        iuf3DPositionDelete(elemPos);
        iuf3DTransducerElementDelete(element);
    }
    iuf3DTransducerElementListDelete(_3dTransducerElementList);
    iuf3DTransducerElementListDelete(saved3DTransducerElementList);

}

TEST_GROUP_RUNNER(Iuf3DTransducerElementList)
{
    RUN_TEST_CASE(Iuf3DTransducerElementList, testIuf3DTransducerElementListDelete);
    RUN_TEST_CASE(Iuf3DTransducerElementList, testIuf3DTransducerElementListCreate);
    RUN_TEST_CASE(Iuf3DTransducerElementList, testIuf3DTransducerElementListSetGet);
    RUN_TEST_CASE(Iuf3DTransducerElementList, testIuf3DTransducerElementListCompare);
    RUN_TEST_CASE(Iuf3DTransducerElementList, testIuf3DTransducerElementListSerialization);
}
