include_directories(${CMAKE_SOURCE_DIR}/external/Unity/src)
add_library(dg STATIC
        dataGenerators.c
        dataGenerators.h)

