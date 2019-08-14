#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <iuf.h>
#include <util.h>
#include <iufReceiveChannelMapDictPrivate.h>

static char *pErrorFilename = "IufReceiveChannelMapDict.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(IufReceiveChannelMapDict);

TEST_SETUP(IufReceiveChannelMapDict)
{
	iufErrorLogClear();
	iufErrorLog(IUF_TRUE);
    iufErrorAutoReportSet(IUF_TRUE);
	fpErrorLogging = fopen(pErrorFilename, "w+");
	iufErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IufReceiveChannelMapDict)
{
	iufErrorLogClear();
	iufErrorLog(IUF_FALSE);
	if (fpErrorLogging != NULL)
		fclose(fpErrorLogging);
	fpErrorLogging=stderr;
	iufErrorSetStream(fpErrorLogging);
	remove(pErrorFilename);
}


TEST(IufReceiveChannelMapDict, testIufReceiveChannelMapDictCreate)
{
	iurcmd_t obj = iufReceiveChannelMapDictCreate();
	iurcmd_t notherObj = iufReceiveChannelMapDictCreate();

	TEST_ASSERT(obj != IURCMD_INVALID);
	TEST_ASSERT(notherObj != IURCMD_INVALID);
    TEST_ASSERT(iufReceiveChannelMapDictDelete(obj) == IUF_E_OK);
    TEST_ASSERT(iufReceiveChannelMapDictDelete(notherObj) == IUF_E_OK);
    TEST_ASSERT(iufReceiveChannelMapDictDelete(NULL) != IUF_E_OK);
    TEST_ASSERT(iufReceiveChannelMapDictDeepDelete(NULL) != IUF_E_OK);
}

TEST(IufReceiveChannelMapDict, testIufReceiveChannelMapDictSetGet)
{
	const int numChannels = 10;
	int oneToOneMap[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	int     status;
	char *pObjLabel = "one-to-one";

	iurcm_t obj = iufReceiveChannelMapCreate(numChannels);
	iurcmd_t dict = iufReceiveChannelMapDictCreate();
	status = iufReceiveChannelMapSetMap(obj, oneToOneMap);
	status |= iufReceiveChannelMapDictSet(dict, pObjLabel, obj);
	TEST_ASSERT(status == IUF_E_OK);

	iurcm_t retrievedObj = iufReceiveChannelMapDictGet(dict, pObjLabel);
	TEST_ASSERT_EQUAL(IUF_TRUE, iufReceiveChannelMapCompare(obj,retrievedObj));

	// Invalid params
	long filePos = ftell(fpErrorLogging);
	TEST_ASSERT_EQUAL(0,iufErrorGetCount());
	TEST_ASSERT_EQUAL(IUTA_INVALID, iufReceiveChannelMapDictGet(dict,NULL));
	TEST_ASSERT_EQUAL(IUTA_INVALID, iufReceiveChannelMapDictGet(NULL,pObjLabel));
	TEST_ASSERT_EQUAL(IUTA_INVALID, iufReceiveChannelMapDictGet(dict,"unknownLabel"));

	TEST_ASSERT_EQUAL(IUF_ERR_VALUE, iufReceiveChannelMapDictSet(dict, pObjLabel, obj));
	TEST_ASSERT_EQUAL(IUF_ERR_VALUE, iufReceiveChannelMapDictSet(NULL, pObjLabel, obj));
	TEST_ASSERT_EQUAL(IUF_ERR_VALUE, iufReceiveChannelMapDictSet(dict, pObjLabel, NULL));

	TEST_ASSERT_EQUAL(6,iufErrorGetCount());
	TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

	iufReceiveChannelMapDelete(obj);
	iufReceiveChannelMapDictDelete(dict);

}


TEST(IufReceiveChannelMapDict, testIufReceiveChannelMapDictGetKeys)
{
    const int numChannels = 10;
    int oneToOneMap[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int     status;
    char *labels[] = { "one" , "two" , "three", "four" , "five"};

    // Create
    iurcm_t obj = iufReceiveChannelMapCreate(numChannels);
    iurcmd_t dict = iufReceiveChannelMapDictCreate();
    status = iufReceiveChannelMapSetMap(obj, oneToOneMap);

    // Fill
    int i;
    int keySize = sizeof(labels)/sizeof(labels[0]);
    for (i=0; i<keySize; i++)
    {
        status = iufReceiveChannelMapDictSet(dict, labels[i], obj);
        TEST_ASSERT(status == IUF_E_OK);
    }

    size_t dictSize = iufReceiveChannelMapDictGetSize(dict);
    TEST_ASSERT_EQUAL(5, dictSize);
    char **keys = iufReceiveChannelMapDictGetKeys(dict);

    // Validate keys
    for (i=0; i<keySize; i++)
    {
        TEST_ASSERT_EQUAL(IUF_TRUE, aInB(keys[i], labels));
    }


    // Invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    keys = iufReceiveChannelMapDictGetKeys(NULL);
    TEST_ASSERT_EQUAL(NULL,keys);

    TEST_ASSERT_EQUAL(1,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

    iufReceiveChannelMapDelete(obj);
    iufReceiveChannelMapDictDelete(dict);

}


TEST(IufReceiveChannelMapDict, testIufReceiveChannelMapDictCompare)
{
	IUF_BOOL equal;
	const int numChannels = 10;
	int oneToOneMap[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	int inverseMap[10] = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
	int     status;
	iurcm_t receiveChannelMap = iufReceiveChannelMapCreate(numChannels);
	iurcm_t inverseReceiveChannelMap = iufReceiveChannelMapCreate(numChannels);

	iurcmd_t dict = iufReceiveChannelMapDictCreate();
	status = iufReceiveChannelMapSetMap(receiveChannelMap, oneToOneMap);
	status |= iufReceiveChannelMapDictSet(dict, "one-to-one", receiveChannelMap);
	TEST_ASSERT(status == IUF_E_OK);

	iurcmd_t notherDict = iufReceiveChannelMapDictCreate();
	status = iufReceiveChannelMapSetMap(inverseReceiveChannelMap, inverseMap);
	status |= iufReceiveChannelMapDictSet(dict, "inverse", receiveChannelMap);
	TEST_ASSERT(status == IUF_E_OK);

	TEST_ASSERT(dict != IURCMD_INVALID);
	TEST_ASSERT(notherDict != IURCMD_INVALID);

	// Same empty lists...
	equal = iufReceiveChannelMapDictCompare(dict, dict);
	TEST_ASSERT_EQUAL(IUF_TRUE, equal);
	equal = iufReceiveChannelMapDictCompare(dict, notherDict);
	TEST_ASSERT_EQUAL(IUF_FALSE, equal);
    equal = iufReceiveChannelMapDictCompare(notherDict, dict);
    TEST_ASSERT_EQUAL(IUF_FALSE, equal);

	// invalid params
	equal = iufReceiveChannelMapDictCompare(dict, NULL);
	TEST_ASSERT_EQUAL(IUF_FALSE, equal);
	equal = iufReceiveChannelMapDictCompare(NULL, dict);
	TEST_ASSERT_EQUAL(IUF_FALSE, equal);

	iufReceiveChannelMapDelete(receiveChannelMap);
	iufReceiveChannelMapDelete(inverseReceiveChannelMap);
	iufReceiveChannelMapDictDelete(dict);
	iufReceiveChannelMapDictDelete(notherDict);
}

TEST(IufReceiveChannelMapDict, testIufReceiveChannelMapDictSerialization)
{
	char *filename = "testIufReceiveChannelMapSerialization.hdf5";

	int numChannels = 8;
	char *label = "one-to-one";
	int channelMap[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };

	// create and save
    iurcm_t obj = iufReceiveChannelMapCreate(numChannels);
	int status = iufReceiveChannelMapSetMap(obj, channelMap);

	iurcmd_t receiveChannelMapDict = iufReceiveChannelMapDictCreate();
	status |= iufReceiveChannelMapDictSet(receiveChannelMapDict, label, obj);
	TEST_ASSERT(status == IUF_E_OK);

    status = iufReceiveChannelMapDictSave(receiveChannelMapDict, H5I_INVALID_HID);
    TEST_ASSERT(status != IUF_E_OK);

    hid_t handle = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
	TEST_ASSERT(handle > 0);
    status = iufReceiveChannelMapDictSave(NULL, handle);
    TEST_ASSERT(status != IUF_E_OK);
    status = iufReceiveChannelMapDictSave(receiveChannelMapDict, handle);
	H5Fclose(handle);
	TEST_ASSERT_EQUAL(IUF_E_OK, status);

	// read back
    iurcmd_t savedDict = iufReceiveChannelMapDictLoad(H5I_INVALID_HID);
    TEST_ASSERT_EQUAL(NULL,savedDict);

	handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT);
	savedDict = iufReceiveChannelMapDictLoad(handle);
	TEST_ASSERT_NOT_EQUAL(NULL, savedDict);
	H5Fclose(handle);

	TEST_ASSERT_EQUAL(IUF_TRUE, iufReceiveChannelMapDictCompare(receiveChannelMapDict, savedDict));
	iufReceiveChannelMapDelete(obj);
	iufReceiveChannelMapDictDelete(receiveChannelMapDict);
	iufReceiveChannelMapDictDelete(savedDict);
}

TEST_GROUP_RUNNER(IufReceiveChannelMapDict)
{
	RUN_TEST_CASE(IufReceiveChannelMapDict, testIufReceiveChannelMapDictCreate);
	RUN_TEST_CASE(IufReceiveChannelMapDict, testIufReceiveChannelMapDictSetGet);
    RUN_TEST_CASE(IufReceiveChannelMapDict, testIufReceiveChannelMapDictGetKeys)
    RUN_TEST_CASE(IufReceiveChannelMapDict, testIufReceiveChannelMapDictCompare);
	RUN_TEST_CASE(IufReceiveChannelMapDict, testIufReceiveChannelMapDictSerialization);
}
