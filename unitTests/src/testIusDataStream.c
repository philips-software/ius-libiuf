//
// Created by nlv09165 on 10/10/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <include/ius.h>
#include <include/iusError.h>
#include <include/iusTypes.h>
#include "include/iusDataStream.h"

TEST_GROUP(IusDataStream);

TEST_SETUP(IusDataStream)
{
}

TEST_TEAR_DOWN(IusDataStream)
{
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
    status = iusDataStreamDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
}


TEST_GROUP_RUNNER(IusDataStream)
{
    RUN_TEST_CASE(IusDataStream, testIusDataStreamCreate);
    RUN_TEST_CASE(IusDataStream, testIusDataStreamDelete);
}
