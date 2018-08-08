

//
// Created by nlv09165 on 08/08/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <include/ius.h>
#include <include/iusError.h>
#include <include/iusTypes.h>
#include "include/iusHLTGCImp.h"

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
    tgc = iusHLTGCCreate(numTGCValues);
    TEST_ASSERT(tgc != IUTGC_INVALID);

    // Invalid operation on nonparametric dta type
    TEST_ASSERT_EQUAL(NULL, iusHLTGCCreate(-1));

    status = iusHLTGCDelete(tgc);
    TEST_ASSERT(status == IUS_E_OK);
}

TEST(IusTGC, testIusTGCDelete)
{
    int numTGCValues=2;
    iutgc_t obj = iusHLTGCCreate(numTGCValues);
    TEST_ASSERT(obj != IUTGC_INVALID);
    int status = iusHLTGCDelete(obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // invalid params
    status = iusHLTGCDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
}



TEST(IusTGC, testIusTGCCompare)
{
    IUS_BOOL equal;
    int numTGCValues=2;
    iutgc_t obj = iusHLTGCCreate(numTGCValues);
    iutgc_t sameObj = iusHLTGCCreate(numTGCValues);
    iutgc_t notherObj = iusHLTGCCreate(numTGCValues + 10);
    TEST_ASSERT(obj != IUTGC_INVALID);
    TEST_ASSERT(notherObj != IUTGC_INVALID);

    equal = iusHLTGCCompare(obj,obj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = iusHLTGCCompare(obj,sameObj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = iusHLTGCCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

    int status = iusHLTGCSet(obj,0,1.0f,2.0f);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    equal = iusHLTGCCompare(obj,sameObj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    status = iusHLTGCSet(sameObj,0,1.0f,2.0f);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    equal = iusHLTGCCompare(obj,sameObj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);

    // invalid params
    equal = iusHLTGCCompare(obj,NULL);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    equal = iusHLTGCCompare(NULL,obj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    
    iusHLTGCDelete(obj);
    iusHLTGCDelete(notherObj);
}


TEST(IusTGC, testIusTGCSetGet)
{
    IUS_BOOL equal;
    int numTGCValues=2;
    iutgc_t obj = iusHLTGCCreate(numTGCValues);
    TEST_ASSERT(obj != IUTGC_INVALID);

    iusHLTGCSet(obj,0,1.0f,2.0f);
    // float param
    TEST_ASSERT_EQUAL_FLOAT(1.0f, iusHLTGCGetTime(obj, 0));
    TEST_ASSERT_EQUAL_FLOAT(2.0f, iusHLTGCGetGain(obj, 0));


    // invalid param
    TEST_ASSERT_EQUAL_FLOAT(NAN, iusHLTGCGetTime(NULL, 1));
    TEST_ASSERT_EQUAL_FLOAT(NAN, iusHLTGCGetGain(NULL, 1));
    TEST_ASSERT_EQUAL_FLOAT(NAN, iusHLTGCGetTime(NULL, 0));
    TEST_ASSERT_EQUAL_FLOAT(NAN, iusHLTGCGetGain(NULL, 0));
    iusHLTGCDelete(obj);
}


TEST(IusTGC, testIusSerialization)
{
    char *filename = "testIusTGCSerialization.hdf5";
    char *pulsePath =  "/TGC";
    char *label = "Created_in_testIusSerialization";

    int numTGCValues = 20;

    // create
    iutgc_t TGC = iusHLTGCCreate(numTGCValues);

    // fill
    int i,status;
    for (i=0;i<numTGCValues;i++)
    {
        status = iusHLTGCSet(TGC,i,i*1.0f,i*2.0f);
        TEST_ASSERT_EQUAL(IUS_E_OK,status);
    }

    // save
    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
    status = iusHLTGCSave(TGC, pulsePath, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iutgc_t savedObj = iusHLTGCLoad(handle, pulsePath, label);
    TEST_ASSERT(savedObj != NULL);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUS_TRUE, iusHLTGCCompare(TGC, savedObj));
    iusHLTGCDelete(TGC);
    iusHLTGCDelete(savedObj);
}

TEST_GROUP_RUNNER(IusTGC)
{
    RUN_TEST_CASE(IusTGC, testIusTGCCreate);
    RUN_TEST_CASE(IusTGC, testIusTGCDelete);
    RUN_TEST_CASE(IusTGC, testIusTGCCompare);
    RUN_TEST_CASE(IusTGC, testIusTGCSetGet);
    RUN_TEST_CASE(IusTGC, testIusSerialization);
}
