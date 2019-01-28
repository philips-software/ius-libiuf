//
// Created by nlv09165 on 08/08/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <ius.h>
#include <iusDemodulationPrivate.h>

TEST_GROUP(IusDemodulation);

TEST_SETUP(IusDemodulation)
{
}

TEST_TEAR_DOWN(IusDemodulation)
{
}


TEST(IusDemodulation, testIusDemodulationCreate)
{
	float sampleFrequency = 4000;
	int numSamplesPerLine = 10;
	int numTGCentries = 1;
	iudm_t obj = iusDemodulationCreate(sampleFrequency, numSamplesPerLine, numTGCentries);
	iudm_t notherObj = iusDemodulationCreate(sampleFrequency, numSamplesPerLine + 10, numTGCentries);
	TEST_ASSERT(obj != IUDM_INVALID);
	TEST_ASSERT(notherObj != IUDM_INVALID);
	iusDemodulationDelete(obj);
	iusDemodulationDelete(notherObj);

	// invalid params
	obj = iusDemodulationCreate((float)-1.0, numSamplesPerLine, numTGCentries);
	TEST_ASSERT(obj == IUDM_INVALID);
	obj = iusDemodulationCreate(sampleFrequency, -1, numTGCentries);
	TEST_ASSERT(obj == IUDM_INVALID);
	obj = iusDemodulationCreate(sampleFrequency, numSamplesPerLine, -1);
	TEST_ASSERT(obj == IUDM_INVALID);
}

TEST(IusDemodulation, testIusDemodulationDelete)
{
	float sampleFrequency = 4000;
	int numSamplesPerLine = 10;
	int numTGCentries = 1;
	iudm_t obj = iusDemodulationCreate(sampleFrequency, numSamplesPerLine, numTGCentries);
	TEST_ASSERT(obj != IUDM_INVALID);
	int status = iusDemodulationDelete(obj);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);

	// invalid params
	status = iusDemodulationDelete(NULL);
	TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
}



TEST(IusDemodulation, testIusDemodulationCompare)
{
	IUS_BOOL equal;
	float sampleFrequency = 4000;
	int numSamplesPerLine = 10;
	int numTGCentries = 1;
	int status;

	iudm_t obj = iusDemodulationCreate(sampleFrequency, numSamplesPerLine, numTGCentries);
	iudm_t notherObj = iusDemodulationCreate(sampleFrequency, numSamplesPerLine, numTGCentries);
	iudm_t differentObj = iusDemodulationCreate(sampleFrequency, numSamplesPerLine, numTGCentries + 1);
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

	// Constructor injected parameters
	iudm_t obj = iusDemodulationCreate(sampleFrequency, numSamplesPerLine, numTGCentries);
	TEST_ASSERT_EQUAL_FLOAT(sampleFrequency, iusDemodulationGetSampleFrequency(obj));
	TEST_ASSERT_EQUAL(numSamplesPerLine, iusDemodulationGetNumSamplesPerLine(obj));
	TEST_ASSERT_EQUAL(numTGCentries, iusDemodulationGetNumTGCentries(obj));

	// TGC
	iutgc_t tgc = iusDemodulationGetTGC(obj);
	TEST_ASSERT(tgc != IUTGC_INVALID);

	iusDemodulationDelete(obj);
}

TEST(IusDemodulation, testIusSerialization)
{
	char *filename = "testIusDemodulation.hdf5";
	float sampleFrequency = 4000;
	int numSamplesPerLine = 10;
	int numTGCentries = 2;
	int i;
	int status;

	// Create
	iudm_t obj = iusDemodulationCreate(sampleFrequency, numSamplesPerLine, numTGCentries);

	iutgc_t tgc = iusDemodulationGetTGC(obj);
	for (i = 0; i<numTGCentries; i++)
	{
		status = iusTGCSet(tgc, i, i*1.314f, i*2.314f);
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
