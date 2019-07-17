#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <iuf.h>
#include <iuf3DParametricSourcePrivate.h>

static char *pErrorFilename = "Iuf3DParametricSource.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(Iuf3DParametricSource);

TEST_SETUP(Iuf3DParametricSource)
{
    iufErrorLogClear();
    iufErrorLog(IUF_TRUE);
    iufErrorAutoReportSet(IUF_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iufErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(Iuf3DParametricSource)
{
    iufErrorLogClear();
    iufErrorLog(IUF_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iufErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
}

TEST(Iuf3DParametricSource, testIuf3DParametricSourceCreate)
{
    float FNumber = -0.955f;
    float startAngle = IUF_PI;
    float startTheta = startAngle;
	float deltaTheta = IUF_PI/12;
	float startPhi = startAngle;
    float deltaPhi = IUF_PI/20;
    int numPhiLocations = 5 ;
	int numThetaLocations = 3;

    iu3dps_t obj = iuf3DParametricSourceCreate(numThetaLocations, numPhiLocations, FNumber, deltaTheta, startTheta, deltaPhi, startPhi);
    iu3dps_t notherObj = iuf3DParametricSourceCreate(numThetaLocations, numPhiLocations, FNumber, deltaTheta, startTheta, deltaPhi, startPhi);
    TEST_ASSERT(obj != IU3DPS_INVALID);
    TEST_ASSERT(notherObj != IU3DPS_INVALID);
    iuf3DParametricSourceDelete(obj);
    iuf3DParametricSourceDelete(notherObj);

    // invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    obj = iuf3DParametricSourceCreate(1, 0, -1, 2.0f, 0, 0, 0);
    TEST_ASSERT(obj == IU3DPS_INVALID);
    obj = iuf3DParametricSourceCreate(-1, 1, 1, -2.0f, 0, 0, 0);
    TEST_ASSERT(obj == IU3DPS_INVALID);

    TEST_ASSERT_EQUAL(2,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

}

TEST(Iuf3DParametricSource, testIuf3DParametricSourceDelete)
{
	float FNumber = -0.955f;
	float startAngle = IUF_PI;
	float startTheta = startAngle;
	float deltaTheta = IUF_PI / 12;
	float startPhi = startAngle;
	float deltaPhi = IUF_PI / 20;
	int numPhiLocations = 5;
	int numThetaLocations = 3;

    iu3dps_t obj = iuf3DParametricSourceCreate(numThetaLocations, numPhiLocations, FNumber, deltaTheta, startTheta, deltaPhi, startPhi);
    TEST_ASSERT(obj != IU3DPS_INVALID);
    int status = iuf3DParametricSourceDelete(obj);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    // invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    status = iuf3DParametricSourceDelete(NULL);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, status);

    TEST_ASSERT_EQUAL(1,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
}



TEST(Iuf3DParametricSource, testIuf3DParametricSourceCompare)
{
    IUF_BOOL equal;
	float FNumber = -0.955f;
	float startAngle = IUF_PI;
	float startTheta = startAngle;
	float deltaTheta = IUF_PI / 12;
	float startPhi = startAngle;
	float deltaPhi = IUF_PI / 20;
	int numPhiLocations = 5;
	int numThetaLocations = 3;

    iu3dps_t obj = iuf3DParametricSourceCreate(numThetaLocations, numPhiLocations, FNumber, deltaTheta, startTheta, deltaPhi, startPhi);
    iu3dps_t notherObj = iuf3DParametricSourceCreate(numThetaLocations, numPhiLocations, FNumber, deltaTheta, startTheta, deltaPhi, startPhi);
    iu3dps_t differentObj =
    iuf3DParametricSourceCreate(numThetaLocations, numPhiLocations, FNumber+0.01f, deltaTheta, startTheta, deltaPhi, startPhi);
    TEST_ASSERT(obj != IU3DPS_INVALID);
    TEST_ASSERT(notherObj != IU3DPS_INVALID);
    equal = iuf3DParametricSourceCompare(obj,obj);
    TEST_ASSERT_EQUAL(IUF_TRUE,equal);
    equal = iuf3DParametricSourceCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUF_TRUE,equal);
    equal = iuf3DParametricSourceCompare(obj,differentObj);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);


    // invalid params
    equal = iuf3DParametricSourceCompare(obj,NULL);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);
    equal = iuf3DParametricSourceCompare(NULL,obj);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);
    
    iuf3DParametricSourceDelete(obj);
    iuf3DParametricSourceDelete(notherObj);
    iuf3DParametricSourceDelete(differentObj);
}

TEST(Iuf3DParametricSource, testIuf3DParametricSourceSetGet)
{
    //IUF_BOOL equal;
	float FNumber = -0.955f;
	float startAngle = IUF_PI;
	float startTheta = startAngle;
	float deltaTheta = IUF_PI / 12;
	float startPhi = startAngle;
	float deltaPhi = IUF_PI / 20;
	int numPhiLocations = 5;
	int numThetaLocations = 3;

    iu3dps_t obj = iuf3DParametricSourceCreate(numThetaLocations, numPhiLocations, FNumber, deltaTheta, startTheta, deltaPhi, startPhi);

    TEST_ASSERT_EQUAL_FLOAT(FNumber,iuf3DParametricSourceGetFNumber(obj));
    TEST_ASSERT_EQUAL_FLOAT(deltaTheta,iuf3DParametricSourceGetDeltaTheta(obj));
    TEST_ASSERT_EQUAL_FLOAT(startTheta,iuf3DParametricSourceGetStartTheta(obj));
    TEST_ASSERT_EQUAL_FLOAT(deltaPhi,iuf3DParametricSourceGetDeltaPhi(obj));
    TEST_ASSERT_EQUAL_FLOAT(startPhi,iuf3DParametricSourceGetStartPhi(obj));
    TEST_ASSERT_EQUAL(numThetaLocations, iuf3DParametricSourceGetNumThetaLocations(obj));
    TEST_ASSERT_EQUAL(numPhiLocations, iuf3DParametricSourceGetNumPhiLocations(obj));


    // invalid param
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    TEST_ASSERT_EQUAL_FLOAT(NAN,iuf3DParametricSourceGetFNumber(NULL));
    TEST_ASSERT_EQUAL_FLOAT(NAN,iuf3DParametricSourceGetDeltaTheta(NULL));
    TEST_ASSERT_EQUAL_FLOAT(NAN,iuf3DParametricSourceGetStartTheta(NULL));
    TEST_ASSERT_EQUAL_FLOAT(NAN,iuf3DParametricSourceGetDeltaPhi(NULL));
    TEST_ASSERT_EQUAL_FLOAT(NAN,iuf3DParametricSourceGetStartPhi(NULL));
    TEST_ASSERT_EQUAL(-1, iuf3DParametricSourceGetNumThetaLocations(NULL));
    TEST_ASSERT_EQUAL(-1, iuf3DParametricSourceGetNumPhiLocations(NULL));

    TEST_ASSERT_EQUAL(7,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));


    iuf3DParametricSourceDelete(obj);
}

TEST(Iuf3DParametricSource, testIuf3DParametricSourceSerialization)
{
    char *filename = "testIuf3DParametricSourceSerialization.hdf5";
	int status;
	float FNumber = -0.955f;
	float startAngle = IUF_PI;
	float startTheta = startAngle;
	float deltaTheta = IUF_PI / 12;
	float startPhi = startAngle;
	float deltaPhi = IUF_PI / 20;
	int numPhiLocations = 5;
	int numThetaLocations = 3;


    // create
    iu3dps_t
    obj = iuf3DParametricSourceCreate(numThetaLocations, numPhiLocations, FNumber, deltaTheta, startTheta, deltaPhi, startPhi);

    // fill
    TEST_ASSERT_EQUAL_FLOAT(FNumber, iuf3DParametricSourceGetFNumber(obj));
    TEST_ASSERT_EQUAL_FLOAT(deltaTheta, iuf3DParametricSourceGetDeltaTheta(obj));
    TEST_ASSERT_EQUAL_FLOAT(startAngle, iuf3DParametricSourceGetStartTheta(obj));
    TEST_ASSERT_EQUAL_FLOAT(deltaPhi, iuf3DParametricSourceGetDeltaPhi(obj));
    TEST_ASSERT_EQUAL_FLOAT(startPhi, iuf3DParametricSourceGetStartPhi(obj));

    // save
    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
    status = iuf3DParametricSourceSave(obj, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iu3dps_t savedObj = iuf3DParametricSourceLoad(handle);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUF_TRUE, iuf3DParametricSourceCompare(obj,savedObj));
    iuf3DParametricSourceDelete(obj);
    iuf3DParametricSourceDelete(savedObj);

}

    TEST_GROUP_RUNNER(Iuf3DParametricSource)
{
    RUN_TEST_CASE(Iuf3DParametricSource, testIuf3DParametricSourceCreate);
    RUN_TEST_CASE(Iuf3DParametricSource, testIuf3DParametricSourceDelete);
    RUN_TEST_CASE(Iuf3DParametricSource, testIuf3DParametricSourceCompare);
    RUN_TEST_CASE(Iuf3DParametricSource, testIuf3DParametricSourceSetGet);
    RUN_TEST_CASE(Iuf3DParametricSource, testIuf3DParametricSourceSerialization);
}
