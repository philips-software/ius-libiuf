

//
// Created by nlv09165 on 26/07/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <include/ius.h>
#include <include/iusError.h>
#include <include/iusTypes.h>
#include "include/iusHL3DParametricSourceImp.h"

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

    iu3dps_t obj = iusHL3DParametricSourceCreate(pLabel, numLocations, FNumber, angularDelta, startAngle, deltaPhi, startPhi);
    iu3dps_t notherObj = iusHL3DParametricSourceCreate(pLabel, numLocations, FNumber, angularDelta, startAngle, deltaPhi, startPhi);
    TEST_ASSERT(obj != IU3DPS_INVALID);
    TEST_ASSERT(notherObj != IU3DPS_INVALID);
    iusHL3DParametricSourceDelete(obj);
    iusHL3DParametricSourceDelete(notherObj);

    // invalid params
    obj = iusHL3DParametricSourceCreate(NULL, 0, -1, 2.0f, 0, 0, 0);
    TEST_ASSERT(obj == IU3DPS_INVALID);
    obj = iusHL3DParametricSourceCreate(NULL, 0, 1, -2.0f, 0, 0, 0);
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

    iu3dps_t obj = iusHL3DParametricSourceCreate(pLabel, numLocations, FNumber, angularDelta, startAngle, deltaPhi, startPhi);
    TEST_ASSERT(obj != IU3DPS_INVALID);
    int status = iusHL3DParametricSourceDelete(obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // invalid params
    status = iusHL3DParametricSourceDelete(NULL);
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

    iu3dps_t obj = iusHL3DParametricSourceCreate(pLabel, numLocations, FNumber, angularDelta, startAngle, deltaPhi, startPhi);
    iu3dps_t notherObj = iusHL3DParametricSourceCreate(pLabel, numLocations, FNumber, angularDelta, startAngle, deltaPhi, startPhi);
    iu3dps_t differentObj =
    iusHL3DParametricSourceCreate(pLabel, numLocations, FNumber + 0.01f, angularDelta, startAngle, deltaPhi, startPhi);
    TEST_ASSERT(obj != IU3DPS_INVALID);
    TEST_ASSERT(notherObj != IU3DPS_INVALID);
    equal = iusHL3DParametricSourceCompare(obj,obj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = iusHL3DParametricSourceCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = iusHL3DParametricSourceCompare(obj,differentObj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

    iu3dp_t pos = iusHL3DPositionCreate(1.0,2.0,3.0);
    iusHL3DParametricSourceSetPosition(obj,pos,0);
    equal = iusHL3DParametricSourceCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

    iusHL3DParametricSourceSetPosition(notherObj,pos,0);
    equal = iusHL3DParametricSourceCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);


    // invalid params
    equal = iusHL3DParametricSourceCompare(obj,NULL);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    equal = iusHL3DParametricSourceCompare(NULL,obj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    
    iusHL3DParametricSourceDelete(obj);
    iusHL3DParametricSourceDelete(notherObj);
    iusHL3DParametricSourceDelete(differentObj);
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

    iu3dps_t obj = iusHL3DParametricSourceCreate(pLabel, numLocations, FNumber, angularDelta, startAngle, deltaPhi, startPhi);

    TEST_ASSERT_EQUAL_FLOAT(FNumber,iusHL3DParametricSourceGetFNumber(obj));
    TEST_ASSERT_EQUAL_FLOAT(angularDelta,iusHL3DParametricSourceGetAngularDelta(obj));
    TEST_ASSERT_EQUAL_FLOAT(startAngle,iusHL3DParametricSourceGetStartAngle(obj));
    TEST_ASSERT_EQUAL_FLOAT(deltaPhi,iusHL3DParametricSourceGetDeltaPhi(obj));
    TEST_ASSERT_EQUAL_FLOAT(startPhi,iusHL3DParametricSourceGetStartPhi(obj));

    // Set/Get location test
    for(p=0; p<numLocations; p++)
    {
        iu3dp_t pos = iusHL3DPositionCreate(p*1.0,p*2.0,p*3.0);
        iusHL3DParametricSourceSetPosition(obj,pos,p);
        iu3dp_t get = iusHL3DParametricSourceGetPosition(obj,p);
        TEST_ASSERT_EQUAL(IUS_TRUE, iusHL3DPositionCompare(pos,get));
    }



    // invalid param
    TEST_ASSERT_EQUAL_FLOAT(NAN,iusHL3DParametricSourceGetFNumber(NULL));
    TEST_ASSERT_EQUAL_FLOAT(NAN,iusHL3DParametricSourceGetAngularDelta(NULL));
    TEST_ASSERT_EQUAL_FLOAT(NAN,iusHL3DParametricSourceGetStartAngle(NULL));
    TEST_ASSERT_EQUAL_FLOAT(NAN,iusHL3DParametricSourceGetDeltaPhi(NULL));
    TEST_ASSERT_EQUAL_FLOAT(NAN,iusHL3DParametricSourceGetStartPhi(NULL));
    TEST_ASSERT_EQUAL(IU3DP_INVALID, iusHL3DParametricSourceGetPosition(NULL,0));
    TEST_ASSERT_EQUAL(IU3DP_INVALID, iusHL3DParametricSourceGetPosition(obj,-1));

    iusHL3DParametricSourceDelete(obj);
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
    obj = iusHL3DParametricSourceCreate(pLabel, numLocations, FNumber, angularDelta, startAngle, deltaPhi, startPhi);

    // fill
    TEST_ASSERT_EQUAL_FLOAT(FNumber, iusHL3DParametricSourceGetFNumber(obj));
    TEST_ASSERT_EQUAL_FLOAT(angularDelta, iusHL3DParametricSourceGetAngularDelta(obj));
    TEST_ASSERT_EQUAL_FLOAT(startAngle, iusHL3DParametricSourceGetStartAngle(obj));
    TEST_ASSERT_EQUAL_FLOAT(deltaPhi, iusHL3DParametricSourceGetDeltaPhi(obj));
    TEST_ASSERT_EQUAL_FLOAT(startPhi, iusHL3DParametricSourceGetStartPhi(obj));

    for (p = 0; p < numLocations; p++)
    {
        iu3dp_t pos = iusHL3DPositionCreate(p * 1.0, p * 2.0, p * 3.0);
        iusHL3DParametricSourceSetPosition(obj, pos, p);
        iu3dp_t get = iusHL3DParametricSourceGetPosition(obj, p);
        TEST_ASSERT_EQUAL(IUS_TRUE, iusHL3DPositionCompare(pos, get));
    }

    // save
    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
    status = iusHL3DParametricSourceSave(obj, sourcePath, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iu3dps_t savedObj = iusHL3DParametricSourceLoad(handle, sourcePath, pLabel);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUS_TRUE, iusHL3DParametricSourceCompare(obj,savedObj));
    iusHL3DParametricSourceDelete(obj);
    iusHL3DParametricSourceDelete(savedObj);

}

    TEST_GROUP_RUNNER(Ius3DParametricSource)
{
    RUN_TEST_CASE(Ius3DParametricSource, testIus3DParametricSourceCreate);
    RUN_TEST_CASE(Ius3DParametricSource, testIus3DParametricSourceDelete);
    RUN_TEST_CASE(Ius3DParametricSource, testIus3DParametricSourceCompare);
    RUN_TEST_CASE(Ius3DParametricSource, testIus3DParametricSourceSetGet);
    RUN_TEST_CASE(Ius3DParametricSource, testIus3DParametricSourceSerialization);
}
