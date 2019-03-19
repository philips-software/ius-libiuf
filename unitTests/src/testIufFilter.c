//
// Created by nlv23902 on 26/02/2019.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <iuf.h>
#include <iufFilterPrivate.h>

static char *pErrorFilename = "IufFirFilter.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(IufFirFilter);

TEST_SETUP(IufFirFilter)
{
    iufErrorLogClear();
    iufErrorLog(IUF_TRUE);
    iufErrorAutoReportSet(IUF_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iufErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IufFirFilter)
{
    iufErrorLogClear();
    iufErrorLog(IUF_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iufErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
}


TEST(IufFirFilter, testIufFirFilterCreate)
{
	int status = 0;
	int kernelSize;
	iuff_t firFilter;

	kernelSize = 5;
	firFilter = iufFirFilterCreate(kernelSize);
	TEST_ASSERT(firFilter != IUFIRFILTER_INVALID);

	// Invalid operation on nonparametric dta type
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    TEST_ASSERT_EQUAL(NULL, iufFirFilterCreate(-1));

    TEST_ASSERT_EQUAL(1,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

	status = iufFirFilterDelete(firFilter);
	TEST_ASSERT(status == IUF_E_OK);
}

TEST(IufFirFilter, testIufFirFilterDelete)
{
	int kernelSize = 7;
	iuff_t obj = iufFirFilterCreate(kernelSize);
	TEST_ASSERT(obj != IUFIRFILTER_INVALID);
	int status = iufFirFilterDelete(obj);
	TEST_ASSERT_EQUAL(IUF_E_OK, status);

	// invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    status = iufFirFilterDelete(NULL);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, status);

    TEST_ASSERT_EQUAL(1,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
}



TEST(IufFirFilter, testIufFirFilterCompare)
{
	IUF_BOOL equal;
	int kernelSize = 5;
	iuff_t obj = iufFirFilterCreate(kernelSize);
	iuff_t sameObj = iufFirFilterCreate(kernelSize);
	iuff_t notherObj = iufFirFilterCreate(kernelSize + 10);
	TEST_ASSERT(obj != IUFIRFILTER_INVALID);
	TEST_ASSERT(notherObj != IUFIRFILTER_INVALID);

	equal = iufFirFilterCompare(obj, obj);
	TEST_ASSERT_EQUAL(IUF_TRUE, equal);
	equal = iufFirFilterCompare(obj, sameObj);
	TEST_ASSERT_EQUAL(IUF_TRUE, equal);
	equal = iufFirFilterCompare(obj, notherObj);
	TEST_ASSERT_EQUAL(IUF_FALSE, equal);

	int status = iufFirFilterSetCoefficient(obj, 0, 1.0f);
	TEST_ASSERT_EQUAL(IUF_E_OK, status);
	equal = iufFirFilterCompare(obj, sameObj);
	TEST_ASSERT_EQUAL(IUF_FALSE, equal);
	status = iufFirFilterSetCoefficient(sameObj, 0, 1.0f);
	TEST_ASSERT_EQUAL(IUF_E_OK, status);
	equal = iufFirFilterCompare(obj, sameObj);
	TEST_ASSERT_EQUAL(IUF_TRUE, equal);

	// invalid params
	equal = iufFirFilterCompare(obj, NULL);
	TEST_ASSERT_EQUAL(IUF_FALSE, equal);
	equal = iufFirFilterCompare(NULL, obj);
	TEST_ASSERT_EQUAL(IUF_FALSE, equal);

	iufFirFilterDelete(obj);
	iufFirFilterDelete(sameObj);
	iufFirFilterDelete(notherObj);
}


TEST(IufFirFilter, testIufFirFilterSetGet)
{
	int kernelSize = 5;
	iuff_t obj = iufFirFilterCreate(kernelSize);
	TEST_ASSERT(obj != IUFIRFILTER_INVALID);

	iufFirFilterSetCoefficient(obj, 1, 1.0f);
	// float param
	TEST_ASSERT_EQUAL_FLOAT(1.0f, iufFirFilterGetCoefficient(obj, 1));


	// invalid param
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    TEST_ASSERT_EQUAL_FLOAT(NAN, iufFirFilterGetCoefficient(NULL, 1));
    TEST_ASSERT_EQUAL_FLOAT(NAN, iufFirFilterGetCoefficient(obj, kernelSize)); //out of bounds

    TEST_ASSERT_EQUAL(2,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
	iufFirFilterDelete(obj);
}


TEST(IufFirFilter, testIufSerialization)
{
	char *filename = "testIufFirFilterSerialization.hdf5";
	int kernelSize = 5;

	// create
	iuff_t firFilter = iufFirFilterCreate(kernelSize);

	// fill
	int i, status;
	for (i = 0; i<kernelSize; i++)
	{
		status = iufFirFilterSetCoefficient(firFilter, i, i*1.0f);
		TEST_ASSERT_EQUAL(IUF_E_OK, status);
	}

	// save
	hid_t handle = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
	TEST_ASSERT(handle > 0);
	status = iufFirFilterSave(firFilter, handle);
	H5Fclose(handle);
	TEST_ASSERT_EQUAL(IUF_E_OK, status);

	// read back
	handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT);
	iuff_t savedObj = iufFirFilterLoad(handle);
	TEST_ASSERT(savedObj != NULL);
	H5Fclose(handle);

	TEST_ASSERT_EQUAL(IUF_TRUE, iufFirFilterCompare(firFilter, savedObj));
	iufFirFilterDelete(firFilter);
	iufFirFilterDelete(savedObj);
}

TEST_GROUP_RUNNER(IufFirFilter)
{
	RUN_TEST_CASE(IufFirFilter, testIufFirFilterCreate);
	RUN_TEST_CASE(IufFirFilter, testIufFirFilterDelete);
	RUN_TEST_CASE(IufFirFilter, testIufFirFilterCompare);
	RUN_TEST_CASE(IufFirFilter, testIufFirFilterSetGet);
	RUN_TEST_CASE(IufFirFilter, testIufSerialization);
}
