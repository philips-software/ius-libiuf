# The version number.
set (IUF_VERSION_MAJOR 3)
set (CMAKE_C_STANDARD 99)

FIND_PACKAGE(Git)
IF(GIT_FOUND)
	# Get the latest abbreviated commit hash of the working branch
	execute_process(
			COMMAND git log -1 --format=%h
			WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
			OUTPUT_VARIABLE GIT_COMMIT_HASH
			OUTPUT_STRIP_TRAILING_WHITESPACE
	)

	execute_process(
			COMMAND git describe --tags --abbrev=0 --exact-match
			WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
			OUTPUT_VARIABLE GIT_TAG
			RESULT_VARIABLE GIT_DESCRIBE_RESULT
			ERROR_VARIABLE GIT_DESCRIBE_ERROR
			OUTPUT_STRIP_TRAILING_WHITESPACE
	)

    if(GIT_TAG)
		# Tag found
		set (IUF_LIB_VERSION_STRING ${IUF_VERSION_MAJOR}.${GIT_TAG})
	else()
		set (IUF_LIB_VERSION_STRING ${IUF_VERSION_MAJOR}.${GIT_COMMIT_HASH})
	endif()
	FILE(WRITE ${PROJECT_BINARY_DIR}/library/VERSION "${IUF_LIB_VERSION_STRING}")
ELSE(GIT_FOUND)
	FILE(READ ${CMAKE_SOURCE_DIR}/library/${CMAKE_SYSTEM_NAME}/VERSION IUF_LIB_VERSION_STRING)
ENDIF(GIT_FOUND)

STRING( REGEX REPLACE "([0-9]+).[0-9]+.[0-9]+" "\\1" VERSION_MAJOR ${IUF_LIB_VERSION_STRING} )
STRING( REGEX REPLACE "[0-9]+.([0-9]+).[0-9]+" "\\1" VERSION_MINOR ${IUF_LIB_VERSION_STRING} )
STRING( REGEX REPLACE "[0-9]+.[0-9]+.([0-9]+)" "\\1" VERSION_PATCH ${IUF_LIB_VERSION_STRING} )

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
        "library_config.h.in"
        "${PROJECT_BINARY_DIR}/library_config.h"
)


include_directories("${PROJECT_BINARY_DIR}")
include_directories(${HDF5_INCLUDE_DIR})
include_directories(${HASHMAP_INCLUDE_DIR})
set(CMAKE_DEBUG_POSTFIX d)
add_library(iuf STATIC
		include/iuf2DNonParametricSource.h
		private/iuf2DNonParametricSourcePrivate.h
		include/iuf2DParametricSource.h
		include/iuf2DSize.h
		private/iuf2DSizePrivate.h
		include/iuf2DTransducer.h
		private/iuf2DTransducerADT.h
		private/iuf2DTransducerPrivate.h
		include/iuf2DTransducerElement.h
		private/iuf2DTransducerElementPrivate.h
		include/iuf2DTransducerElementList.h
		private/iuf2DTransducerElementListPrivate.h
		include/iuf3DAngle.h
		private/iuf3DAnglePrivate.h
		include/iuf3DNonParametricSource.h
		include/iuf3DParametricSource.h
		include/iuf3DSize.h
		private/iuf3DSizePrivate.h
		include/iuf3DTransducer.h
		private/iuf3DTransducerADT.h
		private/iuf3DTransducerPrivate.h
		include/iuf3DTransducerElement.h
		private/iuf3DTransducerElementPrivate.h
		include/iuf3DTransducerElementList.h
		private/iuf3DTransducerElementListPrivate.h
        include/iufData.h
		private/iufDataADT.h
		include/iufDataStream.h
		private/iufDataStreamDictPrivate.h
		include/iufDemodulation.h
		include/iufDemodulationDict.h
		private/iufDemodulationDictPrivate.h
        include/iufAcquisition.h
		private/iufAcquisitionPrivate.h
		include/iufFile.h
		include/iufFilter.h
		include/iufFrame.h
		include/iufFrameList.h
		private/iufFrameListPrivate.h
		include/iufHistoryNode.h
		private/iufHistoryNodePrivate.h
		include/iufHistoryNodeList.h
		private/iufHistoryNodeListPrivate.h
		include/iufHDF5.h
		include/iufInputFile.h
		private/iufInputFilePrivate.h
		include/iufInputFileStructure.h
		include/iufIqFile.h
		private/iufIqFilePrivate.h
		include/iufIqFileStructure.h
		include/iufIqPattern.h
		private/iufIqPatternPrivate.h
		include/iufIqPatternList.h
		private/iufIqPatternListPrivate.h
		include/iufIqPatternListDict.h
		private/iufIqPatternListDictADT.h
		private/iufIqPatternListDictPrivate.h
		include/iufNonParametricPulse.h
		private/iufNonParametricPulsePrivate.h
		include/iufOffset.h
		include/iufParameterDict.h
		include/iufParametricPulse.h
		private/iufParametricPulsePrivate.h
		include/iufPattern.h
		private/iufPatternPrivate.h
		include/iufPatternList.h
		private/iufPatternListPrivate.h
		include/iufPatternListDict.h
		private/iufPatternListDictADT.h
		private/iufPatternListDictPrivate.h
		include/iufPosition.h
		include/iufPulse.h
		include/iufPulseDict.h
		private/iufPulseDictADT.h
		private/iufPulseDictPrivate.h
		private/iufPulsePrivate.h
		include/iufReceiveChannelMap.h
		include/iufReceiveChannelMapDict.h
		private/iufReceiveChannelMapDictADT.h
		private/iufReceiveChannelMapPrivate.h
		include/iufReceiveSettings.h
		include/iufReceiveSettingsDict.h
		private/iufReceiveSettingsDictADT.h
		private/iufReceiveSettingsDictPrivate.h
		include/iufSource.h
		include/iufSourceDict.h
		private/iufSourceDictADT.h
		private/iufSourcePrivate.h
		include/iufTGC.h
		private/iufTGCPrivate.h
		include/iufTransducer.h
		private/iufTransducerADT.h
		private/iufTransducerPrivate.h
		include/iufTransducerElement.h
		private/iufTransducerElementPrivate.h
		private/iufTransducerPrivate.h
		private/iuf2DTransducerPrivate.h
		private/iuf3DTransducerPrivate.h
		include/iufTransmitApodization.h
		include/iufTransmitApodizationDict.h
		private/iufTransmitApodizationDictADT.h
		private/iufTransmitApodizationPrivate.h
		include/iufTypes.h
		include/iufUtil.h
		src/iufHDF5.c
		src/iuf2DNonParametricSource.c
		src/iuf2DParametricSource.c
		src/iuf2DSize.c
		src/iuf2DTransducer.c
		src/iuf2DTransducerElement.c
		src/iuf2DTransducerElementList.c
		src/iuf3DAngle.c
		src/iuf3DNonParametricSource.c
		src/iuf3DParametricSource.c
		src/iuf3DSize.c
		src/iuf3DTransducer.c
		src/iuf3DTransducerElement.c
		src/iuf3DTransducerElementList.c
        src/iufData.c
		src/iufDataStream.c
		src/iufDataStreamDict.c
		src/iufDemodulation.c
		src/iufDemodulationDict.c
        src/iufAcquisition.c
		src/iufError.c
		src/iufFile.c
		src/iufFilter.c
		src/iufFrame.c
		src/iufFrameList.c
		src/iufInputFile.c
		src/iufIqFile.c
		src/iufIqPattern.c
		src/iufIqPatternList.c
		src/iufIqPatternListDict.c
		src/iufHistoryNode.c
		src/iufHistoryNodeList.c
		src/iufNonParametricPulse.c
		src/iufOffset.c
		src/iufParameterDict.c
		src/iufParametricPulse.c
		src/iufPattern.c
		src/iufPatternList.c
		src/iufPatternListDict.c
		src/iufPosition.c
		src/iufPulse.c
		src/iufPulseDict.c
		src/iufReceiveChannelMap.c
		src/iufReceiveChannelMapDict.c
		src/iufReceiveSettings.c
		src/iufReceiveSettingsDict.c
		src/iufSource.c
		src/iufSourceDict.c
		src/iufTGC.c
		src/iufTransducer.c
		src/iufTransducerElement.c
		src/iufTransmitApodization.c
		src/iufTransmitApodizationDict.c
		src/iufUuid.c
		src/iufUtil.c
		${HASHMAP_SOURCES}
		)

target_include_directories(iuf PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
		${CMAKE_CURRENT_LIST_DIR}/private
        )

if(MSVC)
	target_compile_options(iuf PRIVATE /W4 /WX)
    set_target_properties(iuf PROPERTIES LINK_FLAGS "/ignore:4099 /ignore:4098")
else()
	target_compile_options(iuf PRIVATE -Wall -Wextra -pedantic -Werror)
	if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
		target_compile_options(iuf PRIVATE -Wall -Wextra -pedantic -Werror -fPIC)
	endif()
endif()

target_link_libraries(
		iuf
		${HDF5_LIBRARIES}
		${HDF5_HL_LIBRARIES}
		${UUID_LIBRARIES}
)

set_target_properties(iuf PROPERTIES OUTPUT_NAME "iuf-${IUF_LIB_VERSION_STRING}")
set_property(TARGET iuf PROPERTY C_STANDARD 99)
set(IUF_INSTALL_PATH ${PROJECT_BINARY_DIR}/dist/library/${CMAKE_SYSTEM_NAME})
install(TARGETS iuf
		LIBRARY DESTINATION ${IUF_INSTALL_PATH}
		ARCHIVE DESTINATION ${IUF_INSTALL_PATH}
		)

install(FILES ${PROJECT_BINARY_DIR}/library/VERSION  DESTINATION ${IUF_INSTALL_PATH})
