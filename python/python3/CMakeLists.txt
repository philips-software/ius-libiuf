SET(UseSWIG_TARGET_NAME_PREFERENCE STANDARD)
#CMAKE_POLICY(SET CMP0078 NEW)
FIND_PACKAGE(SWIG REQUIRED)
INCLUDE(${SWIG_USE_FILE})

#SET(CMAKE_FIND_LIBRARY_SUFFIXES .a)
#SET(Python3_USE_STATIC_LIBS TRUE)
find_package (Python3 COMPONENTS Development)
if(Python3_FOUND)
    INCLUDE_DIRECTORIES(${Python3_INCLUDE_DIRS})
    LINK_DIRECTORIES(${Python3_LIBRARY_DIRS})


    INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/library/include)
    INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/library/private)
    INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/external/hashmap/src)
    INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/python)
    INCLUDE_DIRECTORIES(${HDF5_INCLUDE_DIR})

    SET(CMAKE_SWIG_FLAGS "")
    SET(UseSWIG_TARGET_NAME_PREFERENCE STANDARD)
    SET(IUS_TARGET "Python3Ius")

    set_property(SOURCE Python3Ius.i PROPERTY DEPENDS ../PythonIus.it)
    set_property(SOURCE ../PythonIus.it PROPERTY DEPENDS ../extendDict.it)
    set_property(SOURCE ../extendDict.it PROPERTY DEPENDS ../s0list.it)
    SWIG_ADD_LIBRARY(${IUS_TARGET}
            LANGUAGE python
            SOURCES Python3Ius.i ../extendDict.it ../s0list.it ../PythonIus.it)

    TARGET_LINK_LIBRARIES(${IUS_TARGET}
            ${Python3_LIBRARIES}
            ${IUS_LIBRARIES}
            ${HDF5_LIBRARIES}
            ${HDF5_HL_LIBRARIES}
            ${UUID_LIBRARIES}
            )

#    if(MSVC)
#        target_compile_options(${IUS_TARGET} PRIVATE /W4 /WX)
#        set_target_properties(unitTests PROPERTIES LINK_FLAGS "/ignore:4100")
#    else()
#        if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
#            target_compile_options(${IUS_TARGET} PRIVATE -Wall -Wextra -pedantic -Werror)
#        endif()
#    endif()

    file(GLOB_RECURSE PYTHON_FILES
            "${PROJECT_BINARY_DIR}/python/*PythonIus*")
    message("PROJECT_BINARY_DIR is ${PROJECT_BINARY_DIR}")
    set(IUS_INSTALL_PATH "${PROJECT_BINARY_DIR}/dist/python3${Python3_VERSION_MINOR}/${CMAKE_SYSTEM_NAME}")
    install(
            TARGETS ${IUS_TARGET}
            RUNTIME DESTINATION ${IUS_INSTALL_PATH}
            LIBRARY DESTINATION ${IUS_INSTALL_PATH}
            ARCHIVE DESTINATION ${IUS_INSTALL_PATH}
    )
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${IUS_TARGET}.py" DESTINATION  ${IUS_INSTALL_PATH})
endif()