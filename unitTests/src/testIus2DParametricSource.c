
//
// Created by nlv09165 on 30/07/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <include/ius.h>
#include "include/ius2DParametricSourcePrivate.h"

static char *pErrorFilename = "Ius2DParametricSource.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(Ius2DParametricSource);

TEST_SETUP(Ius2DParametricSource)
{
    iusErrorLogClear();
    iusErrorLog(IUS_TRUE);
    iusErrorAutoReport(IUS_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iusErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(Ius2DParametricSource)
{
    iusErrorLogClear();
    iusErrorLog(IUS_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iusErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
}


TEST(Ius2DParametricSource, testIus2DParametricSourceCreate)
{
    float deltaTheta = 0.13f;
    float FNumber = -0.955f;
    float startTheta = 3.14f;
    int numLocations = 5 ;

    iu2dps_t obj = ius2DParametricSourceCreate(numLocations, FNumber, deltaTheta, startTheta);
    iu2dps_t notherObj = ius2DParametricSourceCreate(numLocations, FNumber, deltaTheta, startTheta);
    TEST_ASSERT(obj != IU2DPS_INVALID);
    TEST_ASSERT(notherObj != IU2DPS_INVALID);
    ius2DParametricSourceDelete(obj);
    ius2DParametricSourceDelete(notherObj);

    // invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());

    obj = ius2DParametricSourceCreate(0, -1, 2.0f, 0);
    TEST_ASSERT(obj == IU2DPS_INVALID);
    obj = ius2DParametricSourceCreate(0, 1, -2.0f, 0);
    TEST_ASSERT(obj == IU2DPS_INVALID);

    TEST_ASSERT_EQUAL(2,iusErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

}

TEST(Ius2DParametricSource, testIus2DParametricSourceDelete)
{
    float deltaTheta = 0.13f;
    float FNumber = -0.955f;
    float startTheta = 3.14f;
    int numLocations = 5;

    iu2dps_t obj = ius2DParametricSourceCreate(numLocations, FNumber, deltaTheta, startTheta);
    TEST_ASSERT(obj != IU2DPS_INVALID);
    int status = ius2DParametricSourceDelete(obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());

    status = ius2DParametricSourceDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);

    TEST_ASSERT_EQUAL(1,iusErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

}



TEST(Ius2DParametricSource, testIus2DParametricSourceCompare)
{
    IUS_BOOL equal;
    float deltaTheta = 0.13f;
    float FNumber = -0.955f;
    float startTheta = 3.14f;
    int numLocations = 5;

    iu2dps_t obj = ius2DParametricSourceCreate(numLocations, FNumber, deltaTheta, startTheta);
    iu2dps_t notherObj = ius2DParametricSourceCreate(numLocations, FNumber, deltaTheta, startTheta);
    iu2dps_t differentObj =
    ius2DParametricSourceCreate(numLocations, FNumber + 0.01f, deltaTheta, startTheta);
    TEST_ASSERT(obj != IU2DPS_INVALID);
    TEST_ASSERT(notherObj != IU2DPS_INVALID);
    equal = ius2DParametricSourceCompare(obj,obj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = ius2DParametricSourceCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = ius2DParametricSourceCompare(obj,differentObj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

    // invalid params
    equal = ius2DParametricSourceCompare(obj,NULL);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    equal = ius2DParametricSourceCompare(NULL,obj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

    ius2DParametricSourceDelete(obj);
    ius2DParametricSourceDelete(notherObj);
    ius2DParametricSourceDelete(differentObj);
}

TEST(Ius2DParametricSource, testIus2DParametricSourceSetGet)
{
    //IUS_BOOL equal;
    float deltaTheta = 0.13f;
    float FNumber = -0.955f;
    float startTheta = 3.14f;
    int numLocations = 5;

    iu2dps_t obj = ius2DParametricSourceCreate(numLocations, FNumber, deltaTheta, startTheta);

    TEST_ASSERT_EQUAL_FLOAT(FNumber,ius2DParametricSourceGetFNumber(obj));
    TEST_ASSERT_EQUAL_FLOAT(deltaTheta,ius2DParametricSourceGetDeltaTheta(obj));
    TEST_ASSERT_EQUAL_FLOAT(startTheta,ius2DParametricSourceGetStartTheta(obj));
    TEST_ASSERT_EQUAL(numLocations, ius2DParametricSourceGetNumLocations(obj));


    // invalid param
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());

    TEST_ASSERT_EQUAL_FLOAT(NAN,ius2DParametricSourceGetFNumber(NULL));
    TEST_ASSERT_EQUAL_FLOAT(NAN,ius2DParametricSourceGetDeltaTheta(NULL));
    TEST_ASSERT_EQUAL_FLOAT(NAN,ius2DParametricSourceGetStartTheta(NULL));
    TEST_ASSERT_EQUAL(-1, ius2DParametricSourceGetNumLocations(NULL));

    TEST_ASSERT_EQUAL(4,iusErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));


    ius2DParametricSourceDelete(obj);
}

TEST(Ius2DParametricSource, testIus2DParametricSourceSerialization)
{
    char *filename = "testIus2DParametricSourceSerialization.hdf5";
    //char *sourcePath =  "/2DParametricSource";

    //IUS_BOOL equal;
    float deltaTheta = 0.13f;
    float FNumber = -0.955f;
    float startTheta = 3.14f;
    int numLocations = 5, status;


    // create
    iu2dps_t
    obj = ius2DParametricSourceCreate(numLocations, FNumber, deltaTheta, startTheta);

    // fill
    TEST_ASSERT_EQUAL_FLOAT(FNumber, ius2DParametricSourceGetFNumber(obj));
    TEST_ASSERT_EQUAL_FLOAT(deltaTheta, ius2DParametricSourceGetDeltaTheta(obj));
    TEST_ASSERT_EQUAL_FLOAT(startTheta, ius2DParametricSourceGetStartTheta(obj));

    // save
    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
    status = ius2DParametricSourceSave(obj, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iu2dps_t savedObj = ius2DParametricSourceLoad(handle);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUS_TRUE, ius2DParametricSourceCompare(obj,savedObj));
    ius2DParametricSourceDelete(obj);
    ius2DParametricSourceDelete(savedObj);

}

TEST_GROUP_RUNNER(Ius2DParametricSource)
{
    RUN_TEST_CASE(Ius2DParametricSource, testIus2DParametricSourceCreate);
    RUN_TEST_CASE(Ius2DParametricSource, testIus2DParametricSourceDelete);
    RUN_TEST_CASE(Ius2DParametricSource, testIus2DParametricSourceCompare);
    RUN_TEST_CASE(Ius2DParametricSource, testIus2DParametricSourceSetGet);
    RUN_TEST_CASE(Ius2DParametricSource, testIus2DParametricSourceSerialization);
}
