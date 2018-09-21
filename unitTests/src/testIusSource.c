

//
// Created by nlv09165 on 24/07/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <include/ius.h>
#include <include/iusError.h>
#include <include/iusTypes.h>
#include <include/iusHL3DSize.h>
#include <include/iusHL3DParametricSource.h>
#include <include/iusHL3DNonParametricSource.h>
#include <include/iusHL2DParametricSource.h>
#include <include/iusHL2DNonParametricSource.h>
#include <hdf5.h>
#include "include/iusHLSourceImp.h"



TEST_GROUP(IusSource);

TEST_SETUP(IusSource)
{
}

TEST_TEAR_DOWN(IusSource)
{
}


TEST(IusSource, testIusSourceDelete)
{
    char *_3d_parametric_label = "label for 3d parametric source";
    int locationCount = 5;

    float angularDelta = 0.13f;
    float FNumber = -0.955f;
    float startAngle = 3.14f;
    float startPhi = startAngle;
    float deltaPhi = angularDelta;
    ius_t obj = (ius_t) iusHL3DParametricSourceCreate(_3d_parametric_label, locationCount, FNumber,
                                                              angularDelta, startAngle, deltaPhi, startPhi);
    TEST_ASSERT(obj != IUS_INVALID);

    int status = iusHLSourceDelete((ius_t)obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // invalid params
    status = iusHLSourceDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
}



TEST(IusSource, testIusSourceCompare)
{
    IUS_BOOL equal;
    char *_3d_non_parametric_label = "label for 3d non parametric source";
    char *_3d_parametric_label = "label for 3d parametric source";
    char *_2d_non_parametric_label = "label for 2d non parametric source";
    char *_2d_parametric_label = "label for 2d parametric source";
    int locationCount = 5; /**< number of locations */

    // Happy flow
    float angularDelta = 0.13f;
    float FNumber = -0.955f;
    float startAngle = 3.14f;
    float startPhi = startAngle;
    float deltaPhi = angularDelta;
    ius_t _3dps = (ius_t) iusHL3DParametricSourceCreate(_3d_parametric_label, locationCount, FNumber,
                                                        angularDelta, startAngle, deltaPhi, startPhi);

    TEST_ASSERT(_3dps != IUS_INVALID);
    ius_t _nother3dps = (ius_t) iusHL3DParametricSourceCreate(_3d_parametric_label, locationCount, FNumber,
                                                        angularDelta, startAngle, deltaPhi, startPhi);

    TEST_ASSERT(_3dps != IUS_INVALID);
    ius_t _3dnps = (ius_t) iusHL3DNonParametricSourceCreate(_3d_non_parametric_label, locationCount);
    TEST_ASSERT(_3dnps != IUS_INVALID);
    ius_t _2dps = (ius_t) iusHL2DParametricSourceCreate(_3d_parametric_label, locationCount, FNumber,
    angularDelta, startAngle);
    TEST_ASSERT(_2dps != IUS_INVALID);


    // Happy flow
    equal = iusHLSourceCompare(_3dps,_3dps);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = iusHLSourceCompare(_3dps,_nother3dps);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = iusHLSourceCompare(_3dps,_2dps);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

    // invalid params
    equal = iusHLSourceCompare(_3dps,NULL);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    equal = iusHLSourceCompare(NULL,_3dps);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

    int status = iusHLSourceDelete(_3dps);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);
    status = iusHLSourceDelete(_nother3dps);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);
    status = iusHLSourceDelete(_3dnps);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);
    status = iusHLSourceDelete(_2dps);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);
}


TEST(IusSource, testIusSourceGetSet)
{
    int locationCount = 5;
    char *_3d_non_parametric_label = "label for 3d non parametric source";

    // Happy flow
    ius_t obj = (ius_t) iusHL3DNonParametricSourceCreate(_3d_non_parametric_label, locationCount);
    TEST_ASSERT(obj != IUS_INVALID);

    TEST_ASSERT_EQUAL(IUS_3D_NON_PARAMETRIC_SOURCE,iusHLSourceGetType(obj));
    TEST_ASSERT_EQUAL_STRING(_3d_non_parametric_label,iusHLSourceGetLabel(obj));


    // invalid param
    TEST_ASSERT_EQUAL(IUS_INVALID_SOURCE_TYPE,iusHLSourceGetType(NULL));
    TEST_ASSERT_EQUAL(NULL,iusHLSourceGetLabel(NULL));
    iusHLSourceDelete(obj);
}

TEST(IusSource, testIusSerialization)
{
    // create 4 types of object, save, load compare.
    char *filename = "testIusSourceSerialization.hdf5";
    char *sourcePath =  "/Source";
    char *_3d_parametric_label = "label for 3d parametric source";
    int status, locationCount = 50; /**< number of locations */

    float angularDelta = 0.13f;
    float FNumber = -0.955f;
    float startAngle = 3.14f;
    float startPhi = startAngle;
    float deltaPhi = angularDelta;
    ius_t obj = (ius_t) iusHL3DParametricSourceCreate(_3d_parametric_label, locationCount, FNumber,
                                                        angularDelta, startAngle, deltaPhi, startPhi);

    ius_t notherObj = (ius_t) iusHL3DParametricSourceCreate(_3d_parametric_label, locationCount+1, FNumber,
                                                      angularDelta, startAngle, deltaPhi, startPhi);
    // fill
    int p;
    for (p = 0; p < locationCount; p++)
    {
        iu3dp_t pos = iusHL3DPositionCreate(p * 1.0f, p * 2.0f, p * 3.0f);
        iusHL3DParametricSourceSetPosition( (iu3dps_t) obj, pos, p);
    }

    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
    status = iusHLSourceSave( (ius_t) obj, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    ius_t savedObj = iusHLSourceLoad(handle, sourcePath);
    TEST_ASSERT_NOT_EQUAL(NULL, savedObj);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUS_TRUE, iusHLSourceCompare((ius_t)obj,savedObj));
    TEST_ASSERT_EQUAL(IUS_FALSE, iusHLSourceCompare((ius_t)notherObj,savedObj));
    iusHLSourceDelete(obj);
    iusHLSourceDelete(savedObj);
}



TEST_GROUP_RUNNER(IusSource)
{
    RUN_TEST_CASE(IusSource, testIusSourceDelete);
    RUN_TEST_CASE(IusSource, testIusSourceCompare);
    RUN_TEST_CASE(IusSource, testIusSourceGetSet);
    RUN_TEST_CASE(IusSource, testIusSerialization);
}
