#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <iuf.h>
#include <util.h>
#include <iufPatternListDictPrivate.h>

static char *pErrorFilename = "IufPatternListDict.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(IufPatternListDict);

TEST_SETUP(IufPatternListDict)
{
	iufErrorLogClear();
	iufErrorLog(IUF_TRUE);
    iufErrorAutoReportSet(IUF_TRUE);
	fpErrorLogging = fopen(pErrorFilename, "w+");
	iufErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IufPatternListDict)
{
	iufErrorLogClear();
	iufErrorLog(IUF_FALSE);
	if (fpErrorLogging != NULL)
		fclose(fpErrorLogging);
	fpErrorLogging=stderr;
	iufErrorSetStream(fpErrorLogging);
	remove(pErrorFilename);
}


TEST(IufPatternListDict, testIufCreatePatternListDict)
{
	iupald_t obj = iufPatternListDictCreate();
	iupald_t notherObj = iufPatternListDictCreate();
	TEST_ASSERT(obj != IUPALD_INVALID);
	TEST_ASSERT(notherObj != IUPALD_INVALID);
	iufPatternListDictDelete(obj);
	iufPatternListDictDelete(notherObj);
}


TEST(IufPatternListDict, testIufPatternListDictSetGet)
{
	int status;
	char *labelDual = "patternList of size 2 for testIufComparePulseDict";
	iupa_t pattern1 = iufPatternCreate(0.01f, "pulseLabel test 1",
									   "sourceLabel test 1",
									   "channelMapLabel test 1",
									   "apodizationLabel test 1",
									   "receiveSetingsLabel test 1");
	iupa_t pattern2 = iufPatternCreate(0.02f, "pulseLabel test 2",
									   "sourceLabel test 2",
									   "channelMapLabel test 2",
									   "apodizationLabel test 2",
									   "receiveSetingsLabel test 2");
	iupald_t dict = iufPatternListDictCreate();
	TEST_ASSERT_NOT_EQUAL(IUPALD_INVALID, dict);

	// CreatNFill PatternList
	iupal_t obj = iufPatternListCreate(2,NULL,NULL);
	iufPatternListSet(obj, pattern1, 0);
	iufPatternListSet(obj, pattern2, 1);

	status = iufPatternListDictSet(dict, labelDual, obj);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);
    iupal_t retrievedObj = iufPatternListDictGet(dict, labelDual);
    TEST_ASSERT_EQUAL(IUF_TRUE, iufPatternListCompare(obj,retrievedObj));

    // invalid params
    iufErrorLogClear();
	long filePos = ftell(fpErrorLogging);
	TEST_ASSERT_EQUAL(0,iufErrorGetCount());
	TEST_ASSERT_EQUAL(IUTA_INVALID, iufPatternListDictGet(dict,NULL));
	TEST_ASSERT_EQUAL(IUTA_INVALID, iufPatternListDictGet(NULL,labelDual));
	TEST_ASSERT_EQUAL(IUTA_INVALID, iufPatternListDictGet(dict,"unknownLabel"));

	TEST_ASSERT_EQUAL(IUF_ERR_VALUE, iufPatternListDictSet(dict, NULL, obj));
	TEST_ASSERT_EQUAL(IUF_ERR_VALUE, iufPatternListDictSet(NULL, labelDual, obj));
	TEST_ASSERT_EQUAL(IUF_ERR_VALUE, iufPatternListDictSet(dict, labelDual, NULL));

	TEST_ASSERT_EQUAL(6,iufErrorGetCount());
	TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

    iufPatternDelete(pattern1);
    iufPatternDelete(pattern2);
    iufPatternListDelete(obj);
	iufPatternListDictDelete(dict);
}


TEST(IufPatternListDict, testIufPatternListDictGetKeys)
{
    int status;
    char *labels[] = { "one" , "two" , "three", "four" , "five"};
    iupa_t pattern1 = iufPatternCreate(0.01f, "pulseLabel test 1",
                                       "sourceLabel test 1",
                                       "channelMapLabel test 1",
                                       "apodizationLabel test 1",
                                       "receiveSetingsLabel test 1");
    iupa_t pattern2 = iufPatternCreate(0.02f, "pulseLabel test 2",
                                       "sourceLabel test 2",
                                       "channelMapLabel test 2",
                                       "apodizationLabel test 2",
                                       "receiveSetingsLabel test 2");
    // CreatNFill PatternList
    iupal_t obj = iufPatternListCreate(2,NULL,NULL);
    iufPatternListSet(obj, pattern1, 0);
    iufPatternListSet(obj, pattern2, 1);

    // Create
    iupald_t dict = iufPatternListDictCreate();
    TEST_ASSERT_NOT_EQUAL(IUPALD_INVALID, dict);

    // Fill
    int i;
    int keySize = sizeof(labels)/sizeof(labels[0]);
    for (i=0; i<keySize; i++)
    {
        status = iufPatternListDictSet(dict, labels[i], obj);
        TEST_ASSERT(status == IUF_E_OK);
    }

    size_t dictSize = iufPatternListDictGetSize(dict);
    TEST_ASSERT_EQUAL(5, dictSize);
    char **keys = iufPatternListDictGetKeys(dict);

    // Validate keys
    for (i=0; i<keySize; i++)
    {
        TEST_ASSERT_EQUAL(IUF_TRUE, aInB(keys[i], labels));
    }

    // invalid params
    iufErrorLogClear();
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    keys = iufPatternListDictGetKeys(NULL);
    TEST_ASSERT_EQUAL(NULL,keys);

    TEST_ASSERT_EQUAL(1,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

    iufPatternDelete(pattern1);
    iufPatternDelete(pattern2);
    iufPatternListDelete(obj);
    iufPatternListDictDelete(dict);
}


TEST(IufPatternListDict, testIufComparePatternListDict)
{
	IUF_BOOL equal;
	int status;
	char *labelDual = "patternList of size 2 for testIufComparePulseDict";
	char *labelSingle = "patternList of size 1 for testIufComparePulseDict";
	iupa_t pattern1 = iufPatternCreate(0.01f, "pulseLabel test 1",
		"sourceLabel test 1",
		"channelMapLabel test 1",
		"apodizationLabel test 1",
		"receiveSetingsLabel test 1");
	iupa_t pattern2 = iufPatternCreate(0.02f, "pulseLabel test 2",
		"sourceLabel test 2",
		"channelMapLabel test 2",
		"apodizationLabel test 2",
		"receiveSetingsLabel test 2");
	iupald_t dict = iufPatternListDictCreate();
	iupald_t notherDict = iufPatternListDictCreate();
	TEST_ASSERT(dict != IUPALD_INVALID);
	TEST_ASSERT(notherDict != IUPALD_INVALID);

	// Same empty lists...
	equal = iufPatternListDictCompare(dict, dict);
	TEST_ASSERT_EQUAL(IUF_TRUE, equal);
	equal = iufPatternListDictCompare(dict, notherDict);
	TEST_ASSERT_EQUAL(IUF_TRUE, equal);

	iupal_t patternListDual = iufPatternListCreate(2,NULL,NULL);
	iupal_t patternListSingle = iufPatternListCreate(1,NULL,NULL);

	iufPatternListSet(patternListDual, pattern1, 0);
	iufPatternListSet(patternListDual, pattern2, 1);
	iufPatternListSet(patternListSingle, pattern1, 0);
	
	
	status = iufPatternListDictSet(dict, labelDual, patternListDual);
	TEST_ASSERT_EQUAL(IUF_E_OK, status);
	equal = iufPatternListDictCompare(dict, notherDict);
	TEST_ASSERT_EQUAL(IUF_FALSE, equal);
	status = iufPatternListDictSet(notherDict, labelDual, patternListDual);
	TEST_ASSERT_EQUAL(IUF_E_OK, status);
	equal = iufPatternListDictCompare(dict, notherDict);
	TEST_ASSERT_EQUAL(IUF_TRUE, equal);

	status = iufPatternListDictSet(dict, labelSingle, patternListSingle);
	TEST_ASSERT_EQUAL(IUF_E_OK, status);
	equal = iufPatternListDictCompare(dict, notherDict);
	TEST_ASSERT_EQUAL(IUF_FALSE, equal);

	// invalid params
	equal = iufPatternListDictCompare(dict, NULL);
	TEST_ASSERT_EQUAL(IUF_FALSE, equal);
	equal = iufPatternListDictCompare(NULL, dict);
	TEST_ASSERT_EQUAL(IUF_FALSE, equal);

	iufPatternDelete(pattern1);
	iufPatternDelete(pattern2);
	iufPatternListDelete(patternListDual);
	iufPatternListDelete(patternListSingle);
	iufPatternListDictDelete(dict);
	iufPatternListDictDelete(notherDict);
}

TEST(IufPatternListDict, testIufSerialization)

{
	int status;
	char *labelBMode = "patternList of size 2 for testIufComparePulseDict";
	char *labelDoppler = "patternList of size 1 for testIufComparePulseDict";
	iupa_t pattern1 = iufPatternCreate(0.01f, "pulseLabel test 1",
		"sourceLabel test 1",
		"channelMapLabel test 1",
		"apodizationLabel test 1",
		"receiveSetingsLabel test 1");
	iupa_t pattern2 = iufPatternCreate(0.02f, "pulseLabel test 2",
		"sourceLabel test 2",
		"channelMapLabel test 2",
		"apodizationLabel test 2",
		"receiveSetingsLabel test 2");
	char *filename = "testIufPatternListDictSerialization.hdf5";
	//char *dictPath =  "/PulseDict"; fixed to /Pulses

	// create
	iupald_t dict = iufPatternListDictCreate();
	TEST_ASSERT(dict != IUPALD_INVALID);

	// fill
	iupal_t bModePatternList = iufPatternListCreate(2,NULL,NULL);
	iupal_t dopplerPatternList = iufPatternListCreate(1,NULL,NULL);
	
	status = iufPatternListSet(bModePatternList, pattern1, 0);
	TEST_ASSERT(status == IUF_E_OK);
	status = iufPatternListSet(bModePatternList, pattern2, 1);
	TEST_ASSERT(status == IUF_E_OK);
	status = iufPatternListSet(dopplerPatternList, pattern2, 0);
	TEST_ASSERT(status == IUF_E_OK);
	
	status = iufPatternListDictSet(dict, labelBMode, bModePatternList);
	TEST_ASSERT(status == IUF_E_OK);
	status = iufPatternListDictSet(dict, labelDoppler, dopplerPatternList);
	TEST_ASSERT(status == IUF_E_OK);

	// save
	hid_t handle = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
	TEST_ASSERT(handle > 0);
	status = iufPatternListDictSave(dict, handle);
	H5Fclose(handle);
	TEST_ASSERT_EQUAL(IUF_E_OK, status);

	// read back
	handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT);
	iupald_t savedObj = iufPatternListDictLoad(handle);
	TEST_ASSERT(savedObj != NULL);
	H5Fclose(handle);

	TEST_ASSERT_EQUAL(IUF_TRUE, iufPatternListDictCompare(dict, savedObj));

    iufPatternDelete(pattern1);
    iufPatternDelete(pattern2);
    iufPatternListDelete(bModePatternList);
    iufPatternListDelete(dopplerPatternList);
	iufPatternListDictDelete(dict);
	iufPatternListDictDelete(savedObj);
}

TEST_GROUP_RUNNER(IufPatternListDict)
{
	RUN_TEST_CASE(IufPatternListDict, testIufCreatePatternListDict);
	RUN_TEST_CASE(IufPatternListDict, testIufComparePatternListDict);
    RUN_TEST_CASE(IufPatternListDict, testIufPatternListDictSetGet);
    RUN_TEST_CASE(IufPatternListDict, testIufPatternListDictGetKeys)
	RUN_TEST_CASE(IufPatternListDict, testIufSerialization);
}
