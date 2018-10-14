//
// Created by nlv09165 on 26/04/2018.
//
#include <string.h>
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusAcquisitionPrivate.h>

TEST_GROUP(IusExperiment);

TEST_SETUP(IusExperiment)
{
}

TEST_TEAR_DOWN(IusExperiment)
{
}

TEST(IusExperiment, testIusCreateExperiment)
{
    float speedOfSound = 1498.1f;
    int date = 20160124;
    char *pDescription = "My important acquisition notes, by testIusCreateExperiment";

    iua_t obj = iusAcquisitionCreate(speedOfSound, date, pDescription);
    iua_t notherObj = iusAcquisitionCreate(speedOfSound, date, pDescription);
    TEST_ASSERT(obj != IUE_INVALID);
    TEST_ASSERT(notherObj != IUE_INVALID);
    iusAcquisitionDelete(obj);
    iusAcquisitionDelete(notherObj);

    // invalid params
    obj = iusAcquisitionCreate(-1.0f, date, pDescription);
    TEST_ASSERT(obj == IUE_INVALID);
    obj = iusAcquisitionCreate(speedOfSound, -1, pDescription);
    TEST_ASSERT(obj == IUE_INVALID);
    obj = iusAcquisitionCreate(speedOfSound, -1, NULL);
    TEST_ASSERT(obj == IUE_INVALID);
}

TEST(IusExperiment, testIusDeleteExperiment)
{
    float speedOfSound = 1498.1f;
    int date = 20160124;
    char *pDescription = "My important acquisition notes, by testIusCreateExperiment";
    iua_t obj = iusAcquisitionCreate(speedOfSound, date, pDescription);
    TEST_ASSERT(obj != IUE_INVALID);
    int status = iusAcquisitionDelete(obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // invalid params
    status = iusAcquisitionDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
}

TEST(IusExperiment, testIusCompareExperiment)
{
    IUS_BOOL equal;
    float speedOfSound = 1498.1f;
    int date = 20160124;
    char *pDescription = "My important acquisition notes, by testIusCreateExperiment";

    iua_t obj = iusAcquisitionCreate(speedOfSound, date, pDescription);
    iua_t notherObj = iusAcquisitionCreate(speedOfSound, date, pDescription);
    iua_t differentObj = iusAcquisitionCreate(speedOfSound, date + 1, pDescription);
    TEST_ASSERT(obj != IUE_INVALID);
    TEST_ASSERT(notherObj != IUE_INVALID);
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

TEST(IusExperiment, testIusGetExperiment)
{
    float speedOfSound = 1498.1f;
    int date = 20160124;
    char *pDescription = "My important acquisition notes, by testIusCreateExperiment";

    iua_t obj = iusAcquisitionCreate(speedOfSound, date, pDescription);

    TEST_ASSERT_EQUAL_FLOAT(speedOfSound, iusAcquisitionGetSpeedOfSound(obj));
    TEST_ASSERT_EQUAL(date, iusAcquisitionGetDate(obj));
    TEST_ASSERT(strcmp(pDescription, iusAcquisitionGetDescription(obj))==0);

    // invalid params
    TEST_ASSERT_EQUAL_FLOAT(NAN, iusAcquisitionGetSpeedOfSound(NULL));
    TEST_ASSERT_EQUAL(-1, iusAcquisitionGetDate(NULL));
    TEST_ASSERT(iusAcquisitionGetDescription(NULL)==NULL);
    iusAcquisitionDelete(obj);
}

TEST(IusExperiment, testIusSerialization)
{
    float speedOfSound = 1498.1f;
    int date = 20160124;
    char *pDescription = "My important acquisition notes, by testIusCreateExperiment";
    char *filename = "testIusExperiment.hdf5";
    //char *experimentPath =  "/Experiment";

    // create and save
    iua_t obj = iusAcquisitionCreate(speedOfSound, date, pDescription);
    iua_t notherObj = iusAcquisitionCreate(speedOfSound + 1.0f, date + 1, pDescription);

    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
	//hid_t group_id = H5Gcreate(handle, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	//TEST_ASSERT(group_id > 0);
    int status = iusAcquisitionSave(obj, handle);
	//H5Gclose(group_id);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
	TEST_ASSERT(handle > 0);
	//group_id = H5Gopen(handle, experimentPath, H5P_DEFAULT);
	//TEST_ASSERT(group_id > 0);
    iua_t savedObj = iusAcquisitionLoad(handle);
	//H5Gclose(group_id);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUS_TRUE, iusAcquisitionCompare(obj, savedObj));
    TEST_ASSERT_EQUAL(IUS_FALSE, iusAcquisitionCompare(notherObj, savedObj));
    iusAcquisitionDelete(obj);
    iusAcquisitionDelete(savedObj);
}



TEST_GROUP_RUNNER(IusExperiment)
{
    RUN_TEST_CASE(IusExperiment, testIusCreateExperiment);
    RUN_TEST_CASE(IusExperiment, testIusDeleteExperiment);
    RUN_TEST_CASE(IusExperiment, testIusCompareExperiment);
    RUN_TEST_CASE(IusExperiment, testIusGetExperiment);
    RUN_TEST_CASE(IusExperiment, testIusSerialization);
}
