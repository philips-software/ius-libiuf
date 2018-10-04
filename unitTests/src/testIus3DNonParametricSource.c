
//
// Created by nlv09165 on 26/07/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <include/ius.h>
#include <include/iusError.h>
#include <include/iusTypes.h>
#include "include/ius3DNonParametricSourcePrivate.h"

TEST_GROUP(Ius3DNonParametricSource);

TEST_SETUP(Ius3DNonParametricSource)
{
}

TEST_TEAR_DOWN(Ius3DNonParametricSource)
{
}


TEST(Ius3DNonParametricSource, testIus3DNonParametricSourceCreate)
{
    char *pLabel = "label for 3d parametric source";
    int numLocations = 5 ;

    iu3dnps_t obj = ius3DNonParametricSourceCreate(pLabel, numLocations);
    iu3dnps_t notherObj = ius3DNonParametricSourceCreate(pLabel, numLocations);
    TEST_ASSERT(obj != IU3DNPS_INVALID);
    TEST_ASSERT(notherObj != IU3DNPS_INVALID);
    ius3DNonParametricSourceDelete(obj);
    ius3DNonParametricSourceDelete(notherObj);

    // invalid params
    obj = ius3DNonParametricSourceCreate(NULL, 0);
    TEST_ASSERT(obj == IU3DNPS_INVALID);
    obj = ius3DNonParametricSourceCreate(NULL, 0);
    TEST_ASSERT(obj == IU3DNPS_INVALID);
}

TEST(Ius3DNonParametricSource, testIus3DNonParametricSourceDelete)
{
    char *pLabel = "label for 3d parametric source";
    int numLocations = 5;

    iu3dnps_t obj = ius3DNonParametricSourceCreate(pLabel, numLocations);
    TEST_ASSERT(obj != IU3DNPS_INVALID);
    int status = ius3DNonParametricSourceDelete(obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // invalid params
    status = ius3DNonParametricSourceDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
}



TEST(Ius3DNonParametricSource, testIus3DNonParametricSourceCompare)
{
    IUS_BOOL equal;
    char *pLabel = "label for 3d parametric source";
    int numLocations = 5;

    iu3dnps_t obj = ius3DNonParametricSourceCreate(pLabel, numLocations);
    iu3dnps_t notherObj = ius3DNonParametricSourceCreate(pLabel, numLocations);
    iu3dnps_t differentObj =
    ius3DNonParametricSourceCreate(pLabel, numLocations+1);
    TEST_ASSERT(obj != IU3DNPS_INVALID);
    TEST_ASSERT(notherObj != IU3DNPS_INVALID);
    equal = ius3DNonParametricSourceCompare(obj,obj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = ius3DNonParametricSourceCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = ius3DNonParametricSourceCompare(obj,differentObj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

    iu3dp_t pos = ius3DPositionCreate(1.0,2.0,3.0);
    ius3DNonParametricSourceSetPosition(obj,pos,0);
    equal = ius3DNonParametricSourceCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

    ius3DNonParametricSourceSetPosition(notherObj,pos,0);
    equal = ius3DNonParametricSourceCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);


    // invalid params
    equal = ius3DNonParametricSourceCompare(obj,NULL);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    equal = ius3DNonParametricSourceCompare(NULL,obj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    
    ius3DNonParametricSourceDelete(obj);
    ius3DNonParametricSourceDelete(notherObj);
    ius3DNonParametricSourceDelete(differentObj);
}

TEST(Ius3DNonParametricSource, testIus3DNonParametricSourceSetGet)
{
    //IUS_BOOL equal;
    char *pLabel = "label for 3d parametric source";
    int p,numLocations = 5;

    iu3dnps_t obj = ius3DNonParametricSourceCreate(pLabel, numLocations);

    // Set/Get location test
    for(p=0; p<numLocations; p++)
    {
        iu3dp_t pos = ius3DPositionCreate(p*1.0f,p*2.0f,p*3.0f);
        ius3DNonParametricSourceSetPosition(obj,pos,p);
        iu3dp_t get = ius3DNonParametricSourceGetPosition(obj,p);
        TEST_ASSERT_EQUAL(IUS_TRUE, ius3DPositionCompare(pos,get));
    }

    // invalid param
    TEST_ASSERT_EQUAL(IU3DP_INVALID, ius3DNonParametricSourceGetPosition(NULL,0));
    TEST_ASSERT_EQUAL(IU3DP_INVALID, ius3DNonParametricSourceGetPosition(obj,-1));

    ius3DNonParametricSourceDelete(obj);
}

TEST(Ius3DNonParametricSource, testIus3DNonParametricSourceSerialization)
{
    char *filename = "testIus3DNonParametricSourceSerialization.hdf5";
    //char *sourcePath =  "/3DNonParametricSource"; not needed since "pulseSourceDict" is hard-coded

    //IUS_BOOL equal;
    char *pLabel = "label for 3d parametric source";
    int p, numLocations = 5, status;


    // create
    iu3dnps_t
    obj = ius3DNonParametricSourceCreate(pLabel, numLocations);

    // fill
    for (p = 0; p < numLocations; p++)
    {
        iu3dp_t pos = ius3DPositionCreate(p * 1.0f, p * 2.0f, p * 3.0f);
        ius3DNonParametricSourceSetPosition(obj, pos, p);
        iu3dp_t get = ius3DNonParametricSourceGetPosition(obj, p);
        TEST_ASSERT_EQUAL(IUS_TRUE, ius3DPositionCompare(pos, get));
    }

    // save
    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
    status = ius3DNonParametricSourceSave(obj, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iu3dnps_t savedObj = ius3DNonParametricSourceLoad(handle, pLabel);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUS_TRUE, ius3DNonParametricSourceCompare(obj,savedObj));
    ius3DNonParametricSourceDelete(obj);
    ius3DNonParametricSourceDelete(savedObj);

}

    TEST_GROUP_RUNNER(Ius3DNonParametricSource)
{
    RUN_TEST_CASE(Ius3DNonParametricSource, testIus3DNonParametricSourceCreate);
    RUN_TEST_CASE(Ius3DNonParametricSource, testIus3DNonParametricSourceDelete);
    RUN_TEST_CASE(Ius3DNonParametricSource, testIus3DNonParametricSourceCompare);
    RUN_TEST_CASE(Ius3DNonParametricSource, testIus3DNonParametricSourceSetGet);
    RUN_TEST_CASE(Ius3DNonParametricSource, testIus3DNonParametricSourceSerialization);
}
