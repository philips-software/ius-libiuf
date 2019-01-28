//
// Created by nlv09165 on 10/10/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <ius.h>

static char *pErrorFilename = "IusDataStream.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(IusDataStream);

TEST_SETUP(IusDataStream)
{
    iusErrorLogClear();
    iusErrorLog(IUS_TRUE);
    iusErrorAutoReport(IUS_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iusErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IusDataStream)
{
    iusErrorLogClear();
    iusErrorLog(IUS_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iusErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
}


TEST(IusDataStream, testIusDataStreamCreate)
{
    iuds_t obj = iusDataStreamCreate();
    iuds_t notherObj = iusDataStreamCreate();
    TEST_ASSERT(obj != IUDS_INVALID);
    TEST_ASSERT(notherObj != IUDS_INVALID);

    TEST_ASSERT_EQUAL(H5I_INVALID_HID, obj->fileChunkConfig);
    TEST_ASSERT_EQUAL(H5I_INVALID_HID, obj->rfDataset);
    iusDataStreamDelete(obj);
    iusDataStreamDelete(notherObj);
}

TEST(IusDataStream, testIusDataStreamDelete)
{
    iuds_t obj = iusDataStreamCreate();
    TEST_ASSERT(obj != IUDS_INVALID);
    int status = iusDataStreamDelete(obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());

    status = iusDataStreamDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);

    TEST_ASSERT_EQUAL(1,iusErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
}


TEST_GROUP_RUNNER(IusDataStream)
{
    RUN_TEST_CASE(IusDataStream, testIusDataStreamCreate);
    RUN_TEST_CASE(IusDataStream, testIusDataStreamDelete);
}
