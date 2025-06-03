cmake_minimum_required(VERSION 3.31.6)
project(c_clon C)

set(CMAKE_C_STANDARD 99)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

add_subdirectory(src)

option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
