include_directories(include)

add_executable(unitTests
        src/uniTests.c
        )

target_link_libraries(unitTests
        ius
        Unity
        )

add_test(suite_1_test unitTests)