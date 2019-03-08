#add_subdirectory(python2)
add_subdirectory(python3)
install(DIRECTORY "${CMAKE_SOURCE_DIR}/python/test" DESTINATION ${PROJECT_BINARY_DIR}/dist/python PATTERN "*.py")
