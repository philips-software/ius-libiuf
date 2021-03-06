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
#
if(MSVC)
    # disable the security warnings for sprintf()
    add_definitions(/D _CRT_SECURE_NO_WARNINGS /D _CRT_NONSTDC_NO_DEPRECATE)
    set(HDF5_USE_STATIC_LIBRARIES "TRUE")
else()
    set(UUID_LIBRARIES -luuid)
    set(CMAKE_CXX_CPPCHECK "cppcheck")
endif()

set (IUF_INCLUDE_DIR "${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/include")
FILE(READ ${CMAKE_SOURCE_DIR}/library/${CMAKE_SYSTEM_NAME}/VERSION IUF_LIB_VERSION_STRING)
if(IUF_LIB_VERSION_STRING)
    set (IUF_LIBRARIES iuf-${IUF_LIB_VERSION_STRING})
else()
    message( FATAL_ERROR "Error: Unable to read ${CMAKE_SOURCE_DIR}/library/VERSION" )
endif()

FIND_PACKAGE(HDF5 1.8 COMPONENTS C HL REQUIRED)

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

