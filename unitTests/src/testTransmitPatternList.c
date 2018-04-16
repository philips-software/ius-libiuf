//
// Created by Ruijzendaal on 16/04/2018.
//


#include <unity_fixture.h>
#include <include/iusHLDrivingScheme.h>
#include <include/ius.h>
#include <testDataGenerators.h>

TEST_GROUP(InputfileTransmitPatternList);

TEST_SETUP(InputfileTransmitPatternList)
{
}

TEST_TEAR_DOWN(InputfileTransmitPatternList)
{
}

TEST(InputfileTransmitPatternList, testIusHLTransmitPatternList)
{
    iuds_t drivingScheme;
    int numTransmitPulses = 13;
    iutpal_t patternList;
    int i;
    int status;
    float transmitTime;
    int pulseIndex,sourceIndex;
    iutpa_t pattern;


    patternList = iusHLCreateTransmitPatternList(numTransmitPulses);
    TEST_ASSERT_NOT_EQUAL(IUTPAL_INVALID,patternList);
    TEST_ASSERT_EQUAL(numTransmitPulses,iusHLTransmitPatternListGetSize(patternList));

    for( i=0; i<numTransmitPulses; i++ )
    {
        transmitTime = i *1.0f;
        pulseIndex = i;
        sourceIndex = i;
        status = iusHLTransmitPatternListSet( patternList, transmitTime, sourceIndex, pulseIndex, i);
        TEST_ASSERT_EQUAL(IUS_E_OK,status);

        pattern = iusHLTransmitPatternListGet( patternList, i);
        TEST_ASSERT_EQUAL(sourceIndex,iusHLTransmitPatternGetSourceIndex(pattern));
        TEST_ASSERT_EQUAL(pulseIndex,iusHLTransmitPatternGetPulseIndex(pattern));
        TEST_ASSERT_EQUAL_FLOAT(transmitTime,iusHLTransmitPatternGetTime(pattern));
    }
    iusHLDeleteTransmitPatternList(patternList);
}

TEST_GROUP_RUNNER(InputfileTransmitPatternList)
{
    RUN_TEST_CASE(InputfileTransmitPatternList, testIusHLTransmitPatternList);
}
