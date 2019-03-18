
//
// Created by nlv09165 on 30/07/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <iuf.h>
#include <iuf2DParametricSourcePrivate.h>

static char *pErrorFilename = "Iuf2DParametricSource.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(Iuf2DParametricSource);

TEST_SETUP(Iuf2DParametricSource)
{
    iufErrorLogClear();
    iufErrorLog(IUF_TRUE);
    iufErrorAutoReportSet(IUF_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iufErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(Iuf2DParametricSource)
{
    iufErrorLogClear();
    iufErrorLog(IUF_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iufErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
}


TEST(Iuf2DParametricSource, testIuf2DParametricSourceCreate)
{
    float deltaTheta = 0.13f;
    float FNumber = -0.955f;
    float startTheta = 3.14f;
    int numLocations = 5 ;

    iu2dps_t obj = iuf2DParametricSourceCreate(numLocations, FNumber, deltaTheta, startTheta);
    iu2dps_t notherObj = iuf2DParametricSourceCreate(numLocations, FNumber, deltaTheta, startTheta);
    TEST_ASSERT(obj != IU2DPS_INVALID);
    TEST_ASSERT(notherObj != IU2DPS_INVALID);
    iuf2DParametricSourceDelete(obj);
    iuf2DParametricSourceDelete(notherObj);

    // invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    obj = iuf2DParametricSourceCreate(0, -1, 2.0f, 0);
    TEST_ASSERT(obj == IU2DPS_INVALID);
    obj = iuf2DParametricSourceCreate(0, 1, -2.0f, 0);
    TEST_ASSERT(obj == IU2DPS_INVALID);

    TEST_ASSERT_EQUAL(2,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

}

TEST(Iuf2DParametricSource, testIuf2DParametricSourceDelete)
{
    float deltaTheta = 0.13f;
    float FNumber = -0.955f;
    float startTheta = 3.14f;
    int numLocations = 5;

    iu2dps_t obj = iuf2DParametricSourceCreate(numLocations, FNumber, deltaTheta, startTheta);
    TEST_ASSERT(obj != IU2DPS_INVALID);
    int status = iuf2DParametricSourceDelete(obj);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    // invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    status = iuf2DParametricSourceDelete(NULL);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, status);

    TEST_ASSERT_EQUAL(1,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

}



TEST(Iuf2DParametricSource, testIuf2DParametricSourceCompare)
{
    IUF_BOOL equal;
    float deltaTheta = 0.13f;
    float FNumber = -0.955f;
    float startTheta = 3.14f;
    int numLocations = 5;

    iu2dps_t obj = iuf2DParametricSourceCreate(numLocations, FNumber, deltaTheta, startTheta);
    iu2dps_t notherObj = iuf2DParametricSourceCreate(numLocations, FNumber, deltaTheta, startTheta);
    iu2dps_t differentObj =
    iuf2DParametricSourceCreate(numLocations, FNumber + 0.01f, deltaTheta, startTheta);
    TEST_ASSERT(obj != IU2DPS_INVALID);
    TEST_ASSERT(notherObj != IU2DPS_INVALID);
    equal = iuf2DParametricSourceCompare(obj,obj);
    TEST_ASSERT_EQUAL(IUF_TRUE,equal);
    equal = iuf2DParametricSourceCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUF_TRUE,equal);
    equal = iuf2DParametricSourceCompare(obj,differentObj);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);

    // invalid params
    equal = iuf2DParametricSourceCompare(obj,NULL);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);
    equal = iuf2DParametricSourceCompare(NULL,obj);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);

    iuf2DParametricSourceDelete(obj);
    iuf2DParametricSourceDelete(notherObj);
    iuf2DParametricSourceDelete(differentObj);
}

TEST(Iuf2DParametricSource, testIuf2DParametricSourceSetGet)
{
    //IUF_BOOL equal;
    float deltaTheta = 0.13f;
    float FNumber = -0.955f;
    float startTheta = 3.14f;
    int numLocations = 5;

    iu2dps_t obj = iuf2DParametricSourceCreate(numLocations, FNumber, deltaTheta, startTheta);

    TEST_ASSERT_EQUAL_FLOAT(FNumber,iuf2DParametricSourceGetFNumber(obj));
    TEST_ASSERT_EQUAL_FLOAT(deltaTheta,iuf2DParametricSourceGetDeltaTheta(obj));
    TEST_ASSERT_EQUAL_FLOAT(startTheta,iuf2DParametricSourceGetStartTheta(obj));
    TEST_ASSERT_EQUAL(numLocations, iuf2DParametricSourceGetNumLocations(obj));


    // invalid param
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    TEST_ASSERT_EQUAL_FLOAT(NAN,iuf2DParametricSourceGetFNumber(NULL));
    TEST_ASSERT_EQUAL_FLOAT(NAN,iuf2DParametricSourceGetDeltaTheta(NULL));
    TEST_ASSERT_EQUAL_FLOAT(NAN,iuf2DParametricSourceGetStartTheta(NULL));
    TEST_ASSERT_EQUAL(-1, iuf2DParametricSourceGetNumLocations(NULL));

    TEST_ASSERT_EQUAL(4,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));


    iuf2DParametricSourceDelete(obj);
}

TEST(Iuf2DParametricSource, testIuf2DParametricSourceSerialization)
{
    char *filename = "testIuf2DParametricSourceSerialization.hdf5";
    //char *sourcePath =  "/2DParametricSource";

    //IUF_BOOL equal;
    float deltaTheta = 0.13f;
    float FNumber = -0.955f;
    float startTheta = 3.14f;
    int numLocations = 5, status;


    // create
    iu2dps_t
    obj = iuf2DParametricSourceCreate(numLocations, FNumber, deltaTheta, startTheta);

    // fill
    TEST_ASSERT_EQUAL_FLOAT(FNumber, iuf2DParametricSourceGetFNumber(obj));
    TEST_ASSERT_EQUAL_FLOAT(deltaTheta, iuf2DParametricSourceGetDeltaTheta(obj));
    TEST_ASSERT_EQUAL_FLOAT(startTheta, iuf2DParametricSourceGetStartTheta(obj));

    // save
    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
    status = iuf2DParametricSourceSave(obj, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iu2dps_t savedObj = iuf2DParametricSourceLoad(handle);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUF_TRUE, iuf2DParametricSourceCompare(obj,savedObj));
    iuf2DParametricSourceDelete(obj);
    iuf2DParametricSourceDelete(savedObj);

}

TEST_GROUP_RUNNER(Iuf2DParametricSource)
{
    RUN_TEST_CASE(Iuf2DParametricSource, testIuf2DParametricSourceCreate);
    RUN_TEST_CASE(Iuf2DParametricSource, testIuf2DParametricSourceDelete);
    RUN_TEST_CASE(Iuf2DParametricSource, testIuf2DParametricSourceCompare);
    RUN_TEST_CASE(Iuf2DParametricSource, testIuf2DParametricSourceSetGet);
    RUN_TEST_CASE(Iuf2DParametricSource, testIuf2DParametricSourceSerialization);
}
