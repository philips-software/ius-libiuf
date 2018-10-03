//
// Created by nlv09165 on 15/08/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusFrameImp.h>

TEST_GROUP(IusFrame);

TEST_SETUP(IusFrame)
{
}

TEST_TEAR_DOWN(IusFrame)
{
}

TEST(IusFrame, testIusFrameCreate)
{
    iuf_t obj = iusFrameCreate(0,1,1.0f);
    iuf_t notherObj = iusFrameCreate(3,8,2.0f);
    TEST_ASSERT(obj != IUF_INVALID);
    TEST_ASSERT(notherObj != IUF_INVALID);
    iusFrameDelete(obj);
    iusFrameDelete(notherObj);

    // invalid params
    obj = iusFrameCreate(-1,1,1.0f);
    TEST_ASSERT(obj == IUF_INVALID);
    obj = iusFrameCreate(1,-2,1.0f);
    TEST_ASSERT(obj == IUF_INVALID);
}

TEST(IusFrame, testIusFrameDelete)
{
    iuf_t obj = iusFrameCreate(1,2,1.0f);
    TEST_ASSERT(obj != IUF_INVALID);
    int status = iusFrameDelete(obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // invalid params
    status = iusFrameDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
}

TEST(IusFrame, testIusFrameCompare)
{
    IUS_BOOL equal;
    iuf_t obj = iusFrameCreate(1,2,0.01f);
    iuf_t notherObj = iusFrameCreate(1,2,0.01f);
    iuf_t differentObj = iusFrameCreate(3,4,0.11f);
    TEST_ASSERT(obj != IUF_INVALID);
    TEST_ASSERT(notherObj != IUF_INVALID);
    equal = iusFrameCompare(obj,obj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = iusFrameCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = iusFrameCompare(obj,differentObj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

    // invalid params
    equal = iusFrameCompare(obj,NULL);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    equal = iusFrameCompare(NULL,obj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    
    iusFrameDelete(obj);
    iusFrameDelete(notherObj);
    iusFrameDelete(differentObj);
}

TEST(IusFrame, testIusFrameSetGet)
{
    iuf_t obj = iusFrameCreate(1,2,0.01f);
    TEST_ASSERT(obj != IUF_INVALID);
    iuf_t notherObj = iusFrameCreate(101,202,0.03f);
    TEST_ASSERT(notherObj != IUF_INVALID);

    // int param
    TEST_ASSERT_EQUAL(1,iusFrameGetPatternListIndex(obj));
    TEST_ASSERT_EQUAL(2,iusFrameGetDataIndex(obj));
    TEST_ASSERT_EQUAL_FLOAT(0.01f,iusFrameGetTime(obj));
    TEST_ASSERT_EQUAL(101,iusFrameGetPatternListIndex(notherObj));
    TEST_ASSERT_EQUAL(202,iusFrameGetDataIndex(notherObj));
    TEST_ASSERT_EQUAL_FLOAT(0.03f,iusFrameGetTime(notherObj));

    // invalid param
    TEST_ASSERT_EQUAL(-1,iusFrameGetPatternListIndex(NULL));
    TEST_ASSERT_EQUAL(-1,iusFrameGetDataIndex(NULL));
    iusFrameDelete(obj);
}


TEST(IusFrame, testIusSerialization)
{
    char *filename = "testIusFrameSerialization.hdf5";
    //char *framePath =  "/Frame";

    int patternListIndex=101;
    int dataIndex=202;
    float time=0.1f;

    // create and save
    iuf_t obj = iusFrameCreate(patternListIndex,dataIndex,time);

    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
    int status = iusFrameSave( obj, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iuf_t savedObj = iusFrameLoad(handle);
    TEST_ASSERT_NOT_EQUAL(NULL, savedObj);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUS_TRUE, iusFrameCompare(obj,savedObj));
    iusFrameDelete(obj);
    iusFrameDelete(savedObj);
}
TEST_GROUP_RUNNER(IusFrame)
{
    RUN_TEST_CASE(IusFrame, testIusFrameCreate);
    RUN_TEST_CASE(IusFrame, testIusFrameDelete);
    RUN_TEST_CASE(IusFrame, testIusFrameCompare);
    RUN_TEST_CASE(IusFrame, testIusFrameSetGet);
    RUN_TEST_CASE(IusFrame, testIusSerialization);
}
