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
#include <iusTransmitApodization.h>

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
	iuta_t obj = iusTransmitApodizationCreate(numElements);
	int status = iusTransmitApodizationSetApodization(obj, apod);
	TEST_ASSERT(status == IUS_E_OK);
	TEST_ASSERT(obj != IUTA_INVALID);
	iusTransmitApodizationDelete(obj);

	// invalid params
	obj = iusTransmitApodizationCreate(0);
	TEST_ASSERT(obj == IUTA_INVALID);
	obj = iusTransmitApodizationCreate(-1);
	TEST_ASSERT(obj == IUTA_INVALID);
}

TEST(IusTransmitApodization, testIusTransmitApodizationDelete)
{
	const int numElements = 5;

	iuta_t obj = iusTransmitApodizationCreate(numElements);
	TEST_ASSERT(obj != IUTA_INVALID);
	int status = iusTransmitApodizationDelete(obj);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);

	// invalid params
	status = iusTransmitApodizationDelete(NULL);
	TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
}

TEST(IusTransmitApodization, testIusTransmitApodizationCompare)
{
	IUS_BOOL equal;

	iuta_t obj = iusTransmitApodizationCreate(5);
	iuta_t notherObj = iusTransmitApodizationCreate(6);
	TEST_ASSERT(obj != IUTA_INVALID);
	TEST_ASSERT(notherObj != IUTA_INVALID);

	equal = iusTransmitApodizationCompare(obj, obj);
	TEST_ASSERT_EQUAL(IUS_TRUE, equal);
	equal = iusTransmitApodizationCompare(obj, notherObj);
	TEST_ASSERT_EQUAL(IUS_FALSE, equal);

	// invalid params
	equal = iusTransmitApodizationCompare(obj, NULL);
	TEST_ASSERT_EQUAL(IUS_FALSE, equal);
	equal = iusTransmitApodizationCompare(NULL, obj);
	TEST_ASSERT_EQUAL(IUS_FALSE, equal);

	iusTransmitApodizationDelete(obj);
	iusTransmitApodizationDelete(notherObj);
}

TEST(IusTransmitApodization, testIusTransmitApodizationGet)
{
	const int numElements = 5;

	iuta_t obj = iusTransmitApodizationCreate(numElements);

	iusTransmitApodizationDelete(obj);
}

TEST_GROUP_RUNNER(IusTransmitApodization)
{
	RUN_TEST_CASE(IusTransmitApodization, testIusTransmitApodizationCreate);
	RUN_TEST_CASE(IusTransmitApodization, testIusTransmitApodizationDelete);
	RUN_TEST_CASE(IusTransmitApodization, testIusTransmitApodizationCompare);
	RUN_TEST_CASE(IusTransmitApodization, testIusTransmitApodizationGet);
	//RUN_TEST_CASE(IusTransmitApodization, testIusTransmitApodizationSerialization);
}
