cmake_minimum_required(VERSION 3.00)
project(iusLibrary C)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/cmake_modules")

if(MSVC)
    # disable the security warnings for sprintf() in bee2.c
    add_definitions(/D _CRT_SECURE_NO_WARNINGS /D _CRT_NONSTDC_NO_DEPRECATE)
else()
    set(CMAKE_CXX_CPPCHECK "cppcheck")
endif()

#set (UTHASH_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/external/uthash/include")
set (HASHMAP_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/external/hashmap/src")
set (TARGET_GROUP test CACHE STRING "Group to build")
if(TARGET_GROUP STREQUAL production)
    add_subdirectory(main)
elseif(TARGET_GROUP STREQUAL test)
    include(CTest)
    add_subdirectory(external)
    enable_testing ()
    add_subdirectory(unitTests)
else()
    message(FATAL_ERROR "Given TARGET_GROUP unknown")
endif()

add_subdirectory(library)
