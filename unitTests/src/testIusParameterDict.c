//
// Created by nlv09165 on 31/07/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <ius.h>
#include <include/iusParameterDictImp.h>
#include <testDataGenerators.h>

TEST_GROUP(IusParameterDict);

TEST_SETUP(IusParameterDict)
{
}

TEST_TEAR_DOWN(IusParameterDict)
{
}


TEST(IusParameterDict, testIusCreateParameterDict)
{
    iupad_t obj = iusParameterDictCreate();
    iupad_t notherObj = iusParameterDictCreate();
    TEST_ASSERT(obj != IUPAD_INVALID);
    TEST_ASSERT(notherObj != IUPAD_INVALID);
    iusParameterDictDelete(obj);
    iusParameterDictDelete(notherObj);
}


TEST(IusParameterDict, testIusCompareParameterDict)
{
    int elementID,status;

    IUS_BOOL equal;
    iupad_t dict = iusParameterDictCreate();
    iupad_t notherDict = iusParameterDictCreate();
    TEST_ASSERT(dict != IUPAD_INVALID);
    TEST_ASSERT(notherDict != IUPAD_INVALID);


    equal = iusParameterDictCompare(dict, dict);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = iusParameterDictCompare(dict, notherDict);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);

    char generatedKey[1024];
    char generatedValue[1024];
    // TODO:
    // elementID=1000 (iterations) take a second
    // elementID=10000 (iterations) takes more than 10 times
    // which is non linear.
    // Fix.
    for (elementID=0;elementID<1000;elementID++)
    {
        sprintf(generatedKey,"key_%d", elementID);
        sprintf(generatedValue,"value_%d", elementID);
        status = iusParameterDictSet(dict, generatedKey, generatedValue);
        TEST_ASSERT_EQUAL(IUS_E_OK,status);
        TEST_ASSERT_EQUAL_STRING(generatedValue,iusParameterDictGet(dict, generatedKey));
        equal = iusParameterDictCompare(dict, notherDict);
        TEST_ASSERT_EQUAL(IUS_FALSE,equal);

        status = iusParameterDictSet(notherDict, generatedKey, generatedValue);
        TEST_ASSERT_EQUAL(IUS_E_OK,status);
        TEST_ASSERT_EQUAL_STRING(generatedValue,iusParameterDictGet(notherDict, generatedKey));
        equal = iusParameterDictCompare(dict, notherDict);
        TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    }

    iusParameterDictDelete(dict);
    iusParameterDictDelete(notherDict);
}


TEST(IusParameterDict, testIusSerialization)
{
    IUS_BOOL equal;

    // create and fill
    iupad_t dict = dgGenerateParameterDict(100);
    char *filename = "testIusParameterDictSerialization.hdf5";
    char *dictPath =  "/ParameterDict";

    // save
    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
    hid_t group_id = H5Gcreate(handle, dictPath, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    TEST_ASSERT(group_id > 0);

    int status = iusParameterDictSave(dict, group_id);
    H5Gclose(group_id);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
    group_id = H5Gopen(handle, dictPath, H5P_DEFAULT);
    TEST_ASSERT(group_id > 0);

    iupad_t savedObj = iusParameterDictLoad(group_id);
    TEST_ASSERT(savedObj != NULL);
    H5Gclose(group_id);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUS_TRUE, iusParameterDictCompare(dict,savedObj));

    iusParameterDictDelete(dict);
    iusParameterDictDelete(savedObj);

}


TEST(IusParameterDict, testIusSerializationErrorFlow)
{
    IUS_BOOL equal;

    // create and fill
    iupad_t dict = dgGenerateParameterDict(100);
    char *filename = "testIusParameterDictSerializationErrorflow.hdf5";
    char *dictPath =  "/ParameterDict";

    // save
    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
    hid_t group_id = H5Gcreate(handle, dictPath, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    TEST_ASSERT(group_id > 0);

    int status = iusParameterDictSave(NULL, group_id);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE,status);
    status = iusParameterDictSave(dict, H5I_INVALID_HID);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE,status);
    status = iusParameterDictSave(dict, group_id);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    H5Gclose(group_id);
    H5Fclose(handle);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
    group_id = H5Gopen(handle, dictPath, H5P_DEFAULT);
    TEST_ASSERT(group_id > 0);

    iupad_t savedObj = iusParameterDictLoad(H5I_INVALID_HID);
    TEST_ASSERT(savedObj == IUPAD_INVALID);
    savedObj = iusParameterDictLoad(group_id);
    TEST_ASSERT(savedObj != IUPAD_INVALID);
    H5Gclose(group_id);
    H5Fclose(handle);

    iusParameterDictDelete(dict);
    iusParameterDictDelete(savedObj);

}


TEST_GROUP_RUNNER(IusParameterDict)
{
    RUN_TEST_CASE(IusParameterDict, testIusCreateParameterDict);
    RUN_TEST_CASE(IusParameterDict, testIusCompareParameterDict);
    RUN_TEST_CASE(IusParameterDict, testIusSerialization);
    RUN_TEST_CASE(IusParameterDict, testIusSerializationErrorFlow);
}