//
// Created by nlv09165 on 08/08/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <ius.h>
#include <iusDemodulationPrivate.h>


static char *pErrorFilename = "IusDemodulation.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(IusDemodulation);

TEST_SETUP(IusDemodulation)
{
    iusErrorLogClear();
    iusErrorLog(IUS_TRUE);
    iusErrorAutoReportSet(IUS_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iusErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IusDemodulation)
{
    iusErrorLogClear();
    iusErrorLog(IUS_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iusErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
}


TEST(IusDemodulation, testIusDemodulationCreate)
{
	float sampleFrequency = 4000;
	int numSamplesPerLine = 10;
	int numTGCentries = 1;
	int kernelSize = 5;
	IusDemodulationMethod method = IUS_DEMODULATION_FOURX;

	iudm_t obj = iusDemodulationCreate(method, sampleFrequency, numSamplesPerLine, numTGCentries, kernelSize);
	iudm_t notherObj = iusDemodulationCreate(method, sampleFrequency, numSamplesPerLine + 10, numTGCentries, kernelSize);
	TEST_ASSERT(obj != IUDM_INVALID);
	TEST_ASSERT(notherObj != IUDM_INVALID);
	iusDemodulationDelete(obj);
	iusDemodulationDelete(notherObj);

	// invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());

    obj = iusDemodulationCreate(method, (float)-1.0, numSamplesPerLine, numTGCentries, kernelSize);
    TEST_ASSERT(obj == IUDM_INVALID);
    obj = iusDemodulationCreate(method, sampleFrequency, -1, numTGCentries, kernelSize);
    TEST_ASSERT(obj == IUDM_INVALID);
    obj = iusDemodulationCreate(method, sampleFrequency, numSamplesPerLine, -1, kernelSize);
    TEST_ASSERT(obj == IUDM_INVALID);
    obj = iusDemodulationCreate(method, sampleFrequency, numSamplesPerLine, numTGCentries, -1);
    TEST_ASSERT(obj == IUDM_INVALID);

    TEST_ASSERT_EQUAL(4,iusErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

}

TEST(IusDemodulation, testIusDemodulationDelete)
{
	float sampleFrequency = 4000;
	int numSamplesPerLine = 10;
	int numTGCentries = 1;
	int kernelSize = 5;
	IusDemodulationMethod method = IUS_DEMODULATION_FOURX;

	iudm_t obj = iusDemodulationCreate(method, sampleFrequency, numSamplesPerLine, numTGCentries, kernelSize);
	TEST_ASSERT(obj != IUDM_INVALID);
	int status = iusDemodulationDelete(obj);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);

	// invalid params

    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());

    status = iusDemodulationDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);

    TEST_ASSERT_EQUAL(1,iusErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
}



TEST(IusDemodulation, testIusDemodulationCompare)
{
	IUS_BOOL equal;
	float sampleFrequency = 4000;
	int numSamplesPerLine = 10;
	int numTGCentries = 1;
	int kernelSize = 5;
	IusDemodulationMethod method = IUS_DEMODULATION_FOURX;

	int status;

	iudm_t obj = iusDemodulationCreate(method, sampleFrequency, numSamplesPerLine, numTGCentries, kernelSize);
	iudm_t notherObj = iusDemodulationCreate(method, sampleFrequency, numSamplesPerLine, numTGCentries, kernelSize);
	iudm_t differentObj = iusDemodulationCreate(method, sampleFrequency, numSamplesPerLine, numTGCentries + 1, kernelSize);
	TEST_ASSERT(obj != IUDM_INVALID);
	TEST_ASSERT(notherObj != IUDM_INVALID);
	equal = iusDemodulationCompare(obj, obj);
	TEST_ASSERT_EQUAL(IUS_TRUE, equal);
	equal = iusDemodulationCompare(obj, notherObj);
	TEST_ASSERT_EQUAL(IUS_TRUE, equal);

	// Get a handle to the TGC ADT stored in the Receive settings
	// and alter it.
	iutgc_t tgc = iusDemodulationGetTGC(obj);
	TEST_ASSERT(tgc != IUTGC_INVALID);
	status = iusTGCSet(tgc, 0, 1.0f, .0f);
	TEST_ASSERT(status == IUS_E_OK);
	// So now obj has been changed...and should not B equal
	equal = iusDemodulationCompare(obj, notherObj);
	TEST_ASSERT_EQUAL(IUS_FALSE, equal);

	tgc = iusDemodulationGetTGC(notherObj);
	TEST_ASSERT(tgc != IUTGC_INVALID);
	status = iusTGCSet(tgc, 0, 1.0f, .0f);
	TEST_ASSERT(status == IUS_E_OK);
	// So now both notherobj and obj have been changed... equal
	equal = iusDemodulationCompare(obj, notherObj);
	TEST_ASSERT_EQUAL(IUS_TRUE, equal);

	equal = iusDemodulationCompare(obj, differentObj);
	TEST_ASSERT_EQUAL(IUS_FALSE, equal);

	// invalid params
	equal = iusDemodulationCompare(obj, NULL);
	TEST_ASSERT_EQUAL(IUS_FALSE, equal);
	equal = iusDemodulationCompare(NULL, obj);
	TEST_ASSERT_EQUAL(IUS_FALSE, equal);

	iusDemodulationDelete(obj);
	iusDemodulationDelete(notherObj);
	iusDemodulationDelete(differentObj);
}


TEST(IusDemodulation, testIusDemodulationSetGet)
{
	float sampleFrequency = 4000;
	int numSamplesPerLine = 10;
	int numTGCentries = 1;
	int kernelSize = 5;
	IusDemodulationMethod method = IUS_DEMODULATION_FOURX;

	// Constructor injected parameters
	iudm_t obj = iusDemodulationCreate(method, sampleFrequency, numSamplesPerLine, numTGCentries, kernelSize);
	TEST_ASSERT_EQUAL_FLOAT(sampleFrequency, iusDemodulationGetSampleFrequency(obj));
	TEST_ASSERT_EQUAL(numSamplesPerLine, iusDemodulationGetNumSamplesPerLine(obj));
	TEST_ASSERT_EQUAL(numTGCentries, iusDemodulationGetNumTGCentries(obj));
	TEST_ASSERT_EQUAL(kernelSize, iusDemodulationGetPreFilterKernelSize(obj));

	// TGC
	iutgc_t tgc = iusDemodulationGetTGC(obj);
	TEST_ASSERT(tgc != IUTGC_INVALID);

	// PreFilter
	iuff_t prefilter = iusDemodulationGetPreFilter(obj);
	TEST_ASSERT(prefilter != IUFIRFILTER_INVALID);

	iusDemodulationDelete(obj);
}

TEST(IusDemodulation, testIusSerialization)
{
	char *filename = "testIusDemodulation.hdf5";
	float sampleFrequency = 4000;
	int numSamplesPerLine = 10;
	int numTGCentries = 2;
	int kernelSize = 5;
	IusDemodulationMethod method = IUS_DEMODULATION_FOURX;

	int i;
	int status;

	// Create
	iudm_t obj = iusDemodulationCreate(method, sampleFrequency, numSamplesPerLine, numTGCentries, kernelSize);

	iutgc_t tgc = iusDemodulationGetTGC(obj);
	for (i = 0; i<numTGCentries; i++)
	{
		status = iusTGCSet(tgc, i, i*1.314f, i*2.314f);
		TEST_ASSERT_EQUAL(IUS_E_OK, status);
	}

	iuff_t preFilter = iusDemodulationGetPreFilter(obj);
	for (i = 0; i<kernelSize; i++)
	{
		status = iusFirFilterSetCoefficient(preFilter, i, 1.0f/(float)kernelSize);
		TEST_ASSERT_EQUAL(IUS_E_OK, status);
	}

	// save
	hid_t handle = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
	TEST_ASSERT(handle > 0);
	status = iusDemodulationSave(obj, handle);
	H5Fclose(handle);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);

	// read back
	handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT);
	iudm_t savedObj = iusDemodulationLoad(handle);
	TEST_ASSERT(savedObj != NULL);
	H5Fclose(handle);

	TEST_ASSERT_EQUAL(IUS_TRUE, iusDemodulationCompare(obj, savedObj));
	iusDemodulationDelete(obj);
	iusDemodulationDelete(savedObj);
}

TEST_GROUP_RUNNER(IusDemodulation)
{
	RUN_TEST_CASE(IusDemodulation, testIusDemodulationCreate);
	RUN_TEST_CASE(IusDemodulation, testIusDemodulationDelete);
	RUN_TEST_CASE(IusDemodulation, testIusDemodulationCompare);
	RUN_TEST_CASE(IusDemodulation, testIusDemodulationSetGet);
	RUN_TEST_CASE(IusDemodulation, testIusSerialization);
}
