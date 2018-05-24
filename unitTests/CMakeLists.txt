include_directories(include)

add_executable(unitTests
#        src/testDataGenerators.c
        src/unitTests.c
        src/testIusExperiment.c
        src/testIusTransducer.c
        src/testIusParametricPulse.c
        src/testIusNonParametricPulse.c
        src/testIusPulseList.c
        src/testIusPulseDict.c
        src/util.c
#        src/testNode.c
#        src/testTransducer.c
#        src/util.c
#        src/testMisc.c
#        src/testInputFile.c
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
        )

add_test(suite_1_test unitTests)