//
// Created by nlv09165 on 31/07/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <ius.h>
#include <iusDataStreamDictPrivate.h>
#include <testDataGenerators.h>

TEST_GROUP(IusDataStreamDict);

TEST_SETUP(IusDataStreamDict)
{
}

TEST_TEAR_DOWN(IusDataStreamDict)
{
}


TEST(IusDataStreamDict, testIusDataStreamDictCreate)
{
    iudsd_t obj = iusDataStreamDictCreate();
    iudsd_t notherObj = iusDataStreamDictCreate();
    TEST_ASSERT(obj != IUDSD_INVALID);
    TEST_ASSERT(notherObj != IUDSD_INVALID);
    iusDataStreamDictDelete(obj);
    iusDataStreamDictDelete(notherObj);
}


TEST(IusDataStreamDict, testIusDataStreamDictSetGet)
{
    char *key = "testKey";
    iuds_t value = iusDataStreamCreate();
    iudsd_t dict = iusDataStreamDictCreate();
    TEST_ASSERT_EQUAL(IUS_E_OK, iusDataStreamDictSet(dict, key, value));
    TEST_ASSERT_EQUAL_STRING(value, iusDataStreamDictGet(dict, key));

    // invalid args
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, iusDataStreamDictSet(NULL, key, value));
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, iusDataStreamDictSet(dict, NULL, value));
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, iusDataStreamDictSet(dict, key, NULL));

    TEST_ASSERT_EQUAL(NULL, iusDataStreamDictGet(dict, NULL));
    TEST_ASSERT_EQUAL(NULL, iusDataStreamDictGet(dict, "unknownKey"));
}

TEST(IusDataStreamDict, testIusDataStreamDictCompare)
{
    int elementID,status;

    IUS_BOOL equal;
    iudsd_t dict = iusDataStreamDictCreate();
    iudsd_t notherDict = iusDataStreamDictCreate();
    TEST_ASSERT(dict != IUDSD_INVALID);
    TEST_ASSERT(notherDict != IUDSD_INVALID);


    equal = iusDataStreamDictCompare(dict, dict);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = iusDataStreamDictCompare(dict, notherDict);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);

    char generatedKey[1024];
    iuds_t generatedValue;
    for (elementID=0;elementID<1000;elementID++)
    {
        sprintf(generatedKey,"key_%d", elementID);
        generatedValue = iusDataStreamCreate();
        generatedValue->fileChunkConfig=(hid_t)elementID;
        generatedValue->rfDataset=(hid_t)elementID+1;
        status = iusDataStreamDictSet(dict, generatedKey, generatedValue);
        TEST_ASSERT_EQUAL(IUS_E_OK,status);
        TEST_ASSERT_EQUAL(generatedValue,iusDataStreamDictGet(dict, generatedKey));
        equal = iusDataStreamDictCompare(dict, notherDict);
        TEST_ASSERT_EQUAL(IUS_FALSE,equal);

        generatedValue = iusDataStreamCreate();
        generatedValue->fileChunkConfig=(hid_t)elementID;
        generatedValue->rfDataset=(hid_t)elementID+1;
        status = iusDataStreamDictSet(notherDict, generatedKey, generatedValue);
        TEST_ASSERT_EQUAL(IUS_E_OK,status);
        TEST_ASSERT_EQUAL_STRING(generatedValue,iusDataStreamDictGet(notherDict, generatedKey));
        equal = iusDataStreamDictCompare(dict, notherDict);
        TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    }

    iusDataStreamDictDelete(dict);
    iusDataStreamDictDelete(notherDict);
}


TEST_GROUP_RUNNER(IusDataStreamDict)
{
    RUN_TEST_CASE(IusDataStreamDict, testIusDataStreamDictCreate);
    RUN_TEST_CASE(IusDataStreamDict, testIusDataStreamDictCompare);
    RUN_TEST_CASE(IusDataStreamDict, testIusDataStreamDictSetGet);
}
