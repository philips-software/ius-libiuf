#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <iuf.h>
#include <util.h>
#include <iufParameterDictPrivate.h>
#include <dg/dataGenerators.h>


static char *pErrorFilename = "IufParameterDict.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(IufParameterDict);

TEST_SETUP(IufParameterDict)
{
    iufErrorLogClear();
    iufErrorLog(IUF_TRUE);
    iufErrorAutoReportSet(IUF_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iufErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IufParameterDict)
{
    iufErrorLogClear();
    iufErrorLog(IUF_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iufErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
}

TEST(IufParameterDict, testIufParameterDictCreateDelete)
{
    iupad_t obj = iufParameterDictCreate();
    iupad_t notherObj = iufParameterDictCreate();
    TEST_ASSERT(obj != IUPAD_INVALID);
    TEST_ASSERT(notherObj != IUPAD_INVALID);
    TEST_ASSERT(iufParameterDictDelete(obj) == IUF_E_OK);
    TEST_ASSERT(iufParameterDictDelete(notherObj) == IUF_E_OK);
    TEST_ASSERT(iufParameterDictDelete(NULL) != IUF_E_OK);
}


TEST(IufParameterDict, testIufParameterDictSetGet)
{
    char *labels[] = { "one" , "two" , "three", "four" , "five"};
    char *value = "testValue";

    // Create
    iupad_t dict = iufParameterDictCreate();

    // Fill
    int i, status;
    int keySize = sizeof(labels)/sizeof(labels[0]);
    for (i=0; i<keySize; i++)
    {
        status = iufParameterDictSet(dict, labels[i], value);
        TEST_ASSERT(status == IUF_E_OK);
    }

    size_t dictSize = iufParameterDictGetSize(dict);
    TEST_ASSERT_EQUAL(5, dictSize);
    char **keys = iufParameterDictGetKeys(dict);

    // Validate keys
    for (i=0; i<keySize; i++)
    {
        TEST_ASSERT_EQUAL(IUF_TRUE, aInB(keys[i], labels));
    }

    // invalid args
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    keys = iufParameterDictGetKeys(NULL);
    TEST_ASSERT_EQUAL(NULL,keys);

    TEST_ASSERT_EQUAL(1,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
    iufParameterDictDelete(dict);
}


TEST(IufParameterDict, testIufParameterDictGetKeys)
{
    char *key = "testKey";
    char *value = "testValue";
    iupad_t dict = iufParameterDictCreate();
    TEST_ASSERT_EQUAL(IUF_E_OK, iufParameterDictSet(dict, key, value));
    TEST_ASSERT_EQUAL_STRING(value, iufParameterDictGet(dict, key));

    // invalid args
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, iufParameterDictSet(NULL, key, value));
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, iufParameterDictSet(dict, NULL, value));
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, iufParameterDictSet(dict, key, NULL));

    TEST_ASSERT_EQUAL(NULL, iufParameterDictGet(NULL, key));
    TEST_ASSERT_EQUAL(NULL, iufParameterDictGet(dict, NULL));
    TEST_ASSERT_EQUAL(NULL, iufParameterDictGet(dict, "unknownKey"));

    TEST_ASSERT_EQUAL(6,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
    iufParameterDictDelete(dict);
}


TEST(IufParameterDict, testIufParameterDictCompare)
{
    int elementID,status;

    IUF_BOOL equal;
    iupad_t dict = iufParameterDictCreate();
    iupad_t notherDict = iufParameterDictCreate();
    TEST_ASSERT(dict != IUPAD_INVALID);
    TEST_ASSERT(notherDict != IUPAD_INVALID);


    equal = iufParameterDictCompare(dict, dict);
    TEST_ASSERT_EQUAL(IUF_TRUE,equal);
    equal = iufParameterDictCompare(dict, notherDict);
    TEST_ASSERT_EQUAL(IUF_TRUE,equal);
    equal = iufParameterDictCompare(notherDict, dict);
    TEST_ASSERT_EQUAL(IUF_TRUE,equal);

    char generatedKey[1024];
    char generatedValue[1024];
    // TODO:
    // elementID=1000 (iterations) take a second
    // elementID=10000 (iterations) takes more than 10 times
    // which is non linear.
    // Fix.
    for (elementID=0;elementID<1000;elementID++)
    {
        snprintf(generatedKey,sizeof(generatedKey),"key_%d", elementID);
        snprintf(generatedValue,sizeof(generatedValue),"value_%d", elementID);
        status = iufParameterDictSet(dict, generatedKey, generatedValue);
        TEST_ASSERT_EQUAL(IUF_E_OK,status);
        TEST_ASSERT_EQUAL_STRING(generatedValue,iufParameterDictGet(dict, generatedKey));
        equal = iufParameterDictCompare(dict, notherDict);
        TEST_ASSERT_EQUAL(IUF_FALSE,equal);
        equal = iufParameterDictCompare(notherDict, dict);
        TEST_ASSERT_EQUAL(IUF_FALSE,equal);

        status = iufParameterDictSet(notherDict, generatedKey, generatedValue);
        TEST_ASSERT_EQUAL(IUF_E_OK,status);
        TEST_ASSERT_EQUAL_STRING(generatedValue,iufParameterDictGet(notherDict, generatedKey));
        equal = iufParameterDictCompare(dict, notherDict);
        TEST_ASSERT_EQUAL(IUF_TRUE,equal);
        equal = iufParameterDictCompare(notherDict, dict);
        TEST_ASSERT_EQUAL(IUF_TRUE,equal);
    }

    iufParameterDictDelete(dict);
    iufParameterDictDelete(notherDict);
}


TEST(IufParameterDict, testIufSerialization)
{

    // create and fill
    iupad_t dict = dgGenerateParameterDict(100);
    char *filename = "testIufParameterDictSerialization.hdf5";
    char *dictPath =  "/ParameterDict";

    // save
    int status = iufParameterDictSave(dict, H5I_INVALID_HID);
    TEST_ASSERT(status != IUF_E_OK);

    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
    hid_t group_id = H5Gcreate(handle, dictPath, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    TEST_ASSERT(group_id > 0);

    status = iufParameterDictSave(NULL, group_id);
    TEST_ASSERT(status != IUF_E_OK);

    status = iufParameterDictSave(dict, group_id);
    H5Gclose(group_id);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    // read back
    iupad_t savedObj = iufParameterDictLoad(H5I_INVALID_HID);
    TEST_ASSERT_EQUAL(NULL,savedObj);

    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
    group_id = H5Gopen(handle, dictPath, H5P_DEFAULT);
    TEST_ASSERT(group_id > 0);

    savedObj = iufParameterDictLoad(group_id);
    TEST_ASSERT(savedObj != NULL);
    H5Gclose(group_id);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUF_TRUE, iufParameterDictCompare(dict,savedObj));

    iufParameterDictDelete(dict);
    iufParameterDictDelete(savedObj);

}


TEST(IufParameterDict, testIufSerializationErrorFlow)
{

    // create and fill
    iupad_t dict = dgGenerateParameterDict(100);
    char *filename = "testIufParameterDictSerializationErrorflow.hdf5";
    char *dictPath =  "/ParameterDict";

    // save
    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
    hid_t group_id = H5Gcreate(handle, dictPath, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    TEST_ASSERT(group_id > 0);

    int status = iufParameterDictSave(NULL, group_id);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE,status);
    status = iufParameterDictSave(dict, H5I_INVALID_HID);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE,status);
    status = iufParameterDictSave(dict, group_id);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);
    H5Gclose(group_id);
    H5Fclose(handle);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
    group_id = H5Gopen(handle, dictPath, H5P_DEFAULT);
    TEST_ASSERT(group_id > 0);

    iupad_t savedObj = iufParameterDictLoad(H5I_INVALID_HID);
    TEST_ASSERT(savedObj == IUPAD_INVALID);
    savedObj = iufParameterDictLoad(group_id);
    TEST_ASSERT(savedObj != IUPAD_INVALID);
    H5Gclose(group_id);
    H5Fclose(handle);

    iufParameterDictDelete(dict);
    iufParameterDictDelete(savedObj);

}


TEST_GROUP_RUNNER(IufParameterDict)
{
    RUN_TEST_CASE(IufParameterDict, testIufParameterDictCreateDelete);
    RUN_TEST_CASE(IufParameterDict, testIufParameterDictCompare);
    RUN_TEST_CASE(IufParameterDict, testIufParameterDictSetGet);
    RUN_TEST_CASE(IufParameterDict, testIufParameterDictGetKeys)
    RUN_TEST_CASE(IufParameterDict, testIufSerialization);
    RUN_TEST_CASE(IufParameterDict, testIufSerializationErrorFlow);
}
