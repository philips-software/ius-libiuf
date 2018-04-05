# The version number.
set (IUS_VERSION_MAJOR 3)
set (IUS_VERSION_MINOR 0)
set (CMAKE_C_STANDARD 99)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
        "library_config.h.in"
        "${PROJECT_BINARY_DIR}/library_config.h"
)

#set (HDF5_ROOT  /usr/local/HDF_Group/HDF5/1.8.20)
#find_package(HDF5)
#FIND_PACKAGE(HDF5 COMPONENTS C HL NO_MODULE REQUIRED static)
#FIND_PACKAGE(HDF5 COMPONENTS C HL NO_MODULE REQUIRED shared)
#FIND_PACKAGE(HDF5)
FIND_PACKAGE(HDF5 COMPONENTS C HL)
include_directories("${PROJECT_BINARY_DIR}")
include_directories(${HDF5_INCLUDE_DIR})


add_library(ius STATIC
        src/iusInput.c
        src/iusInputFile.c
        src/iusUuid.c
        src/iusHLInput.c
        src/iusHLInputFile.c
        src/iusHLExperiment.c
        src/iusHLTransducer.c
        src/iusHLNode.c
        src/iusHDF5.c
        src/iusNode.c
        src/iusUtil.c
        include/iusUtil.h
        include/iusHLExperiment.h
        src/iusHLExperiment.c
        src/iusHLTransducer.c
        src/iusHLDrivingScheme.c
        include/iusHLDrivingScheme.h include/iusHLPosition.h src/iusHLPosition.c include/iusHLTransmitPulse.h src/iusHLTransmitPulse.c include/iusHLTransmitPattern.h src/iusHLTransmitPattern.c include/iusHLTransmitPatternList.h src/iusHLTransmitPatternList.c include/iusHLTransmitPulseList.h src/iusHLTransmitPulseList.c)

target_include_directories(ius PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
        include
        )

target_link_libraries(
        ius
        ${HDF5_LIBRARIES}
        ${HDF5_HL_LIBRARIES}
        -luuid
)




# the option is here so that we can plug it into other
#find_package(HDF5)
#if(HDF5_FOUND)
#    include_directories(${HDF5_INCLUDE_DIR})
#    set(_hdf5_libs ${HDF5_LIBRARIES})
#    target_link_libraries(
#            Untitled
#            ${_hdf5_libs}
#    )
#else()
#    set(HDF5_ROOT ${PROJECT_BINARY_DIR}/hdf5_local)
#    set(LOCAL_HDF5_LIB ${PROJECT_BINARY_DIR}/hdf5_local/build/bin/libhdf5-static.a)
#    set(LOCAL_HDF5_STATIC_LIB ${HDF5_ROOT}/lib/libhdf5-static.a)
#    set(_hdf5_version "1.8.20")
#    set(HDF5_DOWNLOAD_URL https://support.hdfgroup.org/ftp/HDF5/current18/src/hdf5-${_hdf5_version}.tar.gz)
#    if(NOT EXISTS ${LOCAL_HDF5_LIB})
#        # we did not find it so we fetch it from the web, configure, compile, and link
#        set(CURL_COMMAND curl  ${HDF5_DOWNLOAD_URL} | tar xvz -C ${PROJECT_BINARY_DIR}/hdf5_local/src)
#    else()
#        set(CURL_COMMAND "")
#    endif()
#    if(NOT EXISTS ${LOCAL_HDF5_STATIC_LIB})
#        # we did not find it so install
#        set(INSTALL_CMD make install)
#    else()
#        set(INSTALL_CMD "")
#    endif()
#    include(ExternalProject)
#    set(ExternalProjectCMakeArgs
#            -DHDF5_BUILD_CPP_LIB=ON
#            -DCMAKE_INSTALL_PREFIX=${HDF5_ROOT}
#            )
#    file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/hdf5_local/src)
#    ExternalProject_Add(hdf5_local
#            DOWNLOAD_COMMAND ${CURL_COMMAND}
#            CMAKE_ARGS ${ExternalProjectCMakeArgs}
#            SOURCE_DIR ${PROJECT_BINARY_DIR}/hdf5_local/src/hdf5-${_hdf5_version}
#            BINARY_DIR ${PROJECT_BINARY_DIR}/hdf5_local/build
#            STAMP_DIR  ${PROJECT_BINARY_DIR}/hdf5_local/stamp
#            TMP_DIR    ${PROJECT_BINARY_DIR}/hdf5_local/tmp
#            INSTALL_COMMAND ${INSTALL_CMD}
#            )
#
#    #    find_package(HDF5)
#    #    include_directories(${HDF5_INCLUDE_DIR})
#    set(_hdf5_libs ${HDF5_LIBRARIES})
#    include_directories("${PROJECT_BINARY_DIR}")
#
#    target_link_libraries(
#            Untitled
#            ${_hdf5_libs}
#    )
#
#    add_library(Untitled STATIC
#            library.c
#            )
#
#    target_include_directories(Untitled PUBLIC
#            ${CMAKE_CURRENT_LIST_DIR}
#            )
#
#
#    add_dependencies(Untitled hdf5_local)
#endif()
