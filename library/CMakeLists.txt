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
FIND_PACKAGE(HDF5 1.8 COMPONENTS C HL REQUIRED)
#FIND_PACKAGE(UUID)
#if(UUID_FOUND)
#else(UUID_FOUND)
#endif(UUID_FOUND)

if(NOT MSVC)
	set(UUID_LIBRARIES -luuid)
endif()

include_directories("${PROJECT_BINARY_DIR}")
include_directories(${HDF5_INCLUDE_DIR})
include_directories(${HASHMAP_INCLUDE_DIR})

add_library(ius STATIC
		include/iusHL2DNonParametricSource.h
		include/iusHL2DNonParametricSourceImp.h
		include/iusHL2DParametricSource.h
		include/iusHL2DSize.h
		include/iusHL2DTransducer.h
		include/iusHL2DTransducerElement.h
		include/iusHL2DTransducerElementList.h
		include/iusHL3DAngle.h
		include/iusHL3DNonParametricSource.h
		include/iusHL3DParametricSource.h
		include/iusHL3DSize.h
		include/iusHL3DTransducer.h
		include/iusHL3DTransducerElement.h
		include/iusHL3DTransducerElementList.h
		include/iusHLApodizationList.h
		include/iusHLFrame.h
		include/iusHLFrameList.h
		include/iusHLInputFile.h
		include/iusHLNonParametricPulse.h
		include/iusHLNonParametricPulseImp.h
		include/iusHLParametricPulse.h
		include/iusHLParametricPulseImp.h
		include/iusHLPattern.h
		include/iusHLPatternImp.h
		include/iusHLPatternList.h
		include/iusHLPatternListImp.h
		include/iusHLPosition.h
		include/iusHLPulse.h
		include/iusHLPulseDict.h
		include/iusHLPulseDictImp.h
		include/iusHLPulseImp.h
		include/iusHLReceiveChannelMap.h
		include/iusHLReceiveChannelMapDict.h
		include/iusHLReceiveChannelMapImp.h
		include/iusHLReceiveSettings.h
		include/iusHLReceiveSettingsDict.h
		include/iusHLReceiveSettingsDictImp.h
		include/iusHLSource.h
		include/iusHLSourceDict.h
		include/iusHLSourceImp.h
		include/iusHLTGC.h
		include/iusHLTGCImp.h
		include/iusHLTransducer.h
		include/iusHLTransducerElement.h
		include/iusHLTransducerElement.h
		include/iusHLTransducerElementImp.h
		include/iusHLTransducerFactory.h
		include/iusHLTransducerImp.h
		include/iusHLTransmitApodization.h
		include/iusHLTransmitApodizationDict.h
		include/iusHLTransmitApodizationImp.h
		include/iusHLTransmitPattern.h
		include/iusHLTransmitPatternList.h
		include/iusTypes.h
		include/iusUtil.h
		src/iusHDF5.c
		src/iusHL2DNonParametricSource.c
		src/iusHL2DParametricSource.c
		src/iusHL2DSize.c
		src/iusHL2DTransducer.c
		src/iusHL2DTransducerElement.c
		src/iusHL2DTransducerElementList.c
		src/iusHL3DAngle.c
		src/iusHL3DNonParametricSource.c
		src/iusHL3DParametricSource.c
		src/iusHL3DSize.c
		src/iusHL3DTransducer.c
		src/iusHL3DTransducerElement.c
		src/iusHL3DTransducerElementList.c
		src/iusHLApodizationList.c
		src/iusHLExperiment.c
		src/iusHLFrame.c
		src/iusHLFrameList.c
		src/iusHLInputFile.c
		src/iusHLNonParametricPulse.c
		src/iusHLParametricPulse.c
		src/iusHLPattern.c
		src/iusHLPatternList.c
		src/iusHLPosition.c
		src/iusHLPulse.c
		src/iusHLPulseDict.c
		src/iusHLReceiveChannelMap.c
		src/iusHLReceiveChannelMapDict.c
		src/iusHLReceiveSettings.c
		src/iusHLReceiveSettingsDict.c
		src/iusHLSource.c
		src/iusHLSourceDict.c
		src/iusHLTGC.c
		src/iusHLTransducer.c
		src/iusHLTransducerElement.c
		src/iusHLTransducerFactory.c
		src/iusHLTransducerFactory.c include/iusHLTransducerFactory.h
		src/iusHLTransmitApodization.c
		src/iusHLTransmitApodizationDict.c
		src/iusHLTransmitPattern.c
		src/iusHLTransmitPatternList.c
		src/iusUuid.c
		src/iusUtil.c

		)

target_include_directories(ius PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
        include
        )


target_link_libraries(
        ius
		hashmap
        ${HDF5_LIBRARIES}
        ${HDF5_HL_LIBRARIES}
        ${UUID_LIBRARIES}
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
