//
// Created by Ruijzendaal on 30/03/2018.
//

#include <unity_fixture.h>
#include <include/iusHLDrivingScheme.h>
#include <include/ius.h>
#include <include/iusTypes.h>
#include <include/iusHLTransducer.h>
#include <include/iusError.h>
#include "testDrivingScheme.h"

TEST_GROUP(InputfileTransmitPulse);

TEST_SETUP(InputfileTransmitPulse)
{
}

TEST_TEAR_DOWN(InputfileTransmitPulse)
{
}

void fillAmplitudeTime
    (
        float * pRawPulseAmplitudes,          /**< shape of waveform [in Volts] */
        float * pRawPulseTimes,               /**< corresponding timestamps of amplitudes [in seconds] */
        int     numPulseValues              /**< number of points to describe waveform, 0 implies a parametric description only */
    )
{
    int i;
    for(i=0; i< numPulseValues; i++)
    {
        pRawPulseAmplitudes[i] = 1.0f/((i%10)+1.0f);
        pRawPulseTimes[i] = 1.0f/((i%10)+1.0f);
    }
}

TEST(InputfileTransmitPulse, testCreateTransmitPulse)
{
    int status = 0;
    // Transmit parameters
    float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude=800.0f;       /**< (max) amplitude of the pulse in Volts */
    int     pulseCount=10;               /**< number of cycles that the pulse represents */
    int     numPulseValues;              /**< number of points to describe waveform, 0 implies a parametric description only */
    float * pRawPulseAmplitudes;         /**< shape of waveform [in Volts] */
    float * pRawPulseTimes;              /**< corresponding timestamps of amplitudes [in seconds] */
    iutp_t  parametricPulse,nonParametricPulse;
    float newPulseFrequency;



    // Parametric transmit pulse
    parametricPulse = iusHLCreateParametricPulse(pulseFrequency,pulseAmplitude,pulseCount);


    TEST_ASSERT_EQUAL_FLOAT( iusHLParametricPulseGetFrequency(parametricPulse), pulseFrequency);
    TEST_ASSERT_EQUAL_FLOAT(iusHLParametricPulseGetPulseAmplitude(parametricPulse), pulseAmplitude);
    TEST_ASSERT_EQUAL_FLOAT(iusHLParametricPulseGetCount(parametricPulse),pulseCount);

    // Non Parametric transmit pulse
    // alloc mem
    numPulseValues = 20;
    pRawPulseAmplitudes = (float *)calloc(numPulseValues, sizeof(float));
    pRawPulseTimes = (float *)calloc(numPulseValues, sizeof(float));

    fillAmplitudeTime(pRawPulseAmplitudes,pRawPulseTimes,numPulseValues);
    nonParametricPulse = iusHLCreateNonParametricPulse(pRawPulseAmplitudes,pRawPulseTimes,numPulseValues);
    TEST_ASSERT(nonParametricPulse != IUTP_INVALID);

    // Invalid operation on nonparametric dta type
    TEST_ASSERT_EQUAL_FLOAT(iusHLParametricPulseGetFrequency(nonParametricPulse), NAN);
    TEST_ASSERT_EQUAL_FLOAT(iusHLParametricPulseGetPulseAmplitude(nonParametricPulse), NAN);
    TEST_ASSERT(iusHLParametricPulseGetCount(nonParametricPulse) == -1);


    status = iusHLDeleteTransmitPulse(nonParametricPulse);
    TEST_ASSERT(status == IUS_E_OK);
    status = iusHLDeleteTransmitPulse(parametricPulse);
    TEST_ASSERT(status == IUS_E_OK);
    free(pRawPulseAmplitudes);
    free(pRawPulseTimes);
}


TEST_GROUP_RUNNER(InputfileTransmitPulse)
{
    RUN_TEST_CASE(InputfileTransmitPulse, testCreateTransmitPulse);
}
