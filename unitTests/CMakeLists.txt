include_directories(include)

add_executable(unitTests
        src/unitTests.c
        src/testNode.c
        src/testTransducer.c
        src/util.c
        src/testMisc.c
        src/testInputFile.c
        src/testReceiveSettings.c
        include/testReceiveSettings.h
        src/testDrivingScheme.c
        include/testDrivingScheme.h src/testTransmitPulse.c src/testTransmitPattern.c src/testDataGenerators.c include/testDataGenerators.h src/testTransmitPatternList.c src/testTransmiapodization.c)

target_link_libraries(unitTests
        ius
        Unity
        )

add_test(suite_1_test unitTests)