
//
// Created by nlv09165 on 03/05/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>
#include <include/iusHLPulseDict.h>
#include <include/iusHLParametricPulse.h>
#include <include/iusHLNonParametricPulse.h>
#include <include/ius.h>


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
//    nonParametricPulse = iusHLCreateNonParametricPulse("nonParametricPulse",numPulseValues);

    char *label = iusHLPulseGetLabel((iup_t) parametricPulse);
    status = iusHLPulseDictSet(dict,(iup_t) parametricPulse);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    equal = iusHLPulseDictCompare(dict, notherDict);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    status = iusHLPulseDictSet(notherDict,(iup_t) parametricPulse);
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

TEST_GROUP_RUNNER(IusPulseDict)
{
    RUN_TEST_CASE(IusPulseDict, testIusCreatePulseDict);
    RUN_TEST_CASE(IusPulseDict, testIusComparePulseDict)

}
