cmake_minimum_required(VERSION 3.00)
project(example C)
#
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/cmake_modules")
if(CMAKE_COMPILER_IS_GNUCC)
    include(CodeCoverage)
    APPEND_COVERAGE_COMPILER_FLAGS()
    set(COVERAGE_GCOVR_EXCLUDES 'external/CMakeFiles/hashmap.dir/hashmap/src' 'external/CMakeFiles/Unity.dir/Unity/src' 'external/CMakeFiles/Unity.dir/Unity/extras/fixture/src')
endif()
#
#if(MSVC)
#    # disable the security warnings for sprintf() in bee2.c
#    add_definitions(/D _CRT_SECURE_NO_WARNINGS /D _CRT_NONSTDC_NO_DEPRECATE)
#else()
#    set(CMAKE_CXX_CPPCHECK "cppcheck")
#endif()

#set (HASHMAP_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/external/hashmap/src")

set (IUS_INCLUDE_DIR "${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/include")
set (TARGET_GROUP test CACHE STRING "Group to build")
if(TARGET_GROUP STREQUAL production)
    add_subdirectory(examples)
elseif(TARGET_GROUP STREQUAL test)
    include(CTest)
    add_subdirectory(examples)
    add_subdirectory(external)
    enable_testing ()
    add_subdirectory(unitTests)
else()
    message(FATAL_ERROR "Given TARGET_GROUP unknown")
endif()

