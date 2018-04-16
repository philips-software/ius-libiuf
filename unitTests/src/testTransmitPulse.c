//
// Created by Ruijzendaal on 30/03/2018.
//

#include <unity_fixture.h>
#include <include/ius.h>
#include <include/iusTypes.h>
#include <include/iusError.h>
#include <include/iusHLNonParametricTransmitPulse.h>
#include <include/iusHLParametricTransmitPulse.h>

TEST_GROUP(InputfileTransmitPulse);

TEST_SETUP(InputfileTransmitPulse)
{
}

TEST_TEAR_DOWN(InputfileTransmitPulse)
{
}

int nonParametricPulseFillAmplitudeTime2
(
    iunptp_t transmitPulse,
    float offset
)
{
    int i;
    int status=0;
    float pulseAmplitude;
    float pulseTimes;
    int numPulseValues = iusHLNonParametricPulseGetNumValues(transmitPulse);
    for(i=0; i< numPulseValues; i++)
    {
        pulseAmplitude = offset / ((i % 10) + 1.0f);
        pulseTimes = offset / ((i % 10) + 1.0f);
        status = iusHLNonParametricPulseSetAmplitudeTime(transmitPulse,pulseTimes,pulseAmplitude,i);
        if( status != IUS_E_OK )
        {
            return status;
        }
    }
    return status;
}

//
//int fillNonParametricPulseList
//(
//    iunptpl_t transmitPulseList,
//    float offset
//)
//{
//    int i;
//    int status=0;
//    int numPulseCount  = iusHLNonParametricTransmitPulseListGetSize(transmitPulseList);
//    for(i=0; i< numPulseCount; i++)
//    {
//        iunptp_t transmitPulse = iusHLNonParametricTransmitPulseListGet(transmitPulseList,i);
//        status = nonParametricPulseFillAmplitudeTime( transmitPulse, offset );
//        if( status != IUS_E_OK )
//        {
//            return status;
//        }
//    }
//    return status;
//}
//
//iunptpl_t createNfillNonParametricPulseList
//(
//    int pulses,
//    float offset
//)
//{
//    int valueCount = 10;
//    iunptpl_t transmitPulseList = iusHLCreateNonParametricTransmitPulseList(pulses,valueCount);
//    int status = fillNonParametricPulseList(transmitPulseList, offset);
//    TEST_ASSERT_EQUAL(IUS_E_OK,status);
//    return transmitPulseList;
//}
//
//int fillParametricPulseList
//(
//    iuptpl_t transmitPulseList,
//    float offset
//)
//{
//    int i;
//    int status=0;
//    float pulseAmplitude;          /**< shape of waveform [in Volts] */
//    float pulseFrequency;               /**< corresponding timestamps of amplitudes [in seconds] */
//    int numPulseCount  = iusHLParametricTransmitPulseListGetSize(transmitPulseList)    ;
//    int pulseCount;
//
//    for(i=0; i< numPulseCount; i++)
//    {
//        pulseAmplitude = offset/((i%10)+1.0f);
//        pulseFrequency = offset/((i%10)+1.0f);
//        pulseCount = 10;
//        status = iusHLParametricTransmitPulseListSet( (iuptpl_t) transmitPulseList, pulseFrequency, pulseAmplitude, pulseCount, i );
//        if( status != IUS_E_OK )
//        {
//            return status;
//        }
//    }
//    return status;
//}
//
//iuptpl_t createNfillParametricPulseList(int pulses, float offset){
//    iuptpl_t transmitPulseList = iusHLCreateParametricTransmitPulseList(pulses);
//    int status = fillParametricPulseList(transmitPulseList,offset);
//    TEST_ASSERT_EQUAL(IUS_E_OK,status);
//    return transmitPulseList;
//}
//
//TEST(InputfileTransmitPulse, testTransmitParametricPulseListSet)
//{
//    int status;
//    int     pulseCount=10;               /**< number of cycles that the pulse represents */
//    float offset = 1.0f;
//
//
//    iuptpl_t transmitPulseList = createNfillParametricPulseList(pulseCount,offset);
//    iuptpl_t notherPulseList = createNfillParametricPulseList(pulseCount,offset);
//
//    IUS_BOOL equal = iusCompareParametricTransmitPulseList( transmitPulseList, transmitPulseList);
//    TEST_ASSERT_EQUAL(IUS_TRUE, equal);
//    equal = iusCompareParametricTransmitPulseList(transmitPulseList, notherPulseList);
//    TEST_ASSERT_EQUAL(IUS_TRUE, equal);
//
//    // changing one of the pattern list, should result in a difference
//    status = iusHLParametricTransmitPulseListSet( transmitPulseList, 0.1f, 0.1f, 10, 0 );
//    TEST_ASSERT_EQUAL(IUS_E_OK, status);
//    equal = iusCompareParametricTransmitPulseList(notherPulseList, transmitPulseList);
//    TEST_ASSERT_EQUAL(IUS_FALSE, equal);
//
//    // invalid params
//    status = iusHLParametricTransmitPulseListSet( NULL, 0.1f, 0.1f, 10, 0 );
//    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
//    status = iusHLParametricTransmitPulseListSet( transmitPulseList, -0.1f, 0.1f, 10, 0 );
//    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
//    status = iusHLParametricTransmitPulseListSet( transmitPulseList, 0.1f, 0.1f, -10, 0 );
//    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
//    status = iusHLParametricTransmitPulseListSet( transmitPulseList, 0.1f, 0.1f, 10, -1 );
//    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
//
//}
//
//TEST(InputfileTransmitPulse, testTransmitNonParametricPulseListSet)
//{
//    iuptp_t  parametricPulse;
//    float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
//    float   pulseAmplitude=800.0f;       /**< (max) amplitude of the pulse in Volts */
//    int     pulseCount=10;               /**< number of cycles that the pulse represents */
//    int numTransmitPulses = 100;
//    int status;
//    float offset = 1.0f;
//
//
//    iunptpl_t transmitPulseList = createNfillNonParametricPulseList(numTransmitPulses,offset);
//    iunptpl_t notherPulseList = createNfillNonParametricPulseList(numTransmitPulses,offset);
//
//    IUS_BOOL equal = iusCompareNonParametricTransmitPulseList( transmitPulseList, transmitPulseList);
//    TEST_ASSERT_EQUAL(IUS_TRUE, equal);
//    equal = iusCompareNonParametricTransmitPulseList(transmitPulseList, notherPulseList);
//    TEST_ASSERT_EQUAL(IUS_TRUE, equal);
//
//    // changing one of the pattern list, should result in a difference
//    iunptp_t pulse = iusHLNonParametricTransmitPulseListGet(transmitPulseList,0);
//    TEST_ASSERT(pulse != NULL);
//
//    status = nonParametricPulseFillAmplitudeTime(pulse,offset+1.0f);
//    TEST_ASSERT_EQUAL(IUS_E_OK, status);
//    status = iusHLNonParametricTransmitPulseListSet( transmitPulseList, pulse, 0);
//    TEST_ASSERT_EQUAL(IUS_E_OK, status);
//    equal = iusCompareNonParametricTransmitPulseList(notherPulseList, transmitPulseList);
//    TEST_ASSERT_EQUAL(IUS_FALSE, equal);
//
//    // invalid params
//    parametricPulse = iusHLCreateParametricPulse(pulseFrequency,pulseAmplitude,pulseCount);
//    status = iusHLNonParametricTransmitPulseListSet( (iunptpl_t) parametricPulse, pulse, 0);
//    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
//    status = iusHLNonParametricTransmitPulseListSet( (iunptpl_t) NULL, pulse, 0);
//    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
//    status = iusHLNonParametricTransmitPulseListSet( transmitPulseList, NULL, 0);
//    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
//    status = iusHLNonParametricTransmitPulseListSet( transmitPulseList, pulse, -1);
//    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
//
//}


TEST(InputfileTransmitPulse, testCompareTransmitPulse)
{
    int status = 0;
    // Transmit parameters
    float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude=800.0f;       /**< (max) amplitude of the pulse in Volts */
    int     pulseCount=10;               /**< number of cycles that the pulse represents */
    int     numPulseValues=20;              /**< number of points to describe waveform, 0 implies a parametric description only */
    iuptp_t  parametricPulse,notherParametricPulse;
    iunptp_t nonParametricPulse,notherNonParametricPulse;
    IUS_BOOL isEqual;


    // Parametric transmit pulse
    parametricPulse = iusHLCreateParametricPulse(pulseFrequency,pulseAmplitude,pulseCount);
    notherParametricPulse = iusHLCreateParametricPulse(pulseFrequency,pulseAmplitude,pulseCount);

    nonParametricPulse = iusHLCreateNonParametricPulse(numPulseValues);
    notherNonParametricPulse = iusHLCreateNonParametricPulse(numPulseValues);

    isEqual = iusCompareNonParametricPulse(nonParametricPulse,nonParametricPulse);
    TEST_ASSERT_EQUAL(IUS_TRUE,isEqual);
    isEqual = iusCompareNonParametricPulse(nonParametricPulse,notherNonParametricPulse);
    TEST_ASSERT_EQUAL(IUS_TRUE,isEqual);
    isEqual = iusCompareParametricPulse(parametricPulse,parametricPulse);
    TEST_ASSERT_EQUAL(IUS_TRUE,isEqual);
    isEqual = iusCompareParametricPulse(parametricPulse,notherParametricPulse);
    TEST_ASSERT_EQUAL(IUS_TRUE,isEqual);

    // Invalid arguments
    isEqual = iusCompareParametricPulse(parametricPulse,(iuptp_t) nonParametricPulse);
    TEST_ASSERT_EQUAL(IUS_FALSE,isEqual);
    isEqual = iusCompareNonParametricPulse((iunptp_t) parametricPulse, nonParametricPulse);
    TEST_ASSERT_EQUAL(IUS_FALSE,isEqual);
    isEqual = iusCompareParametricPulse(NULL,NULL);
    TEST_ASSERT_EQUAL(IUS_TRUE,isEqual);
    isEqual = iusCompareParametricPulse(NULL,parametricPulse);
    TEST_ASSERT_EQUAL(IUS_FALSE,isEqual);
    isEqual = iusCompareParametricPulse(parametricPulse,NULL);
    TEST_ASSERT_EQUAL(IUS_FALSE,isEqual);
    isEqual = iusCompareNonParametricPulse(NULL,NULL);
    TEST_ASSERT_EQUAL(IUS_TRUE,isEqual);
    isEqual = iusCompareNonParametricPulse(NULL,nonParametricPulse);
    TEST_ASSERT_EQUAL(IUS_FALSE,isEqual);
    isEqual = iusCompareNonParametricPulse(nonParametricPulse,NULL);
    TEST_ASSERT_EQUAL(IUS_FALSE,isEqual);


    status = iusHLDeleteNonParametricPulse(nonParametricPulse);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    status = iusHLDeleteParametricPulse(parametricPulse);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
}

TEST(InputfileTransmitPulse, testCreateDeleteTransmitPulse)
{
    int status = 0;
    // Transmit parameters
    float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude=800.0f;       /**< (max) amplitude of the pulse in Volts */
    int     pulseCount=10;               /**< number of cycles that the pulse represents */
    int     numPulseValues;              /**< number of points to describe waveform, 0 implies a parametric description only */
    iuptp_t  parametricPulse;
    iunptp_t nonParametricPulse;


    // Parametric transmit pulse
    parametricPulse = iusHLCreateParametricPulse(pulseFrequency,pulseAmplitude,pulseCount);

    TEST_ASSERT_EQUAL_FLOAT( iusHLParametricPulseGetFrequency(parametricPulse), pulseFrequency);
    TEST_ASSERT_EQUAL_FLOAT( iusHLParametricPulseGetPulseAmplitude(parametricPulse), pulseAmplitude);
    TEST_ASSERT_EQUAL_FLOAT( iusHLParametricPulseGetCount(parametricPulse),pulseCount);

    // Non Parametric transmit pulse
    // alloc mem
    numPulseValues = 20;
    nonParametricPulse = iusHLCreateNonParametricPulse(numPulseValues);
    nonParametricPulseFillAmplitudeTime2( nonParametricPulse, 1.0f );
    TEST_ASSERT(nonParametricPulse != IUNPTP_INVALID);

    // Invalid operation on nonparametric dta type
    TEST_ASSERT_EQUAL(NULL, iusHLCreateNonParametricPulse(-1));
    TEST_ASSERT_EQUAL(NULL, iusHLCreateParametricPulse(pulseFrequency,pulseAmplitude,-1));
    TEST_ASSERT_EQUAL(NULL, iusHLCreateParametricPulse(-1.0f,pulseAmplitude,pulseCount));
    TEST_ASSERT_EQUAL_FLOAT(iusHLParametricPulseGetFrequency( (iuptp_t) nonParametricPulse), NAN);
    TEST_ASSERT_EQUAL_FLOAT(iusHLParametricPulseGetPulseAmplitude( (iuptp_t) nonParametricPulse), NAN);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, iusHLParametricPulseGetCount( (iuptp_t) nonParametricPulse));


    status = iusHLDeleteNonParametricPulse( (iunptp_t) parametricPulse);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
    status = iusHLDeleteParametricPulse( (iuptp_t) nonParametricPulse);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);

    status = iusHLDeleteNonParametricPulse(nonParametricPulse);
    TEST_ASSERT(status == IUS_E_OK);
    status = iusHLDeleteParametricPulse(parametricPulse);
    TEST_ASSERT(status == IUS_E_OK);
}


TEST_GROUP_RUNNER(InputfileTransmitPulse)
{
    RUN_TEST_CASE(InputfileTransmitPulse, testCreateDeleteTransmitPulse);
    RUN_TEST_CASE(InputfileTransmitPulse,testCompareTransmitPulse)
}
