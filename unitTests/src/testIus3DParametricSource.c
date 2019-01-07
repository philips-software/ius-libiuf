

//
// Created by nlv09165 on 26/07/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <ius.h>
#include <ius3DParametricSourcePrivate.h>

TEST_GROUP(Ius3DParametricSource);

TEST_SETUP(Ius3DParametricSource)
{
}

TEST_TEAR_DOWN(Ius3DParametricSource)
{
}

TEST(Ius3DParametricSource, testIus3DParametricSourceCreate)
{
    float FNumber = -0.955f;
    float startAngle = IUS_PI;
    float startTheta = startAngle;
	float deltaTheta = IUS_PI/12;
	float startPhi = startAngle;
    float deltaPhi = IUS_PI/20;
    int numPhiLocations = 5 ;
	int numThetaLocations = 3;

    iu3dps_t obj = ius3DParametricSourceCreate(numThetaLocations, numPhiLocations, FNumber, deltaTheta, startTheta, deltaPhi, startPhi);
    iu3dps_t notherObj = ius3DParametricSourceCreate(numThetaLocations, numPhiLocations, FNumber, deltaTheta, startTheta, deltaPhi, startPhi);
    TEST_ASSERT(obj != IU3DPS_INVALID);
    TEST_ASSERT(notherObj != IU3DPS_INVALID);
    ius3DParametricSourceDelete(obj);
    ius3DParametricSourceDelete(notherObj);

    // invalid params
    obj = ius3DParametricSourceCreate(0, 0, -1, 2.0f, 0, 0, 0);
    TEST_ASSERT(obj == IU3DPS_INVALID);
    obj = ius3DParametricSourceCreate(1, 1, 1, -2.0f, 0, 0, 0);
    TEST_ASSERT(obj == IU3DPS_INVALID);
}

TEST(Ius3DParametricSource, testIus3DParametricSourceDelete)
{
	float FNumber = -0.955f;
	float startAngle = IUS_PI;
	float startTheta = startAngle;
	float deltaTheta = IUS_PI / 12;
	float startPhi = startAngle;
	float deltaPhi = IUS_PI / 20;
	int numPhiLocations = 5;
	int numThetaLocations = 3;

    iu3dps_t obj = ius3DParametricSourceCreate(numThetaLocations, numPhiLocations, FNumber, deltaTheta, startTheta, deltaPhi, startPhi);
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
	float FNumber = -0.955f;
	float startAngle = IUS_PI;
	float startTheta = startAngle;
	float deltaTheta = IUS_PI / 12;
	float startPhi = startAngle;
	float deltaPhi = IUS_PI / 20;
	int numPhiLocations = 5;
	int numThetaLocations = 3;

    iu3dps_t obj = ius3DParametricSourceCreate(numThetaLocations, numPhiLocations, FNumber, deltaTheta, startTheta, deltaPhi, startPhi);
    iu3dps_t notherObj = ius3DParametricSourceCreate(numThetaLocations, numPhiLocations, FNumber, deltaTheta, startTheta, deltaPhi, startPhi);
    iu3dps_t differentObj =
    ius3DParametricSourceCreate(numThetaLocations, numPhiLocations, FNumber+0.01f, deltaTheta, startTheta, deltaPhi, startPhi);
    TEST_ASSERT(obj != IU3DPS_INVALID);
    TEST_ASSERT(notherObj != IU3DPS_INVALID);
    equal = ius3DParametricSourceCompare(obj,obj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = ius3DParametricSourceCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = ius3DParametricSourceCompare(obj,differentObj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);


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
    //IUS_BOOL equal;
	float FNumber = -0.955f;
	float startAngle = IUS_PI;
	float startTheta = startAngle;
	float deltaTheta = IUS_PI / 12;
	float startPhi = startAngle;
	float deltaPhi = IUS_PI / 20;
	int numPhiLocations = 5;
	int numThetaLocations = 3;

    iu3dps_t obj = ius3DParametricSourceCreate(numThetaLocations, numPhiLocations, FNumber + 0.01f, deltaTheta, startTheta, deltaPhi, startPhi);

    TEST_ASSERT_EQUAL_FLOAT(FNumber,ius3DParametricSourceGetFNumber(obj));
    TEST_ASSERT_EQUAL_FLOAT(deltaTheta,ius3DParametricSourceGetDeltaTheta(obj));
    TEST_ASSERT_EQUAL_FLOAT(startTheta,ius3DParametricSourceGetStartTheta(obj));
    TEST_ASSERT_EQUAL_FLOAT(deltaPhi,ius3DParametricSourceGetDeltaPhi(obj));
    TEST_ASSERT_EQUAL_FLOAT(startPhi,ius3DParametricSourceGetStartPhi(obj));

    // invalid param
    TEST_ASSERT_EQUAL_FLOAT(NAN,ius3DParametricSourceGetFNumber(NULL));
    TEST_ASSERT_EQUAL_FLOAT(NAN,ius3DParametricSourceGetDeltaTheta(NULL));
    TEST_ASSERT_EQUAL_FLOAT(NAN,ius3DParametricSourceGetStartTheta(NULL));
    TEST_ASSERT_EQUAL_FLOAT(NAN,ius3DParametricSourceGetDeltaPhi(NULL));
    TEST_ASSERT_EQUAL_FLOAT(NAN,ius3DParametricSourceGetStartPhi(NULL));

    ius3DParametricSourceDelete(obj);
}

TEST(Ius3DParametricSource, testIus3DParametricSourceSerialization)
{
    char *filename = "testIus3DParametricSourceSerialization.hdf5";
	int status;
	float FNumber = -0.955f;
	float startAngle = IUS_PI;
	float startTheta = startAngle;
	float deltaTheta = IUS_PI / 12;
	float startPhi = startAngle;
	float deltaPhi = IUS_PI / 20;
	int numPhiLocations = 5;
	int numThetaLocations = 3;


    // create
    iu3dps_t
    obj = ius3DParametricSourceCreate(numThetaLocations, numPhiLocations, FNumber, deltaTheta, startTheta, deltaPhi, startPhi);

    // fill
    TEST_ASSERT_EQUAL_FLOAT(FNumber, ius3DParametricSourceGetFNumber(obj));
    TEST_ASSERT_EQUAL_FLOAT(deltaTheta, ius3DParametricSourceGetDeltaTheta(obj));
    TEST_ASSERT_EQUAL_FLOAT(startAngle, ius3DParametricSourceGetStartTheta(obj));
    TEST_ASSERT_EQUAL_FLOAT(deltaPhi, ius3DParametricSourceGetDeltaPhi(obj));
    TEST_ASSERT_EQUAL_FLOAT(startPhi, ius3DParametricSourceGetStartPhi(obj));

    // save
    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
    status = ius3DParametricSourceSave(obj, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iu3dps_t savedObj = ius3DParametricSourceLoad(handle);
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
