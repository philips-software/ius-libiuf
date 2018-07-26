//
// Created by nlv09165 on 03/05/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <hdf5.h>
#include <ius.h>
#include <iusHLPulseDictImp.h>
#include <iusHLParametricPulse.h>
#include <iusHLNonParametricPulse.h>

TEST_GROUP(IusPulseDict);

TEST_SETUP(IusPulseDict)
{
}

TEST_TEAR_DOWN(IusPulseDict)
{
}


TEST(IusPulseDict, testIusCreatePulseDict)
{
    iupd_t obj = iusHLPulseDictCreate();
    iupd_t notherObj = iusHLPulseDictCreate();
    TEST_ASSERT(obj != IUPD_INVALID);
    TEST_ASSERT(notherObj != IUPD_INVALID);
    iusHLPulseDictDelete(obj);
    iusHLPulseDictDelete(notherObj);
}


TEST(IusPulseDict, testIusComparePulseDict)
{
    IUS_BOOL equal;
    int numPulses = 10;
    int numPulseValues=10;
    float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude=800.0f;       /**< (max) amplitude of the pulse in Volts */
    int     pulseCount=10;               /**< number of cycles that the pulse represents */
    int     status=10;
    iupp_t  parametricPulse;
    iunpp_t nonParametricPulse;

    iupd_t dict = iusHLPulseDictCreate();
    iupd_t notherDict = iusHLPulseDictCreate();
    TEST_ASSERT(dict != IUPD_INVALID);
    TEST_ASSERT(notherDict != IUPD_INVALID);

    // Same empty lists...
    equal = iusHLPulseDictCompare(dict, dict);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = iusHLPulseDictCompare(dict, notherDict);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);

    parametricPulse = iusHLParametricPulseCreate("parametricPulse", pulseFrequency, pulseAmplitude, pulseCount);
    nonParametricPulse = iusHLNonParametricPulseCreate("nonParametricPulse",numPulseValues);

    char *label = iusHLPulseGetLabel((iup_t) parametricPulse);
    status = iusHLPulseDictSet(dict,label,(iup_t) parametricPulse);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    equal = iusHLPulseDictCompare(dict, notherDict);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    status = iusHLPulseDictSet(notherDict,label,(iup_t) parametricPulse);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    equal = iusHLPulseDictCompare(dict, notherDict);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);

    label = iusHLPulseGetLabel((iup_t) nonParametricPulse);
    status = iusHLPulseDictSet(dict,label,(iup_t) nonParametricPulse);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    equal = iusHLPulseDictCompare(dict, notherDict);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    status = iusHLPulseDictSet(notherDict,label,(iup_t) nonParametricPulse);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    equal = iusHLPulseDictCompare(dict, notherDict);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);


    // invalid params
    equal = iusHLPulseDictCompare(dict, NULL);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    equal = iusHLPulseDictCompare(NULL, dict);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

    iusHLPulseDictDelete(dict);
    iusHLPulseDictDelete(notherDict);
}

#if 0
TEST(IusPulseDict, testIusSerialization)

{
    int numPulses = 10;
    int numPulseValues=10;
    float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude=800.0f;       /**< (max) amplitude of the pulse in Volts */
    int     pulseCount=10;               /**< number of cycles that the pulse represents */
    int     status=10;
    char *filename = "testIusPulseDictSerialization.hdf5";
    char *dictPath =  "/PulseDict";

    // create
    iupd_t dict = iusHLPulseDictCreate();
    TEST_ASSERT(dict != IUPD_INVALID);

    // fill
    char *parametricLabel = "parametricPulseLabel";
    char *nonParametricLabel = "nonParametricPulseLabel";
    iupp_t parametricPulse = iusHLParametricPulseCreate(parametricLabel, pulseFrequency, pulseAmplitude, pulseCount);
    iunpp_t nonParametricPulse = iusHLNonParametricPulseCreate(nonParametricLabel,numPulseValues);
    iusHLNonParametricPulseSetValue(nonParametricPulse,0,10.0f,10.0f);
    iusHLNonParametricPulseSetValue(nonParametricPulse,1,20.0f,10.0f);
    status = iusHLPulseDictSet(dict,nonParametricLabel, (iup_t) nonParametricPulse);
    TEST_ASSERT(status == IUS_E_OK);
    status = iusHLPulseDictSet(dict,parametricLabel, (iup_t) parametricPulse);
    TEST_ASSERT(status == IUS_E_OK);

    // save
    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
    status = iusHLPulseDictSave(dict, dictPath, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iupd_t savedObj = iusHLPulseDictLoad(handle, dictPath);
    TEST_ASSERT(savedObj != NULL);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUS_TRUE, iusHLPulseDictCompare(dict,savedObj));

    iusHLPulseDictDelete(dict);
    iusHLPulseDictDelete(savedObj);
}
#endif

TEST_GROUP_RUNNER(IusPulseDict)
{
    RUN_TEST_CASE(IusPulseDict, testIusCreatePulseDict);
    RUN_TEST_CASE(IusPulseDict, testIusComparePulseDict);
    //RUN_TEST_CASE(IusPulseDict, testIusSerialization);
}
