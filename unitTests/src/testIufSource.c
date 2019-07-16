#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <iuf.h>
#include <iufSourcePrivate.h>

static char *pErrorFilename = "IufSource.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(IufSource);

TEST_SETUP(IufSource)
{
    iufErrorLogClear();
    iufErrorLog(IUF_TRUE);
    iufErrorAutoReportSet(IUF_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iufErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IufSource)
{
    iufErrorLogClear();
    iufErrorLog(IUF_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iufErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
}


TEST(IufSource, testIufSourceDelete)
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
    ius_t obj = (ius_t) iuf3DParametricSourceCreate(numLocationsTheta, numLocationsPhi, FNumber,
		deltaTheta, startTheta, deltaPhi, startPhi);
    TEST_ASSERT(obj != IUS_INVALID);

    int status = iufSourceDelete((ius_t)obj);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    // invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    status = iufSourceDelete(NULL);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, status);

    TEST_ASSERT_EQUAL(1,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
}



TEST(IufSource, testIufSourceCompare)
{
    IUF_BOOL equal;
	int numLocationsPhi = 5; /**< number of locations */
	int numLocationsTheta = 3; /**< number of locations */

	float angularDelta = 0.13f;
	float FNumber = -0.955f;
	float startAngle = 3.14f;
	float startPhi = startAngle;
	float deltaPhi = angularDelta;
	float startTheta = startAngle;
	float deltaTheta = angularDelta;
    ius_t _3dps = (ius_t) iuf3DParametricSourceCreate(numLocationsTheta, numLocationsPhi, FNumber,
		deltaTheta, startTheta, deltaPhi, startPhi);

    TEST_ASSERT(_3dps != IUS_INVALID);
    ius_t _nother3dps = (ius_t) iuf3DParametricSourceCreate(numLocationsTheta, numLocationsPhi, FNumber,
		deltaTheta, startTheta, deltaPhi, startPhi);

    TEST_ASSERT(_3dps != IUS_INVALID);
    ius_t _3dnps = (ius_t) iuf3DNonParametricSourceCreate(numLocationsTheta*numLocationsPhi);
    TEST_ASSERT(_3dnps != IUS_INVALID);
    ius_t _2dps = (ius_t) iuf2DParametricSourceCreate(numLocationsTheta*numLocationsPhi, FNumber,
    angularDelta, startAngle);
    TEST_ASSERT(_2dps != IUS_INVALID);


    // Happy flow
    equal = iufSourceCompare(_3dps,_3dps);
    TEST_ASSERT_EQUAL(IUF_TRUE,equal);
    equal = iufSourceCompare(_3dps,_nother3dps);
    TEST_ASSERT_EQUAL(IUF_TRUE,equal);
    equal = iufSourceCompare(_3dps,_2dps);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);

    // invalid params
    equal = iufSourceCompare(_3dps,NULL);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);
    equal = iufSourceCompare(NULL,_3dps);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);

    int status = iufSourceDelete(_3dps);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);
    status = iufSourceDelete(_nother3dps);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);
    status = iufSourceDelete(_3dnps);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);
    status = iufSourceDelete(_2dps);
    TEST_ASSERT_EQUAL(IUF_E_OK, status);
}


TEST(IufSource, testIufSourceGetSet)
{
    int numLocations = 5;

    // Happy flow
    ius_t obj = (ius_t) iuf3DNonParametricSourceCreate(numLocations);
    TEST_ASSERT(obj != IUS_INVALID);
    TEST_ASSERT_EQUAL(IUF_3D_NON_PARAMETRIC_SOURCE,iufSourceGetType(obj));
    iufSourceDelete(obj);

    // invalid param
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());
    TEST_ASSERT_EQUAL(IUF_INVALID_SOURCE_TYPE,iufSourceGetType(NULL));

    TEST_ASSERT_EQUAL(1,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
}

TEST(IufSource, testIufSerialization)
{
    // create 4 types of object, save, load compare.
    char *filename = "testIufSourceSerialization.hdf5";
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
    ius_t obj = (ius_t) iuf3DParametricSourceCreate(numLocationsTheta, numLocationsPhi, FNumber,
		deltaTheta, startTheta, deltaPhi, startPhi);

    ius_t notherObj = (ius_t) iuf3DParametricSourceCreate(numLocationsTheta, numLocationsPhi, FNumber+0.01f,
		deltaTheta, startTheta, deltaPhi, startPhi);
    // fill
    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
    status = iufSourceSave( (ius_t) obj, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    ius_t savedObj = iufSourceLoad(handle);
    TEST_ASSERT_NOT_EQUAL(NULL, savedObj);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUF_TRUE, iufSourceCompare((ius_t)obj,savedObj));
    TEST_ASSERT_EQUAL(IUF_FALSE, iufSourceCompare((ius_t)notherObj,savedObj));

    iufSourceDelete(savedObj);
    iufSourceDelete(obj);
	iufSourceDelete(notherObj);
}



TEST_GROUP_RUNNER(IufSource)
{
    RUN_TEST_CASE(IufSource, testIufSourceDelete);
    RUN_TEST_CASE(IufSource, testIufSourceCompare);
    RUN_TEST_CASE(IufSource, testIufSourceGetSet);
    RUN_TEST_CASE(IufSource, testIufSerialization);
}
