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
#include <iusHLTransmitApodization.h>

TEST_GROUP(IusTransmitApodization);

TEST_SETUP(IusTransmitApodization)
{
}

TEST_TEAR_DOWN(IusTransmitApodization)
{
}


TEST(IusTransmitApodization, testIusTransmitApodizationCreate)
{
	const int numElements = 5;
	float apod[5] = { 0.4f,0.3f,0.2f,0.1f, 0.0f };
	iuta_t obj = iusHLTransmitApodizationCreate(numElements);
	int status = iusHLTransmitApodizationSetApodization(obj, apod);
	TEST_ASSERT(status == IUS_E_OK);
	TEST_ASSERT(obj != IUTA_INVALID);
	iusHLTransmitApodizationDelete(obj);

	// invalid params
	obj = iusHLTransmitApodizationCreate(0);
	TEST_ASSERT(obj == IUTA_INVALID);
	obj = iusHLTransmitApodizationCreate(-1);
	TEST_ASSERT(obj == IUTA_INVALID);
}

TEST(IusTransmitApodization, testIusTransmitApodizationDelete)
{
	const int numElements = 5;

	iuta_t obj = iusHLTransmitApodizationCreate(numElements);
	TEST_ASSERT(obj != IUTA_INVALID);
	int status = iusHLTransmitApodizationDelete(obj);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);

	// invalid params
	status = iusHLTransmitApodizationDelete(NULL);
	TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
}

TEST(IusTransmitApodization, testIusTransmitApodizationCompare)
{
	IUS_BOOL equal;

	iuta_t obj = iusHLTransmitApodizationCreate(5);
	iuta_t notherObj = iusHLTransmitApodizationCreate(6);
	TEST_ASSERT(obj != IUTA_INVALID);
	TEST_ASSERT(notherObj != IUTA_INVALID);

	equal = iusHLTransmitApodizationCompare(obj, obj);
	TEST_ASSERT_EQUAL(IUS_TRUE, equal);
	equal = iusHLTransmitApodizationCompare(obj, notherObj);
	TEST_ASSERT_EQUAL(IUS_FALSE, equal);

	// invalid params
	equal = iusHLTransmitApodizationCompare(obj, NULL);
	TEST_ASSERT_EQUAL(IUS_FALSE, equal);
	equal = iusHLTransmitApodizationCompare(NULL, obj);
	TEST_ASSERT_EQUAL(IUS_FALSE, equal);

	iusHLTransmitApodizationDelete(obj);
	iusHLTransmitApodizationDelete(notherObj);
}

TEST(IusTransmitApodization, testIusTransmitApodizationGet)
{
	const int numElements = 5;

	iuta_t obj = iusHLTransmitApodizationCreate(numElements);

	iusHLTransmitApodizationDelete(obj);
}

TEST_GROUP_RUNNER(IusTransmitApodization)
{
	RUN_TEST_CASE(IusTransmitApodization, testIusTransmitApodizationCreate);
	RUN_TEST_CASE(IusTransmitApodization, testIusTransmitApodizationDelete);
	RUN_TEST_CASE(IusTransmitApodization, testIusTransmitApodizationCompare);
	RUN_TEST_CASE(IusTransmitApodization, testIusTransmitApodizationGet);
	//RUN_TEST_CASE(IusTransmitApodization, testIusTransmitApodizationSerialization);
}
