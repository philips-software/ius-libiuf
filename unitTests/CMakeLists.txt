include_directories(include)
include_directories(${HDF5_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/library/include)
include_directories(${CMAKE_SOURCE_DIR}/library/private)
include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

if(IS_DIRECTORY "${CMAKE_SOURCE_DIR}/library/${CMAKE_SYSTEM_NAME}")
    link_directories(${CMAKE_SOURCE_DIR}/library/${CMAKE_SYSTEM_NAME})
endif()

#add_library(dg STATIC
#        include/dataGenerators.h
#        src/dataGenerators.c)
add_subdirectory(dg)


add_executable(unitTests
        src/unitTests.c
        src/testIuf2DNonParametricSource.c
        src/testIuf2DParametricSource.c
        src/testIuf2DTransducer.c
        src/testIuf2DTransducerElement.c
        src/testIuf2DTransducerElementList.c
        src/testIuf3DNonParametricSource.c
        src/testIuf3DParametricSource.c
        src/testIuf3DTransducer.c
        src/testIuf3DTransducerElement.c
        src/testIuf3DTransducerElementList.c
        src/testIufAcquisition.c
        src/testIufDataStream.c
        src/testIufDataStreamDict.c
		src/testIufDemodulation.c
        src/testIufDemodulationDict.c
        src/testIufErrorHandling.c
        src/testIufFile.c
        src/testIufFilter.c
        src/testIufFrame.c
        src/testIufFrameList.c
        src/testIufInputFile.c
		src/testIufIqFile.c
        src/testIufIqPattern.c
        src/testIufIqPatternList.c
        src/testIufIqPatternListDict.c
        src/testIufHistoryNode.c
        src/testIufHistoryNodeList.c
        src/testIufNonParametricPulse.c
        src/testIufParameterDict.c
        src/testIufParametricPulse.c
        src/testIufPattern.c
        src/testIufPatternList.c
        src/testIufPatternListDict.c
        src/testIufPulseDict.c
        src/testIufPulse.c
        src/testIufReceiveSettings.c
        src/testIufReceiveSettingsDict.c
        src/testIufReceiveChannelMap.c
        src/testIufReceiveChannelMapDict.c
        src/testIufSourceDict.c
        src/testIufSource.c
        src/testIufTransducer.c
        src/testIufTransmitApodization.c
        src/testIufTransmitApodizationDict.c
        src/testIufTGC.c
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
        ${IUF_LIBRARIES}
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
        NAME gcovr-report              # New target name
        EXECUTABLE $ENV{CTEST}         # Executable in PROJECT_BINARY_DIR
        DEPENDENCIES unitTests         # Dependencies to build first
    )
endif()

