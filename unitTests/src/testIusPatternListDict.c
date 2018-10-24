//
// Created by nlv12901 on 08/10/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <ius.h>
#include <iusPatternListDictPrivate.h>

TEST_GROUP(IusPatternListDict);

TEST_SETUP(IusPatternListDict)
{
}

TEST_TEAR_DOWN(IusPatternListDict)
{
}


TEST(IusPatternListDict, testIusCreatePatternListDict)
{
	iupald_t obj = iusPatternListDictCreate();
	iupald_t notherObj = iusPatternListDictCreate();
	TEST_ASSERT(obj != IUPALD_INVALID);
	TEST_ASSERT(notherObj != IUPALD_INVALID);
	iusPatternListDictDelete(obj);
	iusPatternListDictDelete(notherObj);
}


TEST(IusPatternListDict, testIusPatternListDictSetGet)
{
	int status;
	char *labelDual = "patternList of size 2 for testIusComparePulseDict";
	iupa_t pattern1 = iusPatternCreate(0.01f, "pulseLabel test 1",
									   "sourceLabel test 1",
									   "channelMapLabel test 1",
									   "apodizationLabel test 1",
									   "receiveSetingsLabel test 1");
	iupa_t pattern2 = iusPatternCreate(0.02f, "pulseLabel test 2",
									   "sourceLabel test 2",
									   "channelMapLabel test 2",
									   "apodizationLabel test 2",
									   "receiveSetingsLabel test 2");
	iupald_t dict = iusPatternListDictCreate();
	TEST_ASSERT_NOT_EQUAL(IUPALD_INVALID, dict);

	// CreatNFill PatternList
	iupal_t obj = iusPatternListCreate(2);
	iusPatternListSet(obj, pattern1, 0);
	iusPatternListSet(obj, pattern2, 1);

	status = iusPatternListDictSet(dict, labelDual, obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    iupal_t retrievedObj = iusPatternListDictGet(dict, labelDual);
    TEST_ASSERT_EQUAL(IUS_TRUE, iusPatternListCompare(obj,retrievedObj));

    // invalid params
    TEST_ASSERT_EQUAL(IUPAL_INVALID, iusPatternListDictGet(NULL, labelDual));
    TEST_ASSERT_EQUAL(IUPAL_INVALID, iusPatternListDictGet(dict, NULL));
    TEST_ASSERT_EQUAL(IUPAL_INVALID, iusPatternListDictGet(dict, "unknownLabe"));

    iusPatternDelete(pattern1);
    iusPatternDelete(pattern2);
    iusPatternListDelete(obj);
	iusPatternListDictDelete(dict);
}


TEST(IusPatternListDict, testIusComparePatternListDict)
{
	IUS_BOOL equal;
	int status;
	char *labelDual = "patternList of size 2 for testIusComparePulseDict";
	char *labelSingle = "patternList of size 1 for testIusComparePulseDict";
	iupa_t pattern1 = iusPatternCreate(0.01f, "pulseLabel test 1",
		"sourceLabel test 1",
		"channelMapLabel test 1",
		"apodizationLabel test 1",
		"receiveSetingsLabel test 1");
	iupa_t pattern2 = iusPatternCreate(0.02f, "pulseLabel test 2",
		"sourceLabel test 2",
		"channelMapLabel test 2",
		"apodizationLabel test 2",
		"receiveSetingsLabel test 2");
	iupald_t dict = iusPatternListDictCreate();
	iupald_t notherDict = iusPatternListDictCreate();
	TEST_ASSERT(dict != IUPALD_INVALID);
	TEST_ASSERT(notherDict != IUPALD_INVALID);

	// Same empty lists...
	equal = iusPatternListDictCompare(dict, dict);
	TEST_ASSERT_EQUAL(IUS_TRUE, equal);
	equal = iusPatternListDictCompare(dict, notherDict);
	TEST_ASSERT_EQUAL(IUS_TRUE, equal);

	iupal_t patternListDual = iusPatternListCreate(2);
	iupal_t patternListSingle = iusPatternListCreate(1);

	iusPatternListSet(patternListDual, pattern1, 0);
	iusPatternListSet(patternListDual, pattern2, 1);
	iusPatternListSet(patternListSingle, pattern1, 0);
	
	
	status = iusPatternListDictSet(dict, labelDual, patternListDual);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);
	equal = iusPatternListDictCompare(dict, notherDict);
	TEST_ASSERT_EQUAL(IUS_FALSE, equal);
	status = iusPatternListDictSet(notherDict, labelDual, patternListDual);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);
	equal = iusPatternListDictCompare(dict, notherDict);
	TEST_ASSERT_EQUAL(IUS_TRUE, equal);

	status = iusPatternListDictSet(dict, labelSingle, patternListSingle);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);
	equal = iusPatternListDictCompare(dict, notherDict);
	TEST_ASSERT_EQUAL(IUS_FALSE, equal);

	// invalid params
	equal = iusPatternListDictCompare(dict, NULL);
	TEST_ASSERT_EQUAL(IUS_FALSE, equal);
	equal = iusPatternListDictCompare(NULL, dict);
	TEST_ASSERT_EQUAL(IUS_FALSE, equal);

	iusPatternListDictDelete(dict);
	iusPatternListDictDelete(notherDict);
}

TEST(IusPatternListDict, testIusSerialization)

{
	int status;
	char *labelBMode = "patternList of size 2 for testIusComparePulseDict";
	char *labelDoppler = "patternList of size 1 for testIusComparePulseDict";
	iupa_t pattern1 = iusPatternCreate(0.01f, "pulseLabel test 1",
		"sourceLabel test 1",
		"channelMapLabel test 1",
		"apodizationLabel test 1",
		"receiveSetingsLabel test 1");
	iupa_t pattern2 = iusPatternCreate(0.02f, "pulseLabel test 2",
		"sourceLabel test 2",
		"channelMapLabel test 2",
		"apodizationLabel test 2",
		"receiveSetingsLabel test 2");
	char *filename = "testIusPatternListDictSerialization.hdf5";
	//char *dictPath =  "/PulseDict"; fixed to /Pulses

	// create
	iupald_t dict = iusPatternListDictCreate();
	TEST_ASSERT(dict != IUPALD_INVALID);

	// fill
	iupal_t bModePatternList = iusPatternListCreate(2);
	iupal_t dopplerPatternList = iusPatternListCreate(1);
	
	status = iusPatternListSet(bModePatternList, pattern1, 0);
	TEST_ASSERT(status == IUS_E_OK);
	status = iusPatternListSet(bModePatternList, pattern2, 1);
	TEST_ASSERT(status == IUS_E_OK);
	status = iusPatternListSet(dopplerPatternList, pattern2, 0);
	TEST_ASSERT(status == IUS_E_OK);
	
	status = iusPatternListDictSet(dict, labelBMode, bModePatternList);
	TEST_ASSERT(status == IUS_E_OK);
	status = iusPatternListDictSet(dict, labelDoppler, dopplerPatternList);
	TEST_ASSERT(status == IUS_E_OK);

	// save
	hid_t handle = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
	TEST_ASSERT(handle > 0);
	status = iusPatternListDictSave(dict, handle);
	H5Fclose(handle);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);

	// read back
	handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT);
	iupald_t savedObj = iusPatternListDictLoad(handle);
	TEST_ASSERT(savedObj != NULL);
	H5Fclose(handle);

	TEST_ASSERT_EQUAL(IUS_TRUE, iusPatternListDictCompare(dict, savedObj));

	iusPatternListDictDelete(dict);
	iusPatternListDictDelete(savedObj);
}

TEST_GROUP_RUNNER(IusPatternListDict)
{
	RUN_TEST_CASE(IusPatternListDict, testIusCreatePatternListDict);
	RUN_TEST_CASE(IusPatternListDict, testIusComparePatternListDict);
    RUN_TEST_CASE(IusPatternListDict, testIusPatternListDictSetGet);
	RUN_TEST_CASE(IusPatternListDict, testIusSerialization);
}
