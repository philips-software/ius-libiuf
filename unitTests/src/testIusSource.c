

//
// Created by nlv09165 on 24/07/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <ius.h>
#include <iusSourcePrivate.h>

TEST_GROUP(IusSource);

TEST_SETUP(IusSource)
{
}

TEST_TEAR_DOWN(IusSource)
{
}


TEST(IusSource, testIusSourceDelete)
{
    int locationCount = 5;

    float angularDelta = 0.13f;
    float FNumber = -0.955f;
    float startAngle = 3.14f;
    float startPhi = startAngle;
    float deltaPhi = angularDelta;
    ius_t obj = (ius_t) ius3DParametricSourceCreate(locationCount, FNumber,
                                                              angularDelta, startAngle, deltaPhi, startPhi);
    TEST_ASSERT(obj != IUS_INVALID);

    int status = iusSourceDelete((ius_t)obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // invalid params
    status = iusSourceDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
}



TEST(IusSource, testIusSourceCompare)
{
    IUS_BOOL equal;
    int locationCount = 5; /**< number of locations */

    // Happy flow
    float angularDelta = 0.13f;
    float FNumber = -0.955f;
    float startAngle = 3.14f;
    float startPhi = startAngle;
    float deltaPhi = angularDelta;
    ius_t _3dps = (ius_t) ius3DParametricSourceCreate(locationCount, FNumber,
                                                        angularDelta, startAngle, deltaPhi, startPhi);

    TEST_ASSERT(_3dps != IUS_INVALID);
    ius_t _nother3dps = (ius_t) ius3DParametricSourceCreate(locationCount, FNumber,
                                                        angularDelta, startAngle, deltaPhi, startPhi);

    TEST_ASSERT(_3dps != IUS_INVALID);
    ius_t _3dnps = (ius_t) ius3DNonParametricSourceCreate(locationCount);
    TEST_ASSERT(_3dnps != IUS_INVALID);
    ius_t _2dps = (ius_t) ius2DParametricSourceCreate(locationCount, FNumber,
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
    int locationCount = 5;

    // Happy flow
    ius_t obj = (ius_t) ius3DNonParametricSourceCreate(locationCount);
    TEST_ASSERT(obj != IUS_INVALID);
    TEST_ASSERT_EQUAL(IUS_3D_NON_PARAMETRIC_SOURCE,iusSourceGetType(obj));

    // invalid param
    TEST_ASSERT_EQUAL(IUS_INVALID_SOURCE_TYPE,iusSourceGetType(NULL));
    iusSourceDelete(obj);
}

TEST(IusSource, testIusSerialization)
{
    // create 4 types of object, save, load compare.
    char *filename = "testIusSourceSerialization.hdf5";
    int status, locationCount = 50; /**< number of locations */

    float angularDelta = 0.13f;
    float FNumber = -0.955f;
    float startAngle = 3.14f;
    float startPhi = startAngle;
    float deltaPhi = angularDelta;
    ius_t obj = (ius_t) ius3DParametricSourceCreate(locationCount, FNumber,
                                                        angularDelta, startAngle, deltaPhi, startPhi);

    ius_t notherObj = (ius_t) ius3DParametricSourceCreate(locationCount+1, FNumber,
                                                      angularDelta, startAngle, deltaPhi, startPhi);
    // fill
    int p;
    for (p = 0; p < locationCount; p++)
    {
        iu3dp_t pos = ius3DPositionCreate(p * 1.0f, p * 2.0f, p * 3.0f);
        ius3DParametricSourceSetPosition( (iu3dps_t) obj, pos, p);
        ius3DPositionDelete(pos);
    }

    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
    status = iusSourceSave( (ius_t) obj, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

	/* can't be done without knowing the label */
	// todo fix this test
#if 0
    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    ius_t savedObj = iusSourceLoad(handle);
    TEST_ASSERT_NOT_EQUAL(NULL, savedObj);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_TRUE, iusSourceCompare((ius_t)obj,savedObj));
    TEST_ASSERT_EQUAL(IUS_FALSE, iusSourceCompare((ius_t)notherObj,savedObj));
#endif
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
