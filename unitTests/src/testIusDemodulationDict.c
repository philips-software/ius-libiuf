//
// Created by nlv12901 on 15/01/2019.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <ius.h>
#include <iusDemodulationDictPrivate.h>
#include <testDataGenerators.h>

TEST_GROUP(IusDemodulationDict);

TEST_SETUP(IusDemodulationDict)
{
}

TEST_TEAR_DOWN(IusDemodulationDict)
{
}


TEST(IusDemodulationDict, testIusCreateDict)
{
	iudmd_t obj = iusDemodulationDictCreate();
	iudmd_t notherObj = iusDemodulationDictCreate();

	TEST_ASSERT(obj != IUDMD_INVALID);
	TEST_ASSERT(notherObj != IUDMD_INVALID);
	iusDemodulationDictDelete(obj);
	iusDemodulationDictDelete(notherObj);
}


TEST(IusDemodulationDict, testIusSetGetDict)
{
	char *pObjLabel = "bmode";
	float sampleFrequency = 4000;
	int numSamplesPerLine = 10;
	int numTGCentries = 1;
	int status;

	iudm_t obj = iusDemodulationCreate(sampleFrequency, numSamplesPerLine, numTGCentries);


	// Create
	iudmd_t  dict = iusDemodulationDictCreate();

	// Fill
	status = iusDemodulationDictSet(dict, pObjLabel, obj);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);
	iudm_t retrievedObj = iusDemodulationDictGet(dict, pObjLabel);
	TEST_ASSERT_EQUAL(IUS_TRUE, iusDemodulationCompare(obj, retrievedObj));

	// Invalid params
	TEST_ASSERT_EQUAL(IURS_INVALID, iusDemodulationDictGet(dict, NULL));
	TEST_ASSERT_EQUAL(IURS_INVALID, iusDemodulationDictGet(NULL, pObjLabel));
	TEST_ASSERT_EQUAL(IURS_INVALID, iusDemodulationDictGet(dict, "unknownLabel"));
	iusDemodulationDelete(obj);
	iusDemodulationDictDelete(dict);
}

TEST(IusDemodulationDict, testIusCompareDict)
{
	IUS_BOOL equal;
	char *pObjLabel = "Label for IusDemodulationDict, created in testIusCompareSourceDict";
	char *pNotherObjLabel = "Another Label for IusDemodulationDict, created in testIusCompareSourceDict";
	char *pDifferentLabel = "Different Label for IusDemodulationDict, created in testIusCompareSourceDict";
	float sampleFrequency = 4000;
	int numSamplesPerLine = 10;
	int numTGCentries = 1;
	int status;

	iudm_t obj = iusDemodulationCreate(sampleFrequency, numSamplesPerLine, numTGCentries);
	iudm_t notherObj = iusDemodulationCreate(sampleFrequency, numSamplesPerLine, numTGCentries);
	iudm_t differentObj = iusDemodulationCreate(sampleFrequency, numSamplesPerLine, numTGCentries + 1);

	// Create
	iudmd_t  dict = iusDemodulationDictCreate();
	iudmd_t  notherDict = iusDemodulationDictCreate();

	// Same empty lists...
	equal = iusDemodulationDictCompare(dict, dict);
	TEST_ASSERT_EQUAL(IUS_TRUE, equal);
	equal = iusDemodulationDictCompare(dict, notherDict);
	TEST_ASSERT_EQUAL(IUS_TRUE, equal);

	// Fill
	status = iusDemodulationDictSet(dict, pObjLabel, obj);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);
	status = iusDemodulationDictSet(dict, pNotherObjLabel, notherObj);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);

	equal = iusDemodulationDictCompare(dict, notherDict);
	TEST_ASSERT_EQUAL(IUS_FALSE, equal);

	status = iusDemodulationDictSet(notherDict, pObjLabel, obj);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);
	status = iusDemodulationDictSet(notherDict, pNotherObjLabel, notherObj);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);

	equal = iusDemodulationDictCompare(dict, notherDict);
	TEST_ASSERT_EQUAL(IUS_TRUE, equal);

	status = iusDemodulationDictSet(notherDict, pDifferentLabel, differentObj);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);

	equal = iusDemodulationDictCompare(dict, notherDict);
	TEST_ASSERT_EQUAL(IUS_FALSE, equal);

	iusDemodulationDelete(obj);
	iusDemodulationDelete(differentObj);
	iusDemodulationDelete(notherObj);
	iusDemodulationDictDelete(dict);
	iusDemodulationDictDelete(notherDict);
}

TEST(IusDemodulationDict, testIusSerialization)
{
	char *filename = "testIusDemodulationDictSerialization.hdf5";
	//char *ReceiveChannelMapDictPath = "/DemodulationDict";

	// Create and fill
	iudmd_t  dict = dgGenerateDemodulationDict("bmode", 2000);

	// Save
	hid_t handle = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
	TEST_ASSERT(handle > 0);
	int status = iusDemodulationDictSave(dict, handle);
	H5Fclose(handle);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);

	// read back
	handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT);
	iudmd_t savedDict = iusDemodulationDictLoad(handle);
	TEST_ASSERT_NOT_EQUAL(NULL, savedDict);
	H5Fclose(handle);

	TEST_ASSERT_EQUAL(IUS_TRUE, iusDemodulationDictCompare(dict, savedDict));

	iudm_t rs = iusDemodulationDictGet(dict, "bmode");
	iusDemodulationDelete(rs);
	iusDemodulationDictDelete(dict);
	iusDemodulationDictDelete(savedDict);
}


TEST_GROUP_RUNNER(IusDemodulationDict)
{
	RUN_TEST_CASE(IusDemodulationDict, testIusCreateDict);
	RUN_TEST_CASE(IusDemodulationDict, testIusSetGetDict);
	RUN_TEST_CASE(IusDemodulationDict, testIusCompareDict);
	RUN_TEST_CASE(IusDemodulationDict, testIusSerialization);
}