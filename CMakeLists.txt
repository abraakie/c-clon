cmake_minimum_required(VERSION 3.25.1)
project(c_clon C)

set(CMAKE_C_STANDARD 99)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

add_subdirectory(src)

option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

option(ENABLE_COVERAGE "Enable coverage reporting" OFF)
if(ENABLE_COVERAGE)
    add_compile_options(--coverage -O0 -g)
    add_link_options(--coverage)
endif()
