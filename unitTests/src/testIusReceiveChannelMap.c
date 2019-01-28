//
// Created by nlv12901 on 20/07/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <ius.h>
#include <iusReceiveChannelMapPrivate.h>
static char *pErrorFilename = "IusReceiveChannelMap.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(IusReceiveChannelMap);

TEST_SETUP(IusReceiveChannelMap)
{
	iusErrorLogClear();
	iusErrorLog(IUS_TRUE);
	iusErrorAutoReport(IUS_TRUE);
	fpErrorLogging = fopen(pErrorFilename, "w+");
	iusErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IusReceiveChannelMap)
{
	iusErrorLogClear();
	iusErrorLog(IUS_FALSE);
	if (fpErrorLogging != NULL)
		fclose(fpErrorLogging);
	fpErrorLogging=stderr;
	iusErrorSetStream(fpErrorLogging);
	remove(pErrorFilename);
}


TEST(IusReceiveChannelMap, testIusReceiveChannelMapCreate)
{
	const int numChannels = 5;
	int map[5] = { 4,3,2,1,0 };
	iurcm_t obj = iusReceiveChannelMapCreate(numChannels);
	int status = iusReceiveChannelMapSetMap(obj, map);
	TEST_ASSERT(status == IUS_E_OK);
	TEST_ASSERT(obj != IURCM_INVALID);
	iusReceiveChannelMapDelete(obj);
	
	// invalid params
	long filePos = ftell(fpErrorLogging);
	TEST_ASSERT_EQUAL(0,iusErrorGetCount());

	obj = iusReceiveChannelMapCreate(0);
	TEST_ASSERT(obj == IURCM_INVALID);
	obj = iusReceiveChannelMapCreate(-1);
	TEST_ASSERT(obj == IURCM_INVALID);

	TEST_ASSERT_EQUAL(2,iusErrorGetCount());
	TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
}

TEST(IusReceiveChannelMap, testIusReceiveChannelMapDelete)
{
	const int numChannels = 5;

	iurcm_t obj = iusReceiveChannelMapCreate(numChannels);
	TEST_ASSERT(obj != IURCM_INVALID);
	int status = iusReceiveChannelMapDelete(obj);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);

	// invalid params
	status = iusReceiveChannelMapDelete(NULL);
	TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
}

TEST(IusReceiveChannelMap, testIusReceiveChannelMapCompare)
{
	IUS_BOOL equal;

	iurcm_t obj = iusReceiveChannelMapCreate(5);
	iurcm_t notherObj = iusReceiveChannelMapCreate(6);
	TEST_ASSERT(obj != IURCM_INVALID);
	TEST_ASSERT(notherObj != IURCM_INVALID);

	equal = iusReceiveChannelMapCompare(obj, obj);
	TEST_ASSERT_EQUAL(IUS_TRUE, equal);
	equal = iusReceiveChannelMapCompare(obj, notherObj);
	TEST_ASSERT_EQUAL(IUS_FALSE, equal);

	// invalid params
	equal = iusReceiveChannelMapCompare(obj, NULL);
	TEST_ASSERT_EQUAL(IUS_FALSE, equal);
	equal = iusReceiveChannelMapCompare(NULL, obj);
	TEST_ASSERT_EQUAL(IUS_FALSE, equal);

	iusReceiveChannelMapDelete(obj);
	iusReceiveChannelMapDelete(notherObj);
}

TEST(IusReceiveChannelMap, testIusReceiveChannelMapSetGet)
{
    int i;
    int status;
	const int numChannels=5;

	iurcm_t obj = iusReceiveChannelMapCreate(numChannels);
	TEST_ASSERT_EQUAL(numChannels, iusReceiveChannelMapGetNumChannels(obj));
	TEST_ASSERT_EQUAL(numChannels,iusReceiveChannelMapGetNumDelays(obj));

	// Delays
	for(i=0;i<numChannels;i++)
	{
		float delay = i*2.0f;
		status = iusReceiveChannelMapSetStartDelay(obj, i, delay);
		TEST_ASSERT(status == IUS_E_OK);
		TEST_ASSERT_EQUAL_FLOAT(delay, iusReceiveChannelMapGetStartDelay(obj, i));
	}

	iusReceiveChannelMapDelete(obj);
}

TEST(IusReceiveChannelMap, testIusSerialization)
{
    char *filename = "testIusReceiveChannelMap.hdf5";
    int numChannels=10;
    int i;
    int status;

    // Create
    iurcm_t obj = iusReceiveChannelMapCreate(numChannels);

    // Delays
    for(i=0;i<numChannels;i++)
    {
        float delay = i*3.14f;
        status = iusReceiveChannelMapSetStartDelay(obj, i, delay);
        TEST_ASSERT(status == IUS_E_OK);
        TEST_ASSERT_EQUAL_FLOAT(delay, iusReceiveChannelMapGetStartDelay(obj, i));
    }

    // save
    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
    status = iusReceiveChannelMapSave(obj, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iurcm_t savedObj = iusReceiveChannelMapLoad(handle);
    TEST_ASSERT(savedObj != NULL);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUS_TRUE, iusReceiveChannelMapCompare(obj, savedObj));
    iusReceiveChannelMapDelete(obj);
    iusReceiveChannelMapDelete(savedObj);
}

TEST_GROUP_RUNNER(IusReceiveChannelMap)
{
	RUN_TEST_CASE(IusReceiveChannelMap, testIusReceiveChannelMapCreate);
	RUN_TEST_CASE(IusReceiveChannelMap, testIusReceiveChannelMapDelete);
	RUN_TEST_CASE(IusReceiveChannelMap, testIusReceiveChannelMapCompare);
	RUN_TEST_CASE(IusReceiveChannelMap, testIusReceiveChannelMapSetGet);
}
