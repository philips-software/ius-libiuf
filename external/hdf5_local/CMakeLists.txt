#set (HDF5_ROOT             ${PROJECT_BINARY_DIR}/hdf5_local)
set (LOCAL_HDF5_LIB        ${HDF5_ROOT}/build/bin/libhdf5-static.a)
set (LOCAL_HDF5_STATIC_LIB ${HDF5_ROOT}/lib/libhdf5-static.a)
set (HDF5_VERSION          "1.8.20")
set (HDF5_DOWNLOAD_URL     https://support.hdfgroup.org/ftp/HDF5/current18/src/hdf5-${HDF5_VERSION}.tar.gz)

if(NOT EXISTS ${LOCAL_HDF5_LIB})
    # we did not find it so we fetch it from the web, configure, compile, and link
    set(CURL_COMMAND curl  ${HDF5_DOWNLOAD_URL} | tar xvz -C ${PROJECT_BINARY_DIR}/hdf5_local/src)
else()
    set(CURL_COMMAND "")
endif()

if(NOT EXISTS ${LOCAL_HDF5_STATIC_LIB})
    # we did not find it so install
    set(INSTALL_CMD make install)
else()
    set(INSTALL_CMD "")
endif()

include(ExternalProject)
set (
        ExternalProjectCMakeArgs
        -DHDF5_BUILD_CPP_LIB=ON
        -DCMAKE_INSTALL_PREFIX=${HDF5_ROOT}
     )

file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/hdf5_local/src)
ExternalProject_Add(hdf5_local
        DOWNLOAD_COMMAND ${CURL_COMMAND}
        CMAKE_ARGS ${ExternalProjectCMakeArgs}
        SOURCE_DIR ${PROJECT_BINARY_DIR}/hdf5_local/src/hdf5-${HDF5_VERSION}
        BINARY_DIR ${PROJECT_BINARY_DIR}/hdf5_local/build
        STAMP_DIR  ${PROJECT_BINARY_DIR}/hdf5_local/stamp
        TMP_DIR    ${PROJECT_BINARY_DIR}/hdf5_local/tmp
        INSTALL_COMMAND ${INSTALL_CMD}
        )