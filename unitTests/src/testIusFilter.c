//
// Created by nlv23902 on 26/02/2019.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <ius.h>
#include <iusFilterPrivate.h>

static char *pErrorFilename = "IusFirFilter.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(IusFirFilter);

TEST_SETUP(IusFirFilter)
{
    iusErrorLogClear();
    iusErrorLog(IUS_TRUE);
    iusErrorAutoReport(IUS_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iusErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IusFirFilter)
{
    iusErrorLogClear();
    iusErrorLog(IUS_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iusErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
}


TEST(IusFirFilter, testIusFirFilterCreate)
{
	int status = 0;
	int kernelSize;
	iuff_t firFilter;

	kernelSize = 5;
	firFilter = iusFirFilterCreate(kernelSize);
	TEST_ASSERT(firFilter != IUFIRFILTER_INVALID);

	// Invalid operation on nonparametric dta type
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());

    TEST_ASSERT_EQUAL(NULL, iusFirFilterCreate(-1));

    TEST_ASSERT_EQUAL(1,iusErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

	status = iusFirFilterDelete(firFilter);
	TEST_ASSERT(status == IUS_E_OK);
}

TEST(IusFirFilter, testIusFirFilterDelete)
{
	int kernelSize = 7;
	iuff_t obj = iusFirFilterCreate(kernelSize);
	TEST_ASSERT(obj != IUFIRFILTER_INVALID);
	int status = iusFirFilterDelete(obj);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);

	// invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());

    status = iusFirFilterDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);

    TEST_ASSERT_EQUAL(1,iusErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
}



TEST(IusFirFilter, testIusFirFilterCompare)
{
	IUS_BOOL equal;
	int kernelSize = 5;
	iuff_t obj = iusFirFilterCreate(kernelSize);
	iuff_t sameObj = iusFirFilterCreate(kernelSize);
	iuff_t notherObj = iusFirFilterCreate(kernelSize + 10);
	TEST_ASSERT(obj != IUFIRFILTER_INVALID);
	TEST_ASSERT(notherObj != IUFIRFILTER_INVALID);

	equal = iusFirFilterCompare(obj, obj);
	TEST_ASSERT_EQUAL(IUS_TRUE, equal);
	equal = iusFirFilterCompare(obj, sameObj);
	TEST_ASSERT_EQUAL(IUS_TRUE, equal);
	equal = iusFirFilterCompare(obj, notherObj);
	TEST_ASSERT_EQUAL(IUS_FALSE, equal);

	int status = iusFirFilterSetCoefficient(obj, 0, 1.0f);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);
	equal = iusFirFilterCompare(obj, sameObj);
	TEST_ASSERT_EQUAL(IUS_FALSE, equal);
	status = iusFirFilterSetCoefficient(sameObj, 0, 1.0f);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);
	equal = iusFirFilterCompare(obj, sameObj);
	TEST_ASSERT_EQUAL(IUS_TRUE, equal);

	// invalid params
	equal = iusFirFilterCompare(obj, NULL);
	TEST_ASSERT_EQUAL(IUS_FALSE, equal);
	equal = iusFirFilterCompare(NULL, obj);
	TEST_ASSERT_EQUAL(IUS_FALSE, equal);

	iusFirFilterDelete(obj);
	iusFirFilterDelete(sameObj);
	iusFirFilterDelete(notherObj);
}


TEST(IusFirFilter, testIusFirFilterSetGet)
{
	int kernelSize = 5;
	iuff_t obj = iusFirFilterCreate(kernelSize);
	TEST_ASSERT(obj != IUFIRFILTER_INVALID);

	iusFirFilterSetCoefficient(obj, 1, 1.0f);
	// float param
	TEST_ASSERT_EQUAL_FLOAT(1.0f, iusFirFilterGetCoefficient(obj, 1));


	// invalid param
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());

    TEST_ASSERT_EQUAL_FLOAT(NAN, iusFirFilterGetCoefficient(NULL, 1));
    TEST_ASSERT_EQUAL_FLOAT(NAN, iusFirFilterGetCoefficient(obj, kernelSize)); //out of bounds

    TEST_ASSERT_EQUAL(2,iusErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
	iusFirFilterDelete(obj);
}


TEST(IusFirFilter, testIusSerialization)
{
	char *filename = "testIusFirFilterSerialization.hdf5";
	int kernelSize = 5;

	// create
	iuff_t firFilter = iusFirFilterCreate(kernelSize);

	// fill
	int i, status;
	for (i = 0; i<kernelSize; i++)
	{
		status = iusFirFilterSetCoefficient(firFilter, i, i*1.0f);
		TEST_ASSERT_EQUAL(IUS_E_OK, status);
	}

	// save
	hid_t handle = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
	TEST_ASSERT(handle > 0);
	status = iusFirFilterSave(firFilter, handle);
	H5Fclose(handle);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);

	// read back
	handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT);
	iuff_t savedObj = iusFirFilterLoad(handle);
	TEST_ASSERT(savedObj != NULL);
	H5Fclose(handle);

	TEST_ASSERT_EQUAL(IUS_TRUE, iusFirFilterCompare(firFilter, savedObj));
	iusFirFilterDelete(firFilter);
	iusFirFilterDelete(savedObj);
}

TEST_GROUP_RUNNER(IusFirFilter)
{
	RUN_TEST_CASE(IusFirFilter, testIusFirFilterCreate);
	RUN_TEST_CASE(IusFirFilter, testIusFirFilterDelete);
	RUN_TEST_CASE(IusFirFilter, testIusFirFilterCompare);
	RUN_TEST_CASE(IusFirFilter, testIusFirFilterSetGet);
	RUN_TEST_CASE(IusFirFilter, testIusSerialization);
}
