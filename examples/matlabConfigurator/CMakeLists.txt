include_directories(${HDF5_INCLUDE_DIR})

link_directories(${CMAKE_SOURCE_DIR}/library/${CMAKE_SYSTEM_NAME})
add_executable(matlabConfigurator
        src/main.c
        )

target_link_libraries(matlabConfigurator
        ${IUF_LIBRARIES}
        ${HDF5_LIBRARIES}
        ${HDF5_HL_LIBRARIES}
        ${UUID_LIBRARIES}
        )

target_include_directories(matlabConfigurator PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_SOURCE_DIR}/library/include
        ${IUF_INCLUDE_DIR}
        )


set_property(TARGET matlabConfigurator PROPERTY C_STANDARD 99)

if(MSVC)
    target_compile_options(matlabConfigurator PRIVATE /W4 /WX)
    set_target_properties(matlabConfigurator PROPERTIES LINK_FLAGS "/ignore:4099 /ignore:4098")
else()
    target_compile_options(matlabConfigurator PRIVATE -Wall -Wextra -pedantic -Werror)
    if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
        target_compile_options(matlabConfigurator PRIVATE -Wall -Wextra -pedantic -Werror)
    endif()
endif()
