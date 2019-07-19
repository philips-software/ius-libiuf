if (NOT "$ENV{VS140COMNTOOLS}" STREQUAL "")
    set(CMAKE_GENERATOR "Visual Studio 14 2015 Win64" CACHE INTERNAL "Name of generator.")
endif()

#if (NOT "$ENV{VS150COMNTOOLS}" STREQUAL "")
set(CMAKE_GENERATOR "Visual Studio 15 2017 Win64" CACHE INTERNAL "Name of generator.")
#endif()


