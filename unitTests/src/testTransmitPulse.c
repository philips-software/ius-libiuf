//
// Created by Ruijzendaal on 30/03/2018.
//

#include <unity_fixture.h>
#include <include/iusHLDrivingScheme.h>
#include <include/ius.h>
#include <include/iusTypes.h>
#include <include/iusHLTransducer.h>
#include <include/iusError.h>
#include <include/iusHLTransmitPulseList.h>
#include "testDrivingScheme.h"

TEST_GROUP(InputfileTransmitPulse);

TEST_SETUP(InputfileTransmitPulse)
{
}

TEST_TEAR_DOWN(InputfileTransmitPulse)
{
}

int fillAmplitudeTime
(
    iutp_t transmitPulse
)
{
    int i;
    int status=0;
    float pulseAmplitude;
    float pulseTimes;
    int numPulseValues = iusHLNonParametricPulseGetCount(transmitPulse);
    for(i=0; i< numPulseValues; i++)
    {
        pulseAmplitude = 1.0f / ((i % 10) + 1.0f);
        pulseTimes = 1.0f / ((i % 10) + 1.0f);
        status = iusHLNonParametricPulseSetAmplitudeTime(transmitPulse,pulseTimes,pulseAmplitude,i);
        if( status != IUS_E_OK )
        {
            return status;
        }
    }
    return status;
}

int fillParametricPulseList
(
    iutpl_t transmitPulseList,
    int pulseValues
)
{
    int i;
    int status=0;
    float pulseAmplitude;          /**< shape of waveform [in Volts] */
    float pulseFrequency;               /**< corresponding timestamps of amplitudes [in seconds] */
    int   numPulseCount  = iusHLTransmitPulseListGetSize(transmitPulseList)    ;
    iutp_t  parametricPulse;

    for(i=0; i< numPulseCount; i++)
    {
        pulseAmplitude = 1.0f/((i%10)+1.0f);
        pulseFrequency = 1.0f/((i%10)+1.0f);
        parametricPulse = iusHLCreateParametricPulse(pulseFrequency,pulseAmplitude,pulseValues);
        status = iusHLTransmitPulseListSet(transmitPulseList, parametricPulse, i );
        if( status != IUS_E_OK )
        {
            return status;
        }
    }
    return status;
}

iutpl_t createNfillpulselist(int pulses, float time){
    iutpl_t transmitPulseList = iusHLCreateTransmitPulseList(pulses,IUS_PARAMETRIC_PULSETYPE);
    int pulseValues = 100;
    int status = fillParametricPulseList(transmitPulseList,pulseValues);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    return transmitPulseList;
}

TEST(InputfileTransmitPulse, testTransmitPulseListSet)
{
    int numTransmitPulses = 10;
    int status;
    float time = 0.1f;

    iutpl_t transmitPulseList = createNfillpulselist(numTransmitPulses,time);
    iutpl_t notherPulseList = createNfillpulselist(numTransmitPulses,time);

    IUS_BOOL equal = iusCompareTransmitPulseList(transmitPulseList,transmitPulseList);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);
    equal = iusCompareTransmitPulseList(transmitPulseList,notherPulseList);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);

    // changing one of the pattern list, should result in a difference
    iutp_t pulse = iusHLTransmitPulseListGet(transmitPulseList,0);
    status = iusHLTransmitPulseListSet(transmitPulseList,pulse,0);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);
    equal = iusCompareTransmitPulseList(notherPulseList,transmitPulseList);
    TEST_ASSERT_EQUAL(IUS_FALSE, equal);

    // invalid params
    status= iusHLTransmitPulseListSet(transmitPulseList,pulse,-1);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE,status);
    status= iusHLTransmitPulseListSet(transmitPulseList,pulse,numTransmitPulses);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE,status);
    status= iusHLTransmitPulseListSet(transmitPulseList,NULL,0);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE,status);
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
    TEST_ASSERT_EQUAL_FLOAT( iusHLParametricPulseGetPulseAmplitude(parametricPulse), pulseAmplitude);
    TEST_ASSERT_EQUAL_FLOAT( iusHLParametricPulseGetCount(parametricPulse),pulseCount);

    // Non Parametric transmit pulse
    // alloc mem
    numPulseValues = 20;

    nonParametricPulse = iusHLCreateNonParametricPulse(numPulseValues);
    fillAmplitudeTime(nonParametricPulse);
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
//    RUN_TEST_CASE(InputfileTransmitPulse, testCreateTransmitPulse);
//    RUN_TEST_CASE(InputfileTransmitPulse, testTransmitPulseListSet);
}
