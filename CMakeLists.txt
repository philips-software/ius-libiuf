cmake_minimum_required(VERSION 3.00)
project(iufLibrary C)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/cmake_modules" ${CMAKE_MODULE_PATH})
if(CMAKE_COMPILER_IS_GNUCC)
    include(CodeCoverage)
    APPEND_COVERAGE_COMPILER_FLAGS()
    set(COVERAGE_GCOVR_EXCLUDES 'external/CMakeFiles/hashmap.dir/hashmap/src' 'external/CMakeFiles/Unity.dir/Unity/src' 'external/CMakeFiles/Unity.dir/Unity/extras/fixture/src')
endif()

if(MSVC)
    # disable the security warnings for sprintf()
    add_definitions(/D _CRT_SECURE_NO_WARNINGS /D _CRT_NONSTDC_NO_DEPRECATE)
    set(HDF5_USE_STATIC_LIBRARIES "TRUE")
else()
    if(UNIX AND NOT APPLE)
        set(UUID_LIBRARIES -luuid)
    endif()
    set(CMAKE_CXX_CPPCHECK "cppcheck")
endif()

set (IUF_LIBRARIES iuf)
set (HASHMAP_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/external/hashmap/src")
set (HASHMAP_SOURCES "${PROJECT_SOURCE_DIR}/external/hashmap/src/hashmap.c")
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

add_subdirectory(library)

if(NOT CMAKE_CONFIG STREQUAL Debug)
#    add_subdirectory(python)
#    install(DIRECTORY python DESTINATION ${PROJECT_BINARY_DIR}/dist)
endif()

# Distribution stuff
install(DIRECTORY library/include DESTINATION ${PROJECT_BINARY_DIR}/dist
        PATTERN "library/include/*[pP]rivate*.h" EXCLUDE
        )
install(DIRECTORY examples DESTINATION ${PROJECT_BINARY_DIR}/dist)
install(DIRECTORY unitTests DESTINATION ${PROJECT_BINARY_DIR}/dist)
file(GLOB_RECURSE PRIVATE_HEADERS
        "${PROJECT_SOURCE_DIR}/library/private/*[pP]rivate.h"
        )
install(FILES ${PRIVATE_HEADERS}  DESTINATION ${PROJECT_BINARY_DIR}/dist/unitTests/include)
install(FILES package/README.md package/CMakeLists.txt package/PreLoad.cmake  DESTINATION ${PROJECT_BINARY_DIR}/dist)
install(DIRECTORY ci DESTINATION ${PROJECT_BINARY_DIR}/dist
        USE_SOURCE_PERMISSIONS
        PATTERN "ci/bin/dist.*" EXCLUDE
        PATTERN "ci/bin/mksdk.*" EXCLUDE
        )

install(DIRECTORY examples DESTINATION ${PROJECT_BINARY_DIR}/dist)
install(DIRECTORY external DESTINATION ${PROJECT_BINARY_DIR}/dist)
