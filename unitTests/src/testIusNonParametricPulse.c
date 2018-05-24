

//
// Created by nlv09165 on 01/05/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <include/ius.h>
#include <include/iusError.h>
#include <include/iusTypes.h>
#include <include/iusHLParametricPulse.h>
#include <testDataGenerators.h>
#include "include/iusHLNonParametricPulse.h"

TEST_GROUP(IusNonParametricPulse);

TEST_SETUP(IusNonParametricPulse)
{
}

TEST_TEAR_DOWN(IusNonParametricPulse)
{
}



TEST(IusNonParametricPulse, testIusCreateNonParametricPulse)
{
    int status = 0;
    int     numPulseValues;              /**< number of points to describe waveform, 0 implies a parametric description only */
    iunpp_t nonParametricPulse;

    numPulseValues = 20;
    nonParametricPulse = iusHLNonParametricPulseCreate("Created_in_testIusCreateNonParametricPulse", numPulseValues);
    TEST_ASSERT(nonParametricPulse != IUNPP_INVALID);

    // Invalid operation on nonparametric dta type
    TEST_ASSERT_EQUAL(NULL, iusHLNonParametricPulseCreate("Also_Created_in_testIusCreateNonParametricPulse", -1));
    TEST_ASSERT_EQUAL(NULL, iusHLNonParametricPulseCreate("", numPulseValues));
    TEST_ASSERT_EQUAL(NULL, iusHLNonParametricPulseCreate(NULL, numPulseValues));

    status = iusHLNonParametricPulseDelete(nonParametricPulse);
    TEST_ASSERT(status == IUS_E_OK);
}

TEST(IusNonParametricPulse, testIusDeleteNonParametricPulse)
{
    int status = 0;
    // Transmit parameters
    float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude=800.0f;       /**< (max) amplitude of the pulse in Volts */
    int     pulseCount=10;               /**< number of cycles that the pulse represents */
    int     numPulseValues;              /**< number of points to describe waveform, 0 implies a parametric description only */
    iupp_t  parametricPulse;
    iunpp_t nonParametricPulse;

    // Non Parametric transmit pulse
    // alloc mem
    numPulseValues = 20;
    nonParametricPulse =
    iusHLNonParametricPulseCreate("NonParametricPulse_in_testIusDeleteNonParametricPulse", numPulseValues);

    // Parametric transmit pulse
    parametricPulse = iusHLParametricPulseCreate("ParametricPulse_in_testIusDeleteNonParametricPulse",
                                                 pulseFrequency,
                                                 pulseAmplitude,
                                                 pulseCount);

    // Invalid operation on nonparametric dta type
    status = iusHLNonParametricPulseDelete((iunpp_t) parametricPulse);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
    status = iusHLNonParametricPulseDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);

    status = iusHLNonParametricPulseDelete(nonParametricPulse);
    TEST_ASSERT(status == IUS_E_OK);
    status = iusHLParametricPulseDelete(parametricPulse);
    TEST_ASSERT(status == IUS_E_OK);
}



TEST(IusNonParametricPulse, testIusCompareNonParametricPulse)
{
    int status = 0;
    // Transmit parameters
    float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude=800.0f;       /**< (max) amplitude of the pulse in Volts */
    int     pulseCount=10;               /**< number of cycles that the pulse represents */
    int     numPulseValues=20;              /**< number of points to describe waveform, 0 implies a parametric description only */
    iupp_t  parametricPulse;
    iunpp_t nonParametricPulse,notherNonParametricPulse;
    IUS_BOOL isEqual;

    nonParametricPulse = iusHLNonParametricPulseCreate("nonParametricPulse", numPulseValues);
    notherNonParametricPulse = iusHLNonParametricPulseCreate("notherNonParametricPulse", numPulseValues);

    parametricPulse = iusHLParametricPulseCreate("parametricPulse", pulseFrequency, pulseAmplitude, pulseCount);

    isEqual = iusNonParametricPulseCompare(nonParametricPulse, nonParametricPulse);
    TEST_ASSERT_EQUAL(IUS_TRUE,isEqual);
    isEqual = iusNonParametricPulseCompare(nonParametricPulse, notherNonParametricPulse);
    TEST_ASSERT_EQUAL(IUS_TRUE,isEqual);

    // Invalid arguments
    isEqual = iusNonParametricPulseCompare((iunpp_t) parametricPulse, nonParametricPulse);
    TEST_ASSERT_EQUAL(IUS_FALSE,isEqual);
    isEqual = iusNonParametricPulseCompare(NULL, NULL);
    TEST_ASSERT_EQUAL(IUS_TRUE,isEqual);
    isEqual = iusNonParametricPulseCompare(NULL, nonParametricPulse);
    TEST_ASSERT_EQUAL(IUS_FALSE,isEqual);
    isEqual = iusNonParametricPulseCompare(nonParametricPulse, NULL);
    TEST_ASSERT_EQUAL(IUS_FALSE,isEqual);


    status = iusHLNonParametricPulseDelete(nonParametricPulse);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    status = iusHLParametricPulseDelete(parametricPulse);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
}


TEST(IusNonParametricPulse, testIusSetGetNonParametricPulse)
{
    int status = 0;
    // Transmit parameters
    float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
    float   pulseTime=0.00001f;   /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude=40.0f;       /**< (max) amplitude of the pulse in Volts */
    int     pulseCount=10;               /**< number of cycles that the pulse represents */
    int     numPulseValues;              /**< number of points to describe waveform, 0 implies a parametric description only */
    iupp_t  parametricPulse;
    iunpp_t nonParametricPulse;


    // Non Parametric transmit pulse
    // alloc mem
    numPulseValues = 20;
    nonParametricPulse = iusHLNonParametricPulseCreate("nonParametricPulse", numPulseValues);
    TEST_ASSERT(nonParametricPulse != IUNPP_INVALID);
    TEST_ASSERT_EQUAL(numPulseValues,iusHLNonParametricPulseGetNumValues(nonParametricPulse));

    status = iusHLNonParametricPulseSetAmplitudeTime(nonParametricPulse, pulseTime, pulseAmplitude, 0);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    TEST_ASSERT_EQUAL_FLOAT(pulseAmplitude,iusHLNonParametricPulseGetAmplitude(nonParametricPulse, 0));
    TEST_ASSERT_EQUAL_FLOAT(pulseTime,iusHLNonParametricPulseGetTime(nonParametricPulse, 0));

    pulseTime *= numPulseValues;
    pulseAmplitude *= numPulseValues;
    status = iusHLNonParametricPulseSetAmplitudeTime(nonParametricPulse, pulseTime, pulseAmplitude, numPulseValues-1);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    TEST_ASSERT_EQUAL_FLOAT(pulseAmplitude,iusHLNonParametricPulseGetAmplitude(nonParametricPulse, numPulseValues-1));
    TEST_ASSERT_EQUAL_FLOAT(pulseTime,iusHLNonParametricPulseGetTime(nonParametricPulse, numPulseValues-1));

    // Invalid params
    // Parametric transmit pulse
    parametricPulse = iusHLParametricPulseCreate("parametricPulse", pulseFrequency, pulseAmplitude, pulseCount);
    status = iusHLNonParametricPulseSetAmplitudeTime((iunpp_t) parametricPulse, pulseTime, pulseAmplitude, 0);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE,status);
    status = iusHLNonParametricPulseSetAmplitudeTime(NULL, pulseTime, pulseAmplitude, 0);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE,status);
    status = iusHLNonParametricPulseSetAmplitudeTime(nonParametricPulse, -1.0f, pulseAmplitude, 0);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE,status);
    status = iusHLNonParametricPulseSetAmplitudeTime(nonParametricPulse, pulseAmplitude, pulseAmplitude, -1);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE,status);
    status = iusHLNonParametricPulseSetAmplitudeTime(nonParametricPulse, pulseAmplitude, pulseAmplitude, numPulseValues);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE,status);


    status = iusHLNonParametricPulseDelete(nonParametricPulse);
    TEST_ASSERT(status == IUS_E_OK);
    status = iusHLParametricPulseDelete(parametricPulse);
    TEST_ASSERT(status == IUS_E_OK);
}



TEST_GROUP_RUNNER(IusNonParametricPulse)
{
    RUN_TEST_CASE(IusNonParametricPulse, testIusCreateNonParametricPulse);
    RUN_TEST_CASE(IusNonParametricPulse, testIusDeleteNonParametricPulse);
    RUN_TEST_CASE(IusNonParametricPulse, testIusCompareNonParametricPulse);
    RUN_TEST_CASE(IusNonParametricPulse, testIusSetGetNonParametricPulse);
}
