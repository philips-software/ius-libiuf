//
// Created by nlv12901 on 08/10/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <ius.h>
#include <util.h>
#include <iusIqPatternListDictPrivate.h>

static char *pErrorFilename = "IusIqPatternListDict.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(IusIqPatternListDict);

TEST_SETUP(IusIqPatternListDict)
{
	iusErrorLogClear();
	iusErrorLog(IUS_TRUE);
	iusErrorAutoReport(IUS_TRUE);
	fpErrorLogging = fopen(pErrorFilename, "w+");
	iusErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IusIqPatternListDict)
{
	iusErrorLogClear();
	iusErrorLog(IUS_FALSE);
	if (fpErrorLogging != NULL)
		fclose(fpErrorLogging);
	fpErrorLogging=stderr;
	iusErrorSetStream(fpErrorLogging);
	remove(pErrorFilename);
}


TEST(IusIqPatternListDict, testIusCreatePatternListDict)
{
	iuiqpald_t obj = iusIqPatternListDictCreate();
    iuiqpald_t notherObj = iusIqPatternListDictCreate();
	TEST_ASSERT(obj != IUIQPALD_INVALID);
	TEST_ASSERT(notherObj != IUIQPALD_INVALID);
	iusIqPatternListDictDelete(obj);
	iusIqPatternListDictDelete(notherObj);
}


TEST(IusIqPatternListDict, testIusPatternListDictSetGet)
{
	int status;
	char *labelDual = "patternList of size 2 for testIusComparePulseDict";
	iuiqpa_t pattern1 = iusIqPatternCreate(0.01f, "pulseLabel test 1",
									   "sourceLabel test 1",
									   "channelMapLabel test 1",
									   "apodizationLabel test 1",
									   "receiveSetingsLabel test 1");
    iuiqpa_t pattern2 = iusIqPatternCreate(0.02f, "pulseLabel test 2",
									   "sourceLabel test 2",
									   "channelMapLabel test 2",
									   "apodizationLabel test 2",
									   "receiveSetingsLabel test 2");
    iuiqpald_t dict = iusIqPatternListDictCreate();
	TEST_ASSERT_NOT_EQUAL(IUIQPALD_INVALID, dict);

	// CreatNFill PatternList
	iuiqpal_t obj = iusIqPatternListCreate(2,NULL,NULL);
	iusIqPatternListSet(obj, pattern1, 0);
	iusIqPatternListSet(obj, pattern2, 1);

	status = iusIqPatternListDictSet(dict, labelDual, obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    iuiqpal_t retrievedObj = iusIqPatternListDictGet(dict, labelDual);
    TEST_ASSERT_EQUAL(IUS_TRUE, iusIqPatternListCompare(obj,retrievedObj));

    // invalid params
    iusErrorLogClear();
	long filePos = ftell(fpErrorLogging);
	TEST_ASSERT_EQUAL(0,iusErrorGetCount());
	TEST_ASSERT_EQUAL(IUTA_INVALID, iusIqPatternListDictGet(dict,NULL));
	TEST_ASSERT_EQUAL(IUTA_INVALID, iusIqPatternListDictGet(NULL,labelDual));
	TEST_ASSERT_EQUAL(IUTA_INVALID, iusIqPatternListDictGet(dict,"unknownLabel"));

	TEST_ASSERT_EQUAL(IUS_ERR_VALUE, iusIqPatternListDictSet(dict, NULL, obj));
	TEST_ASSERT_EQUAL(IUS_ERR_VALUE, iusIqPatternListDictSet(NULL, labelDual, obj));
	TEST_ASSERT_EQUAL(IUS_ERR_VALUE, iusIqPatternListDictSet(dict, labelDual, NULL));

	TEST_ASSERT_EQUAL(6,iusErrorGetCount());
	TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

    iusIqPatternDelete(pattern1);
    iusIqPatternDelete(pattern2);
    iusIqPatternListDelete(obj);
	iusIqPatternListDictDelete(dict);
}


TEST(IusIqPatternListDict, testIusPatternListDictGetKeys)
{
    int status;
    char *labels[] = { "one" , "two" , "three", "four" , "five"};
    iuiqpa_t pattern1 = iusIqPatternCreate(0.01f, "pulseLabel test 1",
                                       "sourceLabel test 1",
                                       "channelMapLabel test 1",
                                       "apodizationLabel test 1",
                                       "receiveSetingsLabel test 1");
    iuiqpa_t pattern2 = iusIqPatternCreate(0.02f, "pulseLabel test 2",
                                       "sourceLabel test 2",
                                       "channelMapLabel test 2",
                                       "apodizationLabel test 2",
                                       "receiveSetingsLabel test 2");
    // CreatNFill PatternList
    iuiqpal_t obj = iusIqPatternListCreate(2,NULL,NULL);
    iusIqPatternListSet(obj, pattern1, 0);
    iusIqPatternListSet(obj, pattern2, 1);

    // Create
    iuiqpald_t dict = iusIqPatternListDictCreate();
    TEST_ASSERT_NOT_EQUAL(IUIQPALD_INVALID, dict);

    // Fill
    int i;
    int keySize = sizeof(labels)/sizeof(labels[0]);
    for (i=0; i<keySize; i++)
    {
        status = iusIqPatternListDictSet(dict, labels[i], obj);
        TEST_ASSERT(status == IUS_E_OK);
    }

    size_t dictSize = iusIqPatternListDictGetSize(dict);
    TEST_ASSERT_EQUAL(5, dictSize);
    char **keys = iusIqPatternListDictGetKeys(dict);

    // Validate keys
    for (i=0; i<keySize; i++)
    {
        TEST_ASSERT_EQUAL(IUS_TRUE, aInB(keys[i], labels));
    }

    // invalid params
    iusErrorLogClear();
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());

    keys = iusIqPatternListDictGetKeys(NULL);
    TEST_ASSERT_EQUAL(NULL,keys);

    TEST_ASSERT_EQUAL(1,iusErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

    iusIqPatternDelete(pattern1);
    iusIqPatternDelete(pattern2);
    iusIqPatternListDelete(obj);
    iusIqPatternListDictDelete(dict);
}


TEST(IusIqPatternListDict, testIusComparePatternListDict)
{
	IUS_BOOL equal;
	int status;
	char *labelDual = "patternList of size 2 for testIusComparePulseDict";
	char *labelSingle = "patternList of size 1 for testIusComparePulseDict";
	iuiqpa_t pattern1 = iusIqPatternCreate(0.01f, "pulseLabel test 1",
		"sourceLabel test 1",
		"channelMapLabel test 1",
		"apodizationLabel test 1",
		"receiveSetingsLabel test 1");
	iuiqpa_t pattern2 = iusIqPatternCreate(0.02f, "pulseLabel test 2",
		"sourceLabel test 2",
		"channelMapLabel test 2",
		"apodizationLabel test 2",
		"receiveSetingsLabel test 2");
	iuiqpald_t dict = iusIqPatternListDictCreate();
	iuiqpald_t notherDict = iusIqPatternListDictCreate();
	TEST_ASSERT(dict != IUIQPALD_INVALID);
	TEST_ASSERT(notherDict != IUIQPALD_INVALID);

	// Same empty lists...
	equal = iusIqPatternListDictCompare(dict, dict);
	TEST_ASSERT_EQUAL(IUS_TRUE, equal);
	equal = iusIqPatternListDictCompare(dict, notherDict);
	TEST_ASSERT_EQUAL(IUS_TRUE, equal);

	iuiqpal_t patternListDual = iusIqPatternListCreate(2,NULL,NULL);
	iuiqpal_t patternListSingle = iusIqPatternListCreate(1,NULL,NULL);

	iusIqPatternListSet(patternListDual, pattern1, 0);
	iusIqPatternListSet(patternListDual, pattern2, 1);
	iusIqPatternListSet(patternListSingle, pattern1, 0);
	
	
	status = iusIqPatternListDictSet(dict, labelDual, patternListDual);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);
	equal = iusIqPatternListDictCompare(dict, notherDict);
	TEST_ASSERT_EQUAL(IUS_FALSE, equal);
	status = iusIqPatternListDictSet(notherDict, labelDual, patternListDual);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);
	equal = iusIqPatternListDictCompare(dict, notherDict);
	TEST_ASSERT_EQUAL(IUS_TRUE, equal);

	status = iusIqPatternListDictSet(dict, labelSingle, patternListSingle);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);
	equal = iusIqPatternListDictCompare(dict, notherDict);
	TEST_ASSERT_EQUAL(IUS_FALSE, equal);

	// invalid params
	equal = iusIqPatternListDictCompare(dict, NULL);
	TEST_ASSERT_EQUAL(IUS_FALSE, equal);
	equal = iusIqPatternListDictCompare(NULL, dict);
	TEST_ASSERT_EQUAL(IUS_FALSE, equal);

	iusIqPatternDelete(pattern1);
	iusIqPatternDelete(pattern2);
	iusIqPatternListDelete(patternListDual);
	iusIqPatternListDelete(patternListSingle);
	iusIqPatternListDictDelete(dict);
	iusIqPatternListDictDelete(notherDict);
}

TEST(IusIqPatternListDict, testIusSerialization)

{
	int status;
	char *labelBMode = "patternList of size 2 for testIusComparePulseDict";
	char *labelDoppler = "patternList of size 1 for testIusComparePulseDict";
	iuiqpa_t pattern1 = iusIqPatternCreate(0.01f, "pulseLabel test 1",
		"sourceLabel test 1",
		"channelMapLabel test 1",
		"apodizationLabel test 1",
		"receiveSetingsLabel test 1");
	iuiqpa_t pattern2 = iusIqPatternCreate(0.02f, "pulseLabel test 2",
		"sourceLabel test 2",
		"channelMapLabel test 2",
		"apodizationLabel test 2",
		"receiveSetingsLabel test 2");
	char *filename = "testIqIusPatternListDictSerialization.hdf5";
	//char *dictPath =  "/PulseDict"; fixed to /Pulses

	// create
	iuiqpald_t dict = iusIqPatternListDictCreate();
	TEST_ASSERT(dict != IUIQPALD_INVALID);

	// fill
	iuiqpal_t bModePatternList = iusIqPatternListCreate(2,NULL,NULL);
	iuiqpal_t dopplerPatternList = iusIqPatternListCreate(1,NULL,NULL);
	
	status = iusIqPatternListSet(bModePatternList, pattern1, 0);
	TEST_ASSERT(status == IUS_E_OK);
	status = iusIqPatternListSet(bModePatternList, pattern2, 1);
	TEST_ASSERT(status == IUS_E_OK);
	status = iusIqPatternListSet(dopplerPatternList, pattern2, 0);
	TEST_ASSERT(status == IUS_E_OK);
	
	status = iusIqPatternListDictSet(dict, labelBMode, bModePatternList);
	TEST_ASSERT(status == IUS_E_OK);
	status = iusIqPatternListDictSet(dict, labelDoppler, dopplerPatternList);
	TEST_ASSERT(status == IUS_E_OK);

	// save
	hid_t handle = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
	TEST_ASSERT(handle > 0);
	status = iusIqPatternListDictSave(dict, handle);
	H5Fclose(handle);
	TEST_ASSERT_EQUAL(IUS_E_OK, status);

	// read back
	handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT);
	iuiqpald_t savedObj = iusIqPatternListDictLoad(handle);
	TEST_ASSERT(savedObj != NULL);
	H5Fclose(handle);

	TEST_ASSERT_EQUAL(IUS_TRUE, iusIqPatternListDictCompare(dict, savedObj));

    iusIqPatternDelete(pattern1);
    iusIqPatternDelete(pattern2);
    iusIqPatternListDelete(bModePatternList);
    iusIqPatternListDelete(dopplerPatternList);
	iusIqPatternListDictDelete(dict);
	iusIqPatternListDictDelete(savedObj);
}

TEST_GROUP_RUNNER(IusIqPatternListDict)
{
	RUN_TEST_CASE(IusIqPatternListDict, testIusCreatePatternListDict);
	RUN_TEST_CASE(IusIqPatternListDict, testIusComparePatternListDict);
    RUN_TEST_CASE(IusIqPatternListDict, testIusPatternListDictSetGet);
    RUN_TEST_CASE(IusIqPatternListDict, testIusPatternListDictGetKeys)
	RUN_TEST_CASE(IusIqPatternListDict, testIusSerialization);
}
