//
// Created by nlv09165 on 31/07/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <ius.h>
#include <iusSourceDictPrivate.h>

TEST_GROUP(IusSourceDict);

TEST_SETUP(IusSourceDict)
{
}

TEST_TEAR_DOWN(IusSourceDict)
{
}


TEST(IusSourceDict, testIusCreateSourceDict)
{
    iusd_t obj = iusSourceDictCreate();
    iusd_t notherObj = iusSourceDictCreate();
    TEST_ASSERT(obj != IUSD_INVALID);
    TEST_ASSERT(notherObj != IUSD_INVALID);
    iusSourceDictDelete(obj);
    iusSourceDictDelete(notherObj);
}

TEST(IusSourceDict, testIusSourceDictSetGet)
{
    char *pObjLabel = "label for 3d parametric source";
    int numLocations = 5; /**< number of locations */

    // Happy flow
    float angularDelta = 0.13f;
    float FNumber = -0.955f;
    float startAngle = 3.14f;
    float startPhi = startAngle;
    float deltaPhi = angularDelta;
    iu3dps_t obj = ius3DParametricSourceCreate(numLocations, FNumber,
                                                            angularDelta, startAngle, deltaPhi, startPhi);

    TEST_ASSERT(obj != IU3DPS_INVALID);

    iusd_t dict = iusSourceDictCreate();
    int status = iusSourceDictSet(dict, pObjLabel,(ius_t) obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    ius_t retrievedObj = iusSourceDictGet(dict, pObjLabel);
    TEST_ASSERT_EQUAL(IUS_TRUE, ius3DParametricSourceCompare(obj, (iu3dps_t) retrievedObj));

    // Invalid params
    TEST_ASSERT_EQUAL(IUS_INVALID, iusSourceDictGet(dict,NULL));
    TEST_ASSERT_EQUAL(IUS_INVALID, iusSourceDictGet(NULL,pObjLabel));
    TEST_ASSERT_EQUAL(IUS_INVALID, iusSourceDictGet(dict,"unknownLabel"));
    ius3DParametricSourceDelete(obj);
    iusSourceDictDelete(dict);

}

TEST(IusSourceDict, testIusCompareSourceDict)
{
    IUS_BOOL equal;

    char *_3d_non_parametric_label = "label for 3d non parametric source";
    char *_3d_parametric_label = "label for 3d parametric source";
    int numLocations = 5; /**< number of locations */

    // Happy flow
    float angularDelta = 0.13f;
    float FNumber = -0.955f;
    float startAngle = 3.14f;
    float startPhi = startAngle;
    float deltaPhi = angularDelta;
    iu3dps_t parametricSource = ius3DParametricSourceCreate(numLocations, FNumber,
                                                        angularDelta, startAngle, deltaPhi, startPhi);

    TEST_ASSERT(parametricSource != IU3DPS_INVALID);
    iu3dnps_t nonParametricSource = ius3DNonParametricSourceCreate(numLocations);
    TEST_ASSERT(nonParametricSource != IU3DNPS_INVALID);

    // Same empty lists...
    iusd_t dict = iusSourceDictCreate();
    iusd_t notherDict = iusSourceDictCreate();
    TEST_ASSERT(dict != IUSD_INVALID);
    TEST_ASSERT(notherDict != IUSD_INVALID);

    equal = iusSourceDictCompare(dict, dict);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = iusSourceDictCompare(dict, notherDict);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);

    int status = iusSourceDictSet(dict, _3d_parametric_label,(ius_t) parametricSource);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    equal = iusSourceDictCompare(dict, notherDict);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    status = iusSourceDictSet(notherDict,_3d_parametric_label,(ius_t) parametricSource);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    equal = iusSourceDictCompare(dict, notherDict);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);

    status = iusSourceDictSet(dict,_3d_non_parametric_label,(ius_t) nonParametricSource);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    equal = iusSourceDictCompare(dict, notherDict);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    status = iusSourceDictSet(notherDict,_3d_non_parametric_label,(ius_t) nonParametricSource);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    equal = iusSourceDictCompare(dict, notherDict);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);


    // invalid params
    equal = iusSourceDictCompare(dict, NULL);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    equal = iusSourceDictCompare(NULL, dict);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

    iusSourceDictDelete(dict);
    iusSourceDictDelete(notherDict);
    ius3DParametricSourceDelete(parametricSource);
    ius3DNonParametricSourceDelete(nonParametricSource);

}

TEST(IusSourceDict, testIusSerialization)
{
    char *_3d_non_parametric_label = "label for 3d non parametric source";
    char *_3d_parametric_label = "label for 3d parametric source";
    int numLocations = 5; /**< number of locations */
    char *filename = "testIusSourceDictSerialization.hdf5";

    // Happy flow
    float angularDelta = 0.13f;
    float FNumber = -0.955f;
    float startAngle = 3.14f;
    float startPhi = startAngle;
    float deltaPhi = angularDelta;
    iu3dps_t parametricSource = ius3DParametricSourceCreate(numLocations, FNumber,
                                                              angularDelta, startAngle, deltaPhi, startPhi);

    TEST_ASSERT(parametricSource != IU3DPS_INVALID);
    iu3dnps_t nonParametricSource = ius3DNonParametricSourceCreate(numLocations);
    TEST_ASSERT(nonParametricSource != IU3DNPS_INVALID);

    // create
    iusd_t dict = iusSourceDictCreate();
    TEST_ASSERT(dict != IUSD_INVALID);

    // fill
    int status = iusSourceDictSet(dict, _3d_parametric_label,(ius_t) parametricSource);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    status = iusSourceDictSet(dict, _3d_non_parametric_label,(ius_t) nonParametricSource);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // save
    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
    status = iusSourceDictSave(dict, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iusd_t savedObj = iusSourceDictLoad(handle);
    TEST_ASSERT(savedObj != NULL);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUS_TRUE, iusSourceDictCompare(dict,savedObj));

    iusSourceDictDelete(dict);
    iusSourceDictDelete(savedObj);
    ius3DParametricSourceDelete(parametricSource);
    ius3DNonParametricSourceDelete(nonParametricSource);
}


TEST_GROUP_RUNNER(IusSourceDict)
{
    RUN_TEST_CASE(IusSourceDict, testIusCreateSourceDict);
    RUN_TEST_CASE(IusSourceDict, testIusSourceDictSetGet);
    RUN_TEST_CASE(IusSourceDict, testIusCompareSourceDict);
    RUN_TEST_CASE(IusSourceDict, testIusSerialization);
}
