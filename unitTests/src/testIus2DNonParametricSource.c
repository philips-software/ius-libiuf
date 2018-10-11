
//
// Created by nlv09165 on 31/07/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <include/ius.h>
#include <include/iusError.h>
#include <include/iusTypes.h>
#include <include/iusPosition.h>
#include "include/ius2DNonParametricSourcePrivate.h"

TEST_GROUP(Ius2DNonParametricSource);

TEST_SETUP(Ius2DNonParametricSource)
{
}

TEST_TEAR_DOWN(Ius2DNonParametricSource)
{
}


TEST(Ius2DNonParametricSource, testIus2DNonParametricSourceCreate)
{
    int numLocations = 5 ;

    iu2dnps_t obj = ius2DNonParametricSourceCreate(numLocations);
    iu2dnps_t notherObj = ius2DNonParametricSourceCreate(numLocations);
    TEST_ASSERT(obj != IU2DNPS_INVALID);
    TEST_ASSERT(notherObj != IU2DNPS_INVALID);
    ius2DNonParametricSourceDelete(obj);
    ius2DNonParametricSourceDelete(notherObj);

    // invalid params
    obj = ius2DNonParametricSourceCreate(0);
    TEST_ASSERT(obj == IU2DNPS_INVALID);
    obj = ius2DNonParametricSourceCreate(0);
    TEST_ASSERT(obj == IU2DNPS_INVALID);
}

TEST(Ius2DNonParametricSource, testIus2DNonParametricSourceDelete)
{
    int numLocations = 5;

    iu2dnps_t obj = ius2DNonParametricSourceCreate(numLocations);
    TEST_ASSERT(obj != IU2DNPS_INVALID);
    int status = ius2DNonParametricSourceDelete(obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // invalid params
    status = ius2DNonParametricSourceDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
}



TEST(Ius2DNonParametricSource, testIus2DNonParametricSourceCompare)
{
    IUS_BOOL equal;
    int numLocations = 5;

    iu2dnps_t obj = ius2DNonParametricSourceCreate(numLocations);
    iu2dnps_t notherObj = ius2DNonParametricSourceCreate(numLocations);
    iu2dnps_t differentObj = ius2DNonParametricSourceCreate(numLocations+2);
    TEST_ASSERT(obj != IU2DNPS_INVALID);
    TEST_ASSERT(notherObj != IU2DNPS_INVALID);
    equal = ius2DNonParametricSourceCompare(obj,obj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = ius2DNonParametricSourceCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = ius2DNonParametricSourceCompare(obj,differentObj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

    iu2dp_t pos = ius2DPositionCreate(1.0,2.0);
    ius2DNonParametricSourceSetPosition(obj,pos,0);
    equal = ius2DNonParametricSourceCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

    ius2DNonParametricSourceSetPosition(notherObj,pos,0);
    equal = ius2DNonParametricSourceCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);


    // invalid params
    equal = ius2DNonParametricSourceCompare(obj,NULL);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    equal = ius2DNonParametricSourceCompare(NULL,obj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

    ius2DNonParametricSourceDelete(obj);
    ius2DNonParametricSourceDelete(notherObj);
    ius2DNonParametricSourceDelete(differentObj);
}

TEST(Ius2DNonParametricSource, testIus2DNonParametricSourceSetGet)
{
    int p,numLocations = 5;

    iu2dnps_t obj = ius2DNonParametricSourceCreate(numLocations);

    // Set/Get location test
    for(p=0; p<numLocations; p++)
    {
        iu2dp_t pos = ius2DPositionCreate(p*1.0f,p*3.0f);
        ius2DNonParametricSourceSetPosition(obj,pos,p);
        iu2dp_t get = ius2DNonParametricSourceGetPosition(obj,p);
        TEST_ASSERT_EQUAL(IUS_TRUE, ius2DPositionCompare(pos,get));
    }

    // invalid param
    TEST_ASSERT_EQUAL(IU2DP_INVALID, ius2DNonParametricSourceGetPosition(NULL,0));
    TEST_ASSERT_EQUAL(IU2DP_INVALID, ius2DNonParametricSourceGetPosition(obj,-1));

    ius2DNonParametricSourceDelete(obj);
}

TEST(Ius2DNonParametricSource, testIus2DNonParametricSourceSerialization)
{
    char *filename = "testIus2DNonParametricSourceSerialization.hdf5";
    int p, numLocations = 5, status;


    // create
    iu2dnps_t
    obj = ius2DNonParametricSourceCreate(numLocations);

    // fill
    for (p = 0; p < numLocations; p++)
    {
        iu2dp_t pos = ius2DPositionCreate(p * 1.0f, p * 3.0f);
        ius2DNonParametricSourceSetPosition(obj, pos, p);
        iu2dp_t get = ius2DNonParametricSourceGetPosition(obj, p);
        TEST_ASSERT_EQUAL(IUS_TRUE, ius2DPositionCompare(pos, get));
    }

    // save
    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
    status = ius2DNonParametricSourceSave(obj, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iu2dnps_t savedObj = ius2DNonParametricSourceLoad(handle);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUS_TRUE, ius2DNonParametricSourceCompare(obj,savedObj));
    ius2DNonParametricSourceDelete(obj);
    ius2DNonParametricSourceDelete(savedObj);
}

TEST_GROUP_RUNNER(Ius2DNonParametricSource)
{
    RUN_TEST_CASE(Ius2DNonParametricSource, testIus2DNonParametricSourceCreate);
    RUN_TEST_CASE(Ius2DNonParametricSource, testIus2DNonParametricSourceDelete);
    RUN_TEST_CASE(Ius2DNonParametricSource, testIus2DNonParametricSourceCompare);
    RUN_TEST_CASE(Ius2DNonParametricSource, testIus2DNonParametricSourceSetGet);
    RUN_TEST_CASE(Ius2DNonParametricSource, testIus2DNonParametricSourceSerialization);
}
