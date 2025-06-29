cmake_minimum_required(VERSION 3.14)
project(clon_sample C)

set(CMAKE_C_STANDARD 99)

include(FetchContent)

FetchContent_Declare(
        c_clon
        GIT_REPOSITORY https://github.com/abraakie/c-clon.git
        GIT_TAG main
)

FetchContent_MakeAvailable(c_clon)

add_executable(clon_sample main.c)
target_link_libraries(clon_sample PRIVATE c_clon)
