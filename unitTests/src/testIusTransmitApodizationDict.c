//
// Created by nlv09165 on 03/05/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <hdf5.h>
#include <ius.h>
#include <iusHLTransmitApodization.h>
#include <iusHLTransmitApodizationDictImp.h>

TEST_GROUP(IusTransmitApodizationDict);

TEST_SETUP(IusTransmitApodizationDict)
{
}

TEST_TEAR_DOWN(IusTransmitApodizationDict)
{
}


TEST(IusTransmitApodizationDict, testIusTransmitApodizationDictCreate)
{
	iutad_t obj = iusHLTransmitApodizationDictCreate();
	iutad_t notherObj = iusHLTransmitApodizationDictCreate();

	TEST_ASSERT(obj != IUTAD_INVALID);
	TEST_ASSERT(notherObj != IUTAD_INVALID);
	iusHLTransmitApodizationDictDelete(obj);
	iusHLTransmitApodizationDictDelete(notherObj);
}


TEST(IusTransmitApodizationDict, testIusTransmitApodizationDictCompare)
{
	IUS_BOOL equal;
	const int numElements = 8;
	float ones[8] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
	float ramp[8] = { 0.1f, 0.2f, 0.3f, 0.4f, 0.4f, 0.3f, 0.2f, 0.1f };
	int     status;
	iuta_t transmitApodization = iusHLTransmitApodizationCreate(numElements);
	iuta_t otherTransmitApodization = iusHLTransmitApodizationCreate(numElements);

	iutad_t dict = iusHLTransmitApodizationDictCreate();
	status = iusHLTransmitApodizationSetApodization(transmitApodization, ones);	
	status |= iusHLTransmitApodizationDictSet(dict, "ones", transmitApodization);
	TEST_ASSERT(status == IUS_E_OK);

	iutad_t notherDict = iusHLTransmitApodizationDictCreate();
	status = iusHLTransmitApodizationSetApodization(otherTransmitApodization, ramp);
	status |= iusHLTransmitApodizationDictSet(dict, "triangle", transmitApodization);

	TEST_ASSERT(dict != IUTAD_INVALID);
	TEST_ASSERT(notherDict != IUTAD_INVALID);

	// Same empty lists...
	equal = iusHLTransmitApodizationDictCompare(dict, dict);
	TEST_ASSERT_EQUAL(IUS_TRUE, equal);
	equal = iusHLTransmitApodizationDictCompare(dict, notherDict);
	TEST_ASSERT_EQUAL(IUS_FALSE, equal);

	// invalid params
	equal = iusHLTransmitApodizationDictCompare(dict, NULL);
	TEST_ASSERT_EQUAL(IUS_FALSE, equal);
	equal = iusHLTransmitApodizationDictCompare(NULL, dict);
	TEST_ASSERT_EQUAL(IUS_FALSE, equal);

	iusHLTransmitApodizationDictDelete(dict);
	iusHLTransmitApodizationDictDelete(notherDict);
}

TEST(IusTransmitApodizationDict, testIusTransmitApodizationDictSerialization)
{
	//hid_t group_id;
	char *filename = "testIusTransmitApodizationSerialization.hdf5";
	//char *TransmitApodizationDictPath = "/TransmitApodization";

	int numElements = 8;
	char *label = "ones";
	float ones[8] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };

	// create and save
	iuta_t obj = iusHLTransmitApodizationCreate(numElements);
	int status = iusHLTransmitApodizationSetApodization(obj, ones);

	iutad_t transmitApodizationDict = iusHLTransmitApodizationDictCreate();
	status |= iusHLTransmitApodizationDictSet(transmitApodizationDict, label, obj);

	hid_t handle = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
	TEST_ASSERT(handle > 0);
	//group_id = H5Gcreate(handle, TransmitApodizationDictPath, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	//TEST_ASSERT(group_id > 0);
	status = iusHLTransmitApodizationDictSave(transmitApodizationDict, handle);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);
	//H5Gclose(group_id);
	H5Fclose(handle);

	// read back
	handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT);
	TEST_ASSERT(handle > 0);
	//group_id = H5Gcreate(handle, TransmitApodizationDictPath, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	//TEST_ASSERT(group_id > 0);
	iutad_t savedDict = iusHLTransmitApodizationDictLoad(handle);
	TEST_ASSERT_NOT_EQUAL(NULL, savedDict);
	//H5Gclose(group_id);
	H5Fclose(handle);

	TEST_ASSERT_EQUAL(IUS_TRUE, iusHLTransmitApodizationDictCompare(transmitApodizationDict, savedDict));
	iusHLTransmitApodizationDictDelete(transmitApodizationDict);
	iusHLTransmitApodizationDictDelete(savedDict);
}



TEST_GROUP_RUNNER(IusTransmitApodizationDict)
{
	RUN_TEST_CASE(IusTransmitApodizationDict, testIusTransmitApodizationDictCreate);
	RUN_TEST_CASE(IusTransmitApodizationDict, testIusTransmitApodizationDictCompare);
	RUN_TEST_CASE(IusTransmitApodizationDict, testIusTransmitApodizationDictSerialization);
}
