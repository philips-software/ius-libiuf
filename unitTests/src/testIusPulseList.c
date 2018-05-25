//
// Created by nlv09165 on 02/05/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusHLParametricPulse.h>
#include <iusHLNonParametricPulse.h>
#include <iusHLPulseList.h>

TEST_GROUP(IusPulseList);

TEST_SETUP(IusPulseList)
{
}

TEST_TEAR_DOWN(IusPulseList)
{
}


TEST(IusPulseList, testIusCreatePulseList)
{
    int numPulses = 10;
    iupl_t obj = iusHLPulseListCreate(numPulses);
    iupl_t notherObj = iusHLPulseListCreate(numPulses);
    TEST_ASSERT(obj != IUPL_INVALID);
    TEST_ASSERT(notherObj != IUPL_INVALID);
    iusHLPulseListDelete(obj);
    iusHLPulseListDelete(notherObj);

    // invalid params
    obj = iusHLPulseListCreate(-1);
    TEST_ASSERT(obj == IUPL_INVALID);
}

TEST(IusPulseList, testIusDeletePulseList)
{
    int numPulses = 10;
    iupl_t obj = iusHLPulseListCreate(numPulses);
    TEST_ASSERT(obj != IUPL_INVALID);
    int status = iusHLPulseListDelete(obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // invalid params
    status = iusHLPulseListDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
}



TEST(IusPulseList, testIusComparePulseList)
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

    iupl_t obj = iusHLPulseListCreate(numPulses);
    iupl_t notherObj = iusHLPulseListCreate(numPulses);
    TEST_ASSERT(obj != IUPL_INVALID);
    TEST_ASSERT(notherObj != IUPL_INVALID);

    // Same empty lists...
    equal = iusHLPulseListCompare(obj, obj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = iusHLPulseListCompare(obj, notherObj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);

    parametricPulse = iusHLParametricPulseCreate("parametricPulse", pulseFrequency, pulseAmplitude, pulseCount);
    nonParametricPulse = iusHLNonParametricPulseCreate("nonParametricPulse", numPulseValues);

    status = iusHLPulseListSet(obj,(iup_t) parametricPulse,0);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    equal = iusHLPulseListCompare(obj, notherObj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    status = iusHLPulseListSet(notherObj,(iup_t) parametricPulse,0);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    equal = iusHLPulseListCompare(obj, notherObj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    status = iusHLPulseListSet(notherObj,(iup_t) parametricPulse,numPulses-1);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    equal = iusHLPulseListCompare(obj, notherObj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    status = iusHLPulseListSet(obj,(iup_t) parametricPulse,numPulses-1);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    equal = iusHLPulseListCompare(obj, notherObj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    status = iusHLPulseListSet(obj,(iup_t) nonParametricPulse,1);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    equal = iusHLPulseListCompare(obj, notherObj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    status = iusHLPulseListSet(notherObj,(iup_t) nonParametricPulse,1);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    equal = iusHLPulseListCompare(obj, notherObj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);

    // invalid params
    equal = iusHLPulseListCompare(obj, NULL);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    equal = iusHLPulseListCompare(NULL, obj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

    iusHLPulseListDelete(obj);
    iusHLPulseListDelete(notherObj);
}



TEST(IusPulseList, testIusSetGetPulseList)
{
    IUS_BOOL equal;
    int numPulses = 3;
    int numPulseValues=10;
    float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude=800.0f;       /**< (max) amplitude of the pulse in Volts */
    int     pulseCount=10;               /**< number of cycles that the pulse represents */
    int     status=10;
    iupp_t  parametricPulse,notherParametricPulse;
    iunpp_t nonParametricPulse,notherNonParametricPulse;

    iupl_t obj = iusHLPulseListCreate(numPulses);
    TEST_ASSERT(obj != IUPL_INVALID);

    parametricPulse = iusHLParametricPulseCreate("parametricPulse", pulseFrequency, pulseAmplitude, pulseCount);
    nonParametricPulse = iusHLNonParametricPulseCreate("nonParametricPulse", numPulseValues);

    iusHLPulseListSet(obj,(iup_t)parametricPulse,0);
    iusHLPulseListSet(obj,(iup_t)nonParametricPulse,numPulses-1);
    notherParametricPulse = (iupp_t)iusHLPulseListGet(obj,0);
    equal = iusHLPulseCompare((iup_t) parametricPulse, (iup_t) notherParametricPulse);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    notherNonParametricPulse = (iunpp_t)iusHLPulseListGet(obj,numPulses-1);
    equal = iusHLPulseCompare((iup_t) nonParametricPulse, (iup_t) notherNonParametricPulse);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);

    //invalid
    TEST_ASSERT_EQUAL(IUP_INVALID, iusHLPulseListGet(obj,numPulses));
    TEST_ASSERT_EQUAL(IUP_INVALID, iusHLPulseListGet(obj,1));
    iusHLPulseListDelete(obj);
}



TEST_GROUP_RUNNER(IusPulseList)
{
    RUN_TEST_CASE(IusPulseList, testIusCreatePulseList);
    RUN_TEST_CASE(IusPulseList, testIusDeletePulseList);
    RUN_TEST_CASE(IusPulseList, testIusComparePulseList);
    RUN_TEST_CASE(IusPulseList, testIusSetGetPulseList);
}
