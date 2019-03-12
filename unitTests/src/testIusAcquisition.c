//
// Created by nlv09165 on 26/04/2018.
//
#include <string.h>
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <ius.h>
#include <iusAcquisitionPrivate.h>

static char *pErrorFilename = "IusAcquisition.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(IusAcquisition);

TEST_SETUP(IusAcquisition)
{
    iusErrorLogClear();
    iusErrorLog(IUS_TRUE);
    iusErrorAutoReportSet(IUS_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iusErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IusAcquisition)
{
    iusErrorLogClear();
    iusErrorLog(IUS_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iusErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
}


TEST(IusAcquisition, testIusCreateAcquisition)
{
    float speedOfSound = 1498.1f;
    int date = 20160124;
    char *pDescription = "My important acquisition notes, by testIusCreateAcquisition";

    iua_t obj = iusAcquisitionCreate(speedOfSound, date, pDescription);
    iua_t notherObj = iusAcquisitionCreate(speedOfSound, date, pDescription);
    TEST_ASSERT(obj != IUA_INVALID);
    TEST_ASSERT(notherObj != IUA_INVALID);
    iusAcquisitionDelete(obj);
    iusAcquisitionDelete(notherObj);

    // invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());

    obj = iusAcquisitionCreate(-1.0f, date, pDescription);
    TEST_ASSERT(obj == IUA_INVALID);
    obj = iusAcquisitionCreate(speedOfSound, -1, pDescription);
    TEST_ASSERT(obj == IUA_INVALID);
    obj = iusAcquisitionCreate(speedOfSound, -1, NULL);
    TEST_ASSERT(obj == IUA_INVALID);

    TEST_ASSERT_EQUAL(3,iusErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
}

TEST(IusAcquisition, testIusDeleteAcquisition)
{
    float speedOfSound = 1498.1f;
    int date = 20160124;
    char *pDescription = "My important acquisition notes, by testIusCreateAcquisition";
    iua_t obj = iusAcquisitionCreate(speedOfSound, date, pDescription);
    TEST_ASSERT(obj != IUA_INVALID);
    int status = iusAcquisitionDelete(obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());

    status = iusAcquisitionDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);

    TEST_ASSERT_EQUAL(1,iusErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
}

TEST(IusAcquisition, testIusCompareAcquisition)
{
    IUS_BOOL equal;
    float speedOfSound = 1498.1f;
    int date = 20160124;
    char *pDescription = "My important acquisition notes, by testIusCreateAcquisition";

    iua_t obj = iusAcquisitionCreate(speedOfSound, date, pDescription);
    iua_t notherObj = iusAcquisitionCreate(speedOfSound, date, pDescription);
    iua_t differentObj = iusAcquisitionCreate(speedOfSound, date + 1, pDescription);
    TEST_ASSERT(obj != IUA_INVALID);
    TEST_ASSERT(notherObj != IUA_INVALID);
    equal = iusAcquisitionCompare(obj, obj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = iusAcquisitionCompare(NULL, NULL);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = iusAcquisitionCompare(obj, notherObj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = iusAcquisitionCompare(obj, differentObj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

    // invalid params
    equal = iusAcquisitionCompare(obj, NULL);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    equal = iusAcquisitionCompare(NULL, obj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

    iusAcquisitionDelete(obj);
    iusAcquisitionDelete(notherObj);
    iusAcquisitionDelete(differentObj);
}

TEST(IusAcquisition, testIusGetAcquisition)
{
    float speedOfSound = 1498.1f;
    int date = 20160124;
    char *pDescription = "My important acquisition notes, by testIusCreateAcquisition";

    iua_t obj = iusAcquisitionCreate(speedOfSound, date, pDescription);

    TEST_ASSERT_EQUAL_FLOAT(speedOfSound, iusAcquisitionGetSpeedOfSound(obj));
    TEST_ASSERT_EQUAL(date, iusAcquisitionGetDate(obj));
    TEST_ASSERT(strcmp(pDescription, iusAcquisitionGetDescription(obj))==0);

    // invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());

    TEST_ASSERT_EQUAL_FLOAT(NAN, iusAcquisitionGetSpeedOfSound(NULL));
    TEST_ASSERT_EQUAL(-1, iusAcquisitionGetDate(NULL));
    TEST_ASSERT(iusAcquisitionGetDescription(NULL)==NULL);

    TEST_ASSERT_EQUAL(3,iusErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

    iusAcquisitionDelete(obj);
}

TEST(IusAcquisition, testIusSerialization)
{
    float speedOfSound = 1498.1f;
    int date = 20160124;
    char *pDescription = "My important acquisition notes, by testIusCreateAcquisition";
    char *filename = "testIusAcquisition.hdf5";
    //char *acquisitionPath =  "/Acquisition";

    // create and save
    iua_t obj = iusAcquisitionCreate(speedOfSound, date, pDescription);
    iua_t notherObj = iusAcquisitionCreate(speedOfSound + 1.0f, date + 1, pDescription);

    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);

    int status = iusAcquisitionSave(obj, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
	TEST_ASSERT(handle > 0);
    iua_t savedObj = iusAcquisitionLoad(handle);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUS_TRUE, iusAcquisitionCompare(obj, savedObj));
    TEST_ASSERT_EQUAL(IUS_FALSE, iusAcquisitionCompare(notherObj, savedObj));
    iusAcquisitionDelete(obj);
    iusAcquisitionDelete(notherObj);
    iusAcquisitionDelete(savedObj);
}



TEST_GROUP_RUNNER(IusAcquisition)
{
    RUN_TEST_CASE(IusAcquisition, testIusCreateAcquisition);
    RUN_TEST_CASE(IusAcquisition, testIusDeleteAcquisition);
    RUN_TEST_CASE(IusAcquisition, testIusCompareAcquisition);
    RUN_TEST_CASE(IusAcquisition, testIusGetAcquisition);
    RUN_TEST_CASE(IusAcquisition, testIusSerialization);
}
