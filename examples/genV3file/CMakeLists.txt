include_directories(${HDF5_INCLUDE_DIR})

link_directories(${CMAKE_SOURCE_DIR}/library/${CMAKE_SYSTEM_NAME})
add_executable(genV3file
        src/main.c
        src/nodeConfig.c
        )

target_link_libraries(genV3file
        ${IUF_LIBRARIES}
        ${HDF5_LIBRARIES}
        ${HDF5_HL_LIBRARIES}
        ${UUID_LIBRARIES}
        )

target_include_directories(genV3file PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${CMAKE_SOURCE_DIR}/library/include
        ${IUF_INCLUDE_DIR}
        )


set_property(TARGET genV3file PROPERTY C_STANDARD 99)

if(MSVC)
    target_compile_options(genV3file PRIVATE /W4 /WX)
    set_target_properties(genV3file PROPERTIES LINK_FLAGS "/ignore:4099 /ignore:4098")
else()
    target_compile_options(genV3file PRIVATE -Wall -Wextra -pedantic -Werror)
    if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
        target_compile_options(genV3file PRIVATE -Wall -Wextra -pedantic -Werror)
    endif()
endif()
