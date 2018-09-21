//
// Created by nlv09165 on 31/07/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <hdf5.h>
#include <ius.h>
#include <iusHLSourceDictImp.h>
#include <include/iusHL3DParametricSource.h>
#include <include/iusHL3DNonParametricSource.h>

TEST_GROUP(IusSourceDict);

TEST_SETUP(IusSourceDict)
{
}

TEST_TEAR_DOWN(IusSourceDict)
{
}


TEST(IusSourceDict, testIusCreateSourceDict)
{
    iusd_t obj = iusHLSourceDictCreate();
    iusd_t notherObj = iusHLSourceDictCreate();
    TEST_ASSERT(obj != IUSD_INVALID);
    TEST_ASSERT(notherObj != IUSD_INVALID);
    iusHLSourceDictDelete(obj);
    iusHLSourceDictDelete(notherObj);
}


TEST(IusSourceDict, testIusCompareSourceDict)
{
    IUS_BOOL equal;

    char *_3d_non_parametric_label = "label for 3d non parametric source";
    char *_3d_parametric_label = "label for 3d parametric source";
    int locationCount = 5; /**< number of locations */

    // Happy flow
    float angularDelta = 0.13f;
    float FNumber = -0.955f;
    float startAngle = 3.14f;
    float startPhi = startAngle;
    float deltaPhi = angularDelta;
    iu3dps_t parametricSource = iusHL3DParametricSourceCreate(_3d_parametric_label, locationCount, FNumber,
                                                        angularDelta, startAngle, deltaPhi, startPhi);

    TEST_ASSERT(parametricSource != IU3DPS_INVALID);
    iu3dps_t _nother3dps = iusHL3DParametricSourceCreate(_3d_parametric_label, locationCount, FNumber,
                                                              angularDelta, startAngle, deltaPhi, startPhi);

    TEST_ASSERT(_nother3dps != IU3DPS_INVALID);
    iu3dnps_t nonParametricSource = iusHL3DNonParametricSourceCreate(_3d_non_parametric_label, locationCount);
    TEST_ASSERT(nonParametricSource != IU3DNPS_INVALID);

    // Same empty lists...
    iusd_t dict = iusHLSourceDictCreate();
    iusd_t notherDict = iusHLSourceDictCreate();
    TEST_ASSERT(dict != IUSD_INVALID);
    TEST_ASSERT(notherDict != IUSD_INVALID);

    equal = iusHLSourceDictCompare(dict, dict);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = iusHLSourceDictCompare(dict, notherDict);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);

    int status = iusHLSourceDictSet(dict, _3d_parametric_label,(ius_t) parametricSource);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    equal = iusHLSourceDictCompare(dict, notherDict);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    status = iusHLSourceDictSet(notherDict,_3d_parametric_label,(ius_t) parametricSource);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    equal = iusHLSourceDictCompare(dict, notherDict);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);

    status = iusHLSourceDictSet(dict,_3d_non_parametric_label,(ius_t) nonParametricSource);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    equal = iusHLSourceDictCompare(dict, notherDict);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    status = iusHLSourceDictSet(notherDict,_3d_non_parametric_label,(ius_t) nonParametricSource);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    equal = iusHLSourceDictCompare(dict, notherDict);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);


    // invalid params
    equal = iusHLSourceDictCompare(dict, NULL);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    equal = iusHLSourceDictCompare(NULL, dict);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

    iusHLSourceDictDelete(dict);
    iusHLSourceDictDelete(notherDict);
}

TEST(IusSourceDict, testIusSerialization)
{
//    IUS_BOOL equal;

    char *_3d_non_parametric_label = "label for 3d non parametric source";
    char *_3d_parametric_label = "label for 3d parametric source";
    int locationCount = 5; /**< number of locations */
    char *filename = "testIusSourceDictSerialization.hdf5";
    //char *dictPath =  "/SourceDict";

    // Happy flow
    float angularDelta = 0.13f;
    float FNumber = -0.955f;
    float startAngle = 3.14f;
    float startPhi = startAngle;
    float deltaPhi = angularDelta;
    iu3dps_t parametricSource = iusHL3DParametricSourceCreate(_3d_parametric_label, locationCount, FNumber,
                                                              angularDelta, startAngle, deltaPhi, startPhi);

    TEST_ASSERT(parametricSource != IU3DPS_INVALID);
    iu3dps_t _nother3dps = iusHL3DParametricSourceCreate(_3d_parametric_label, locationCount, FNumber,
                                                         angularDelta, startAngle, deltaPhi, startPhi);

    TEST_ASSERT(_nother3dps != IU3DPS_INVALID);
    iu3dnps_t nonParametricSource = iusHL3DNonParametricSourceCreate(_3d_non_parametric_label, locationCount);
    TEST_ASSERT(nonParametricSource != IU3DNPS_INVALID);

    // create
    iusd_t dict = iusHLSourceDictCreate();
    TEST_ASSERT(dict != IUSD_INVALID);

    // fill
    int status = iusHLSourceDictSet(dict, _3d_parametric_label,(ius_t) parametricSource);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    status = iusHLSourceDictSet(dict, _3d_non_parametric_label,(ius_t) nonParametricSource);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // save
    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
    status = iusHLSourceDictSave(dict, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iusd_t savedObj = iusHLSourceDictLoad(handle);
    TEST_ASSERT(savedObj != NULL);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUS_TRUE, iusHLSourceDictCompare(dict,savedObj));

    iusHLSourceDictDelete(dict);
    iusHLSourceDictDelete(savedObj);

}


TEST_GROUP_RUNNER(IusSourceDict)
{
    RUN_TEST_CASE(IusSourceDict, testIusCreateSourceDict);
    RUN_TEST_CASE(IusSourceDict, testIusCompareSourceDict);
    RUN_TEST_CASE(IusSourceDict, testIusSerialization);
}
