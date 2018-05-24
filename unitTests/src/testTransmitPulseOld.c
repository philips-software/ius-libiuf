//
// Created by Ruijzendaal on 30/03/2018.
//

#include <unity_fixture.h>
#include <include/ius.h>
#include <include/iusTypes.h>
#include <include/iusError.h>
#include <include/iusHLNonParametricPulse.h>
#include <include/iusHLParametricPulse.h>

TEST_GROUP(InputfileTransmitPulse);

TEST_SETUP(InputfileTransmitPulse)
{
}

TEST_TEAR_DOWN(InputfileTransmitPulse)
{
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




TEST_GROUP_RUNNER(InputfileTransmitPulse)
{
}
