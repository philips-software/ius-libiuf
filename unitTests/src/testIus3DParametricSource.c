

//
// Created by nlv09165 on 26/07/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <include/ius.h>
#include <include/iusError.h>
#include <include/iusTypes.h>
#include "include/ius3DParametricSourceImp.h"

TEST_GROUP(Ius3DParametricSource);

TEST_SETUP(Ius3DParametricSource)
{
}

TEST_TEAR_DOWN(Ius3DParametricSource)
{
}


TEST(Ius3DParametricSource, testIus3DParametricSourceCreate)
{
    float angularDelta = 0.13f;
    float FNumber = -0.955f;
    float startAngle = 3.14f;
    float startPhi = startAngle;
    float deltaPhi = angularDelta;
    char *pLabel = "label for 3d parametric source";
    int numLocations = 5 ;

    iu3dps_t obj = ius3DParametricSourceCreate(pLabel, numLocations, FNumber, angularDelta, startAngle, deltaPhi, startPhi);
    iu3dps_t notherObj = ius3DParametricSourceCreate(pLabel, numLocations, FNumber, angularDelta, startAngle, deltaPhi, startPhi);
    TEST_ASSERT(obj != IU3DPS_INVALID);
    TEST_ASSERT(notherObj != IU3DPS_INVALID);
    ius3DParametricSourceDelete(obj);
    ius3DParametricSourceDelete(notherObj);

    // invalid params
    obj = ius3DParametricSourceCreate(NULL, 0, -1, 2.0f, 0, 0, 0);
    TEST_ASSERT(obj == IU3DPS_INVALID);
    obj = ius3DParametricSourceCreate(NULL, 0, 1, -2.0f, 0, 0, 0);
    TEST_ASSERT(obj == IU3DPS_INVALID);
}

TEST(Ius3DParametricSource, testIus3DParametricSourceDelete)
{
    float angularDelta = 0.13f;
    float FNumber = -0.955f;
    float startAngle = 3.14f;
    float startPhi = startAngle;
    float deltaPhi = angularDelta;
    char *pLabel = "label for 3d parametric source";
    int numLocations = 5;

    iu3dps_t obj = ius3DParametricSourceCreate(pLabel, numLocations, FNumber, angularDelta, startAngle, deltaPhi, startPhi);
    TEST_ASSERT(obj != IU3DPS_INVALID);
    int status = ius3DParametricSourceDelete(obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // invalid params
    status = ius3DParametricSourceDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
}



TEST(Ius3DParametricSource, testIus3DParametricSourceCompare)
{
    IUS_BOOL equal;
    float angularDelta = 0.13f;
    float FNumber = -0.955f;
    float startAngle = 3.14f;
    float startPhi = startAngle;
    float deltaPhi = angularDelta;
    char *pLabel = "label for 3d parametric source";
    int numLocations = 5;

    iu3dps_t obj = ius3DParametricSourceCreate(pLabel, numLocations, FNumber, angularDelta, startAngle, deltaPhi, startPhi);
    iu3dps_t notherObj = ius3DParametricSourceCreate(pLabel, numLocations, FNumber, angularDelta, startAngle, deltaPhi, startPhi);
    iu3dps_t differentObj =
    ius3DParametricSourceCreate(pLabel, numLocations, FNumber + 0.01f, angularDelta, startAngle, deltaPhi, startPhi);
    TEST_ASSERT(obj != IU3DPS_INVALID);
    TEST_ASSERT(notherObj != IU3DPS_INVALID);
    equal = ius3DParametricSourceCompare(obj,obj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = ius3DParametricSourceCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = ius3DParametricSourceCompare(obj,differentObj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

    iu3dp_t pos = ius3DPositionCreate(1.0,2.0,3.0);
    ius3DParametricSourceSetPosition(obj,pos,0);
    equal = ius3DParametricSourceCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

    ius3DParametricSourceSetPosition(notherObj,pos,0);
    equal = ius3DParametricSourceCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);


    // invalid params
    equal = ius3DParametricSourceCompare(obj,NULL);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    equal = ius3DParametricSourceCompare(NULL,obj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    
    ius3DParametricSourceDelete(obj);
    ius3DParametricSourceDelete(notherObj);
    ius3DParametricSourceDelete(differentObj);
}

TEST(Ius3DParametricSource, testIus3DParametricSourceSetGet)
{
    IUS_BOOL equal;
    float angularDelta = 0.13f;
    float FNumber = -0.955f;
    float startAngle = 3.14f;
    float startPhi = startAngle;
    float deltaPhi = angularDelta;
    char *pLabel = "label for 3d parametric source";
    int p,numLocations = 5;

    iu3dps_t obj = ius3DParametricSourceCreate(pLabel, numLocations, FNumber, angularDelta, startAngle, deltaPhi, startPhi);

    TEST_ASSERT_EQUAL_FLOAT(FNumber,ius3DParametricSourceGetFNumber(obj));
    TEST_ASSERT_EQUAL_FLOAT(angularDelta,ius3DParametricSourceGetAngularDelta(obj));
    TEST_ASSERT_EQUAL_FLOAT(startAngle,ius3DParametricSourceGetStartAngle(obj));
    TEST_ASSERT_EQUAL_FLOAT(deltaPhi,ius3DParametricSourceGetDeltaPhi(obj));
    TEST_ASSERT_EQUAL_FLOAT(startPhi,ius3DParametricSourceGetStartPhi(obj));

    // Set/Get location test
    for(p=0; p<numLocations; p++)
    {
        iu3dp_t pos = ius3DPositionCreate(p*1.0,p*2.0,p*3.0);
        ius3DParametricSourceSetPosition(obj,pos,p);
        iu3dp_t get = ius3DParametricSourceGetPosition(obj,p);
        TEST_ASSERT_EQUAL(IUS_TRUE, ius3DPositionCompare(pos,get));
    }



    // invalid param
    TEST_ASSERT_EQUAL_FLOAT(NAN,ius3DParametricSourceGetFNumber(NULL));
    TEST_ASSERT_EQUAL_FLOAT(NAN,ius3DParametricSourceGetAngularDelta(NULL));
    TEST_ASSERT_EQUAL_FLOAT(NAN,ius3DParametricSourceGetStartAngle(NULL));
    TEST_ASSERT_EQUAL_FLOAT(NAN,ius3DParametricSourceGetDeltaPhi(NULL));
    TEST_ASSERT_EQUAL_FLOAT(NAN,ius3DParametricSourceGetStartPhi(NULL));
    TEST_ASSERT_EQUAL(IU3DP_INVALID, ius3DParametricSourceGetPosition(NULL,0));
    TEST_ASSERT_EQUAL(IU3DP_INVALID, ius3DParametricSourceGetPosition(obj,-1));

    ius3DParametricSourceDelete(obj);
}

TEST(Ius3DParametricSource, testIus3DParametricSourceSerialization)
{
    char *filename = "testIus3DParametricSourceSerialization.hdf5";
    char *sourcePath =  "/3DParametricSource";

    IUS_BOOL equal;
    float angularDelta = 0.13f;
    float FNumber = -0.955f;
    float startAngle = 3.14f;
    float startPhi = startAngle;
    float deltaPhi = angularDelta;
    char *pLabel = "label for 3d parametric source";
    int p, numLocations = 5, status;


    // create
    iu3dps_t
    obj = ius3DParametricSourceCreate(pLabel, numLocations, FNumber, angularDelta, startAngle, deltaPhi, startPhi);

    // fill
    TEST_ASSERT_EQUAL_FLOAT(FNumber, ius3DParametricSourceGetFNumber(obj));
    TEST_ASSERT_EQUAL_FLOAT(angularDelta, ius3DParametricSourceGetAngularDelta(obj));
    TEST_ASSERT_EQUAL_FLOAT(startAngle, ius3DParametricSourceGetStartAngle(obj));
    TEST_ASSERT_EQUAL_FLOAT(deltaPhi, ius3DParametricSourceGetDeltaPhi(obj));
    TEST_ASSERT_EQUAL_FLOAT(startPhi, ius3DParametricSourceGetStartPhi(obj));

    for (p = 0; p < numLocations; p++)
    {
        iu3dp_t pos = ius3DPositionCreate(p * 1.0, p * 2.0, p * 3.0);
        ius3DParametricSourceSetPosition(obj, pos, p);
    }

    // save
    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
    status = ius3DParametricSourceSave(obj, sourcePath, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iu3dps_t savedObj = ius3DParametricSourceLoad(handle, sourcePath, pLabel);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUS_TRUE, ius3DParametricSourceCompare(obj,savedObj));
    ius3DParametricSourceDelete(obj);
    ius3DParametricSourceDelete(savedObj);

}

    TEST_GROUP_RUNNER(Ius3DParametricSource)
{
    RUN_TEST_CASE(Ius3DParametricSource, testIus3DParametricSourceCreate);
    RUN_TEST_CASE(Ius3DParametricSource, testIus3DParametricSourceDelete);
    RUN_TEST_CASE(Ius3DParametricSource, testIus3DParametricSourceCompare);
    RUN_TEST_CASE(Ius3DParametricSource, testIus3DParametricSourceSetGet);
    RUN_TEST_CASE(Ius3DParametricSource, testIus3DParametricSourceSerialization);
}
