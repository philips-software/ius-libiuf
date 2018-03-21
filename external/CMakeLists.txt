cmake_minimum_required(VERSION 3.6)
add_library(Unity STATIC
        Unity/src/unity.c
        Unity/extras/fixture/src/unity_fixture.c
        )

target_include_directories(Unity PUBLIC
        Unity/src
        Unity/extras/fixture/src
        )

# the option is here so that we can plug it into other
#set (HDF5_ROOT  /usr/local/HDF_Group/HDF5/1.8.20)
#find_package(HDF5)
#if(NOT HDF5_FOUND)
#    set (HDF5_ROOT  ${PROJECT_BINARY_DIR}/hdf5_local)
#    add_subdirectory(hdf5_local)
#endif()
