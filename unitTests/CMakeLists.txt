
FIND_PACKAGE(HDF5 COMPONENTS C HL)

include_directories(include)
include_directories(${HDF5_INCLUDE_DIR})

add_executable(unitTests
        include/testDataGenerators.h
        src/testDataGenerators.c
        src/unitTests.c
        src/testIus2DNonParametricSource.c
        src/testIus2DParametricSource.c
        src/testIus2DTransducer.c
        src/testIus2DTransducerElement.c
        src/testIus2DTransducerElementList.c
        src/testIus3DNonParametricSource.c
        src/testIus3DParametricSource.c
        src/testIus3DTransducer.c
        src/testIus3DTransducerElement.c
        src/testIus3DTransducerElementList.c
        src/testIusExperiment.c
        src/testIusFile.c
        src/testIusFrame.c
        src/testIusFrameList.c
        src/testIusInputFile.c
        src/testIusHistoryNode.c
        src/testIusNonParametricPulse.c
        src/testIusParameterDict.c
        src/testIusParametricPulse.c
        src/testIusPattern.c
        src/testIusPatternList.c
        src/testIusPulseDict.c
        src/testIusPulse.c
        src/testIusReceiveSettings.c
        src/testIusReceiveSettingsDict.c
        src/testIusReceiveChannelMap.c
        src/testIusReceiveChannelMapDict.c
        src/testIusSourceDict.c
        src/testIusSource.c
        src/testIusTransducer.c
        src/testIusTransmitApodization.c
        src/testIusTransmitApodizationDict.c
        src/testIusTGC.c
        src/util.c
        )

target_link_libraries(unitTests
        ius
        Unity
        ${HDF5_LIBRARIES}
        ${HDF5_HL_LIBRARIES}
        )

add_test(suite_1_test unitTests)