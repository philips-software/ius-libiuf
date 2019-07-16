#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <iuf.h>

static char *pErrorFilename = "IufDataStream.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(IufDataStream);

TEST_SETUP(IufDataStream)
{
    iufErrorLogClear();
    iufErrorLog(IUF_TRUE);
    iufErrorAutoReportSet(IUF_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iufErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IufDataStream)
{
    iufErrorLogClear();
    iufErrorLog(IUF_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iufErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
}


TEST(IufDataStream, testIufDataStreamCreate)
{
    iuds_t obj = iufDataStreamCreate();
    iuds_t notherObj = iufDataStreamCreate();
    TEST_ASSERT(obj != IUDS_INVALID);
    TEST_ASSERT(notherObj != IUDS_INVALID);

    TEST_ASSERT_EQUAL(H5I_INVALID_HID, obj->fileChunkConfig);
    TEST_ASSERT_EQUAL(H5I_INVALID_HID, obj->rfDataset);
    iufDataStreamDelete(obj);
    iufDataStreamDelete(notherObj);
}

TEST(IufDataStream, testIufDataStreamDelete)
{
    iuds_t obj = iufDataStreamCreate();
    TEST_ASSERT(obj != IUDS_INVALID);
    int status = iufDataStreamDelete(obj);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    // invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    status = iufDataStreamDelete(NULL);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, status);

    TEST_ASSERT_EQUAL(1,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
}


TEST_GROUP_RUNNER(IufDataStream)
{
    RUN_TEST_CASE(IufDataStream, testIufDataStreamCreate);
    RUN_TEST_CASE(IufDataStream, testIufDataStreamDelete);
}
