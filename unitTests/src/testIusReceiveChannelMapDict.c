//
// Created by nlv09165 on 03/05/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <ius.h>
#include <iusReceiveChannelMapDictPrivate.h>

static char *pErrorFilename = "IusReceiveChannelMapDict.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(IusReceiveChannelMapDict);

TEST_SETUP(IusReceiveChannelMapDict)
{
	iusErrorLogClear();
	iusErrorLog(IUS_TRUE);
	iusErrorAutoReport(IUS_TRUE);
	fpErrorLogging = fopen(pErrorFilename, "w+");
	iusErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IusReceiveChannelMapDict)
{
	iusErrorLogClear();
	iusErrorLog(IUS_FALSE);
	if (fpErrorLogging != NULL)
		fclose(fpErrorLogging);
	fpErrorLogging=stderr;
	iusErrorSetStream(fpErrorLogging);
	remove(pErrorFilename);
}


TEST(IusReceiveChannelMapDict, testIusReceiveChannelMapDictCreate)
{
	iurcmd_t obj = iusReceiveChannelMapDictCreate();
	iurcmd_t notherObj = iusReceiveChannelMapDictCreate();

	TEST_ASSERT(obj != IURCMD_INVALID);
	TEST_ASSERT(notherObj != IURCMD_INVALID);
	iusReceiveChannelMapDictDelete(obj);
	iusReceiveChannelMapDictDelete(notherObj);
}

TEST(IusReceiveChannelMapDict, testIusReceiveChannelMapDictSetGet)
{
	const int numChannels = 10;
	int oneToOneMap[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	int     status;
	char *pObjLabel = "one-to-one";

	iurcm_t obj = iusReceiveChannelMapCreate(numChannels);
	iurcmd_t dict = iusReceiveChannelMapDictCreate();
	status = iusReceiveChannelMapSetMap(obj, oneToOneMap);
	status |= iusReceiveChannelMapDictSet(dict, pObjLabel, obj);
	TEST_ASSERT(status == IUS_E_OK);

	iurcm_t retrievedObj = iusReceiveChannelMapDictGet(dict, pObjLabel);
	TEST_ASSERT_EQUAL(IUS_TRUE, iusReceiveChannelMapCompare(obj,retrievedObj));

	// Invalid params
	long filePos = ftell(fpErrorLogging);
	TEST_ASSERT_EQUAL(0,iusErrorGetCount());
	TEST_ASSERT_EQUAL(IUTA_INVALID, iusReceiveChannelMapDictGet(dict,NULL));
	TEST_ASSERT_EQUAL(IUTA_INVALID, iusReceiveChannelMapDictGet(NULL,pObjLabel));
	TEST_ASSERT_EQUAL(IUTA_INVALID, iusReceiveChannelMapDictGet(dict,"unknownLabel"));

	TEST_ASSERT_EQUAL(IUS_ERR_VALUE, iusReceiveChannelMapDictSet(dict, pObjLabel, obj));
	TEST_ASSERT_EQUAL(IUS_ERR_VALUE, iusReceiveChannelMapDictSet(NULL, pObjLabel, obj));
	TEST_ASSERT_EQUAL(IUS_ERR_VALUE, iusReceiveChannelMapDictSet(dict, pObjLabel, NULL));

	TEST_ASSERT_EQUAL(6,iusErrorGetCount());
	TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

	iusReceiveChannelMapDelete(obj);
	iusReceiveChannelMapDictDelete(dict);

}


TEST(IusReceiveChannelMapDict, testIusReceiveChannelMapDictCompare)
{
	IUS_BOOL equal;
	const int numChannels = 10;
	int oneToOneMap[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	int inverseMap[10] = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
	int     status;
	iurcm_t receiveChannelMap = iusReceiveChannelMapCreate(numChannels);
	iurcm_t inverseReceiveChannelMap = iusReceiveChannelMapCreate(numChannels);

	iurcmd_t dict = iusReceiveChannelMapDictCreate();
	status = iusReceiveChannelMapSetMap(receiveChannelMap, oneToOneMap);
	status |= iusReceiveChannelMapDictSet(dict, "one-to-one", receiveChannelMap);
	TEST_ASSERT(status == IUS_E_OK);

	iurcmd_t notherDict = iusReceiveChannelMapDictCreate();
	status = iusReceiveChannelMapSetMap(inverseReceiveChannelMap, inverseMap);
	status |= iusReceiveChannelMapDictSet(dict, "inverse", receiveChannelMap);
	TEST_ASSERT(status == IUS_E_OK);

	TEST_ASSERT(dict != IURCMD_INVALID);
	TEST_ASSERT(notherDict != IURCMD_INVALID);

	// Same empty lists...
	equal = iusReceiveChannelMapDictCompare(dict, dict);
	TEST_ASSERT_EQUAL(IUS_TRUE, equal);
	equal = iusReceiveChannelMapDictCompare(dict, notherDict);
	TEST_ASSERT_EQUAL(IUS_FALSE, equal);

	// invalid params
	equal = iusReceiveChannelMapDictCompare(dict, NULL);
	TEST_ASSERT_EQUAL(IUS_FALSE, equal);
	equal = iusReceiveChannelMapDictCompare(NULL, dict);
	TEST_ASSERT_EQUAL(IUS_FALSE, equal);

	iusReceiveChannelMapDelete(receiveChannelMap);
	iusReceiveChannelMapDelete(inverseReceiveChannelMap);
	iusReceiveChannelMapDictDelete(dict);
	iusReceiveChannelMapDictDelete(notherDict);
}

TEST(IusReceiveChannelMapDict, testIusReceiveChannelMapDictSerialization)
{
	char *filename = "testIusReceiveChannelMapSerialization.hdf5";

	int numChannels = 8;
	char *label = "one-to-one";
	int channelMap[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };

	// create and save
	iurcm_t obj = iusReceiveChannelMapCreate(numChannels);
	int status = iusReceiveChannelMapSetMap(obj, channelMap);

	iurcmd_t receiveChannelMapDict = iusReceiveChannelMapDictCreate();
	status |= iusReceiveChannelMapDictSet(receiveChannelMapDict, label, obj);
	TEST_ASSERT(status == IUS_E_OK);

	hid_t handle = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
	TEST_ASSERT(handle > 0);
	status = iusReceiveChannelMapDictSave(receiveChannelMapDict, handle);
	H5Fclose(handle);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);

	// read back
	handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT);
	iurcmd_t savedDict = iusReceiveChannelMapDictLoad(handle);
	TEST_ASSERT_NOT_EQUAL(NULL, savedDict);
	H5Fclose(handle);

	TEST_ASSERT_EQUAL(IUS_TRUE, iusReceiveChannelMapDictCompare(receiveChannelMapDict, savedDict));
	iusReceiveChannelMapDelete(obj);
	iusReceiveChannelMapDictDelete(receiveChannelMapDict);
	iusReceiveChannelMapDictDelete(savedDict);
}

TEST_GROUP_RUNNER(IusReceiveChannelMapDict)
{
	RUN_TEST_CASE(IusReceiveChannelMapDict, testIusReceiveChannelMapDictCreate);
	RUN_TEST_CASE(IusReceiveChannelMapDict, testIusReceiveChannelMapDictSetGet);
	RUN_TEST_CASE(IusReceiveChannelMapDict, testIusReceiveChannelMapDictCompare);
	RUN_TEST_CASE(IusReceiveChannelMapDict, testIusReceiveChannelMapDictSerialization);
}
