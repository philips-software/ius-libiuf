
FIND_PACKAGE(HDF5 COMPONENTS C HL)

include_directories(include)
include_directories(${HDF5_INCLUDE_DIR})

add_executable(unitTests
        include/testDataGenerators.h
        src/testDataGenerators.c
        src/unitTests.c
        src/testIusExperiment.c
        src/testIusTransducer.c
        src/testIusParametricPulse.c
        src/testIusNonParametricPulse.c
        src/testIusPulseDict.c
        src/testIusPulse.c
        src/testIusReceiveChannelMap.c
        src/testIusReceiveChannelMapDict.c
        src/testIusTransmitApodization.c
        src/testIusTransmitApodizationDict.c
        src/testIusInputFile.c
        src/util.c
#        src/testNode.c
#        src/testTransducer.c
#        src/util.c
#        src/testMisc.c
#        src/testReceiveSettings.c
#        include/testReceiveSettings.h
#        src/testTransmitPulse.c
#        src/testTransmitPattern.c
#        include/testDataGenerators.h
#        src/testTransmitPatternList.c
#        src/testIusApodizationList.c
#        src/testIusInputInstance.c
#        src/testIusDrivingScheme.c
        )

target_link_libraries(unitTests
        ius
        Unity
        ${HDF5_LIBRARIES}
        ${HDF5_HL_LIBRARIES}
        )

add_test(suite_1_test unitTests)