#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <iuf.h>
#include <iufFramePrivate.h>

static char *pErrorFilename = "IufFrame.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(IufFrame);

TEST_SETUP(IufFrame)
{
    iufErrorLogClear();
    iufErrorLog(IUF_TRUE);
    iufErrorAutoReportSet(IUF_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iufErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IufFrame)
{
    iufErrorLogClear();
    iufErrorLog(IUF_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iufErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
}

TEST(IufFrame, testIufFrameCreate)
{
    iufr_t obj = iufFrameCreate("bmode", 1, 1.0f);
    iufr_t notherObj = iufFrameCreate("doppler", 8, 2.0f);
    TEST_ASSERT(obj != IUF_INVALID);
    TEST_ASSERT(notherObj != IUF_INVALID);
    iufFrameDelete(obj);
    iufFrameDelete(notherObj);

    // invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    obj = iufFrameCreate(NULL, 1, 1.0f);
    TEST_ASSERT(obj == IUF_INVALID);
    obj = iufFrameCreate("", 1, 1.0f);
    TEST_ASSERT(obj == IUF_INVALID);
    obj = iufFrameCreate("test",-2, 1.0f);
    TEST_ASSERT(obj == IUF_INVALID);

    TEST_ASSERT_EQUAL(3,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
}

TEST(IufFrame, testIufFrameDelete)
{
    iufr_t obj = iufFrameCreate("test",2,1.0f);
    TEST_ASSERT(obj != IUF_INVALID);
    int status = iufFrameDelete(obj);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    // invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    status = iufFrameDelete(NULL);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, status);

    TEST_ASSERT_EQUAL(1,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
}

TEST(IufFrame, testIufFrameCompare)
{
    IUF_BOOL equal;
    iufr_t obj = iufFrameCreate("test",2,0.01f);
    iufr_t notherObj = iufFrameCreate("test",2,0.01f);
    iufr_t differentObj = iufFrameCreate("test",4,0.11f);
    TEST_ASSERT(obj != IUF_INVALID);
    TEST_ASSERT(notherObj != IUF_INVALID);
    equal = iufFrameCompare(obj,obj);
    TEST_ASSERT_EQUAL(IUF_TRUE,equal);
    equal = iufFrameCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUF_TRUE,equal);
    equal = iufFrameCompare(obj,differentObj);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);

    // invalid params
    equal = iufFrameCompare(obj,NULL);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);
    equal = iufFrameCompare(NULL,obj);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);
    
    iufFrameDelete(obj);
    iufFrameDelete(notherObj);
    iufFrameDelete(differentObj);
}

TEST(IufFrame, testIufFrameSetGet)
{
    iufr_t obj = iufFrameCreate("test",2,0.01f);
    TEST_ASSERT(obj != IUF_INVALID);
    iufr_t notherObj = iufFrameCreate("bmode",202,0.03f);
    TEST_ASSERT(notherObj != IUF_INVALID);

    // int param
    TEST_ASSERT_EQUAL_STRING("test",iufFrameGetPatternListLabel(obj));
    TEST_ASSERT_EQUAL(2,iufFrameGetDataIndex(obj));
    TEST_ASSERT_EQUAL_FLOAT(0.01f,iufFrameGetTime(obj));
    TEST_ASSERT_EQUAL_STRING("bmode",iufFrameGetPatternListLabel(notherObj));
    TEST_ASSERT_EQUAL(202,iufFrameGetDataIndex(notherObj));
    TEST_ASSERT_EQUAL_FLOAT(0.03f,iufFrameGetTime(notherObj));

    // invalid param
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    TEST_ASSERT_EQUAL(NULL,iufFrameGetPatternListLabel(NULL));
    TEST_ASSERT_EQUAL(-1,iufFrameGetDataIndex(NULL));

    TEST_ASSERT_EQUAL(2,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

    iufFrameDelete(obj);
    iufFrameDelete(notherObj);
}


TEST(IufFrame, testIufSerialization)
{
    char *filename = "testIufFrameSerialization.hdf5";

    char *patternListLabel="test";
    int dataIndex=202;
    float time=0.1f;

    // create and save
    iufr_t obj = iufFrameCreate(patternListLabel, dataIndex, time);

    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
    int status = iufFrameSave( obj, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iufr_t savedObj = iufFrameLoad(handle);
    TEST_ASSERT_NOT_EQUAL(NULL, savedObj);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUF_TRUE, iufFrameCompare(obj,savedObj));
    iufFrameDelete(obj);
    iufFrameDelete(savedObj);
}
TEST_GROUP_RUNNER(IufFrame)
{
    RUN_TEST_CASE(IufFrame, testIufFrameCreate);
    RUN_TEST_CASE(IufFrame, testIufFrameDelete);
    RUN_TEST_CASE(IufFrame, testIufFrameCompare);
    RUN_TEST_CASE(IufFrame, testIufFrameSetGet);
    RUN_TEST_CASE(IufFrame, testIufSerialization);
}
