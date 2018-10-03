//
// Created by nlv09165 on 03/05/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <hdf5.h>
#include <ius.h>
#include <iusPulseDictImp.h>
#include <iusParametricPulse.h>
#include <iusNonParametricPulse.h>

TEST_GROUP(IusPulseDict);

TEST_SETUP(IusPulseDict)
{
}

TEST_TEAR_DOWN(IusPulseDict)
{
}


TEST(IusPulseDict, testIusCreatePulseDict)
{
    iupd_t obj = iusPulseDictCreate();
    iupd_t notherObj = iusPulseDictCreate();
    TEST_ASSERT(obj != IUPD_INVALID);
    TEST_ASSERT(notherObj != IUPD_INVALID);
    iusPulseDictDelete(obj);
    iusPulseDictDelete(notherObj);
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

    iupd_t dict = iusPulseDictCreate();
    iupd_t notherDict = iusPulseDictCreate();
    TEST_ASSERT(dict != IUPD_INVALID);
    TEST_ASSERT(notherDict != IUPD_INVALID);

    // Same empty lists...
    equal = iusPulseDictCompare(dict, dict);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = iusPulseDictCompare(dict, notherDict);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);

    parametricPulse = iusParametricPulseCreate("parametricPulse", pulseFrequency, pulseAmplitude, pulseCount);
    nonParametricPulse = iusNonParametricPulseCreate("nonParametricPulse",numPulseValues);

    char *label = iusPulseGetLabel((iup_t) parametricPulse);
    status = iusPulseDictSet(dict,label,(iup_t) parametricPulse);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    equal = iusPulseDictCompare(dict, notherDict);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    status = iusPulseDictSet(notherDict,label,(iup_t) parametricPulse);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    equal = iusPulseDictCompare(dict, notherDict);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);

    label = iusPulseGetLabel((iup_t) nonParametricPulse);
    status = iusPulseDictSet(dict,label,(iup_t) nonParametricPulse);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    equal = iusPulseDictCompare(dict, notherDict);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    status = iusPulseDictSet(notherDict,label,(iup_t) nonParametricPulse);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    equal = iusPulseDictCompare(dict, notherDict);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);


    // invalid params
    equal = iusPulseDictCompare(dict, NULL);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    equal = iusPulseDictCompare(NULL, dict);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

    iusPulseDictDelete(dict);
    iusPulseDictDelete(notherDict);
}

TEST(IusPulseDict, testIusSerialization)

{
    int numPulses = 10;
    int numPulseValues=10;
    float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude=800.0f;       /**< (max) amplitude of the pulse in Volts */
    int     pulseCount=10;               /**< number of cycles that the pulse represents */
    int     status=10;
    char *filename = "testIusPulseDictSerialization.hdf5";
    //char *dictPath =  "/PulseDict"; fixed to /Pulses

    // create
    iupd_t dict = iusPulseDictCreate();
    TEST_ASSERT(dict != IUPD_INVALID);

    // fill
    char *parametricLabel = "parametricPulseLabel";
    char *nonParametricLabel = "nonParametricPulseLabel";
    iupp_t parametricPulse = iusParametricPulseCreate(parametricLabel, pulseFrequency, pulseAmplitude, pulseCount);
    iunpp_t nonParametricPulse = iusNonParametricPulseCreate(nonParametricLabel,numPulseValues);
    iusNonParametricPulseSetValue(nonParametricPulse,0,10.0f,10.0f);
    iusNonParametricPulseSetValue(nonParametricPulse,1,20.0f,10.0f);
    status = iusPulseDictSet(dict,nonParametricLabel, (iup_t) nonParametricPulse);
    TEST_ASSERT(status == IUS_E_OK);
    status = iusPulseDictSet(dict,parametricLabel, (iup_t) parametricPulse);
    TEST_ASSERT(status == IUS_E_OK);

    // save
    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
    status = iusPulseDictSave(dict, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iupd_t savedObj = iusPulseDictLoad(handle);
    TEST_ASSERT(savedObj != NULL);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUS_TRUE, iusPulseDictCompare(dict,savedObj));

    iusPulseDictDelete(dict);
    iusPulseDictDelete(savedObj);
}

TEST_GROUP_RUNNER(IusPulseDict)
{
    RUN_TEST_CASE(IusPulseDict, testIusCreatePulseDict);
    RUN_TEST_CASE(IusPulseDict, testIusComparePulseDict);
    RUN_TEST_CASE(IusPulseDict, testIusSerialization);
}
