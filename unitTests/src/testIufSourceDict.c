//
// Created by nlv09165 on 31/07/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <iuf.h>
#include <util.h>
#include <iufSourceDictPrivate.h>
#include <string.h>

static char *pErrorFilename = "IufSourceDict.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(IufSourceDict);

TEST_SETUP(IufSourceDict)
{
    iufErrorLogClear();
    iufErrorLog(IUF_TRUE);
    iufErrorAutoReportSet(IUF_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iufErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IufSourceDict)
{
    iufErrorLogClear();
    iufErrorLog(IUF_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iufErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
}



TEST(IufSourceDict, testIufCreateSourceDict)
{
    iusd_t obj = iufSourceDictCreate();
    iusd_t notherObj = iufSourceDictCreate();
    TEST_ASSERT(obj != IUFD_INVALID);
    TEST_ASSERT(notherObj != IUFD_INVALID);
    iufSourceDictDelete(obj);
    iufSourceDictDelete(notherObj);
}

TEST(IufSourceDict, testIufSourceDictSetGet)
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
    iu3dps_t obj = iuf3DParametricSourceCreate(numLocationsTheta, numLocationsPhi, FNumber,
                                                            deltaTheta, startTheta, deltaPhi, startPhi);

    TEST_ASSERT(obj != IU3DPS_INVALID);

    iusd_t dict = iufSourceDictCreate();
    int status = iufSourceDictSet(dict, pObjLabel,(ius_t) obj);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    ius_t retrievedObj = iufSourceDictGet(dict, pObjLabel);
    TEST_ASSERT_EQUAL(IUF_TRUE, iuf3DParametricSourceCompare(obj, (iu3dps_t) retrievedObj));

    // Invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    TEST_ASSERT_EQUAL(IUF_INVALID, iufSourceDictGet(dict,NULL));
    TEST_ASSERT_EQUAL(IUF_INVALID, iufSourceDictGet(NULL,pObjLabel));
    TEST_ASSERT_EQUAL(IUF_INVALID, iufSourceDictGet(dict,"unknownLabel"));

    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, iufSourceDictSet(dict, pObjLabel, (ius_t) obj));
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, iufSourceDictSet(NULL, pObjLabel, (ius_t) obj));
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, iufSourceDictSet(dict, pObjLabel, NULL));

    TEST_ASSERT_EQUAL(6,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

    iuf3DParametricSourceDelete(obj);
    iufSourceDictDelete(dict);

}


TEST(IufSourceDict, testIufSourceDictKeys)
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
    iu3dps_t obj = iuf3DParametricSourceCreate(numLocationsTheta, numLocationsPhi, FNumber,
                                               deltaTheta, startTheta, deltaPhi, startPhi);

    TEST_ASSERT(obj != IU3DPS_INVALID);

    iusd_t dict = iufSourceDictCreate();

    // Fill dict
    int i, status;
    int keySize = sizeof(labels)/sizeof(labels[0]);
    for (i=0; i<keySize; i++)
    {
        status = iufSourceDictSet(dict, labels[i], (ius_t) obj);
        TEST_ASSERT(status == IUF_E_OK);
    }

    // Get keys
    size_t dictSize = iufSourceDictGetSize(dict);
    TEST_ASSERT_EQUAL(5, dictSize);
    char **keys = iufSourceDictGetKeys(dict);

    // Validate keys
    for (i=0; i<keySize; i++)
    {
        TEST_ASSERT_EQUAL(IUF_TRUE, aInB(keys[i], labels));
    }

    // Invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    keys = iufSourceDictGetKeys(NULL);
    TEST_ASSERT_EQUAL(NULL,keys);

    TEST_ASSERT_EQUAL(1,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
    iuf3DParametricSourceDelete(obj);
    iufSourceDictDelete(dict);
}

TEST(IufSourceDict, testIufCompareSourceDict)
{
    IUF_BOOL equal;

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
    iu3dps_t parametricSource = iuf3DParametricSourceCreate(numLocationsTheta, numLocationsPhi, FNumber,
		deltaTheta, startTheta, deltaPhi, startPhi);

    TEST_ASSERT(parametricSource != IU3DPS_INVALID);
    iu3dnps_t nonParametricSource = iuf3DNonParametricSourceCreate(numLocationsPhi*numLocationsTheta);
    TEST_ASSERT(nonParametricSource != IU3DNPS_INVALID);

    // Same empty lists...
    iusd_t dict = iufSourceDictCreate();
    iusd_t notherDict = iufSourceDictCreate();
    TEST_ASSERT(dict != IUFD_INVALID);
    TEST_ASSERT(notherDict != IUFD_INVALID);

    equal = iufSourceDictCompare(dict, dict);
    TEST_ASSERT_EQUAL(IUF_TRUE,equal);
    equal = iufSourceDictCompare(dict, notherDict);
    TEST_ASSERT_EQUAL(IUF_TRUE,equal);

    int status = iufSourceDictSet(dict, _3d_parametric_label,(ius_t) parametricSource);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);
    equal = iufSourceDictCompare(dict, notherDict);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);
    status = iufSourceDictSet(notherDict,_3d_parametric_label,(ius_t) parametricSource);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);
    equal = iufSourceDictCompare(dict, notherDict);
    TEST_ASSERT_EQUAL(IUF_TRUE,equal);

    status = iufSourceDictSet(dict,_3d_non_parametric_label,(ius_t) nonParametricSource);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);
    equal = iufSourceDictCompare(dict, notherDict);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);
    status = iufSourceDictSet(notherDict,_3d_non_parametric_label,(ius_t) nonParametricSource);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);
    equal = iufSourceDictCompare(dict, notherDict);
    TEST_ASSERT_EQUAL(IUF_TRUE,equal);


    // invalid params
    equal = iufSourceDictCompare(dict, NULL);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);
    equal = iufSourceDictCompare(NULL, dict);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);

    iufSourceDictDelete(dict);
    iufSourceDictDelete(notherDict);
    iuf3DParametricSourceDelete(parametricSource);
    iuf3DNonParametricSourceDelete(nonParametricSource);

}

TEST(IufSourceDict, testIufSerialization)
{
    char *_3d_non_parametric_label = "label for 3d non parametric source";
    char *_3d_parametric_label = "label for 3d parametric source";

    char *filename = "testIufSourceDictSerialization.hdf5";

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
    iu3dps_t parametricSource = iuf3DParametricSourceCreate(numLocationsTheta, numLocationsPhi, FNumber,
															deltaTheta, startTheta, deltaPhi, startPhi);

    TEST_ASSERT(parametricSource != IU3DPS_INVALID);
    iu3dnps_t nonParametricSource = iuf3DNonParametricSourceCreate(numLocationsPhi*numLocationsTheta);
    TEST_ASSERT(nonParametricSource != IU3DNPS_INVALID);

    // create
    iusd_t dict = iufSourceDictCreate();
    TEST_ASSERT(dict != IUFD_INVALID);

    // fill
    int status = iufSourceDictSet(dict, _3d_parametric_label,(ius_t) parametricSource);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);
    status = iufSourceDictSet(dict, _3d_non_parametric_label,(ius_t) nonParametricSource);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    // save
    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
    status = iufSourceDictSave(dict, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iusd_t savedObj = iufSourceDictLoad(handle);
    TEST_ASSERT(savedObj != NULL);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUF_TRUE, iufSourceDictCompare(dict,savedObj));

    iufSourceDictDelete(dict);
    iufSourceDictDelete(savedObj);
    iuf3DParametricSourceDelete(parametricSource);
    iuf3DNonParametricSourceDelete(nonParametricSource);
}


TEST_GROUP_RUNNER(IufSourceDict)
{
    RUN_TEST_CASE(IufSourceDict, testIufCreateSourceDict);
    RUN_TEST_CASE(IufSourceDict, testIufSourceDictKeys);
    RUN_TEST_CASE(IufSourceDict, testIufSourceDictSetGet);
    RUN_TEST_CASE(IufSourceDict, testIufCompareSourceDict);
    RUN_TEST_CASE(IufSourceDict, testIufSerialization);
}
