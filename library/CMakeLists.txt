# The version number.
set (IUS_VERSION_MAJOR 3)
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
		set (IUS_LIB_VERSION_STRING ${IUS_VERSION_MAJOR}.${GIT_TAG})
	else()
		set (IUS_LIB_VERSION_STRING ${IUS_VERSION_MAJOR}.${GIT_COMMIT_HASH})
	endif()
	FILE(WRITE ${PROJECT_BINARY_DIR}/library/VERSION "${IUS_LIB_VERSION_STRING}")
ELSE(GIT_FOUND)
	FILE(READ ${CMAKE_SOURCE_DIR}/library/${CMAKE_SYSTEM_NAME}/VERSION IUS_LIB_VERSION_STRING)
ENDIF(GIT_FOUND)

STRING( REGEX REPLACE "([0-9]+).[0-9]+.[0-9]+" "\\1" VERSION_MAJOR ${IUS_LIB_VERSION_STRING} )
STRING( REGEX REPLACE "[0-9]+.([0-9]+).[0-9]+" "\\1" VERSION_MINOR ${IUS_LIB_VERSION_STRING} )
STRING( REGEX REPLACE "[0-9]+.[0-9]+.([0-9]+)" "\\1" VERSION_PATCH ${IUS_LIB_VERSION_STRING} )

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
        "library_config.h.in"
        "${PROJECT_BINARY_DIR}/library_config.h"
)

#FIND_PACKAGE(HDF5 1.8 COMPONENTS C HL REQUIRED)

include_directories("${PROJECT_BINARY_DIR}")
include_directories(${HDF5_INCLUDE_DIR})
include_directories(${HASHMAP_INCLUDE_DIR})
set(CMAKE_DEBUG_POSTFIX d)
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
        include/iusData.h
		include/iusDataStream.h
		include/iusDataStreamDictPrivate.h
		include/iusDemodulation.h
		include/iusDemodulationDict.h
		include/iusDemodulationDictPrivate.h
        include/iusAcquisition.h
        include/iusAcquisitionPrivate.h
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
		include/iusIqFile.h
		include/iusInputFilePrivate.h
		include/iusNonParametricPulse.h
		include/iusNonParametricPulsePrivate.h
		include/iusOffset.h
		include/iusParameterDict.h
		include/iusParametricPulse.h
		include/iusParametricPulsePrivate.h
		include/iusPattern.h
		include/iusPatternPrivate.h
		include/iusPatternList.h
		include/iusPatternListPrivate.h
		include/iusPatternListDict.h
		include/iusPatternListDictPrivate.h
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
        src/iusData.c
		src/iusDataStream.c
		src/iusDataStreamDict.c
		src/iusDemodulation.c
		src/iusDemodulationDict.c
        src/iusAcquisition.c
		src/iusFile.c
		src/iusFrame.c
		src/iusFrameList.c
		src/iusInputFile.c
		src/iusIqFile.c
		src/iusHistoryNode.c
		src/iusHistoryNodeList.c
		src/iusNonParametricPulse.c
		src/iusOffset.c
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
		${HASHMAP_SOURCES}
		)

target_include_directories(ius PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
        include
        )

if(MSVC)
	target_compile_options(ius PRIVATE /W4 /WX)
    set_target_properties(ius PROPERTIES LINK_FLAGS "/ignore:4099 /ignore:4098")
else()
	target_compile_options(ius PRIVATE -Wall -Wextra -pedantic -Werror)
	if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
		target_compile_options(ius PRIVATE -Wall -Wextra -pedantic -Werror)
	endif()
endif()

target_link_libraries(
		ius
		${HDF5_LIBRARIES}
		${HDF5_HL_LIBRARIES}
		${UUID_LIBRARIES}
)

set_target_properties(ius PROPERTIES OUTPUT_NAME "ius-${IUS_LIB_VERSION_STRING}")
set_property(TARGET ius PROPERTY C_STANDARD 99)
set(IUS_INSTALL_PATH ${PROJECT_BINARY_DIR}/dist/library/${CMAKE_SYSTEM_NAME})
install(TARGETS ius
		LIBRARY DESTINATION ${IUS_INSTALL_PATH}
		ARCHIVE DESTINATION ${IUS_INSTALL_PATH}
		)

install(FILES ${PROJECT_BINARY_DIR}/library/VERSION  DESTINATION ${IUS_INSTALL_PATH})
