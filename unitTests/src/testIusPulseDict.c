//
// Created by nlv09165 on 03/05/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <ius.h>
#include <iusPulseDictPrivate.h>

TEST_GROUP(IusPulseDict);

TEST_SETUP(IusPulseDict)
{
}

TEST_TEAR_DOWN(IusPulseDict)
{
}


TEST(IusPulseDict, testIusPulseDictCreate)
{
    iupd_t obj = iusPulseDictCreate();
    iupd_t notherObj = iusPulseDictCreate();
    TEST_ASSERT(obj != IUPD_INVALID);
    TEST_ASSERT(notherObj != IUPD_INVALID);
    iusPulseDictDelete(obj);
    iusPulseDictDelete(notherObj);
}

TEST(IusPulseDict, testIusPulseDictSetGet)
{
    float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude=800.0f;       /**< (max) amplitude of the pulse in Volts */
    int     pulseCount=10;               /**< number of cycles that the pulse represents */
    int     status;

    iupd_t dict = iusPulseDictCreate();
    TEST_ASSERT(dict != IUPD_INVALID);

    iupp_t obj = iusParametricPulseCreate(pulseFrequency, pulseAmplitude, pulseCount);
    char *pObjLabel = "Parametric pulse for testIusPulseDictCompare";
    status = iusPulseDictSet(dict,pObjLabel,(iup_t) obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    iup_t retrievedObj = iusPulseDictGet(dict, pObjLabel);
    TEST_ASSERT_EQUAL(IUS_TRUE, iusPulseCompare((iup_t)obj, retrievedObj));

    // Invalid params
    TEST_ASSERT_EQUAL(IUP_INVALID, iusPulseDictGet(dict,NULL));
    TEST_ASSERT_EQUAL(IUP_INVALID, iusPulseDictGet(NULL,pObjLabel));
    TEST_ASSERT_EQUAL(IUP_INVALID, iusPulseDictGet(dict,"unknownLabel"));
    iusParametricPulseDelete(obj);
    iusPulseDictDelete(dict);
}

TEST(IusPulseDict, testIusPulseDictCompare)
{
    IUS_BOOL equal;
    int numPulseValues=10;
    float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude=800.0f;       /**< (max) amplitude of the pulse in Volts */
    int     pulseCount=10;               /**< number of cycles that the pulse represents */
    int     status;
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

    parametricPulse = iusParametricPulseCreate(pulseFrequency, pulseAmplitude, pulseCount);
    nonParametricPulse = iusNonParametricPulseCreate(numPulseValues);

    char *label = "Parametric pulse for testIusPulseDictCompare";
    status = iusPulseDictSet(dict,label,(iup_t) parametricPulse);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    equal = iusPulseDictCompare(dict, notherDict);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    status = iusPulseDictSet(notherDict,label,(iup_t) parametricPulse);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    equal = iusPulseDictCompare(dict, notherDict);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);

    char *label2 = "NON Parametric pulse for testIusPulseDictCompare";
    status = iusPulseDictSet(dict,label2,(iup_t) nonParametricPulse);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    equal = iusPulseDictCompare(dict, notherDict);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    status = iusPulseDictSet(notherDict,label2,(iup_t) nonParametricPulse);
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
    int numPulseValues=10;
    float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude=800.0f;       /**< (max) amplitude of the pulse in Volts */
    int     pulseCount=10;               /**< number of cycles that the pulse represents */
    int     status;
    char *filename = "testIusPulseDictSerialization.hdf5";
    //char *dictPath =  "/PulseDict"; fixed to /Pulses

    // create
    iupd_t dict = iusPulseDictCreate();
    TEST_ASSERT(dict != IUPD_INVALID);

    // fill
    char *parametricLabel = "parametricPulseLabel";
    char *nonParametricLabel = "nonParametricPulseLabel";
    iupp_t parametricPulse = iusParametricPulseCreate(pulseFrequency, pulseAmplitude, pulseCount);
    iunpp_t nonParametricPulse = iusNonParametricPulseCreate(numPulseValues);
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
    RUN_TEST_CASE(IusPulseDict, testIusPulseDictCreate);
    RUN_TEST_CASE(IusPulseDict, testIusPulseDictCompare);
//    RUN_TEST_CASE(IusPulseDict, testIusPulseDictSetGet);
    RUN_TEST_CASE(IusPulseDict, testIusSerialization);
}
