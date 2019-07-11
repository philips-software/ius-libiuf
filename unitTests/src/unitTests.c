#include <stdio.h>
#include <unity_fixture.h>
#include <iuf.h>

static void RunAllTests(void)
{
    RUN_TEST_GROUP(Iuf2DNonParametricSource);
    RUN_TEST_GROUP(Iuf2DParametricSource);
    RUN_TEST_GROUP(Iuf2DTransducer);
    RUN_TEST_GROUP(Iuf2DTransducerElement);
    RUN_TEST_GROUP(Iuf2DTransducerElementList);
    RUN_TEST_GROUP(Iuf3DNonParametricSource);
    RUN_TEST_GROUP(Iuf3DParametricSource);
    RUN_TEST_GROUP(Iuf3DTransducer);
    RUN_TEST_GROUP(Iuf3DTransducerElement);
    RUN_TEST_GROUP(Iuf3DTransducerElementList);
    RUN_TEST_GROUP(IufAcquisition);
    //RUN_TEST_GROUP(IufCompoundFile);
    RUN_TEST_GROUP(IufDataStream);
    RUN_TEST_GROUP(IufDataStreamDict);
    RUN_TEST_GROUP(IufDemodulation);
    RUN_TEST_GROUP(IufDemodulationDict);
//    RUN_TEST_GROUP(IufErrorHandling);
    RUN_TEST_GROUP(IufFile);
	RUN_TEST_GROUP(IufFirFilter);
    RUN_TEST_GROUP(IufFrame);
    RUN_TEST_GROUP(IufFrameList);
    RUN_TEST_GROUP(IufInputFile);
	RUN_TEST_GROUP(IufIqFile);
    RUN_TEST_GROUP(IufIqPattern);
    RUN_TEST_GROUP(IufIqPatternList);
    RUN_TEST_GROUP(IufIqPatternListDict);
    RUN_TEST_GROUP(IufHistoryNodeList);
    RUN_TEST_GROUP(IufHistoryNode);
    RUN_TEST_GROUP(IufParameterDict);
    RUN_TEST_GROUP(IufNonParametricPulse);
    RUN_TEST_GROUP(IufParametricPulse);
    RUN_TEST_GROUP(IufPattern);
    RUN_TEST_GROUP(IufPatternList);
    RUN_TEST_GROUP(IufPatternListDict);
    RUN_TEST_GROUP(IufPulse);
    RUN_TEST_GROUP(IufPulseDict);
    RUN_TEST_GROUP(IufReceiveChannelMap);
    RUN_TEST_GROUP(IufReceiveChannelMapDict);
    RUN_TEST_GROUP(IufReceiveSettings);
    RUN_TEST_GROUP(IufReceiveSettingsDict);
    RUN_TEST_GROUP(IufSource);
    RUN_TEST_GROUP(IufSourceDict);
    RUN_TEST_GROUP(IufTGC);
    RUN_TEST_GROUP(IufTransducer);
    RUN_TEST_GROUP(IufTransmitApodization);
    RUN_TEST_GROUP(IufTransmitApodizationDict);
}

int main(int argc, const char * argv[])
{
    iufHDF5ErrorLog(IUF_FALSE);
    iufErrorLog(IUF_FALSE);
    return UnityMain(argc, argv, RunAllTests);
}
