//
// Created by Ruijzendaal on 07/03/2018.
//

#include <stdio.h>
#include <unity_fixture.h>
#include <uniTests.h>

#include <iusTypes.h>
#include <iusUtil.h>


static void RunAllTests(void)
{
//    RUN_TEST_GROUP(Misc);
//    RUN_TEST_GROUP(InputfileTransducer);
//    RUN_TEST_GROUP(InputfileTransmitPulse);
//    RUN_TEST_GROUP(InputfileTransmitPattern);
//    RUN_TEST_GROUP(InputfileTransmitPatternList);
//    RUN_TEST_GROUP(InputfileNode);
//    RUN_TEST_GROUP(IusInputInstance);
//    RUN_TEST_GROUP(IusDrivingScheme);
    RUN_TEST_GROUP(IusInputFile);
    RUN_TEST_GROUP(IusPulse);
    RUN_TEST_GROUP(IusParametricPulse);
    RUN_TEST_GROUP(IusExperiment);
    RUN_TEST_GROUP(IusNonParametricPulse);
    RUN_TEST_GROUP(IusPattern);
    RUN_TEST_GROUP(IusParametricPulse);
    RUN_TEST_GROUP(IusPatternList);
    RUN_TEST_GROUP(IusSource);
    RUN_TEST_GROUP(Ius3DParametricSource);
    RUN_TEST_GROUP(Ius3DNonParametricSource);
    RUN_TEST_GROUP(Ius2DParametricSource);
    RUN_TEST_GROUP(Ius2DNonParametricSource);
    RUN_TEST_GROUP(Ius2DParametricSource);
    RUN_TEST_GROUP(IusSourceDict);
    RUN_TEST_GROUP(IusPulse);
    RUN_TEST_GROUP(IusPulseDict);
	RUN_TEST_GROUP(IusReceiveChannelMap);
    RUN_TEST_GROUP(IusReceiveChannelMapDict);
    RUN_TEST_GROUP(IusTransducer);
    RUN_TEST_GROUP(IusTransducer);
	RUN_TEST_GROUP(IusTransmitApodization);
	RUN_TEST_GROUP(IusTransmitApodizationDict);
}

int main(int argc, const char * argv[])
{
    iusHLDiagDisable();
    return UnityMain(argc, argv, RunAllTests);
}