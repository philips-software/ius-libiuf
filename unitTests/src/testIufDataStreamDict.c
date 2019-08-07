#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <iuf.h>
#include <iufDataStreamDictPrivate.h>
#include <dg/dataGenerators.h>

static char *pErrorFilename = "IufDataStreamDict.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(IufDataStreamDict);

TEST_SETUP(IufDataStreamDict)
{
    iufErrorLogClear();
    iufErrorLog(IUF_TRUE);
    iufErrorAutoReportSet(IUF_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iufErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IufDataStreamDict)
{
    iufErrorLogClear();
    iufErrorLog(IUF_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iufErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
}


TEST(IufDataStreamDict, testIufDataStreamDictCreate)
{
    iudsd_t obj = iufDataStreamDictCreate();
    iudsd_t notherObj = iufDataStreamDictCreate();
    TEST_ASSERT(obj != IUDSD_INVALID);
    TEST_ASSERT(notherObj != IUDSD_INVALID);
    iufDataStreamDictDelete(obj);
    iufDataStreamDictDelete(notherObj);
}


TEST(IufDataStreamDict, testIufDataStreamDictSetGet)
{
    char *key = "testKey";
    iuds_t value = iufDataStreamCreate();
    iudsd_t dict = iufDataStreamDictCreate();
    TEST_ASSERT_EQUAL(IUF_E_OK, iufDataStreamDictSet(dict, key, value));
    TEST_ASSERT_EQUAL_FLOAT(value->rfDataset,iufDataStreamDictGet(dict, key)->rfDataset);
    TEST_ASSERT_EQUAL_FLOAT(value->fileChunkConfig,iufDataStreamDictGet(dict, key)->fileChunkConfig);

    // invalid args
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, iufDataStreamDictSet(NULL, key, value));
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, iufDataStreamDictSet(dict, NULL, value));
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, iufDataStreamDictSet(dict, key, NULL));

    TEST_ASSERT_EQUAL(NULL, iufDataStreamDictGet(dict, NULL));
    TEST_ASSERT_EQUAL(NULL, iufDataStreamDictGet(dict, "unknownKey"));
    TEST_ASSERT_EQUAL(NULL, iufDataStreamDictGet(NULL, ""));


    TEST_ASSERT_EQUAL(6,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));


    iufDataStreamDictDelete(dict);
}

TEST(IufDataStreamDict, testIufDataStreamDictCompare)
{
    int elementID,status;

    IUF_BOOL equal;
    iudsd_t dict = iufDataStreamDictCreate();
    iudsd_t notherDict = iufDataStreamDictCreate();
    TEST_ASSERT(dict != IUDSD_INVALID);
    TEST_ASSERT(notherDict != IUDSD_INVALID);


    equal = iufDataStreamDictCompare(dict, dict);
    TEST_ASSERT_EQUAL(IUF_TRUE,equal);
    equal = iufDataStreamDictCompare(dict, notherDict);
    TEST_ASSERT_EQUAL(IUF_TRUE,equal);

    char generatedKey[1024];
    iuds_t generatedValue;
    for (elementID=0;elementID<1000;elementID++)
    {
        snprintf(generatedKey,sizeof(generatedKey),"key_%d", elementID);
        generatedValue = iufDataStreamCreate();
        generatedValue->fileChunkConfig=(hid_t)elementID;
        generatedValue->rfDataset=(hid_t)elementID+1;
        status = iufDataStreamDictSet(dict, generatedKey, generatedValue);
        TEST_ASSERT_EQUAL(IUF_E_OK,status);
        TEST_ASSERT_EQUAL(generatedValue,iufDataStreamDictGet(dict, generatedKey));
        equal = iufDataStreamDictCompare(dict, notherDict);
        TEST_ASSERT_EQUAL(IUF_FALSE,equal);

        generatedValue = iufDataStreamCreate();
        generatedValue->fileChunkConfig=(hid_t)elementID;
        generatedValue->rfDataset=(hid_t)elementID+1;
        status = iufDataStreamDictSet(notherDict, generatedKey, generatedValue);
        TEST_ASSERT_EQUAL(IUF_E_OK,status);
        TEST_ASSERT_EQUAL_STRING(generatedValue,iufDataStreamDictGet(notherDict, generatedKey));
        equal = iufDataStreamDictCompare(dict, notherDict);
        TEST_ASSERT_EQUAL(IUF_TRUE,equal);
    }

    iufDataStreamDictDelete(dict);
    iufDataStreamDictDelete(notherDict);
}


TEST_GROUP_RUNNER(IufDataStreamDict)
{
    RUN_TEST_CASE(IufDataStreamDict, testIufDataStreamDictCreate);
    RUN_TEST_CASE(IufDataStreamDict, testIufDataStreamDictCompare);
    RUN_TEST_CASE(IufDataStreamDict, testIufDataStreamDictSetGet);
}
