//
// Created by nlv09165 on 31/07/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <ius.h>
#include <util.h>
#include <iusSourceDictPrivate.h>
#include <string.h>

static char *pErrorFilename = "IusSourceDict.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(IusSourceDict);

TEST_SETUP(IusSourceDict)
{
    iusErrorLogClear();
    iusErrorLog(IUS_TRUE);
    iusErrorAutoReport(IUS_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iusErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IusSourceDict)
{
    iusErrorLogClear();
    iusErrorLog(IUS_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iusErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
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
    int numLocationsPhi = 5; /**< number of locations */
	int numLocationsTheta = 3; /**< number of locations */
    // Happy flow
    float angularDelta = 0.13f;
    float FNumber = -0.955f;
    float startAngle = 3.14f;
    float startPhi = startAngle;
    float deltaPhi = angularDelta;
	float startTheta = startAngle;
	float deltaTheta = angularDelta;
    iu3dps_t obj = ius3DParametricSourceCreate(numLocationsTheta, numLocationsPhi, FNumber,
                                                            deltaTheta, startTheta, deltaPhi, startPhi);

    TEST_ASSERT(obj != IU3DPS_INVALID);

    iusd_t dict = iusSourceDictCreate();
    int status = iusSourceDictSet(dict, pObjLabel,(ius_t) obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    ius_t retrievedObj = iusSourceDictGet(dict, pObjLabel);
    TEST_ASSERT_EQUAL(IUS_TRUE, ius3DParametricSourceCompare(obj, (iu3dps_t) retrievedObj));

    // Invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());

    TEST_ASSERT_EQUAL(IUS_INVALID, iusSourceDictGet(dict,NULL));
    TEST_ASSERT_EQUAL(IUS_INVALID, iusSourceDictGet(NULL,pObjLabel));
    TEST_ASSERT_EQUAL(IUS_INVALID, iusSourceDictGet(dict,"unknownLabel"));

    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, iusSourceDictSet(dict, pObjLabel, (ius_t) obj));
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, iusSourceDictSet(NULL, pObjLabel, (ius_t) obj));
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, iusSourceDictSet(dict, pObjLabel, NULL));

    TEST_ASSERT_EQUAL(6,iusErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

    ius3DParametricSourceDelete(obj);
    iusSourceDictDelete(dict);

}


TEST(IusSourceDict, testIusSourceDictKeys)
{
    int numLocationsPhi = 5; /**< number of locations */
    int numLocationsTheta = 3; /**< number of locations */
    // Happy flow
    float angularDelta = 0.13f;
    float FNumber = -0.955f;
    float startAngle = 3.14f;
    float startPhi = startAngle;
    float deltaPhi = angularDelta;
    float startTheta = startAngle;
    float deltaTheta = angularDelta;
    char *labels[] = { "one" , "two" , "three", "four" , "five"};
    iu3dps_t obj = ius3DParametricSourceCreate(numLocationsTheta, numLocationsPhi, FNumber,
                                               deltaTheta, startTheta, deltaPhi, startPhi);

    TEST_ASSERT(obj != IU3DPS_INVALID);

    iusd_t dict = iusSourceDictCreate();

    // Fill dict
    int i, status;
    int keySize = sizeof(labels)/sizeof(labels[0]);
    for (i=0; i<keySize; i++)
    {
        status = iusSourceDictSet(dict, labels[i], (ius_t) obj);
        TEST_ASSERT(status == IUS_E_OK);
    }

    // Get keys
    size_t dictSize = iusSourceDictGetSize(dict);
    TEST_ASSERT_EQUAL(5, dictSize);
    char **keys = iusSourceDictGetKeys(dict);

    // Validate keys
    for (i=0; i<keySize; i++)
    {
        TEST_ASSERT_EQUAL(IUS_TRUE, aInB(keys[i], labels));
    }

    // Invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());

    keys = iusSourceDictGetKeys(NULL);
    TEST_ASSERT_EQUAL(NULL,keys);

    TEST_ASSERT_EQUAL(1,iusErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
    ius3DParametricSourceDelete(obj);
    iusSourceDictDelete(dict);
}

TEST(IusSourceDict, testIusCompareSourceDict)
{
    IUS_BOOL equal;

    char *_3d_non_parametric_label = "label for 3d non parametric source";
    char *_3d_parametric_label = "label for 3d parametric source";
	int numLocationsPhi = 5; /**< number of locations */
	int numLocationsTheta = 3; /**< number of locations */
	// Happy flow
	float angularDelta = 0.13f;
	float FNumber = -0.955f;
	float startAngle = 3.14f;
	float startPhi = startAngle;
	float deltaPhi = angularDelta;
	float startTheta = startAngle;
	float deltaTheta = angularDelta;
    iu3dps_t parametricSource = ius3DParametricSourceCreate(numLocationsTheta, numLocationsPhi, FNumber,
		deltaTheta, startTheta, deltaPhi, startPhi);

    TEST_ASSERT(parametricSource != IU3DPS_INVALID);
    iu3dnps_t nonParametricSource = ius3DNonParametricSourceCreate(numLocationsPhi*numLocationsTheta);
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

    char *filename = "testIusSourceDictSerialization.hdf5";

    // Happy flow
	int numLocationsPhi = 5; /**< number of locations */
	int numLocationsTheta = 3; /**< number of locations */

	float angularDelta = 0.13f;
	float FNumber = -0.955f;
	float startAngle = 3.14f;
	float startPhi = startAngle;
	float deltaPhi = angularDelta;
	float startTheta = startAngle;
	float deltaTheta = angularDelta;
    iu3dps_t parametricSource = ius3DParametricSourceCreate(numLocationsTheta, numLocationsPhi, FNumber,
															deltaTheta, startTheta, deltaPhi, startPhi);

    TEST_ASSERT(parametricSource != IU3DPS_INVALID);
    iu3dnps_t nonParametricSource = ius3DNonParametricSourceCreate(numLocationsPhi*numLocationsTheta);
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
    RUN_TEST_CASE(IusSourceDict, testIusSourceDictKeys);
    RUN_TEST_CASE(IusSourceDict, testIusSourceDictSetGet);
    RUN_TEST_CASE(IusSourceDict, testIusCompareSourceDict);
    RUN_TEST_CASE(IusSourceDict, testIusSerialization);
}
