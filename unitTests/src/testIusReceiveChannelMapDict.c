//
// Created by nlv09165 on 03/05/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <hdf5.h>
#include <ius.h>
#include <iusHLReceiveChannelMapDict.h>
//#include <iusHLReceiveChannelMapDictImp.h>

TEST_GROUP(IusReceiveChannelMapDict);

TEST_SETUP(IusReceiveChannelMapDict)
{
}

TEST_TEAR_DOWN(IusReceiveChannelMapDict)
{
}


TEST(IusReceiveChannelMapDict, testIusReceiveChannelMapDictCreate)
{
	iurcmd_t obj = iusHLReceiveChannelMapDictCreate();
	iurcmd_t notherObj = iusHLReceiveChannelMapDictCreate();

	TEST_ASSERT(obj != IURCMD_INVALID);
	TEST_ASSERT(notherObj != IURCMD_INVALID);
	iusHLReceiveChannelMapDictDelete(obj);
	iusHLReceiveChannelMapDictDelete(notherObj);
}


TEST(IusReceiveChannelMapDict, testIusReceiveChannelMapDictCompare)
{
	IUS_BOOL equal;
	const int numChannels = 10;
	int oneToOneMap[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	int inverseMap[10] = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
	int     status;
	iurcm_t receiveChannelMap = iusHLReceiveChannelMapCreate(numChannels);
	iurcm_t inverseReceiveChannelMap = iusHLReceiveChannelMapCreate(numChannels);

	iurcmd_t dict = iusHLReceiveChannelMapDictCreate();
	status = iusHLReceiveChannelMapSetMap(receiveChannelMap, oneToOneMap);	
	status |= iusHLReceiveChannelMapDictSet(dict, "one-to-one", receiveChannelMap);
	TEST_ASSERT(status == IUS_E_OK);

	iurcmd_t notherDict = iusHLReceiveChannelMapDictCreate();
	status = iusHLReceiveChannelMapSetMap(inverseReceiveChannelMap, inverseMap);
	status |= iusHLReceiveChannelMapDictSet(dict, "inverse", receiveChannelMap);

	TEST_ASSERT(dict != IURCMD_INVALID);
	TEST_ASSERT(notherDict != IURCMD_INVALID);

	// Same empty lists...
	equal = iusHLReceiveChannelMapDictCompare(dict, dict);
	TEST_ASSERT_EQUAL(IUS_TRUE, equal);
	equal = iusHLReceiveChannelMapDictCompare(dict, notherDict);
	TEST_ASSERT_EQUAL(IUS_FALSE, equal);

	// invalid params
	equal = iusHLReceiveChannelMapDictCompare(dict, NULL);
	TEST_ASSERT_EQUAL(IUS_FALSE, equal);
	equal = iusHLReceiveChannelMapDictCompare(NULL, dict);
	TEST_ASSERT_EQUAL(IUS_FALSE, equal);

	iusHLReceiveChannelMapDictDelete(dict);
	iusHLReceiveChannelMapDictDelete(notherDict);
}

TEST(IusReceiveChannelMapDict, testIusReceiveChannelMapDictSerialization)
{
	char *filename = "testIusReceiveChannelMapSerialization.hdf5";
	char *ReceiveChannelMapDictPath = "/ReceiveChannelMap";

	int numChannels = 8;
	char *label = "one-to-one";
	int channelMap[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };

	// create and save
	iurcm_t obj = iusHLReceiveChannelMapCreate(numChannels);
	int status = iusHLReceiveChannelMapSetMap(obj, channelMap);

	iurcmd_t receiveChannelMapDict = iusHLReceiveChannelMapDictCreate();
	status |= iusHLReceiveChannelMapDictSet(receiveChannelMapDict, label, obj);

	hid_t handle = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
	TEST_ASSERT(handle > 0);
	status = iusHLReceiveChannelMapDictSave(receiveChannelMapDict, ReceiveChannelMapDictPath, handle);
	H5Fclose(handle);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);

	// read back
	handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT);
	iurcmd_t savedDict = iusHLReceiveChannelMapDictLoad(handle, ReceiveChannelMapDictPath);
	TEST_ASSERT_NOT_EQUAL(NULL, savedDict);
	H5Fclose(handle);

	TEST_ASSERT_EQUAL(IUS_TRUE, iusHLReceiveChannelMapDictCompare(receiveChannelMapDict, savedDict));
	iusHLReceiveChannelMapDictDelete(receiveChannelMapDict);
	iusHLReceiveChannelMapDictDelete(savedDict);
}


#if 0
TEST(IusReceiveChannelMapDict, testIusReceiveChannelMapSerialization)

{
	IUS_BOOL equal;
	int numPulses = 10;
	int numPulseValues = 10;
	float   pulseFrequency = 8000000.0f;   /**< frequency that the pulse represents in Hz */
	float   pulseAmplitude = 800.0f;       /**< (max) amplitude of the pulse in Volts */
	int     pulseCount = 10;               /**< number of cycles that the pulse represents */
	int     status = 10;
	char *filename = "testIusPulseDictSerialization.hdf5";
	char *dictPath = "/PulseDict";

	// create
	iupd_t dict = iusHLPulseDictCreate();
	TEST_ASSERT(dict != IUPD_INVALID);

	// fill
	char *parametricLabel = "parametricPulseLabel";
	char *nonParametricLabel = "nonParametricPulseLabel";
	iupp_t parametricPulse = iusHLParametricPulseCreate(parametricLabel, pulseFrequency, pulseAmplitude, pulseCount);
	iunpp_t nonParametricPulse = iusHLNonParametricPulseCreate(nonParametricLabel, numPulseValues);
	iusHLNonParametricPulseSetValue(nonParametricPulse, 0, 10.0f, 10.0f);
	iusHLNonParametricPulseSetValue(nonParametricPulse, 1, 20.0f, 10.0f);
	status = iusHLPulseDictSet(dict, nonParametricLabel, (iup_t)nonParametricPulse);
	TEST_ASSERT(status == IUS_E_OK);
	status = iusHLPulseDictSet(dict, parametricLabel, (iup_t)parametricPulse);
	TEST_ASSERT(status == IUS_E_OK);

	// save
	hid_t handle = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
	TEST_ASSERT(handle > 0);
	status = iusHLPulseDictSave(dict, dictPath, handle);
	H5Fclose(handle);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);

	// read back
	handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT);
	iupd_t savedObj = iusHLPulseDictLoad(handle, dictPath);
	TEST_ASSERT(savedObj != NULL);
	H5Fclose(handle);

	TEST_ASSERT_EQUAL(IUS_TRUE, iusHLPulseDictCompare(dict, savedObj));

	iusHLPulseDictDelete(dict);
	iusHLPulseDictDelete(savedObj);
}
#endif

TEST_GROUP_RUNNER(IusReceiveChannelMapDict)
{
	RUN_TEST_CASE(IusReceiveChannelMapDict, testIusReceiveChannelMapDictCreate);
	RUN_TEST_CASE(IusReceiveChannelMapDict, testIusReceiveChannelMapDictCompare);
	RUN_TEST_CASE(IusReceiveChannelMapDict, testIusReceiveChannelMapDictSerialization);
}
