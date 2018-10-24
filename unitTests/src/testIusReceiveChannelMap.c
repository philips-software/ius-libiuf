//
// Created by nlv12901 on 20/07/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <ius.h>


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
	iurcm_t obj = iusReceiveChannelMapCreate(numChannels);
	int status = iusReceiveChannelMapSetMap(obj, map);
	TEST_ASSERT(status == IUS_E_OK);
	TEST_ASSERT(obj != IURCM_INVALID);
	iusReceiveChannelMapDelete(obj);
	
	// invalid params
	obj = iusReceiveChannelMapCreate(0);
	TEST_ASSERT(obj == IURCM_INVALID);
	obj = iusReceiveChannelMapCreate(-1);
	TEST_ASSERT(obj == IURCM_INVALID);
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

TEST(IusReceiveChannelMap, testIusReceiveChannelMapGet)
{
	const int numChannels=5;

	iurcm_t obj = iusReceiveChannelMapCreate(numChannels);
	TEST_ASSERT_EQUAL(numChannels, iusReceiveChannelMapGetNumChannels(obj));
	iusReceiveChannelMapDelete(obj);
}

TEST_GROUP_RUNNER(IusReceiveChannelMap)
{
	RUN_TEST_CASE(IusReceiveChannelMap, testIusReceiveChannelMapCreate);
	RUN_TEST_CASE(IusReceiveChannelMap, testIusReceiveChannelMapDelete);
	RUN_TEST_CASE(IusReceiveChannelMap, testIusReceiveChannelMapCompare);
	RUN_TEST_CASE(IusReceiveChannelMap, testIusReceiveChannelMapGet);
}
