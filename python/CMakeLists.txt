find_package(PythonLibs REQUIRED)
find_package(SWIG REQUIRED)


include(${SWIG_USE_FILE})

include_directories(
        ${PYTHON_INCLUDE_PATH}
        ${CMAKE_SOURCE_DIR}/library/include
        ${HDF5_INCLUDE_DIR})

set(CMAKE_SWIG_FLAGS "")
set(UseSWIG_TARGET_NAME_PREFERENCE STANDARD)
set(LIBRARY_NAME piuf)

# Add swig module
swig_add_library(${LIBRARY_NAME}
        TYPE SHARED
        LANGUAGE python
        SOURCES ${LIBRARY_NAME}.i)

swig_link_libraries(${LIBRARY_NAME}
        ${PYTHON_LIBRARIES}
        ${IUF_LIBRARIES}
        ${HDF5_LIBRARIES}
        ${HDF5_HL_LIBRARIES}
        ${UUID_LIBRARIES}
        dg
        Unity)


if(APPLE)
    set_target_properties(${LIBRARY_NAME} PROPERTIES
            SUFFIX ".so"
            )
    set_property(TARGET ${LIBRARY_NAME} APPEND PROPERTY
            LINK_FLAGS "-flat_namespace -undefined suppress"
            )
endif()



if(MSVC)
    # Files to install with Python
    set(SWIG_LIBIUF_PY ${LIBRARY_NAME}.py)
    set(SWIG_LIBIUF_SO _${LIBRARY_NAME}.pyd)
    set(PYTHON_COMMAND python)
    set_target_properties( ${LIBRARY_NAME}
            PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/python"
            LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/python"
            RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/python"
            )
else()
    # Files to install with Python
    set(SWIG_LIBIUF_PY ${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME}.py)
    set(SWIG_LIBIUF_SO ${CMAKE_CURRENT_BINARY_DIR}/_${LIBRARY_NAME}.so)
    set(PYTHON_COMMAND python3)
endif()

set(PYTHON_INSTALL_FILES ${SWIG_LIBIUF_PY} ${SWIG_LIBIUF_SO})

# Configure setup.py and copy to output directory
set(SETUP_PY_IN ${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in)
set(SETUP_PY_OUT ${CMAKE_CURRENT_BINARY_DIR}/setup.py)
configure_file(${SETUP_PY_IN} ${SETUP_PY_OUT})
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/README.md ${CMAKE_CURRENT_BINARY_DIR}/README.md COPYONLY)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/tests DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Install target to call setup.py
add_custom_target(install-python
        COMMAND ${PYTHON_COMMAND} setup.py install)