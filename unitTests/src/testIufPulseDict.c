#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <iuf.h>
#include <util.h>
#include <iufPulseDictPrivate.h>

static char *pErrorFilename = "IufPulseDict.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(IufPulseDict);

TEST_SETUP(IufPulseDict)
{
    iufErrorLogClear();
    iufErrorLog(IUF_TRUE);
    iufErrorAutoReportSet(IUF_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iufErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IufPulseDict)
{
    iufErrorLogClear();
    iufErrorLog(IUF_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iufErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
}


TEST(IufPulseDict, testIufPulseDictCreateDelete)
{
    iupd_t obj = iufPulseDictCreate();
    iupd_t notherObj = iufPulseDictCreate();
    TEST_ASSERT(obj != IUPD_INVALID);
    TEST_ASSERT(notherObj != IUPD_INVALID);
    TEST_ASSERT(iufPulseDictDelete(obj) == IUF_E_OK);
    TEST_ASSERT(iufPulseDictDelete(notherObj) == IUF_E_OK);
    TEST_ASSERT(iufPulseDictDelete(NULL) != IUF_E_OK);
    TEST_ASSERT(iufPulseDictDeepDelete(NULL) != IUF_E_OK);
}

TEST(IufPulseDict, testIufPulseDictSetGet)
{
    float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude=800.0f;       /**< (max) amplitude of the pulse in Volts */
    int     numPulses=10;               /**< number of cycles that the pulse represents */
    int     status;

    iupd_t dict = iufPulseDictCreate();
    TEST_ASSERT(dict != IUPD_INVALID);

    iupp_t obj = iufParametricPulseCreate(pulseFrequency, pulseAmplitude, numPulses);
    char *pObjLabel = "Parametric pulse for testIufPulseDictCompare";
    status = iufPulseDictSet(dict,pObjLabel,(iup_t) obj);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    iup_t retrievedObj = iufPulseDictGet(dict, pObjLabel);
    TEST_ASSERT_EQUAL(IUF_TRUE, iufPulseCompare((iup_t)obj, retrievedObj));

    // Invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());
    TEST_ASSERT_EQUAL(IUTA_INVALID, iufPulseDictGet(dict,NULL));
    TEST_ASSERT_EQUAL(IUTA_INVALID, iufPulseDictGet(NULL,pObjLabel));
    TEST_ASSERT_EQUAL(IUTA_INVALID, iufPulseDictGet(dict,"unknownLabel"));

    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, iufPulseDictSet(dict, pObjLabel, (iup_t) obj));
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, iufPulseDictSet(NULL, pObjLabel, (iup_t) obj));
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, iufPulseDictSet(dict, pObjLabel, NULL));

    TEST_ASSERT_EQUAL(6,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

    iufParametricPulseDelete(obj);
    iufPulseDictDelete(dict);
}


TEST(IufPulseDict, testIufPulseDictGetKeys)
{
    float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude=800.0f;       /**< (max) amplitude of the pulse in Volts */
    int     numPulses=10;               /**< number of cycles that the pulse represents */
    int     status;
    char *labels[] = { "one" , "two" , "three", "four" , "five"};

    iupd_t dict = iufPulseDictCreate();
    TEST_ASSERT(dict != IUPD_INVALID);

    // Create
    iupp_t obj = iufParametricPulseCreate(pulseFrequency, pulseAmplitude, numPulses);


    // Fill
    int i;
    int keySize = sizeof(labels)/sizeof(labels[0]);
    for (i=0; i<keySize; i++)
    {
        status = iufPulseDictSet(dict, labels[i], (iup_t) obj);
        TEST_ASSERT(status == IUF_E_OK);
    }

    size_t dictSize = iufPulseDictGetSize(dict);
    TEST_ASSERT_EQUAL(5, dictSize);
    char **keys = iufPulseDictGetKeys(dict);

    // Validate keys
    for (i=0; i<keySize; i++)
    {
        TEST_ASSERT_EQUAL(IUF_TRUE, aInB(keys[i], labels));
    }

    // Invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    keys = iufPulseDictGetKeys(NULL);
    TEST_ASSERT_EQUAL(NULL,keys);

    TEST_ASSERT_EQUAL(1,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

    iufParametricPulseDelete(obj);
    iufPulseDictDelete(dict);
}

TEST(IufPulseDict, testIufPulseDictCompare)
{
    IUF_BOOL equal;
    int numPulseValues=10;
    float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude=800.0f;       /**< (max) amplitude of the pulse in Volts */
    int     numPulses=10;               /**< number of cycles that the pulse represents */
    int     status;
    iupp_t  parametricPulse;
    iunpp_t nonParametricPulse;

    iupd_t dict = iufPulseDictCreate();
    iupd_t notherDict = iufPulseDictCreate();
    TEST_ASSERT(dict != IUPD_INVALID);
    TEST_ASSERT(notherDict != IUPD_INVALID);

    // Same empty lists...
    equal = iufPulseDictCompare(dict, dict);
    TEST_ASSERT_EQUAL(IUF_TRUE,equal);
    equal = iufPulseDictCompare(dict, notherDict);
    TEST_ASSERT_EQUAL(IUF_TRUE,equal);

    parametricPulse = iufParametricPulseCreate(pulseFrequency, pulseAmplitude, numPulses);
    nonParametricPulse = iufNonParametricPulseCreate(numPulseValues);

    char *label = "Parametric pulse for testIufPulseDictCompare";
    status = iufPulseDictSet(dict,label,(iup_t) parametricPulse);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);
    equal = iufPulseDictCompare(dict, notherDict);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);
    equal = iufPulseDictCompare(notherDict, dict);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);
    status = iufPulseDictSet(notherDict,label,(iup_t) parametricPulse);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);
    equal = iufPulseDictCompare(dict, notherDict);
    TEST_ASSERT_EQUAL(IUF_TRUE,equal);
    equal = iufPulseDictCompare(notherDict, dict);
    TEST_ASSERT_EQUAL(IUF_TRUE,equal);

    char *label2 = "NON Parametric pulse for testIufPulseDictCompare";
    status = iufPulseDictSet(dict,label2,(iup_t) nonParametricPulse);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);
    equal = iufPulseDictCompare(dict, notherDict);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);
    equal = iufPulseDictCompare(notherDict, dict);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);
    status = iufPulseDictSet(notherDict,label2,(iup_t) nonParametricPulse);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);
    equal = iufPulseDictCompare(dict, notherDict);
    TEST_ASSERT_EQUAL(IUF_TRUE,equal);
    equal = iufPulseDictCompare(notherDict, dict);
    TEST_ASSERT_EQUAL(IUF_TRUE,equal);

    // invalid params
    equal = iufPulseDictCompare(dict, NULL);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);
    equal = iufPulseDictCompare(NULL, dict);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);

    iufPulseDelete((iup_t)parametricPulse);
    iufPulseDelete((iup_t)nonParametricPulse);
    iufPulseDictDelete(dict);
    iufPulseDictDelete(notherDict);
}

TEST(IufPulseDict, testIufSerialization)

{
    int numPulseValues=10;
    float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude=800.0f;       /**< (max) amplitude of the pulse in Volts */
    int     numPulses=10;               /**< number of cycles that the pulse represents */
    int     status;
    char *filename = "testIufPulseDictSerialization.hdf5";
    //char *dictPath =  "/PulseDict"; fixed to /Pulses

    // create
    iupd_t dict = iufPulseDictCreate();
    TEST_ASSERT(dict != IUPD_INVALID);

    // fill
    char *parametricLabel = "parametricPulseLabel";
    char *nonParametricLabel = "nonParametricPulseLabel";
    iupp_t parametricPulse = iufParametricPulseCreate(pulseFrequency, pulseAmplitude, numPulses);
    iunpp_t nonParametricPulse = iufNonParametricPulseCreate(numPulseValues);
    iufNonParametricPulseSetValue(nonParametricPulse,0,10.0f,10.0f);
    iufNonParametricPulseSetValue(nonParametricPulse,1,20.0f,10.0f);
    status = iufPulseDictSet(dict,nonParametricLabel, (iup_t) nonParametricPulse);
    TEST_ASSERT(status == IUF_E_OK);
    status = iufPulseDictSet(dict,parametricLabel, (iup_t) parametricPulse);
    TEST_ASSERT(status == IUF_E_OK);

    // save
    status = iufPulseDictSave(dict, H5I_INVALID_HID);
    TEST_ASSERT(status != IUF_E_OK);

    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);

    status = iufPulseDictSave(NULL, handle);
    TEST_ASSERT(status != IUF_E_OK);

    status = iufPulseDictSave(dict, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    // read back
    iupd_t savedObj = iufPulseDictLoad(H5I_INVALID_HID);
    TEST_ASSERT_EQUAL(NULL,savedObj);

    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    savedObj = iufPulseDictLoad(handle);
    TEST_ASSERT(savedObj != NULL);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUF_TRUE, iufPulseDictCompare(dict,savedObj));
    TEST_ASSERT_EQUAL(IUF_TRUE, iufPulseDictCompare(savedObj,dict));

    iufPulseDelete((iup_t)parametricPulse);
    iufPulseDelete((iup_t)nonParametricPulse);
    iufPulseDictDelete(dict);
    iufPulseDictDelete(savedObj);
}

TEST_GROUP_RUNNER(IufPulseDict)
{
    RUN_TEST_CASE(IufPulseDict, testIufPulseDictCreateDelete);
    RUN_TEST_CASE(IufPulseDict, testIufPulseDictCompare);
    RUN_TEST_CASE(IufPulseDict, testIufPulseDictSetGet);
    RUN_TEST_CASE(IufPulseDict, testIufPulseDictGetKeys)
    RUN_TEST_CASE(IufPulseDict, testIufSerialization);
}
