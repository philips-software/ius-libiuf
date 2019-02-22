

//
// Created by nlv09165 on 24/07/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <ius.h>
#include <iusSourcePrivate.h>

static char *pErrorFilename = "IusSource.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(IusSource);

TEST_SETUP(IusSource)
{
    iusErrorLogClear();
    iusErrorLog(IUS_TRUE);
    iusErrorAutoReport(IUS_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iusErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IusSource)
{
    iusErrorLogClear();
    iusErrorLog(IUS_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iusErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
}


TEST(IusSource, testIusSourceDelete)
{
	int numLocationsPhi = 5; /**< number of locations */
	int numLocationsTheta = 3; /**< number of locations */

	float angularDelta = 0.13f;
	float FNumber = -0.955f;
	float startAngle = 3.14f;
	float startPhi = startAngle;
	float deltaPhi = angularDelta;
	float startTheta = startAngle;
	float deltaTheta = angularDelta;
    ius_t obj = (ius_t) ius3DParametricSourceCreate(numLocationsTheta, numLocationsPhi, FNumber,
		deltaTheta, startTheta, deltaPhi, startPhi);
    TEST_ASSERT(obj != IUS_INVALID);

    int status = iusSourceDelete((ius_t)obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());

    status = iusSourceDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);

    TEST_ASSERT_EQUAL(1,iusErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
}



TEST(IusSource, testIusSourceCompare)
{
    IUS_BOOL equal;
	int numLocationsPhi = 5; /**< number of locations */
	int numLocationsTheta = 3; /**< number of locations */

	float angularDelta = 0.13f;
	float FNumber = -0.955f;
	float startAngle = 3.14f;
	float startPhi = startAngle;
	float deltaPhi = angularDelta;
	float startTheta = startAngle;
	float deltaTheta = angularDelta;
    ius_t _3dps = ius3DParametricSourceCreate(numLocationsTheta, numLocationsPhi, FNumber,
		deltaTheta, startTheta, deltaPhi, startPhi);

    TEST_ASSERT(_3dps != IUS_INVALID);
    ius_t _nother3dps = ius3DParametricSourceCreate(numLocationsTheta, numLocationsPhi, FNumber,
		deltaTheta, startTheta, deltaPhi, startPhi);

    TEST_ASSERT(_3dps != IUS_INVALID);
    ius_t _3dnps = ius3DNonParametricSourceCreate(numLocationsTheta*numLocationsPhi);
    TEST_ASSERT(_3dnps != IUS_INVALID);
    ius_t _2dps = ius2DParametricSourceCreate(numLocationsTheta*numLocationsPhi, FNumber,
    angularDelta, startAngle);
    TEST_ASSERT(_2dps != IUS_INVALID);


    // Happy flow
    equal = iusSourceCompare(_3dps,_3dps);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = iusSourceCompare(_3dps,_nother3dps);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = iusSourceCompare(_3dps,_2dps);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

    // invalid params
    equal = iusSourceCompare(_3dps,NULL);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    equal = iusSourceCompare(NULL,_3dps);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

    int status = iusSourceDelete(_3dps);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);
    status = iusSourceDelete(_nother3dps);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);
    status = iusSourceDelete(_3dnps);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);
    status = iusSourceDelete(_2dps);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);
}


TEST(IusSource, testIusSourceGetSet)
{
    int numLocations = 5;

    // Happy flow
    ius_t obj = ius3DNonParametricSourceCreate(numLocations);
    TEST_ASSERT(obj != IUS_INVALID);
    TEST_ASSERT_EQUAL(IUS_3D_NON_PARAMETRIC_SOURCE,iusSourceGetType(obj));
    iusSourceDelete(obj);

    // invalid param
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());
    TEST_ASSERT_EQUAL(IUS_INVALID_SOURCE_TYPE,iusSourceGetType(NULL));

    TEST_ASSERT_EQUAL(1,iusErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
}

TEST(IusSource, testIusSerialization)
{
    // create 4 types of object, save, load compare.
    char *filename = "testIusSourceSerialization.hdf5";
	int numLocationsPhi = 5; /**< number of locations */
	int numLocationsTheta = 3; /**< number of locations */
	int status;
	float angularDelta = 0.13f;
	float FNumber = -0.955f;
	float startAngle = 3.14f;
	float startPhi = startAngle;
	float deltaPhi = angularDelta;
	float startTheta = startAngle;
	float deltaTheta = angularDelta;
    ius_t obj = (ius_t) ius3DParametricSourceCreate(numLocationsTheta, numLocationsPhi, FNumber,
		deltaTheta, startTheta, deltaPhi, startPhi);

    ius_t notherObj = (ius_t) ius3DParametricSourceCreate(numLocationsTheta, numLocationsPhi, FNumber+0.01f,
		deltaTheta, startTheta, deltaPhi, startPhi);
    // fill
    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
    status = iusSourceSave( (ius_t) obj, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    ius_t savedObj = iusSourceLoad(handle);
    TEST_ASSERT_NOT_EQUAL(NULL, savedObj);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_TRUE, iusSourceCompare((ius_t)obj,savedObj));
    TEST_ASSERT_EQUAL(IUS_FALSE, iusSourceCompare((ius_t)notherObj,savedObj));

    iusSourceDelete(savedObj);
    iusSourceDelete(obj);
	iusSourceDelete(notherObj);
}



TEST_GROUP_RUNNER(IusSource)
{
    RUN_TEST_CASE(IusSource, testIusSourceDelete);
    RUN_TEST_CASE(IusSource, testIusSourceCompare);
    RUN_TEST_CASE(IusSource, testIusSourceGetSet);
    RUN_TEST_CASE(IusSource, testIusSerialization);
}
