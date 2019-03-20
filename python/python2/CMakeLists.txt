SET(UseSWIG_TARGET_NAME_PREFERENCE STANDARD)

if(POLICY CMP0086)
    cmake_policy(SET CMP0086 NEW)
endif()

if(POLICY CMP0078)
    cmake_policy(SET CMP0078 NEW)
endif()

FIND_PACKAGE(SWIG REQUIRED)
INCLUDE(${SWIG_USE_FILE})

find_package (Python2 COMPONENTS Development NumPy)
if(Python2_FOUND)
    INCLUDE_DIRECTORIES(${Python2_INCLUDE_DIRS})
    LINK_DIRECTORIES(${Python2_LIBRARY_DIRS})


    INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/unitTests/dg)
    INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/library/include)
    INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/library/private)
    INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/external/hashmap/src)
    INCLUDE_DIRECTORIES(${HDF5_INCLUDE_DIR})
    INCLUDE_DIRECTORIES(${Python2_NumPy_INCLUDE_DIRS})


SET(CMAKE_SWIG_FLAGS "")
    SET(UseSWIG_TARGET_NAME_PREFERENCE STANDARD)
    SET(IUF_TARGET "Python2Iuf")
    set_property(SOURCE Python2Iuf.i PROPERTY DEPENDS ../PythonIuf.it)
    set_property(SOURCE ../PythonIuf.it PROPERTY DEPENDS ../extendDict.it)
    set_property(SOURCE ../extendDict.it PROPERTY DEPENDS ../s0list.it)
    SWIG_ADD_LIBRARY(${IUF_TARGET}
            LANGUAGE python
            SOURCES Python2Iuf.i ../extendDict.it ../s0list.it ../PythonIuf.it)

    TARGET_LINK_LIBRARIES(${IUF_TARGET}
            ${Python2_LIBRARIES}
            ${IUF_LIBRARIES}
            ${HDF5_LIBRARIES}
            ${HDF5_HL_LIBRARIES}
            ${UUID_LIBRARIES}
            dg
            Unity
            )

#    if(MSVC)
#        target_compile_options(${IUF_TARGET} PRIVATE /W4 /WX)
#        set_target_properties(unitTests PROPERTIES LINK_FLAGS "/ignore:4100")
#    else()
#        if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
#            target_compile_options(${IUF_TARGET} PRIVATE -Wall -Wextra -pedantic -Werror)
#        endif()
#    endif()



    file(GLOB_RECURSE PYTHON_FILES
            "${PROJECT_BINARY_DIR}/python/*PythonIuf*")
    message("PROJECT_BINARY_DIR is ${PROJECT_BINARY_DIR}")
    set(IUF_INSTALL_PATH "${PROJECT_BINARY_DIR}/dist/python/python2/${CMAKE_SYSTEM_NAME}")
    install(
    TARGETS ${IUF_TARGET}
    RUNTIME DESTINATION ${IUF_INSTALL_PATH}
    LIBRARY DESTINATION ${IUF_INSTALL_PATH}
    ARCHIVE DESTINATION ${IUF_INSTALL_PATH}
    )
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${IUF_TARGET}.py" DESTINATION  ${IUF_INSTALL_PATH})
endif()
