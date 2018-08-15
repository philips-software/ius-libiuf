//
// Created by nlv09165 on 15/08/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusHLFrameImp.h>

TEST_GROUP(IusFrame);

TEST_SETUP(IusFrame)
{
}

TEST_TEAR_DOWN(IusFrame)
{
}

TEST(IusFrame, testIusFrameCreate)
{
    iuf_t obj = iusHLFrameCreate(0,1,1.0f);
    iuf_t notherObj = iusHLFrameCreate(3,8,2.0f);
    TEST_ASSERT(obj != IUF_INVALID);
    TEST_ASSERT(notherObj != IUF_INVALID);
    iusHLFrameDelete(obj);
    iusHLFrameDelete(notherObj);

    // invalid params
    obj = iusHLFrameCreate(-1,1,1.0f);
    TEST_ASSERT(obj == IUF_INVALID);
    obj = iusHLFrameCreate(1,-2,1.0f);
    TEST_ASSERT(obj == IUF_INVALID);
}

TEST(IusFrame, testIusFrameDelete)
{
    iuf_t obj = iusHLFrameCreate(1,2,1.0f);
    TEST_ASSERT(obj != IUF_INVALID);
    int status = iusHLFrameDelete(obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // invalid params
    status = iusHLFrameDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
}

TEST(IusFrame, testIusFrameCompare)
{
    IUS_BOOL equal;
    iuf_t obj = iusHLFrameCreate(1,2,0.01f);
    iuf_t notherObj = iusHLFrameCreate(1,2,0.01f);
    iuf_t differentObj = iusHLFrameCreate(3,4,0.11f);
    TEST_ASSERT(obj != IUF_INVALID);
    TEST_ASSERT(notherObj != IUF_INVALID);
    equal = iusHLFrameCompare(obj,obj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = iusHLFrameCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = iusHLFrameCompare(obj,differentObj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

    // invalid params
    equal = iusHLFrameCompare(obj,NULL);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    equal = iusHLFrameCompare(NULL,obj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    
    iusHLFrameDelete(obj);
    iusHLFrameDelete(notherObj);
    iusHLFrameDelete(differentObj);
}

TEST(IusFrame, testIusFrameSetGet)
{
    iuf_t obj = iusHLFrameCreate(1,2,0.01f);
    TEST_ASSERT(obj != IUF_INVALID);
    iuf_t notherObj = iusHLFrameCreate(101,202,0.03f);
    TEST_ASSERT(notherObj != IUF_INVALID);

    // int param
    TEST_ASSERT_EQUAL(1,iusHLFrameGetPatternListIndex(obj));
    TEST_ASSERT_EQUAL(2,iusHLFrameGetDataIndex(obj));
    TEST_ASSERT_EQUAL_FLOAT(0.01f,iusHLFrameGetTime(obj));
    TEST_ASSERT_EQUAL(101,iusHLFrameGetPatternListIndex(notherObj));
    TEST_ASSERT_EQUAL(202,iusHLFrameGetDataIndex(notherObj));
    TEST_ASSERT_EQUAL_FLOAT(0.03f,iusHLFrameGetTime(notherObj));

    // invalid param
    TEST_ASSERT_EQUAL(-1,iusHLFrameGetPatternListIndex(NULL));
    TEST_ASSERT_EQUAL(-1,iusHLFrameGetDataIndex(NULL));
    iusHLFrameDelete(obj);
}


TEST(IusFrame, testIusSerialization)
{
    char *filename = "testIusFrameSerialization.hdf5";
    char *framePath =  "/Frame";

    int patternListIndex=101;
    int dataIndex=202;
    float time=0.1f;

    // create and save
    iuf_t obj = iusHLFrameCreate(patternListIndex,dataIndex,time);

    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
    int status = iusHLFrameSave( obj, framePath, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iuf_t savedObj = iusHLFrameLoad(handle, framePath);
    TEST_ASSERT_NOT_EQUAL(NULL, savedObj);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUS_TRUE, iusHLFrameCompare(obj,savedObj));
    iusHLFrameDelete(obj);
    iusHLFrameDelete(savedObj);
}
TEST_GROUP_RUNNER(IusFrame)
{
    RUN_TEST_CASE(IusFrame, testIusFrameCreate);
    RUN_TEST_CASE(IusFrame, testIusFrameDelete);
    RUN_TEST_CASE(IusFrame, testIusFrameCompare);
    RUN_TEST_CASE(IusFrame, testIusFrameSetGet);
    RUN_TEST_CASE(IusFrame, testIusSerialization);
}
