SET(UseSWIG_TARGET_NAME_PREFERENCE STANDARD)
#CMAKE_POLICY(SET CMP0078 NEW)
FIND_PACKAGE(SWIG REQUIRED)
INCLUDE(${SWIG_USE_FILE})

#
# Todo:
# - Get windows build up and running
# - Find out whether upgrade to Python3 is needed or not.
#
# FIND_PACKAGE(PythonLibs)
# does not work on centos 7 distribution
# for now disabled and replaced with
# execute_process.
#
#execute_process(COMMAND python -c "from distutils.sysconfig import get_python_inc; print(get_python_inc())"  OUTPUT_VARIABLE PYTHON_INCLUDE_DIR)
#string(REGEX REPLACE "\n$" "" PYTHON_INCLUDE_DIR "${PYTHON_INCLUDE_DIR}")
#execute_process(COMMAND python -c "import distutils.sysconfig as sysconfig; print(sysconfig.get_config_var('LIBDIR'))"  OUTPUT_VARIABLE PYTHON_LIBRARY_DIR)
#string(REGEX REPLACE "\n$" "" PYTHON_LIBRARY_DIR "${PYTHON_LIBRARY_DIR}")
#execute_process(COMMAND python-config --libs OUTPUT_VARIABLE PYTHON_LIBRARIES)
#string(REGEX REPLACE "\n$" "" PYTHON_LIBRARIES "${PYTHON_LIBRARIES}")
#
#SET(PYTHON_INCLUDE_DIR  "${PYTHON_INCLUDE_DIR}" CACHE STRING "1")
#SET(PYTHON_LIBRARY  "${PYTHON_LIBRARY_DIR}" CACHE STRING "2")

#FIND_PACKAGE(PythonInterp)
#FIND_PACKAGE(PythonLibs)
#INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})
#INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIR})
#LINK_DIRECTORIES(${PYTHON_LIBRARY_DIR})

#SET(CMAKE_FIND_LIBRARY_SUFFIXES .a)
find_package (Python2 COMPONENTS Development)
INCLUDE_DIRECTORIES(${Python2_INCLUDE_DIRS})
LINK_DIRECTORIES(${Python2_LIBRARY_DIRS})


INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/library/include)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/library/private)
INCLUDE_DIRECTORIES(${HDF5_INCLUDE_DIR})

SET(CMAKE_SWIG_FLAGS "")

SET(UseSWIG_TARGET_NAME_PREFERENCE STANDARD)
SWIG_ADD_LIBRARY(PythonIus LANGUAGE python SOURCES PythonIus.i)

if(MSVC)
    TARGET_LINK_LIBRARIES(PythonIus
            optimized ${Python2_LIBRARIES}
            )
    TARGET_LINK_LIBRARIES(PythonIus
            ${IUS_LIBRARIES}
            ${HDF5_LIBRARIES}
            ${HDF5_HL_LIBRARIES}
            ${UUID_LIBRARIES}
            )
else(MSVC)
    TARGET_LINK_LIBRARIES(PythonIus
            ${Python2_LIBRARIES}
            ${IUS_LIBRARIES}
            ${HDF5_LIBRARIES}
            ${HDF5_HL_LIBRARIES}
            ${UUID_LIBRARIES}
            )
endif(MSVC)


