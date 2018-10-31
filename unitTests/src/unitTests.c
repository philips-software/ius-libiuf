//
// Created by Ruijzendaal on 07/03/2018.
//

#include <stdio.h>
#include <unity_fixture.h>
#include <iusUtil.h>

static void RunAllTests(void)
{
    RUN_TEST_GROUP(Ius2DNonParametricSource);
    RUN_TEST_GROUP(Ius2DParametricSource);
    RUN_TEST_GROUP(Ius2DTransducer);
    RUN_TEST_GROUP(Ius2DTransducerElement);
    RUN_TEST_GROUP(Ius2DTransducerElementList);
    RUN_TEST_GROUP(Ius3DNonParametricSource);
    RUN_TEST_GROUP(Ius3DParametricSource);
    RUN_TEST_GROUP(Ius3DTransducer);
    RUN_TEST_GROUP(Ius3DTransducerElement);
    RUN_TEST_GROUP(Ius3DTransducerElementList);
    RUN_TEST_GROUP(IusDataStream);
    RUN_TEST_GROUP(IusDataStreamDict);
    RUN_TEST_GROUP(IusAcquisition);
    RUN_TEST_GROUP(IusFrame);
    RUN_TEST_GROUP(IusFrameList);
//    RUN_TEST_GROUP(IusInputFile);
//    RUN_TEST_GROUP(IusFile);
//    RUN_TEST_GROUP(IusHistoryNodeList);
//    RUN_TEST_GROUP(IusHistoryNode);
//    RUN_TEST_GROUP(IusParameterDict);
//    RUN_TEST_GROUP(IusNonParametricPulse);
//    RUN_TEST_GROUP(IusParametricPulse);
//    RUN_TEST_GROUP(IusPattern);
//    RUN_TEST_GROUP(IusPatternList);
//    RUN_TEST_GROUP(IusPatternListDict);
    RUN_TEST_GROUP(IusPulse);
    RUN_TEST_GROUP(IusPulseDict);
    RUN_TEST_GROUP(IusReceiveChannelMap);
    RUN_TEST_GROUP(IusReceiveChannelMapDict);
    RUN_TEST_GROUP(IusReceiveSettings);
    RUN_TEST_GROUP(IusReceiveSettingsDict);
    RUN_TEST_GROUP(IusSource);
    RUN_TEST_GROUP(IusSourceDict);
    RUN_TEST_GROUP(IusTGC);
    RUN_TEST_GROUP(IusTransducer);
    RUN_TEST_GROUP(IusTransmitApodization);
    RUN_TEST_GROUP(IusTransmitApodizationDict);
}

int main(int argc, const char * argv[])
{
    iusDiagDisable();
    return UnityMain(argc, argv, RunAllTests);
}
