
//
// Created by nlv09165 on 31/07/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <include/ius.h>
#include <include/iusError.h>
#include <include/iusTypes.h>
#include <include/iusHLPosition.h>
#include "include/iusHL2DNonParametricSourceImp.h"

TEST_GROUP(Ius2DNonParametricSource);

TEST_SETUP(Ius2DNonParametricSource)
{
}

TEST_TEAR_DOWN(Ius2DNonParametricSource)
{
}


TEST(Ius2DNonParametricSource, testIus2DNonParametricSourceCreate)
{
    char *pLabel = "label for 2d parametric source";
    int numLocations = 5 ;

    iu2dnps_t obj = iusHL2DNonParametricSourceCreate(pLabel, numLocations);
    iu2dnps_t notherObj = iusHL2DNonParametricSourceCreate(pLabel, numLocations);
    TEST_ASSERT(obj != IU2DNPS_INVALID);
    TEST_ASSERT(notherObj != IU2DNPS_INVALID);
    iusHL2DNonParametricSourceDelete(obj);
    iusHL2DNonParametricSourceDelete(notherObj);

    // invalid params
    obj = iusHL2DNonParametricSourceCreate(NULL, 0);
    TEST_ASSERT(obj == IU2DNPS_INVALID);
    obj = iusHL2DNonParametricSourceCreate(NULL, 0);
    TEST_ASSERT(obj == IU2DNPS_INVALID);
}

TEST(Ius2DNonParametricSource, testIus2DNonParametricSourceDelete)
{
    char *pLabel = "label for 2d parametric source";
    int numLocations = 5;

    iu2dnps_t obj = iusHL2DNonParametricSourceCreate(pLabel, numLocations);
    TEST_ASSERT(obj != IU2DNPS_INVALID);
    int status = iusHL2DNonParametricSourceDelete(obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // invalid params
    status = iusHL2DNonParametricSourceDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
}



TEST(Ius2DNonParametricSource, testIus2DNonParametricSourceCompare)
{
    IUS_BOOL equal;
    char *pLabel = "label for 2d parametric source";
    int numLocations = 5;

    iu2dnps_t obj = iusHL2DNonParametricSourceCreate(pLabel, numLocations);
    iu2dnps_t notherObj = iusHL2DNonParametricSourceCreate(pLabel, numLocations);
    iu2dnps_t differentObj =
    iusHL2DNonParametricSourceCreate(pLabel, numLocations+2);
    TEST_ASSERT(obj != IU2DNPS_INVALID);
    TEST_ASSERT(notherObj != IU2DNPS_INVALID);
    equal = iusHL2DNonParametricSourceCompare(obj,obj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = iusHL2DNonParametricSourceCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = iusHL2DNonParametricSourceCompare(obj,differentObj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

    iu2dp_t pos = iusHL2DPositionCreate(1.0,2.0);
    iusHL2DNonParametricSourceSetPosition(obj,pos,0);
    equal = iusHL2DNonParametricSourceCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

    iusHL2DNonParametricSourceSetPosition(notherObj,pos,0);
    equal = iusHL2DNonParametricSourceCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);


    // invalid params
    equal = iusHL2DNonParametricSourceCompare(obj,NULL);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    equal = iusHL2DNonParametricSourceCompare(NULL,obj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

    iusHL2DNonParametricSourceDelete(obj);
    iusHL2DNonParametricSourceDelete(notherObj);
    iusHL2DNonParametricSourceDelete(differentObj);
}

TEST(Ius2DNonParametricSource, testIus2DNonParametricSourceSetGet)
{
    IUS_BOOL equal;
    char *pLabel = "label for 2d parametric source";
    int p,numLocations = 5;

    iu2dnps_t obj = iusHL2DNonParametricSourceCreate(pLabel, numLocations);

    // Set/Get location test
    for(p=0; p<numLocations; p++)
    {
        iu2dp_t pos = iusHL2DPositionCreate(p*1.0,p*3.0);
        iusHL2DNonParametricSourceSetPosition(obj,pos,p);
        iu2dp_t get = iusHL2DNonParametricSourceGetPosition(obj,p);
        TEST_ASSERT_EQUAL(IUS_TRUE, iusHL2DPositionCompare(pos,get));
    }



    // invalid param
    TEST_ASSERT_EQUAL(IU2DP_INVALID, iusHL2DNonParametricSourceGetPosition(NULL,0));
    TEST_ASSERT_EQUAL(IU2DP_INVALID, iusHL2DNonParametricSourceGetPosition(obj,-1));

    iusHL2DNonParametricSourceDelete(obj);
}

TEST(Ius2DNonParametricSource, testIus2DNonParametricSourceSerialization)
{
    char *filename = "testIus2DNonParametricSourceSerialization.hdf5";
    char *sourcePath =  "/2DNonParametricSource";

    IUS_BOOL equal;
    float angularDelta = 0.13f;
    float FNumber = -0.955f;
    float startAngle = 3.14f;
    char *pLabel = "label for 2d parametric source";
    int p, numLocations = 5, status;


    // create
    iu2dnps_t
    obj = iusHL2DNonParametricSourceCreate(pLabel, numLocations);

    // fill
    for (p = 0; p < numLocations; p++)
    {
        iu2dp_t pos = iusHL2DPositionCreate(p * 1.0, p * 3.0);
        iusHL2DNonParametricSourceSetPosition(obj, pos, p);
        iu2dp_t get = iusHL2DNonParametricSourceGetPosition(obj, p);
        TEST_ASSERT_EQUAL(IUS_TRUE, iusHL2DPositionCompare(pos, get));
    }

    // save
    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
    status = iusHL2DNonParametricSourceSave(obj, sourcePath, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iu2dnps_t savedObj = iusHL2DNonParametricSourceLoad(handle, sourcePath, pLabel);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUS_TRUE, iusHL2DNonParametricSourceCompare(obj,savedObj));
    iusHL2DNonParametricSourceDelete(obj);
    iusHL2DNonParametricSourceDelete(savedObj);

}

TEST_GROUP_RUNNER(Ius2DNonParametricSource)
{
    RUN_TEST_CASE(Ius2DNonParametricSource, testIus2DNonParametricSourceCreate);
    RUN_TEST_CASE(Ius2DNonParametricSource, testIus2DNonParametricSourceDelete);
    RUN_TEST_CASE(Ius2DNonParametricSource, testIus2DNonParametricSourceCompare);
    RUN_TEST_CASE(Ius2DNonParametricSource, testIus2DNonParametricSourceSetGet);
    RUN_TEST_CASE(Ius2DNonParametricSource, testIus2DNonParametricSourceSerialization);
}
