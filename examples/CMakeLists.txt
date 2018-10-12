FIND_PACKAGE(HDF5 COMPONENTS C HL)
include_directories(${HDF5_INCLUDE_DIR})

link_directories(${CMAKE_SOURCE_DIR}/lib)
add_executable(examples
        src/main.c
        src/nodeConfig.c
        )

target_link_libraries(examples
        ius
        ${HDF5_LIBRARIES}
        ${HDF5_HL_LIBRARIES}
        ${UUID_LIBRARIES}
        )

target_include_directories(examples PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${CMAKE_SOURCE_DIR}/include
        ${IUS_INCLUDE_DIR}
        )


set_property(TARGET examples PROPERTY C_STANDARD 99)

if(MSVC)
    target_compile_options(examples PRIVATE /W4 /WX)
else()
    set(UUID_LIBRARIES -luuid)
    target_compile_options(examples PRIVATE -Wall -Wextra -pedantic -Werror)
    if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
        target_compile_options(examples PRIVATE -Wall -Wextra -pedantic -Werror)
    endif()
endif()
