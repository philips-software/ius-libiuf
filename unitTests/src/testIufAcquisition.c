//
// Created by nlv09165 on 26/04/2018.
//
#include <string.h>
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <iuf.h>
#include <iufAcquisitionPrivate.h>

static char *pErrorFilename = "IufAcquisition.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(IufAcquisition);

TEST_SETUP(IufAcquisition)
{
    iufErrorLogClear();
    iufErrorLog(IUF_TRUE);
    iufErrorAutoReportSet(IUF_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iufErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IufAcquisition)
{
    iufErrorLogClear();
    iufErrorLog(IUF_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iufErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
}


TEST(IufAcquisition, testIufCreateAcquisition)
{
    float speedOfSound = 1498.1f;
    int date = 20160124;
    char *pDescription = "My important acquisition notes, by testIufCreateAcquisition";

    iua_t obj = iufAcquisitionCreate(speedOfSound, date, pDescription);
    iua_t notherObj = iufAcquisitionCreate(speedOfSound, date, pDescription);
    TEST_ASSERT(obj != IUA_INVALID);
    TEST_ASSERT(notherObj != IUA_INVALID);
    iufAcquisitionDelete(obj);
    iufAcquisitionDelete(notherObj);

    // invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    obj = iufAcquisitionCreate(-1.0f, date, pDescription);
    TEST_ASSERT(obj == IUA_INVALID);
    obj = iufAcquisitionCreate(speedOfSound, -1, pDescription);
    TEST_ASSERT(obj == IUA_INVALID);
    obj = iufAcquisitionCreate(speedOfSound, -1, NULL);
    TEST_ASSERT(obj == IUA_INVALID);

    TEST_ASSERT_EQUAL(3,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
}

TEST(IufAcquisition, testIufDeleteAcquisition)
{
    float speedOfSound = 1498.1f;
    int date = 20160124;
    char *pDescription = "My important acquisition notes, by testIufCreateAcquisition";
    iua_t obj = iufAcquisitionCreate(speedOfSound, date, pDescription);
    TEST_ASSERT(obj != IUA_INVALID);
    int status = iufAcquisitionDelete(obj);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    // invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    status = iufAcquisitionDelete(NULL);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, status);

    TEST_ASSERT_EQUAL(1,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
}

TEST(IufAcquisition, testIufCompareAcquisition)
{
    IUF_BOOL equal;
    float speedOfSound = 1498.1f;
    int date = 20160124;
    char *pDescription = "My important acquisition notes, by testIufCreateAcquisition";

    iua_t obj = iufAcquisitionCreate(speedOfSound, date, pDescription);
    iua_t notherObj = iufAcquisitionCreate(speedOfSound, date, pDescription);
    iua_t differentObj = iufAcquisitionCreate(speedOfSound, date + 1, pDescription);
    TEST_ASSERT(obj != IUA_INVALID);
    TEST_ASSERT(notherObj != IUA_INVALID);
    equal = iufAcquisitionCompare(obj, obj);
    TEST_ASSERT_EQUAL(IUF_TRUE,equal);
    equal = iufAcquisitionCompare(NULL, NULL);
    TEST_ASSERT_EQUAL(IUF_TRUE,equal);
    equal = iufAcquisitionCompare(obj, notherObj);
    TEST_ASSERT_EQUAL(IUF_TRUE,equal);
    equal = iufAcquisitionCompare(obj, differentObj);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);

    // invalid params
    equal = iufAcquisitionCompare(obj, NULL);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);
    equal = iufAcquisitionCompare(NULL, obj);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);

    iufAcquisitionDelete(obj);
    iufAcquisitionDelete(notherObj);
    iufAcquisitionDelete(differentObj);
}

TEST(IufAcquisition, testIufGetAcquisition)
{
    float speedOfSound = 1498.1f;
    int date = 20160124;
    char *pDescription = "My important acquisition notes, by testIufCreateAcquisition";

    iua_t obj = iufAcquisitionCreate(speedOfSound, date, pDescription);

    TEST_ASSERT_EQUAL_FLOAT(speedOfSound, iufAcquisitionGetSpeedOfSound(obj));
    TEST_ASSERT_EQUAL(date, iufAcquisitionGetDate(obj));
    TEST_ASSERT(strcmp(pDescription, iufAcquisitionGetDescription(obj))==0);

    // invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    TEST_ASSERT_EQUAL_FLOAT(NAN, iufAcquisitionGetSpeedOfSound(NULL));
    TEST_ASSERT_EQUAL(-1, iufAcquisitionGetDate(NULL));
    TEST_ASSERT(iufAcquisitionGetDescription(NULL)==NULL);

    TEST_ASSERT_EQUAL(3,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

    iufAcquisitionDelete(obj);
}

TEST(IufAcquisition, testIufSerialization)
{
    float speedOfSound = 1498.1f;
    int date = 20160124;
    char *pDescription = "My important acquisition notes, by testIufCreateAcquisition";
    char *filename = "testIufAcquisition.hdf5";
    //char *acquisitionPath =  "/Acquisition";

    // create and save
    iua_t obj = iufAcquisitionCreate(speedOfSound, date, pDescription);
    iua_t notherObj = iufAcquisitionCreate(speedOfSound + 1.0f, date + 1, pDescription);

    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);

    int status = iufAcquisitionSave(obj, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
	TEST_ASSERT(handle > 0);
    iua_t savedObj = iufAcquisitionLoad(handle);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUF_TRUE, iufAcquisitionCompare(obj, savedObj));
    TEST_ASSERT_EQUAL(IUF_FALSE, iufAcquisitionCompare(notherObj, savedObj));
    iufAcquisitionDelete(obj);
    iufAcquisitionDelete(notherObj);
    iufAcquisitionDelete(savedObj);
}



TEST_GROUP_RUNNER(IufAcquisition)
{
    RUN_TEST_CASE(IufAcquisition, testIufCreateAcquisition);
    RUN_TEST_CASE(IufAcquisition, testIufDeleteAcquisition);
    RUN_TEST_CASE(IufAcquisition, testIufCompareAcquisition);
    RUN_TEST_CASE(IufAcquisition, testIufGetAcquisition);
    RUN_TEST_CASE(IufAcquisition, testIufSerialization);
}
