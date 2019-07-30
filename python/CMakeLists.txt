find_package(PythonLibs REQUIRED)
find_package(SWIG REQUIRED)


include(${SWIG_USE_FILE})

set(CMAKE_SWIG_FLAGS "")
include_directories(
        ${PYTHON_INCLUDE_PATH}
        ${CMAKE_SOURCE_DIR}/library/include
        ${HDF5_INCLUDE_DIR})

set(UseSWIG_TARGET_NAME_PREFERENCE STANDARD)

# Add swig module
swig_add_library(libiuf
        TYPE SHARED
        LANGUAGE python
        SOURCES libiuf.i)

swig_link_libraries(libiuf
        ${PYTHON_LIBRARIES}
        ${IUF_LIBRARIES}
        ${HDF5_LIBRARIES}
        ${HDF5_HL_LIBRARIES}
        ${UUID_LIBRARIES}
        dg
        Unity)


if(APPLE)
    set_target_properties(libiuf PROPERTIES
            SUFFIX ".so"
            )
    set_property(TARGET libiuf APPEND PROPERTY
            LINK_FLAGS "-flat_namespace -undefined suppress"
            )
endif()


# Files to install with Python
if(MSVC)
    set(PYTHON_INSTALL_FILES
            ${CMAKE_CURRENT_BINARY_DIR}/libiuf.py
            ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CONFIG}/_libiuf.pyd)
else()
    set(PYTHON_INSTALL_FILES
            ${CMAKE_CURRENT_BINARY_DIR}/libiuf.py
            ${CMAKE_CURRENT_BINARY_DIR}/_libiuf.so)
endif()

# Configure setup.py and copy to output directory
set(SETUP_PY_IN ${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in)
set(SETUP_PY_OUT ${CMAKE_CURRENT_BINARY_DIR}/setup.py)
configure_file(${SETUP_PY_IN} ${SETUP_PY_OUT})

# Install target to call setup.py
add_custom_target(install-python
        COMMAND python ${SETUP_PY_OUT} install)