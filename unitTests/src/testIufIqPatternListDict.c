#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <iuf.h>
#include <util.h>
#include <iufIqPatternListDictPrivate.h>

static char *pErrorFilename = "IufIqPatternListDict.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(IufIqPatternListDict);

TEST_SETUP(IufIqPatternListDict)
{
	iufErrorLogClear();
	iufErrorLog(IUF_TRUE);
    iufErrorAutoReportSet(IUF_TRUE);
	fpErrorLogging = fopen(pErrorFilename, "w+");
	iufErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IufIqPatternListDict)
{
	iufErrorLogClear();
	iufErrorLog(IUF_FALSE);
	if (fpErrorLogging != NULL)
		fclose(fpErrorLogging);
	fpErrorLogging=stderr;
	iufErrorSetStream(fpErrorLogging);
	remove(pErrorFilename);
}


TEST(IufIqPatternListDict, testIufCreatePatternListDict)
{
	iuiqpald_t obj = iufIqPatternListDictCreate();
    iuiqpald_t notherObj = iufIqPatternListDictCreate();
	TEST_ASSERT(obj != IUIQPALD_INVALID);
	TEST_ASSERT(notherObj != IUIQPALD_INVALID);
	iufIqPatternListDictDelete(obj);
	iufIqPatternListDictDelete(notherObj);
}


TEST(IufIqPatternListDict, testIufPatternListDictSetGet)
{
	int status;
	char *labelDual = "patternList of size 2 for testIufComparePulseDict";
	iuiqpa_t pattern1 = iufIqPatternCreate(0.01f, "pulseLabel test 1",
									   "sourceLabel test 1",
									   "channelMapLabel test 1",
									   "apodizationLabel test 1",
									   "receiveSetingsLabel test 1");
    iuiqpa_t pattern2 = iufIqPatternCreate(0.02f, "pulseLabel test 2",
									   "sourceLabel test 2",
									   "channelMapLabel test 2",
									   "apodizationLabel test 2",
									   "receiveSetingsLabel test 2");
    iuiqpald_t dict = iufIqPatternListDictCreate();
	TEST_ASSERT_NOT_EQUAL(IUIQPALD_INVALID, dict);

	// CreatNFill PatternList
	iuiqpal_t obj = iufIqPatternListCreate(2,NULL,NULL);
	iufIqPatternListSet(obj, pattern1, 0);
	iufIqPatternListSet(obj, pattern2, 1);

	status = iufIqPatternListDictSet(dict, labelDual, obj);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);
    iuiqpal_t retrievedObj = iufIqPatternListDictGet(dict, labelDual);
    TEST_ASSERT_EQUAL(IUF_TRUE, iufIqPatternListCompare(obj,retrievedObj));

    // invalid params
    iufErrorLogClear();
	long filePos = ftell(fpErrorLogging);
	TEST_ASSERT_EQUAL(0,iufErrorGetCount());
	TEST_ASSERT_EQUAL(IUTA_INVALID, iufIqPatternListDictGet(dict,NULL));
	TEST_ASSERT_EQUAL(IUTA_INVALID, iufIqPatternListDictGet(NULL,labelDual));
	TEST_ASSERT_EQUAL(IUTA_INVALID, iufIqPatternListDictGet(dict,"unknownLabel"));

	TEST_ASSERT_EQUAL(IUF_ERR_VALUE, iufIqPatternListDictSet(dict, NULL, obj));
	TEST_ASSERT_EQUAL(IUF_ERR_VALUE, iufIqPatternListDictSet(NULL, labelDual, obj));
	TEST_ASSERT_EQUAL(IUF_ERR_VALUE, iufIqPatternListDictSet(dict, labelDual, NULL));

	TEST_ASSERT_EQUAL(6,iufErrorGetCount());
	TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

    iufIqPatternDelete(pattern1);
    iufIqPatternDelete(pattern2);
    iufIqPatternListDelete(obj);
	iufIqPatternListDictDelete(dict);
}


TEST(IufIqPatternListDict, testIufPatternListDictGetKeys)
{
    int status;
    char *labels[] = { "one" , "two" , "three", "four" , "five"};
    iuiqpa_t pattern1 = iufIqPatternCreate(0.01f, "pulseLabel test 1",
                                       "sourceLabel test 1",
                                       "channelMapLabel test 1",
                                       "apodizationLabel test 1",
                                       "receiveSetingsLabel test 1");
    iuiqpa_t pattern2 = iufIqPatternCreate(0.02f, "pulseLabel test 2",
                                       "sourceLabel test 2",
                                       "channelMapLabel test 2",
                                       "apodizationLabel test 2",
                                       "receiveSetingsLabel test 2");
    // CreatNFill PatternList
    iuiqpal_t obj = iufIqPatternListCreate(2,NULL,NULL);
    iufIqPatternListSet(obj, pattern1, 0);
    iufIqPatternListSet(obj, pattern2, 1);

    // Create
    iuiqpald_t dict = iufIqPatternListDictCreate();
    TEST_ASSERT_NOT_EQUAL(IUIQPALD_INVALID, dict);

    // Fill
    int i;
    int keySize = sizeof(labels)/sizeof(labels[0]);
    for (i=0; i<keySize; i++)
    {
        status = iufIqPatternListDictSet(dict, labels[i], obj);
        TEST_ASSERT(status == IUF_E_OK);
    }

    size_t dictSize = iufIqPatternListDictGetSize(dict);
    TEST_ASSERT_EQUAL(5, dictSize);
    char **keys = iufIqPatternListDictGetKeys(dict);

    // Validate keys
    for (i=0; i<keySize; i++)
    {
        TEST_ASSERT_EQUAL(IUF_TRUE, aInB(keys[i], labels));
    }

    // invalid params
    iufErrorLogClear();
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    keys = iufIqPatternListDictGetKeys(NULL);
    TEST_ASSERT_EQUAL(NULL,keys);

    TEST_ASSERT_EQUAL(1,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

    iufIqPatternDelete(pattern1);
    iufIqPatternDelete(pattern2);
    iufIqPatternListDelete(obj);
    iufIqPatternListDictDelete(dict);
}


TEST(IufIqPatternListDict, testIufComparePatternListDict)
{
	IUF_BOOL equal;
	int status;
	char *labelDual = "patternList of size 2 for testIufComparePulseDict";
	char *labelSingle = "patternList of size 1 for testIufComparePulseDict";
	iuiqpa_t pattern1 = iufIqPatternCreate(0.01f, "pulseLabel test 1",
		"sourceLabel test 1",
		"channelMapLabel test 1",
		"apodizationLabel test 1",
		"receiveSetingsLabel test 1");
	iuiqpa_t pattern2 = iufIqPatternCreate(0.02f, "pulseLabel test 2",
		"sourceLabel test 2",
		"channelMapLabel test 2",
		"apodizationLabel test 2",
		"receiveSetingsLabel test 2");
	iuiqpald_t dict = iufIqPatternListDictCreate();
	iuiqpald_t notherDict = iufIqPatternListDictCreate();
	TEST_ASSERT(dict != IUIQPALD_INVALID);
	TEST_ASSERT(notherDict != IUIQPALD_INVALID);

	// Same empty lists...
	equal = iufIqPatternListDictCompare(dict, dict);
	TEST_ASSERT_EQUAL(IUF_TRUE, equal);
	equal = iufIqPatternListDictCompare(dict, notherDict);
	TEST_ASSERT_EQUAL(IUF_TRUE, equal);

	iuiqpal_t patternListDual = iufIqPatternListCreate(2,NULL,NULL);
	iuiqpal_t patternListSingle = iufIqPatternListCreate(1,NULL,NULL);

	iufIqPatternListSet(patternListDual, pattern1, 0);
	iufIqPatternListSet(patternListDual, pattern2, 1);
	iufIqPatternListSet(patternListSingle, pattern1, 0);
	
	
	status = iufIqPatternListDictSet(dict, labelDual, patternListDual);
	TEST_ASSERT_EQUAL(IUF_E_OK, status);
	equal = iufIqPatternListDictCompare(dict, notherDict);
	TEST_ASSERT_EQUAL(IUF_FALSE, equal);
	status = iufIqPatternListDictSet(notherDict, labelDual, patternListDual);
	TEST_ASSERT_EQUAL(IUF_E_OK, status);
	equal = iufIqPatternListDictCompare(dict, notherDict);
	TEST_ASSERT_EQUAL(IUF_TRUE, equal);

	status = iufIqPatternListDictSet(dict, labelSingle, patternListSingle);
	TEST_ASSERT_EQUAL(IUF_E_OK, status);
	equal = iufIqPatternListDictCompare(dict, notherDict);
	TEST_ASSERT_EQUAL(IUF_FALSE, equal);

	// invalid params
	equal = iufIqPatternListDictCompare(dict, NULL);
	TEST_ASSERT_EQUAL(IUF_FALSE, equal);
	equal = iufIqPatternListDictCompare(NULL, dict);
	TEST_ASSERT_EQUAL(IUF_FALSE, equal);

	iufIqPatternDelete(pattern1);
	iufIqPatternDelete(pattern2);
	iufIqPatternListDelete(patternListDual);
	iufIqPatternListDelete(patternListSingle);
	iufIqPatternListDictDelete(dict);
	iufIqPatternListDictDelete(notherDict);
}

TEST(IufIqPatternListDict, testIufSerialization)

{
	int status;
	char *labelBMode = "patternList of size 2 for testIufComparePulseDict";
	char *labelDoppler = "patternList of size 1 for testIufComparePulseDict";
	iuiqpa_t pattern1 = iufIqPatternCreate(0.01f, "pulseLabel test 1",
		"sourceLabel test 1",
		"channelMapLabel test 1",
		"apodizationLabel test 1",
		"receiveSetingsLabel test 1");
	iuiqpa_t pattern2 = iufIqPatternCreate(0.02f, "pulseLabel test 2",
		"sourceLabel test 2",
		"channelMapLabel test 2",
		"apodizationLabel test 2",
		"receiveSetingsLabel test 2");
	char *filename = "testIqIufPatternListDictSerialization.hdf5";
	//char *dictPath =  "/PulseDict"; fixed to /Pulses

	// create
	iuiqpald_t dict = iufIqPatternListDictCreate();
	TEST_ASSERT(dict != IUIQPALD_INVALID);

	// fill
	iuiqpal_t bModePatternList = iufIqPatternListCreate(2,NULL,NULL);
	iuiqpal_t dopplerPatternList = iufIqPatternListCreate(1,NULL,NULL);
	
	status = iufIqPatternListSet(bModePatternList, pattern1, 0);
	TEST_ASSERT(status == IUF_E_OK);
	status = iufIqPatternListSet(bModePatternList, pattern2, 1);
	TEST_ASSERT(status == IUF_E_OK);
	status = iufIqPatternListSet(dopplerPatternList, pattern2, 0);
	TEST_ASSERT(status == IUF_E_OK);
	
	status = iufIqPatternListDictSet(dict, labelBMode, bModePatternList);
	TEST_ASSERT(status == IUF_E_OK);
	status = iufIqPatternListDictSet(dict, labelDoppler, dopplerPatternList);
	TEST_ASSERT(status == IUF_E_OK);

	// save
	hid_t handle = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
	TEST_ASSERT(handle > 0);
	status = iufIqPatternListDictSave(dict, handle);
	H5Fclose(handle);
	TEST_ASSERT_EQUAL(IUF_E_OK, status);

	// read back
	handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT);
	iuiqpald_t savedObj = iufIqPatternListDictLoad(handle);
	TEST_ASSERT(savedObj != NULL);
	H5Fclose(handle);

	TEST_ASSERT_EQUAL(IUF_TRUE, iufIqPatternListDictCompare(dict, savedObj));

    iufIqPatternDelete(pattern1);
    iufIqPatternDelete(pattern2);
    iufIqPatternListDelete(bModePatternList);
    iufIqPatternListDelete(dopplerPatternList);
	iufIqPatternListDictDelete(dict);
	iufIqPatternListDictDelete(savedObj);
}

TEST_GROUP_RUNNER(IufIqPatternListDict)
{
	RUN_TEST_CASE(IufIqPatternListDict, testIufCreatePatternListDict);
	RUN_TEST_CASE(IufIqPatternListDict, testIufComparePatternListDict);
    RUN_TEST_CASE(IufIqPatternListDict, testIufPatternListDictSetGet);
    RUN_TEST_CASE(IufIqPatternListDict, testIufPatternListDictGetKeys)
	RUN_TEST_CASE(IufIqPatternListDict, testIufSerialization);
}
