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

add_subdirectory(dg)

file(GLOB UNITTESTS_SRCS "src/*.c")
add_executable(unitTests ${UNITTESTS_SRCS})

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
    include(CodeCoverage)
    APPEND_COVERAGE_COMPILER_FLAGS()
    SETUP_TARGET_FOR_COVERAGE_GCOVR_XML(
        NAME gcovr-report              # New target name
        EXECUTABLE $ENV{CTEST} unitTests        # Executable in PROJECT_BINARY_DIR
        DEPENDENCIES unitTests         # Dependencies to build first
    )
endif()
