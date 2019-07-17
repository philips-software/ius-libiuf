#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <iuf.h>



static char *pErrorFilename = "IufTransmitApodization.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(IufTransmitApodization);

TEST_SETUP(IufTransmitApodization)
{
	iufErrorLogClear();
	iufErrorLog(IUF_TRUE);
    iufErrorAutoReportSet(IUF_TRUE);
	fpErrorLogging = fopen(pErrorFilename, "w+");
	iufErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IufTransmitApodization)
{
	iufErrorLogClear();
	iufErrorLog(IUF_FALSE);
	if (fpErrorLogging != NULL)
		fclose(fpErrorLogging);
	fpErrorLogging=stderr;
	iufErrorSetStream(fpErrorLogging);
	remove(pErrorFilename);
}

TEST(IufTransmitApodization, testIufTransmitApodizationCreate)
{
	const int numElements = 5;
	float apod[5] = { 0.4f,0.3f,0.2f,0.1f, 0.0f };
	iuta_t obj = iufTransmitApodizationCreate(apod, numElements);
	TEST_ASSERT(obj != IUTA_INVALID);
	iufTransmitApodizationDelete(obj);

	// invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

	obj = iufTransmitApodizationCreate(apod, 0);
	TEST_ASSERT(obj == IUTA_INVALID);
    obj = iufTransmitApodizationCreate(NULL, numElements);
    TEST_ASSERT(obj == IUTA_INVALID);
	obj = iufTransmitApodizationCreate(apod, -1);
	TEST_ASSERT(obj == IUTA_INVALID);

    TEST_ASSERT_EQUAL(3,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
}

TEST(IufTransmitApodization, testIufTransmitApodizationDelete)
{
	const int numElements = 5;
    float apod[5] = { 0.4f,0.3f,0.2f,0.1f, 0.0f };

	iuta_t obj = iufTransmitApodizationCreate(apod, numElements);
	TEST_ASSERT(obj != IUTA_INVALID);
	int status = iufTransmitApodizationDelete(obj);
	TEST_ASSERT_EQUAL(IUF_E_OK, status);

	// invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

	status = iufTransmitApodizationDelete(NULL);
	TEST_ASSERT_EQUAL(IUF_ERR_VALUE, status);

    TEST_ASSERT_EQUAL(1,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
}

TEST(IufTransmitApodization, testIufTransmitApodizationCompare)
{
	IUF_BOOL equal;

    float apod[5] = { 0.4f,0.3f,0.2f,0.1f, 0.0f };
    float apod2[6] = { 0.4f,0.3f,0.2f,0.1f, 0.0f, -0.1f };
	iuta_t obj = iufTransmitApodizationCreate(apod, 5);
	iuta_t notherObj = iufTransmitApodizationCreate(apod2, 6);

	TEST_ASSERT(obj != IUTA_INVALID);
	TEST_ASSERT(notherObj != IUTA_INVALID);

	equal = iufTransmitApodizationCompare(obj, obj);
	TEST_ASSERT_EQUAL(IUF_TRUE, equal);
	equal = iufTransmitApodizationCompare(obj, notherObj);
	TEST_ASSERT_EQUAL(IUF_FALSE, equal);

	// invalid params
	equal = iufTransmitApodizationCompare(obj, NULL);
	TEST_ASSERT_EQUAL(IUF_FALSE, equal);
	equal = iufTransmitApodizationCompare(NULL, obj);
	TEST_ASSERT_EQUAL(IUF_FALSE, equal);

	iufTransmitApodizationDelete(obj);
	iufTransmitApodizationDelete(notherObj);
}

TEST(IufTransmitApodization, testIufTransmitApodizationSetGet)
{
	const int numElements = 5;
	int i;
    float apod[5] = { 0.4f,0.3f,0.2f,0.1f, 0.0f };
    iuta_t obj = iufTransmitApodizationCreate(apod, numElements);
    TEST_ASSERT_EQUAL(numElements, iufTransmitApodizationGetNumElements(obj));

    for (i=0; i<numElements;i++)
    {
        TEST_ASSERT_EQUAL_FLOAT(apod[i], iufTransmitApodizationGetElement(obj, i));
        iufTransmitApodizationSetElement(obj, i, 0.1f+apod[i]);
        TEST_ASSERT_EQUAL_FLOAT(apod[i]+0.1, iufTransmitApodizationGetElement(obj, i));
    }

	// invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    TEST_ASSERT_EQUAL_FLOAT(NAN, iufTransmitApodizationGetElement(NULL, 0));
    TEST_ASSERT_EQUAL_FLOAT(NAN, iufTransmitApodizationGetElement(obj, -1));
    TEST_ASSERT_EQUAL_FLOAT(NAN, iufTransmitApodizationGetElement(NULL, 1));
    TEST_ASSERT_EQUAL_FLOAT(NAN, iufTransmitApodizationGetElement(obj,-1));
    TEST_ASSERT_EQUAL_FLOAT(NAN, iufTransmitApodizationGetElement(obj, 5));
    TEST_ASSERT_EQUAL(-1, iufTransmitApodizationGetNumElements(NULL));


    TEST_ASSERT_EQUAL(6,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

    iufTransmitApodizationDelete(obj);
}

TEST_GROUP_RUNNER(IufTransmitApodization)
{
	RUN_TEST_CASE(IufTransmitApodization, testIufTransmitApodizationCreate);
	RUN_TEST_CASE(IufTransmitApodization, testIufTransmitApodizationDelete);
	RUN_TEST_CASE(IufTransmitApodization, testIufTransmitApodizationCompare);
	RUN_TEST_CASE(IufTransmitApodization, testIufTransmitApodizationSetGet);
}
