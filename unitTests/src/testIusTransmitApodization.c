//
// Created by nlv12901 on 20/07/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <ius.h>



static char *pErrorFilename = "IusTransmitApodization.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(IusTransmitApodization);

TEST_SETUP(IusTransmitApodization)
{
	iusErrorLogClear();
	iusErrorLog(IUS_TRUE);
	iusErrorAutoReport(IUS_TRUE);
	fpErrorLogging = fopen(pErrorFilename, "w+");
	iusErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IusTransmitApodization)
{
	iusErrorLogClear();
	iusErrorLog(IUS_FALSE);
	if (fpErrorLogging != NULL)
		fclose(fpErrorLogging);
	fpErrorLogging=stderr;
	iusErrorSetStream(fpErrorLogging);
	remove(pErrorFilename);
}

TEST(IusTransmitApodization, testIusTransmitApodizationCreate)
{
	const int numElements = 5;
	float apod[5] = { 0.4f,0.3f,0.2f,0.1f, 0.0f };
	iuta_t obj = iusTransmitApodizationCreate(apod, numElements);
	TEST_ASSERT(obj != IUTA_INVALID);
	iusTransmitApodizationDelete(obj);

	// invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());

	obj = iusTransmitApodizationCreate(apod, 0);
	TEST_ASSERT(obj == IUTA_INVALID);
    obj = iusTransmitApodizationCreate(NULL, numElements);
    TEST_ASSERT(obj == IUTA_INVALID);
	obj = iusTransmitApodizationCreate(apod, -1);
	TEST_ASSERT(obj == IUTA_INVALID);

    TEST_ASSERT_EQUAL(3,iusErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
}

TEST(IusTransmitApodization, testIusTransmitApodizationDelete)
{
	const int numElements = 5;
    float apod[5] = { 0.4f,0.3f,0.2f,0.1f, 0.0f };

	iuta_t obj = iusTransmitApodizationCreate(apod, numElements);
	TEST_ASSERT(obj != IUTA_INVALID);
	int status = iusTransmitApodizationDelete(obj);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);

	// invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());

	status = iusTransmitApodizationDelete(NULL);
	TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);

    TEST_ASSERT_EQUAL(1,iusErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
}

TEST(IusTransmitApodization, testIusTransmitApodizationCompare)
{
	IUS_BOOL equal;

    float apod[5] = { 0.4f,0.3f,0.2f,0.1f, 0.0f };
    float apod2[6] = { 0.4f,0.3f,0.2f,0.1f, 0.0f, -0.1f };
	iuta_t obj = iusTransmitApodizationCreate(apod, 5);
	iuta_t notherObj = iusTransmitApodizationCreate(apod2, 6);

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

TEST(IusTransmitApodization, testIusTransmitApodizationSetGet)
{
	const int numElements = 5;
	int i;
    float apod[5] = { 0.4f,0.3f,0.2f,0.1f, 0.0f };
    iuta_t obj = iusTransmitApodizationCreate(apod, numElements);
    TEST_ASSERT_EQUAL(numElements, iusTransmitApodizationGetNumElements(obj));

    for (i=0; i<numElements;i++)
    {
        TEST_ASSERT_EQUAL_FLOAT(apod[i], iusTransmitApodizationGetElement(obj, i));
        iusTransmitApodizationSetElement(obj, i, 0.1f+apod[i]);
        TEST_ASSERT_EQUAL_FLOAT(apod[i]+0.1, iusTransmitApodizationGetElement(obj, i));
    }

	// invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());

    TEST_ASSERT_EQUAL_FLOAT(NAN, iusTransmitApodizationGetElement(NULL, 0));
    TEST_ASSERT_EQUAL_FLOAT(NAN, iusTransmitApodizationGetElement(obj, -1));
    TEST_ASSERT_EQUAL_FLOAT(NAN, iusTransmitApodizationGetElement(NULL, 1));
    TEST_ASSERT_EQUAL_FLOAT(NAN, iusTransmitApodizationGetElement(obj,-1));
    TEST_ASSERT_EQUAL_FLOAT(NAN, iusTransmitApodizationGetElement(obj, 5));
    TEST_ASSERT_EQUAL(-1, iusTransmitApodizationGetNumElements(NULL));


    TEST_ASSERT_EQUAL(6,iusErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

    iusTransmitApodizationDelete(obj);
}

TEST_GROUP_RUNNER(IusTransmitApodization)
{
	RUN_TEST_CASE(IusTransmitApodization, testIusTransmitApodizationCreate);
	RUN_TEST_CASE(IusTransmitApodization, testIusTransmitApodizationDelete);
	RUN_TEST_CASE(IusTransmitApodization, testIusTransmitApodizationCompare);
	RUN_TEST_CASE(IusTransmitApodization, testIusTransmitApodizationSetGet);
}
