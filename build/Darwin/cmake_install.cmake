# Install script for directory: /Users/frankvanheesch/Documents/GitHub/ius-libiuf

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/include")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist" TYPE DIRECTORY FILES "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/library/include" REGEX "/library\\/include\\/[^/]*[pP]rivate[^/]*\\.h$" EXCLUDE)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/examples")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist" TYPE DIRECTORY FILES "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/examples")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/unitTests")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist" TYPE DIRECTORY FILES "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/unitTests")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/unitTests/include/iuf2DNonParametricSourcePrivate.h;/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/unitTests/include/iuf2DParametricSourcePrivate.h;/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/unitTests/include/iuf2DSizePrivate.h;/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/unitTests/include/iuf2DTransducerElementListPrivate.h;/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/unitTests/include/iuf2DTransducerElementPrivate.h;/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/unitTests/include/iuf2DTransducerPrivate.h;/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/unitTests/include/iuf3DAnglePrivate.h;/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/unitTests/include/iuf3DNonParametricSourcePrivate.h;/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/unitTests/include/iuf3DParametricSourcePrivate.h;/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/unitTests/include/iuf3DSizePrivate.h;/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/unitTests/include/iuf3DTransducerElementListPrivate.h;/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/unitTests/include/iuf3DTransducerElementPrivate.h;/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/unitTests/include/iuf3DTransducerPrivate.h;/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/unitTests/include/iufAcquisitionPrivate.h;/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/unitTests/include/iufDataStreamDictPrivate.h;/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/unitTests/include/iufDemodulationDictPrivate.h;/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/unitTests/include/iufDemodulationPrivate.h;/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/unitTests/include/iufFilterPrivate.h;/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/unitTests/include/iufFrameListPrivate.h;/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/unitTests/include/iufFramePrivate.h;/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/unitTests/include/iufHistoryNodeListPrivate.h;/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/unitTests/include/iufHistoryNodePrivate.h;/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/unitTests/include/iufInputFilePrivate.h;/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/unitTests/include/iufIqFilePrivate.h;/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/unitTests/include/iufIqPatternListDictPrivate.h;/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/unitTests/include/iufIqPatternListPrivate.h;/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/unitTests/include/iufIqPatternPrivate.h;/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/unitTests/include/iufNonParametricPulsePrivate.h;/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/unitTests/include/iufParameterDictPrivate.h;/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/unitTests/include/iufParametricPulsePrivate.h;/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/unitTests/include/iufPatternListDictPrivate.h;/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/unitTests/include/iufPatternListPrivate.h;/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/unitTests/include/iufPatternPrivate.h;/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/unitTests/include/iufPositionPrivate.h;/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/unitTests/include/iufPulseDictPrivate.h;/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/unitTests/include/iufPulsePrivate.h;/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/unitTests/include/iufReceiveChannelMapDictPrivate.h;/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/unitTests/include/iufReceiveChannelMapPrivate.h;/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/unitTests/include/iufReceiveSettingsDictPrivate.h;/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/unitTests/include/iufReceiveSettingsPrivate.h;/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/unitTests/include/iufSourceDictPrivate.h;/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/unitTests/include/iufSourcePrivate.h;/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/unitTests/include/iufTGCPrivate.h;/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/unitTests/include/iufTransducerElementPrivate.h;/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/unitTests/include/iufTransducerPrivate.h;/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/unitTests/include/iufTransmitApodizationDictPrivate.h;/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/unitTests/include/iufTransmitApodizationPrivate.h")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/unitTests/include" TYPE FILE FILES
    "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/library/private/iuf2DNonParametricSourcePrivate.h"
    "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/library/private/iuf2DParametricSourcePrivate.h"
    "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/library/private/iuf2DSizePrivate.h"
    "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/library/private/iuf2DTransducerElementListPrivate.h"
    "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/library/private/iuf2DTransducerElementPrivate.h"
    "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/library/private/iuf2DTransducerPrivate.h"
    "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/library/private/iuf3DAnglePrivate.h"
    "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/library/private/iuf3DNonParametricSourcePrivate.h"
    "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/library/private/iuf3DParametricSourcePrivate.h"
    "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/library/private/iuf3DSizePrivate.h"
    "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/library/private/iuf3DTransducerElementListPrivate.h"
    "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/library/private/iuf3DTransducerElementPrivate.h"
    "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/library/private/iuf3DTransducerPrivate.h"
    "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/library/private/iufAcquisitionPrivate.h"
    "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/library/private/iufDataStreamDictPrivate.h"
    "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/library/private/iufDemodulationDictPrivate.h"
    "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/library/private/iufDemodulationPrivate.h"
    "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/library/private/iufFilterPrivate.h"
    "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/library/private/iufFrameListPrivate.h"
    "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/library/private/iufFramePrivate.h"
    "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/library/private/iufHistoryNodeListPrivate.h"
    "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/library/private/iufHistoryNodePrivate.h"
    "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/library/private/iufInputFilePrivate.h"
    "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/library/private/iufIqFilePrivate.h"
    "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/library/private/iufIqPatternListDictPrivate.h"
    "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/library/private/iufIqPatternListPrivate.h"
    "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/library/private/iufIqPatternPrivate.h"
    "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/library/private/iufNonParametricPulsePrivate.h"
    "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/library/private/iufParameterDictPrivate.h"
    "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/library/private/iufParametricPulsePrivate.h"
    "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/library/private/iufPatternListDictPrivate.h"
    "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/library/private/iufPatternListPrivate.h"
    "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/library/private/iufPatternPrivate.h"
    "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/library/private/iufPositionPrivate.h"
    "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/library/private/iufPulseDictPrivate.h"
    "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/library/private/iufPulsePrivate.h"
    "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/library/private/iufReceiveChannelMapDictPrivate.h"
    "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/library/private/iufReceiveChannelMapPrivate.h"
    "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/library/private/iufReceiveSettingsDictPrivate.h"
    "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/library/private/iufReceiveSettingsPrivate.h"
    "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/library/private/iufSourceDictPrivate.h"
    "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/library/private/iufSourcePrivate.h"
    "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/library/private/iufTGCPrivate.h"
    "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/library/private/iufTransducerElementPrivate.h"
    "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/library/private/iufTransducerPrivate.h"
    "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/library/private/iufTransmitApodizationDictPrivate.h"
    "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/library/private/iufTransmitApodizationPrivate.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/README.md;/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/CMakeLists.txt;/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/PreLoad.cmake")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist" TYPE FILE FILES
    "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/package/README.md"
    "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/package/CMakeLists.txt"
    "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/package/PreLoad.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/ci")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist" TYPE DIRECTORY FILES "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/ci" USE_SOURCE_PERMISSIONS REGEX "/ci\\/bin\\/dist\\.[^/]*$" EXCLUDE REGEX "/ci\\/bin\\/mksdk\\.[^/]*$" EXCLUDE)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/examples")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist" TYPE DIRECTORY FILES "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/examples")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist/external")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/dist" TYPE DIRECTORY FILES "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/external")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/examples/cmake_install.cmake")
  include("/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/external/cmake_install.cmake")
  include("/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/unitTests/cmake_install.cmake")
  include("/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/library/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/Users/frankvanheesch/Documents/GitHub/ius-libiuf/build/Darwin/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
