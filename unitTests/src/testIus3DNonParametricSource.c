
//
// Created by nlv09165 on 26/07/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <include/ius.h>
#include <include/iusError.h>
#include <include/iusTypes.h>
#include "include/iusHL3DNonParametricSourceImp.h"

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

    iu3dnps_t obj = iusHL3DNonParametricSourceCreate(pLabel, numLocations);
    iu3dnps_t notherObj = iusHL3DNonParametricSourceCreate(pLabel, numLocations);
    TEST_ASSERT(obj != IU3DNPS_INVALID);
    TEST_ASSERT(notherObj != IU3DNPS_INVALID);
    iusHL3DNonParametricSourceDelete(obj);
    iusHL3DNonParametricSourceDelete(notherObj);

    // invalid params
    obj = iusHL3DNonParametricSourceCreate(NULL, 0);
    TEST_ASSERT(obj == IU3DNPS_INVALID);
    obj = iusHL3DNonParametricSourceCreate(NULL, 0);
    TEST_ASSERT(obj == IU3DNPS_INVALID);
}

TEST(Ius3DNonParametricSource, testIus3DNonParametricSourceDelete)
{
    char *pLabel = "label for 3d parametric source";
    int numLocations = 5;

    iu3dnps_t obj = iusHL3DNonParametricSourceCreate(pLabel, numLocations);
    TEST_ASSERT(obj != IU3DNPS_INVALID);
    int status = iusHL3DNonParametricSourceDelete(obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // invalid params
    status = iusHL3DNonParametricSourceDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
}



TEST(Ius3DNonParametricSource, testIus3DNonParametricSourceCompare)
{
    IUS_BOOL equal;
    char *pLabel = "label for 3d parametric source";
    int numLocations = 5;

    iu3dnps_t obj = iusHL3DNonParametricSourceCreate(pLabel, numLocations);
    iu3dnps_t notherObj = iusHL3DNonParametricSourceCreate(pLabel, numLocations);
    iu3dnps_t differentObj =
    iusHL3DNonParametricSourceCreate(pLabel, numLocations+1);
    TEST_ASSERT(obj != IU3DNPS_INVALID);
    TEST_ASSERT(notherObj != IU3DNPS_INVALID);
    equal = iusHL3DNonParametricSourceCompare(obj,obj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = iusHL3DNonParametricSourceCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = iusHL3DNonParametricSourceCompare(obj,differentObj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

    iu3dp_t pos = iusHL3DPositionCreate(1.0,2.0,3.0);
    iusHL3DNonParametricSourceSetPosition(obj,pos,0);
    equal = iusHL3DNonParametricSourceCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

    iusHL3DNonParametricSourceSetPosition(notherObj,pos,0);
    equal = iusHL3DNonParametricSourceCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);


    // invalid params
    equal = iusHL3DNonParametricSourceCompare(obj,NULL);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    equal = iusHL3DNonParametricSourceCompare(NULL,obj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    
    iusHL3DNonParametricSourceDelete(obj);
    iusHL3DNonParametricSourceDelete(notherObj);
    iusHL3DNonParametricSourceDelete(differentObj);
}

TEST(Ius3DNonParametricSource, testIus3DNonParametricSourceSetGet)
{
    IUS_BOOL equal;
    char *pLabel = "label for 3d parametric source";
    int p,numLocations = 5;

    iu3dnps_t obj = iusHL3DNonParametricSourceCreate(pLabel, numLocations);

    // Set/Get location test
    for(p=0; p<numLocations; p++)
    {
        iu3dp_t pos = iusHL3DPositionCreate(p*1.0,p*2.0,p*3.0);
        iusHL3DNonParametricSourceSetPosition(obj,pos,p);
        iu3dp_t get = iusHL3DNonParametricSourceGetPosition(obj,p);
        TEST_ASSERT_EQUAL(IUS_TRUE, iusHL3DPositionCompare(pos,get));
    }

    // invalid param
    TEST_ASSERT_EQUAL(IU3DP_INVALID, iusHL3DNonParametricSourceGetPosition(NULL,0));
    TEST_ASSERT_EQUAL(IU3DP_INVALID, iusHL3DNonParametricSourceGetPosition(obj,-1));

    iusHL3DNonParametricSourceDelete(obj);
}

TEST(Ius3DNonParametricSource, testIus3DNonParametricSourceSerialization)
{
    char *filename = "testIus3DNonParametricSourceSerialization.hdf5";
    char *sourcePath =  "/3DNonParametricSource";

    IUS_BOOL equal;
    char *pLabel = "label for 3d parametric source";
    int p, numLocations = 5, status;


    // create
    iu3dnps_t
    obj = iusHL3DNonParametricSourceCreate(pLabel, numLocations);

    // fill
    for (p = 0; p < numLocations; p++)
    {
        iu3dp_t pos = iusHL3DPositionCreate(p * 1.0, p * 2.0, p * 3.0);
        iusHL3DNonParametricSourceSetPosition(obj, pos, p);
        iu3dp_t get = iusHL3DNonParametricSourceGetPosition(obj, p);
        TEST_ASSERT_EQUAL(IUS_TRUE, iusHL3DPositionCompare(pos, get));
    }

    // save
    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
    status = iusHL3DNonParametricSourceSave(obj, sourcePath, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iu3dnps_t savedObj = iusHL3DNonParametricSourceLoad(handle, sourcePath, pLabel);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUS_TRUE, iusHL3DNonParametricSourceCompare(obj,savedObj));
    iusHL3DNonParametricSourceDelete(obj);
    iusHL3DNonParametricSourceDelete(savedObj);

}

    TEST_GROUP_RUNNER(Ius3DNonParametricSource)
{
    RUN_TEST_CASE(Ius3DNonParametricSource, testIus3DNonParametricSourceCreate);
    RUN_TEST_CASE(Ius3DNonParametricSource, testIus3DNonParametricSourceDelete);
    RUN_TEST_CASE(Ius3DNonParametricSource, testIus3DNonParametricSourceCompare);
    RUN_TEST_CASE(Ius3DNonParametricSource, testIus3DNonParametricSourceSetGet);
    RUN_TEST_CASE(Ius3DNonParametricSource, testIus3DNonParametricSourceSerialization);
}
