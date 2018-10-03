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
#include <iusExperimentImp.h>

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
    char *pDescription = "My important experiment notes, by testIusCreateExperiment";

    iue_t obj = iusExperimentCreate(speedOfSound, date, pDescription);
    iue_t notherObj = iusExperimentCreate(speedOfSound, date, pDescription);
    TEST_ASSERT(obj != IUE_INVALID);
    TEST_ASSERT(notherObj != IUE_INVALID);
    iusExperimentDelete(obj);
    iusExperimentDelete(notherObj);

    // invalid params
    obj = iusExperimentCreate(-1.0f, date, pDescription);
    TEST_ASSERT(obj == IUE_INVALID);
    obj = iusExperimentCreate(speedOfSound, -1, pDescription);
    TEST_ASSERT(obj == IUE_INVALID);
    obj = iusExperimentCreate(speedOfSound, -1, NULL);
    TEST_ASSERT(obj == IUE_INVALID);
}

TEST(IusExperiment, testIusDeleteExperiment)
{
    float speedOfSound = 1498.1f;
    int date = 20160124;
    char *pDescription = "My important experiment notes, by testIusCreateExperiment";
    iue_t obj = iusExperimentCreate(speedOfSound, date, pDescription);
    TEST_ASSERT(obj != IUE_INVALID);
    int status = iusExperimentDelete(obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // invalid params
    status = iusExperimentDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
}

TEST(IusExperiment, testIusCompareExperiment)
{
    IUS_BOOL equal;
    float speedOfSound = 1498.1f;
    int date = 20160124;
    char *pDescription = "My important experiment notes, by testIusCreateExperiment";

    iue_t obj = iusExperimentCreate(speedOfSound, date, pDescription);
    iue_t notherObj = iusExperimentCreate(speedOfSound, date, pDescription);
    iue_t differentObj = iusExperimentCreate(speedOfSound, date + 1, pDescription);
    TEST_ASSERT(obj != IUE_INVALID);
    TEST_ASSERT(notherObj != IUE_INVALID);
    equal = iusExperimentCompare(obj, obj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = iusExperimentCompare(NULL, NULL);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = iusExperimentCompare(obj, notherObj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = iusExperimentCompare(obj, differentObj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

    // invalid params
    equal = iusExperimentCompare(obj, NULL);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    equal = iusExperimentCompare(NULL, obj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

  iusExperimentDelete(obj);
  iusExperimentDelete(notherObj);
  iusExperimentDelete(differentObj);
}

TEST(IusExperiment, testIusGetExperiment)
{
    float speedOfSound = 1498.1f;
    int date = 20160124;
    char *pDescription = "My important experiment notes, by testIusCreateExperiment";

    iue_t obj = iusExperimentCreate(speedOfSound, date, pDescription);

    TEST_ASSERT_EQUAL_FLOAT(speedOfSound,iusExperimentGetSpeedOfSound(obj));
    TEST_ASSERT_EQUAL(date,iusExperimentGetDate(obj));
    TEST_ASSERT(strcmp(pDescription,iusExperimentGetDescription(obj))==0);

    // invalid params
    TEST_ASSERT_EQUAL_FLOAT(NAN,iusExperimentGetSpeedOfSound(NULL));
    TEST_ASSERT_EQUAL(-1,iusExperimentGetDate(NULL));
    TEST_ASSERT(iusExperimentGetDescription(NULL)==NULL);
  iusExperimentDelete(obj);
}

TEST(IusExperiment, testIusSerialization)
{
    float speedOfSound = 1498.1f;
    int date = 20160124;
    char *pDescription = "My important experiment notes, by testIusCreateExperiment";
    char *filename = "testIusExperiment.hdf5";
    //char *experimentPath =  "/Experiment";

    // create and save
    iue_t obj = iusExperimentCreate(speedOfSound, date, pDescription);
    iue_t notherObj = iusExperimentCreate(speedOfSound+1.0f, date+1, pDescription);

    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
	//hid_t group_id = H5Gcreate(handle, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	//TEST_ASSERT(group_id > 0);
    int status = iusExperimentSave(obj, handle);
	//H5Gclose(group_id);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
	TEST_ASSERT(handle > 0);
	//group_id = H5Gopen(handle, experimentPath, H5P_DEFAULT);
	//TEST_ASSERT(group_id > 0);
    iue_t savedObj = iusExperimentLoad(handle);
	//H5Gclose(group_id);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUS_TRUE, iusExperimentCompare(obj,savedObj));
    TEST_ASSERT_EQUAL(IUS_FALSE, iusExperimentCompare(notherObj,savedObj));
    iusExperimentDelete(obj);
    iusExperimentDelete(savedObj);
}



TEST_GROUP_RUNNER(IusExperiment)
{
    RUN_TEST_CASE(IusExperiment, testIusCreateExperiment);
    RUN_TEST_CASE(IusExperiment, testIusDeleteExperiment);
    RUN_TEST_CASE(IusExperiment, testIusCompareExperiment);
    RUN_TEST_CASE(IusExperiment, testIusGetExperiment);
    RUN_TEST_CASE(IusExperiment, testIusSerialization);
}
