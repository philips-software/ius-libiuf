//
// Created by nlv12901 on 20/07/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <iuf.h>
#include <iufReceiveChannelMapPrivate.h>
static char *pErrorFilename = "IufReceiveChannelMap.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(IufReceiveChannelMap);

TEST_SETUP(IufReceiveChannelMap)
{
	iufErrorLogClear();
	iufErrorLog(IUF_TRUE);
    iufErrorAutoReportSet(IUF_TRUE);
	fpErrorLogging = fopen(pErrorFilename, "w+");
	iufErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IufReceiveChannelMap)
{
	iufErrorLogClear();
	iufErrorLog(IUF_FALSE);
	if (fpErrorLogging != NULL)
		fclose(fpErrorLogging);
	fpErrorLogging=stderr;
	iufErrorSetStream(fpErrorLogging);
	remove(pErrorFilename);
}


TEST(IufReceiveChannelMap, testIufReceiveChannelMapCreate)
{
	const int numChannels = 5;
	int map[5] = { 4,3,2,1,0 };
	iurcm_t obj = iufReceiveChannelMapCreate(numChannels);
	int status = iufReceiveChannelMapSetMap(obj, map);
	TEST_ASSERT(status == IUF_E_OK);
	TEST_ASSERT(obj != IURCM_INVALID);
	iufReceiveChannelMapDelete(obj);
	
	// invalid params
	long filePos = ftell(fpErrorLogging);
	TEST_ASSERT_EQUAL(0,iufErrorGetCount());

	obj = iufReceiveChannelMapCreate(0);
	TEST_ASSERT(obj == IURCM_INVALID);
	obj = iufReceiveChannelMapCreate(-1);
	TEST_ASSERT(obj == IURCM_INVALID);

	TEST_ASSERT_EQUAL(2,iufErrorGetCount());
	TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
}

TEST(IufReceiveChannelMap, testIufReceiveChannelMapDelete)
{
	const int numChannels = 5;

	iurcm_t obj = iufReceiveChannelMapCreate(numChannels);
	TEST_ASSERT(obj != IURCM_INVALID);
	int status = iufReceiveChannelMapDelete(obj);
	TEST_ASSERT_EQUAL(IUF_E_OK, status);

	// invalid params
	status = iufReceiveChannelMapDelete(NULL);
	TEST_ASSERT_EQUAL(IUF_ERR_VALUE, status);
}

TEST(IufReceiveChannelMap, testIufReceiveChannelMapCompare)
{
	IUF_BOOL equal;

	iurcm_t obj = iufReceiveChannelMapCreate(5);
	iurcm_t notherObj = iufReceiveChannelMapCreate(6);
	TEST_ASSERT(obj != IURCM_INVALID);
	TEST_ASSERT(notherObj != IURCM_INVALID);

	equal = iufReceiveChannelMapCompare(obj, obj);
	TEST_ASSERT_EQUAL(IUF_TRUE, equal);
	equal = iufReceiveChannelMapCompare(obj, notherObj);
	TEST_ASSERT_EQUAL(IUF_FALSE, equal);

	// invalid params
	equal = iufReceiveChannelMapCompare(obj, NULL);
	TEST_ASSERT_EQUAL(IUF_FALSE, equal);
	equal = iufReceiveChannelMapCompare(NULL, obj);
	TEST_ASSERT_EQUAL(IUF_FALSE, equal);

	iufReceiveChannelMapDelete(obj);
	iufReceiveChannelMapDelete(notherObj);
}

TEST(IufReceiveChannelMap, testIufReceiveChannelMapSetGet)
{
    int i;
    int status;
	const int numChannels=5;

	iurcm_t obj = iufReceiveChannelMapCreate(numChannels);
	TEST_ASSERT_EQUAL(numChannels, iufReceiveChannelMapGetNumChannels(obj));
	TEST_ASSERT_EQUAL(numChannels,iufReceiveChannelMapGetNumDelays(obj));

	// Delays
	for(i=0;i<numChannels;i++)
	{
		float delay = i*2.0f;
		status = iufReceiveChannelMapSetStartDelay(obj, i, delay);
		TEST_ASSERT(status == IUF_E_OK);
		TEST_ASSERT_EQUAL_FLOAT(delay, iufReceiveChannelMapGetStartDelay(obj, i));
	}

	iufReceiveChannelMapDelete(obj);
}

TEST(IufReceiveChannelMap, testIufSerialization)
{
    char *filename = "testIufReceiveChannelMap.hdf5";
    int numChannels=10;
    int i;
    int status;

    // Create
    iurcm_t obj = iufReceiveChannelMapCreate(numChannels);

    // Delays
    for(i=0;i<numChannels;i++)
    {
        float delay = i*3.14f;
        status = iufReceiveChannelMapSetStartDelay(obj, i, delay);
        TEST_ASSERT(status == IUF_E_OK);
        TEST_ASSERT_EQUAL_FLOAT(delay, iufReceiveChannelMapGetStartDelay(obj, i));
    }

    // save
    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
    status = iufReceiveChannelMapSave(obj, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iurcm_t savedObj = iufReceiveChannelMapLoad(handle);
    TEST_ASSERT(savedObj != NULL);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUF_TRUE, iufReceiveChannelMapCompare(obj, savedObj));
    iufReceiveChannelMapDelete(obj);
    iufReceiveChannelMapDelete(savedObj);
}

TEST_GROUP_RUNNER(IufReceiveChannelMap)
{
	RUN_TEST_CASE(IufReceiveChannelMap, testIufReceiveChannelMapCreate);
	RUN_TEST_CASE(IufReceiveChannelMap, testIufReceiveChannelMapDelete);
	RUN_TEST_CASE(IufReceiveChannelMap, testIufReceiveChannelMapCompare);
	RUN_TEST_CASE(IufReceiveChannelMap, testIufReceiveChannelMapSetGet);
}
