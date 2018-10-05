

//
// Created by nlv09165 on 08/08/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <include/ius.h>
#include <include/iusError.h>
#include <include/iusTypes.h>
#include "include/iusTGCPrivate.h"

TEST_GROUP(IusTGC);

TEST_SETUP(IusTGC)
{
}

TEST_TEAR_DOWN(IusTGC)
{
}


TEST(IusTGC, testIusTGCCreate)
{
    int status = 0;
    int numTGCValues;
    iutgc_t tgc;

    numTGCValues = 20;
    tgc = iusTGCCreate(numTGCValues);
    TEST_ASSERT(tgc != IUTGC_INVALID);

    // Invalid operation on nonparametric dta type
    TEST_ASSERT_EQUAL(NULL, iusTGCCreate(-1));

    status = iusTGCDelete(tgc);
    TEST_ASSERT(status == IUS_E_OK);
}

TEST(IusTGC, testIusTGCDelete)
{
    int numTGCValues=2;
    iutgc_t obj = iusTGCCreate(numTGCValues);
    TEST_ASSERT(obj != IUTGC_INVALID);
    int status = iusTGCDelete(obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // invalid params
    status = iusTGCDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
}



TEST(IusTGC, testIusTGCCompare)
{
    IUS_BOOL equal;
    int numTGCValues=2;
    iutgc_t obj = iusTGCCreate(numTGCValues);
    iutgc_t sameObj = iusTGCCreate(numTGCValues);
    iutgc_t notherObj = iusTGCCreate(numTGCValues + 10);
    TEST_ASSERT(obj != IUTGC_INVALID);
    TEST_ASSERT(notherObj != IUTGC_INVALID);

    equal = iusTGCCompare(obj,obj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = iusTGCCompare(obj,sameObj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = iusTGCCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

    int status = iusTGCSet(obj,0,1.0f,2.0f);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    equal = iusTGCCompare(obj,sameObj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    status = iusTGCSet(sameObj,0,1.0f,2.0f);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    equal = iusTGCCompare(obj,sameObj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);

    // invalid params
    equal = iusTGCCompare(obj,NULL);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    equal = iusTGCCompare(NULL,obj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    
    iusTGCDelete(obj);
    iusTGCDelete(sameObj);
    iusTGCDelete(notherObj);
}


TEST(IusTGC, testIusTGCSetGet)
{
//    IUS_BOOL equal;
    int numTGCValues=2;
    iutgc_t obj = iusTGCCreate(numTGCValues);
    TEST_ASSERT(obj != IUTGC_INVALID);

    iusTGCSet(obj,0,1.0f,2.0f);
    // float param
    TEST_ASSERT_EQUAL_FLOAT(1.0f, iusTGCGetTime(obj, 0));
    TEST_ASSERT_EQUAL_FLOAT(2.0f, iusTGCGetGain(obj, 0));


    // invalid param
    TEST_ASSERT_EQUAL_FLOAT(NAN, iusTGCGetTime(NULL, 1));
    TEST_ASSERT_EQUAL_FLOAT(NAN, iusTGCGetGain(NULL, 1));
    TEST_ASSERT_EQUAL_FLOAT(NAN, iusTGCGetTime(NULL, 0));
    TEST_ASSERT_EQUAL_FLOAT(NAN, iusTGCGetGain(NULL, 0));
    iusTGCDelete(obj);
}


TEST(IusTGC, testIusSerialization)
{
    char *filename = "testIusTGCSerialization.hdf5";
    //char *pulsePath =  "/TGC";
    char *label = "Created_in_testIusSerialization";

    int numTGCValues = 20;

    // create
    iutgc_t TGC = iusTGCCreate(numTGCValues);

    // fill
    int i,status;
    for (i=0;i<numTGCValues;i++)
    {
        status = iusTGCSet(TGC,i,i*1.0f,i*2.0f);
        TEST_ASSERT_EQUAL(IUS_E_OK,status);
    }

    // save
    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
    status = iusTGCSave(TGC, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iutgc_t savedObj = iusTGCLoad(handle);
    TEST_ASSERT(savedObj != NULL);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUS_TRUE, iusTGCCompare(TGC, savedObj));
    iusTGCDelete(TGC);
    iusTGCDelete(savedObj);
}

TEST_GROUP_RUNNER(IusTGC)
{
    RUN_TEST_CASE(IusTGC, testIusTGCCreate);
    RUN_TEST_CASE(IusTGC, testIusTGCDelete);
    RUN_TEST_CASE(IusTGC, testIusTGCCompare);
    RUN_TEST_CASE(IusTGC, testIusTGCSetGet);
    RUN_TEST_CASE(IusTGC, testIusSerialization);
}
