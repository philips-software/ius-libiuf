#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <iuf.h>
#include <iufDemodulationPrivate.h>


static char *pErrorFilename = "IufDemodulation.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(IufDemodulation);

TEST_SETUP(IufDemodulation)
{
    iufErrorLogClear();
    iufErrorLog(IUF_TRUE);
    iufErrorAutoReportSet(IUF_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iufErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IufDemodulation)
{
    iufErrorLogClear();
    iufErrorLog(IUF_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iufErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
}


TEST(IufDemodulation, testIufDemodulationCreate)
{
	float sampleFrequency = 4000;
	float centerFrequency = 2000;
	int numSamplesPerLine = 10;
	int numTGCentries = 1;
	int kernelSize = 5;
	IufDemodulationMethod method = IUF_DEMODULATION_FOURX;

	iudm_t obj = iufDemodulationCreate(method, sampleFrequency, centerFrequency, numSamplesPerLine, numTGCentries, kernelSize);
	iudm_t notherObj = iufDemodulationCreate(method, sampleFrequency, centerFrequency, numSamplesPerLine + 10, numTGCentries, kernelSize);
	TEST_ASSERT(obj != IUDM_INVALID);
	TEST_ASSERT(notherObj != IUDM_INVALID);
	iufDemodulationDelete(obj);
	iufDemodulationDelete(notherObj);

	// invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    obj = iufDemodulationCreate(method, (float)-1.0, centerFrequency, numSamplesPerLine, numTGCentries, kernelSize);
    TEST_ASSERT(obj == IUDM_INVALID);
    obj = iufDemodulationCreate(method, sampleFrequency, (float)-1.0, numSamplesPerLine, numTGCentries, kernelSize);
    TEST_ASSERT(obj == IUDM_INVALID);
    obj = iufDemodulationCreate(method, sampleFrequency, centerFrequency, -1, numTGCentries, kernelSize);
    TEST_ASSERT(obj == IUDM_INVALID);
    obj = iufDemodulationCreate(method, sampleFrequency, centerFrequency, numSamplesPerLine, -1, kernelSize);
    TEST_ASSERT(obj == IUDM_INVALID);
    obj = iufDemodulationCreate(method, sampleFrequency, centerFrequency, numSamplesPerLine, numTGCentries, -1);
    TEST_ASSERT(obj == IUDM_INVALID);

    TEST_ASSERT_EQUAL(5,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

}

TEST(IufDemodulation, testIufDemodulationDelete)
{
	float sampleFrequency = 4000;
	float centerFrequency = 2000;
	int numSamplesPerLine = 10;
	int numTGCentries = 1;
	int kernelSize = 5;
	IufDemodulationMethod method = IUF_DEMODULATION_FOURX;

	iudm_t obj = iufDemodulationCreate(method, sampleFrequency, centerFrequency, numSamplesPerLine, numTGCentries, kernelSize);
	TEST_ASSERT(obj != IUDM_INVALID);
	int status = iufDemodulationDelete(obj);
	TEST_ASSERT_EQUAL(IUF_E_OK, status);

	// invalid params

    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    status = iufDemodulationDelete(NULL);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, status);

    TEST_ASSERT_EQUAL(1,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
}



TEST(IufDemodulation, testIufDemodulationCompare)
{
	IUF_BOOL equal;
	float sampleFrequency = 4000;
	float centerFrequency = 2000;
	int numSamplesPerLine = 10;
	int numTGCentries = 1;
	int kernelSize = 5;
	IufDemodulationMethod method = IUF_DEMODULATION_FOURX;

	int status;

	iudm_t obj = iufDemodulationCreate(method, sampleFrequency, centerFrequency, numSamplesPerLine, numTGCentries, kernelSize);
	iudm_t notherObj = iufDemodulationCreate(method, sampleFrequency, centerFrequency, numSamplesPerLine, numTGCentries, kernelSize);
	iudm_t differentObj = iufDemodulationCreate(method, sampleFrequency, centerFrequency, numSamplesPerLine, numTGCentries + 1, kernelSize);
	TEST_ASSERT(obj != IUDM_INVALID);
	TEST_ASSERT(notherObj != IUDM_INVALID);
	equal = iufDemodulationCompare(obj, obj);
	TEST_ASSERT_EQUAL(IUF_TRUE, equal);
	equal = iufDemodulationCompare(obj, notherObj);
	TEST_ASSERT_EQUAL(IUF_TRUE, equal);

	// Get a handle to the TGC ADT stored in the Receive settings
	// and alter it.
	iutgc_t tgc = iufDemodulationGetTGC(obj);
	TEST_ASSERT(tgc != IUTGC_INVALID);
	status = iufTGCSet(tgc, 0, 1.0f, .0f);
	TEST_ASSERT(status == IUF_E_OK);
	// So now obj has been changed...and should not B equal
	equal = iufDemodulationCompare(obj, notherObj);
	TEST_ASSERT_EQUAL(IUF_FALSE, equal);

	tgc = iufDemodulationGetTGC(notherObj);
	TEST_ASSERT(tgc != IUTGC_INVALID);
	status = iufTGCSet(tgc, 0, 1.0f, .0f);
	TEST_ASSERT(status == IUF_E_OK);
	// So now both notherobj and obj have been changed... equal
	equal = iufDemodulationCompare(obj, notherObj);
	TEST_ASSERT_EQUAL(IUF_TRUE, equal);

	equal = iufDemodulationCompare(obj, differentObj);
	TEST_ASSERT_EQUAL(IUF_FALSE, equal);

	// invalid params
	equal = iufDemodulationCompare(obj, NULL);
	TEST_ASSERT_EQUAL(IUF_FALSE, equal);
	equal = iufDemodulationCompare(NULL, obj);
	TEST_ASSERT_EQUAL(IUF_FALSE, equal);

	iufDemodulationDelete(obj);
	iufDemodulationDelete(notherObj);
	iufDemodulationDelete(differentObj);
}


TEST(IufDemodulation, testIufDemodulationSetGet)
{
	float sampleFrequency = 4000;
	float centerFrequency = 2000;
	int numSamplesPerLine = 10;
	int numTGCentries = 1;
	int kernelSize = 5;
	IufDemodulationMethod method = IUF_DEMODULATION_FOURX;

	// Constructor injected parameters
	iudm_t obj = iufDemodulationCreate(method, sampleFrequency, centerFrequency, numSamplesPerLine, numTGCentries, kernelSize);
	TEST_ASSERT_EQUAL_FLOAT(sampleFrequency, iufDemodulationGetSampleFrequency(obj));
	TEST_ASSERT_EQUAL_FLOAT(centerFrequency, iufDemodulationGetCenterFrequency(obj));
	TEST_ASSERT_EQUAL(numSamplesPerLine, iufDemodulationGetNumSamplesPerLine(obj));
	TEST_ASSERT_EQUAL(numTGCentries, iufDemodulationGetNumTGCentries(obj));
	TEST_ASSERT_EQUAL(kernelSize, iufDemodulationGetPreFilterKernelSize(obj));

	// TGC
	iutgc_t tgc = iufDemodulationGetTGC(obj);
	TEST_ASSERT(tgc != IUTGC_INVALID);

	// PreFilter
	iuff_t prefilter = iufDemodulationGetPreFilter(obj);
	TEST_ASSERT(prefilter != IUFIRFILTER_INVALID);

	iufDemodulationDelete(obj);
}

TEST(IufDemodulation, testIufSerialization)
{
	char *filename = "testIufDemodulation.hdf5";
	float sampleFrequency = 4000;
	float centerFrequency = 2000;
	int numSamplesPerLine = 10;
	int numTGCentries = 2;
	int kernelSize = 5;
	IufDemodulationMethod method = IUF_DEMODULATION_FOURX;

	int i;
	int status;

	// Create
	iudm_t obj = iufDemodulationCreate(method, sampleFrequency, centerFrequency, numSamplesPerLine, numTGCentries, kernelSize);

	iutgc_t tgc = iufDemodulationGetTGC(obj);
	for (i = 0; i<numTGCentries; i++)
	{
		status = iufTGCSet(tgc, i, i*1.314f, i*2.314f);
		TEST_ASSERT_EQUAL(IUF_E_OK, status);
	}

	iuff_t preFilter = iufDemodulationGetPreFilter(obj);
	for (i = 0; i<kernelSize; i++)
	{
		status = iufFirFilterSetCoefficient(preFilter, i, 1.0f/(float)kernelSize);
		TEST_ASSERT_EQUAL(IUF_E_OK, status);
	}

	// save
	hid_t handle = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
	TEST_ASSERT(handle > 0);
	status = iufDemodulationSave(obj, handle);
	H5Fclose(handle);
	TEST_ASSERT_EQUAL(IUF_E_OK, status);

	// read back
	handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT);
	iudm_t savedObj = iufDemodulationLoad(handle);
	TEST_ASSERT(savedObj != NULL);
	H5Fclose(handle);

	TEST_ASSERT_EQUAL(IUF_TRUE, iufDemodulationCompare(obj, savedObj));
	iufDemodulationDelete(obj);
	iufDemodulationDelete(savedObj);
}

TEST_GROUP_RUNNER(IufDemodulation)
{
	RUN_TEST_CASE(IufDemodulation, testIufDemodulationCreate);
	RUN_TEST_CASE(IufDemodulation, testIufDemodulationDelete);
	RUN_TEST_CASE(IufDemodulation, testIufDemodulationCompare);
	RUN_TEST_CASE(IufDemodulation, testIufDemodulationSetGet);
	RUN_TEST_CASE(IufDemodulation, testIufSerialization);
}
