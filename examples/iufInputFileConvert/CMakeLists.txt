include_directories(${HDF5_INCLUDE_DIR})

link_directories(${CMAKE_SOURCE_DIR}/library/${CMAKE_SYSTEM_NAME})
add_executable(iufv2tov3
        src/main.c
        src/readInputFileV2.c
        )

target_link_libraries(iufv2tov3
        ${IUF_LIBRARIES}
        ${HDF5_LIBRARIES}
        ${HDF5_HL_LIBRARIES}
        ${UUID_LIBRARIES}
        )

target_include_directories(iufv2tov3 PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${CMAKE_SOURCE_DIR}/library/include
        ${IUF_INCLUDE_DIR}
        )


set_property(TARGET iufv2tov3 PROPERTY C_STANDARD 99)

if(MSVC)
    set_target_properties(iufv2tov3 PROPERTIES LINK_FLAGS "/ignore:4099 /ignore:4098")
    target_compile_options(iufv2tov3 PRIVATE /W4 /WX)
else()
    target_compile_options(iufv2tov3 PRIVATE -Wall -Wextra -pedantic -Werror)
    if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
        target_compile_options(iufv2tov3 PRIVATE -Wall -Wextra -pedantic -Werror)
    endif()
endif()
