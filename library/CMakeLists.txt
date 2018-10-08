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

include_directories("${PROJECT_BINARY_DIR}")
include_directories(${HDF5_INCLUDE_DIR})
include_directories(${HASHMAP_INCLUDE_DIR})

add_library(ius STATIC
		include/ius2DNonParametricSource.h
		include/ius2DNonParametricSourcePrivate.h
		include/ius2DParametricSource.h
		include/ius2DSize.h
		include/ius2DSizePrivate.h
		include/ius2DTransducer.h
		include/ius2DTransducerElement.h
		include/ius2DTransducerElementPrivate.h
		include/ius2DTransducerElementList.h
		include/ius2DTransducerElementListPrivate.h
		include/ius3DAngle.h
		include/ius3DAnglePrivate.h
		include/ius3DNonParametricSource.h
		include/ius3DParametricSource.h
		include/ius3DSize.h
		include/ius3DSizePrivate.h
		include/ius3DTransducer.h
		include/ius3DTransducerElement.h
		include/ius3DTransducerElementPrivate.h
		include/ius3DTransducerElementList.h
        include/ius3DTransducerElementListPrivate.h
        include/iusExperiment.h
        include/iusExperimentPrivate.h
		include/iusFile.h
		include/iusFrame.h
		include/iusFrameList.h
		include/iusFrameListPrivate.h
		include/iusHistoryNode.h
		include/iusHistoryNodePrivate.h
		include/iusHistoryNodeList.h
		include/iusHistoryNodeListPrivate.h
		include/iusHDF5.h
		include/iusInputFile.h
		include/iusInputFilePrivate.h
		include/iusNonParametricPulse.h
		include/iusNonParametricPulsePrivate.h
		include/iusParameterDict.h
		include/iusParametricPulse.h
		include/iusParametricPulsePrivate.h
		include/iusPattern.h
		include/iusPatternPrivate.h
		include/iusPatternList.h
		include/iusPatternListPrivate.h
		include/iusPatternListDict.h
		include/iusPatternListPrivateDict.h
		include/iusPosition.h
		include/iusPulse.h
		include/iusPulseDict.h
		include/iusPulseDictPrivate.h
		include/iusPulsePrivate.h
		include/iusReceiveChannelMap.h
		include/iusReceiveChannelMapDict.h
		include/iusReceiveChannelMapPrivate.h
		include/iusReceiveSettings.h
		include/iusReceiveSettingsDict.h
		include/iusReceiveSettingsDictPrivate.h
		include/iusSource.h
		include/iusSourceDict.h
		include/iusSourcePrivate.h
		include/iusTGC.h
		include/iusTGCPrivate.h
		include/iusTransducer.h
		include/iusTransducerElement.h
		include/iusTransducerElementPrivate.h
		include/iusTransducerPrivate.h
        include/ius2DTransducerPrivate.h
        include/ius3DTransducerPrivate.h
		include/iusTransmitApodization.h
		include/iusTransmitApodizationDict.h
		include/iusTransmitApodizationPrivate.h
		include/iusTypes.h
		include/iusUtil.h
		src/iusHDF5.c
		src/ius2DNonParametricSource.c
		src/ius2DParametricSource.c
		src/ius2DSize.c
		src/ius2DTransducer.c
		src/ius2DTransducerElement.c
		src/ius2DTransducerElementList.c
		src/ius3DAngle.c
		src/ius3DNonParametricSource.c
		src/ius3DParametricSource.c
		src/ius3DSize.c
		src/ius3DTransducer.c
		src/ius3DTransducerElement.c
		src/ius3DTransducerElementList.c
		src/iusExperiment.c
		src/iusFile.c
		src/iusFrame.c
		src/iusFrameList.c
		src/iusInputFile.c
		src/iusHistoryNode.c
		src/iusHistoryNodeList.c
		src/iusNonParametricPulse.c
		src/iusParameterDict.c
		src/iusParametricPulse.c
		src/iusPattern.c
		src/iusPatternList.c
		src/iusPatternListDict.c
		src/iusPosition.c
		src/iusPulse.c
		src/iusPulseDict.c
		src/iusReceiveChannelMap.c
		src/iusReceiveChannelMapDict.c
		src/iusReceiveSettings.c
		src/iusReceiveSettingsDict.c
		src/iusSource.c
		src/iusSourceDict.c
		src/iusTGC.c
		src/iusTransducer.c
		src/iusTransducerElement.c
		src/iusTransmitApodization.c
		src/iusTransmitApodizationDict.c
		src/iusUuid.c
		src/iusUtil.c
		)

target_include_directories(ius PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
        include
        )

if(MSVC)
	target_compile_options(ius PRIVATE /W4 /WX)
else()
	set(UUID_LIBRARIES -luuid)
	target_compile_options(ius PRIVATE -Wall -Wextra -pedantic -Werror)
	if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
		target_compile_options(ius PRIVATE -Wall -Wextra -pedantic -Werror)
	endif()
endif()

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
