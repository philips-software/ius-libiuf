//
// Created by Ruijzendaal on 04/04/2018.
//



#include <unity_fixture.h>
#include <include/iusTypes.h>
#include <iusHLTransmitPattern.h>
#include <include/ius.h>
#include <include/iusError.h>
#include <include/iusHLTransmitPatternList.h>

TEST_GROUP(InputfileTransmitPattern);

TEST_SETUP(InputfileTransmitPattern)
{
}

TEST_TEAR_DOWN(InputfileTransmitPattern)
{
}

iutpal_t createNfillpatternlist(int size, float baseTime)
{
    int i;
    int pulseIndex;
    int status;
    float time;

    iutpal_t transmitPatternList = iusHLCreateTransmitPatternList(size);
    TEST_ASSERT( transmitPatternList != IUTPAL_INVALID);
    TEST_ASSERT_EQUAL(size, iusHLTransmitPatternListGetSize(transmitPatternList));
    for(i=0; i < size ; i++)
    {
        time = baseTime * i;
        pulseIndex = size - 1 - i;
        status = iusHLTransmitPatternListSet(transmitPatternList,time,pulseIndex,i);
        TEST_ASSERT(status == IUS_E_OK);
    }
    return transmitPatternList;
}

TEST(InputfileTransmitPattern, testTransmitPatternListSet)
{
    int numTransmitPulses = 10;
    int status;
    float time = 0.1f;

    iutpal_t transmitPatternList = createNfillpatternlist(numTransmitPulses,time);
    iutpal_t notherPatternList = createNfillpatternlist(numTransmitPulses,time);

    IUS_BOOL equal = iusCompareTransmitPatternList(transmitPatternList,transmitPatternList);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);
    equal = iusCompareTransmitPatternList(notherPatternList,transmitPatternList);
    TEST_ASSERT_EQUAL(IUS_TRUE, equal);

    // changing one of the pattern list, should result in a difference
    status = iusHLTransmitPatternListSet(transmitPatternList,time,numTransmitPulses,0);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);
    equal = iusCompareTransmitPatternList(notherPatternList,transmitPatternList);
    TEST_ASSERT_EQUAL(IUS_FALSE, equal);

    // invalid params
    status= iusHLTransmitPatternListSet(transmitPatternList,time,1,numTransmitPulses);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE,status);
    status= iusHLTransmitPatternListSet(transmitPatternList,time,1,-1);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE,status);
    status= iusHLTransmitPatternListSet(transmitPatternList,time,-1,0);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE,status);

}

TEST(InputfileTransmitPattern, testCreateTransmitPattern)
{
    int numTransmitPulses = 10;
    iutpal_t transmitPatternList = iusHLCreateTransmitPatternList(numTransmitPulses);
    TEST_ASSERT( transmitPatternList != IUTPAL_INVALID);
    TEST_ASSERT_EQUAL(numTransmitPulses, iusHLTransmitPatternListGetSize(transmitPatternList));
}


TEST_GROUP_RUNNER(InputfileTransmitPattern)
{
    RUN_TEST_CASE(InputfileTransmitPattern, testCreateTransmitPattern);
    RUN_TEST_CASE(InputfileTransmitPattern, testTransmitPatternListSet);
}
