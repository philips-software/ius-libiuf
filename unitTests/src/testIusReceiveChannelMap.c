//
// Created by nlv12901 on 20/07/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <ius.h>
#include <iusHDF5.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusHLReceiveChannelMap.h>
#include <iusHLReceiveChannelMapDict.h>

TEST_GROUP(IusReceiveChannelMap);

TEST_SETUP(IusReceiveChannelMap)
{
}

TEST_TEAR_DOWN(IusReceiveChannelMap)
{
}


TEST(IusReceiveChannelMap, testIusReceiveChannelMapCreate)
{
	const int numChannels = 5;
	int map[5] = { 4,3,2,1,0 };
	iurcm_t obj = iusHLReceiveChannelMapCreate(numChannels);
	int status = iusHLReceiveChannelMapSetMap(obj, map);
	TEST_ASSERT(status == IUS_E_OK);
	TEST_ASSERT(obj != IURCM_INVALID);
	iusHLReceiveChannelMapDelete(obj);
	
	// invalid params
	obj = iusHLReceiveChannelMapCreate(0);
	TEST_ASSERT(obj == IURCM_INVALID);
	obj = iusHLReceiveChannelMapCreate(-1);
	TEST_ASSERT(obj == IURCM_INVALID);
}

TEST(IusReceiveChannelMap, testIusReceiveChannelMapDelete)
{
	const int numChannels = 5;

	iurcm_t obj = iusHLReceiveChannelMapCreate(numChannels);
	TEST_ASSERT(obj != IURCM_INVALID);
	int status = iusHLReceiveChannelMapDelete(obj);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);

	// invalid params
	status = iusHLReceiveChannelMapDelete(NULL);
	TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
}

TEST(IusReceiveChannelMap, testIusReceiveChannelMapCompare)
{
	IUS_BOOL equal;

	iurcm_t obj = iusHLReceiveChannelMapCreate(5);
	iurcm_t notherObj = iusHLReceiveChannelMapCreate(6);
	TEST_ASSERT(obj != IURCM_INVALID);
	TEST_ASSERT(notherObj != IURCM_INVALID);

	equal = iusHLReceiveChannelMapCompare(obj, obj);
	TEST_ASSERT_EQUAL(IUS_TRUE, equal);
	equal = iusHLReceiveChannelMapCompare(obj, notherObj);
	TEST_ASSERT_EQUAL(IUS_FALSE, equal);

	// invalid params
	equal = iusHLReceiveChannelMapCompare(obj, NULL);
	TEST_ASSERT_EQUAL(IUS_FALSE, equal);
	equal = iusHLReceiveChannelMapCompare(NULL, obj);
	TEST_ASSERT_EQUAL(IUS_FALSE, equal);

	iusHLReceiveChannelMapDelete(obj);
	iusHLReceiveChannelMapDelete(notherObj);
}

TEST(IusReceiveChannelMap, testIusReceiveChannelMapGet)
{
	const int numChannels=5;

	iurcm_t obj = iusHLReceiveChannelMapCreate(numChannels);

	iusHLReceiveChannelMapDelete(obj);
}

TEST_GROUP_RUNNER(IusReceiveChannelMap)
{
	RUN_TEST_CASE(IusReceiveChannelMap, testIusReceiveChannelMapCreate);
	RUN_TEST_CASE(IusReceiveChannelMap, testIusReceiveChannelMapDelete);
	RUN_TEST_CASE(IusReceiveChannelMap, testIusReceiveChannelMapCompare);
	RUN_TEST_CASE(IusReceiveChannelMap, testIusReceiveChannelMapGet);
	//RUN_TEST_CASE(IusReceiveChannelMap, testIusReceiveChannelMapSerialization);
}
