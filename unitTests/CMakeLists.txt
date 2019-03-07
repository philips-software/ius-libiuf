include_directories(include)
include_directories(${HDF5_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/library/include)
include_directories(${CMAKE_SOURCE_DIR}/library/private)
include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/library/${CMAKE_SYSTEM_NAME})

#add_library(dg STATIC
#        include/dataGenerators.h
#        src/dataGenerators.c)
add_subdirectory(dg)


add_executable(unitTests
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
        src/testIusAcquisition.c
        src/testIusDataStream.c
        src/testIusDataStreamDict.c
		src/testIusDemodulation.c
        src/testIusDemodulationDict.c
        src/testIusErrorHandling.c
        src/testIusFile.c
        src/testIusFilter.c
        src/testIusFrame.c
        src/testIusFrameList.c
        src/testIusInputFile.c
		src/testIusIqFile.c
        src/testIusIqPattern.c
        src/testIusIqPatternList.c
        src/testIusIqPatternListDict.c
        src/testIusHistoryNode.c
        src/testIusHistoryNodeList.c
        src/testIusNonParametricPulse.c
        src/testIusParameterDict.c
        src/testIusParametricPulse.c
        src/testIusPattern.c
        src/testIusPatternList.c
        src/testIusPatternListDict.c
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

if(MSVC)
    # disable the security warnings for sprintf() in bee2.c
    add_definitions(/D _CRT_SECURE_NO_WARNINGS /D _CRT_NONSTDC_NO_DEPRECATE)
else()
    set(CMAKE_CXX_CPPCHECK "cppcheck")
endif()

if(MSVC)
    target_compile_options(unitTests PRIVATE /W4 /WX)
    set_target_properties(unitTests PROPERTIES LINK_FLAGS "/ignore:4099 /ignore:4098")
else()
    target_compile_options(unitTests PRIVATE -Wall -Wextra -pedantic -Werror)
    if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
        target_compile_options(unitTests PRIVATE -Wall -Wextra -pedantic -Werror)
    endif()
endif()

target_link_libraries(unitTests
        ${IUS_LIBRARIES}
        Unity
        ${HDF5_LIBRARIES}
        ${HDF5_HL_LIBRARIES}
        ${UUID_LIBRARIES}
        dg
        )

set_property(TARGET unitTests PROPERTY C_STANDARD 99)

add_test(suite_1_test unitTests)

if(CMAKE_COMPILER_IS_GNUCC)
    SETUP_TARGET_FOR_COVERAGE_GCOVR_XML(
        NAME gcovr-report                    # New target name
        EXECUTABLE ctest3 -j ${PROCESSOR_COUNT} # Executable in PROJECT_BINARY_DIR
        DEPENDENCIES unitTests         # Dependencies to build first
    )
endif()

