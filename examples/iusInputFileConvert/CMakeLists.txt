include_directories(${HDF5_INCLUDE_DIR})

link_directories(${CMAKE_SOURCE_DIR}/library/${CMAKE_SYSTEM_NAME})
add_executable(iusv2tov3
        src/main.c
        src/readInputFileV2.c
        )

target_link_libraries(iusv2tov3
        ${IUS_LIBRARIES}
        ${HDF5_LIBRARIES}
        ${HDF5_HL_LIBRARIES}
        ${UUID_LIBRARIES}
        )

target_include_directories(iusv2tov3 PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${CMAKE_SOURCE_DIR}/include
        ${IUS_INCLUDE_DIR}
        )


set_property(TARGET iusv2tov3 PROPERTY C_STANDARD 99)

if(MSVC)
    set_target_properties(iusv2tov3 PROPERTIES LINK_FLAGS "/ignore:4099 /ignore:4098")
    target_compile_options(iusv2tov3 PRIVATE /W4 /WX)
else()
    target_compile_options(iusv2tov3 PRIVATE -Wall -Wextra -pedantic -Werror)
    if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
        target_compile_options(iusv2tov3 PRIVATE -Wall -Wextra -pedantic -Werror)
    endif()
endif()
