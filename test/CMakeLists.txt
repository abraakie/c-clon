include(FetchContent)
FetchContent_Declare(
        unity
        GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
        GIT_TAG v2.5.2
)
FetchContent_MakeAvailable(unity)

target_compile_definitions(unity
        PUBLIC
        UNITY_INCLUDE_DOUBLE
        UNITY_DOUBLE_PRECISION=1e-12
)

add_library(test_helper STATIC
        helper/error_helper.c
        helper/node_helper.c
        helper/token_helper.c
)

target_include_directories(test_helper
        PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(test_helper
        PUBLIC
        ${PROJECT_NAME}
        unity
)

add_library(mock_lexer STATIC mock/mock_lexer.c)
target_include_directories(mock_lexer
        PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)
target_link_libraries(mock_lexer
        PUBLIC
        ${PROJECT_NAME}
        test_helper
)

add_executable(parser_test parser_test.c)
target_link_libraries(parser_test
        PRIVATE
        ${PROJECT_NAME}
        unity
        test_helper
        mock_lexer
)
target_include_directories(parser_test
        PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src
)
add_test(NAME parser_test COMMAND parser_test)

file(GLOB TEST_SOURCES "*_test.c")
list(REMOVE_ITEM TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/parser_test.c")
foreach(TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_SOURCE})
    target_link_libraries(${TEST_NAME}
            PRIVATE
            ${PROJECT_NAME}
            unity
            test_helper
    )
    target_include_directories(${TEST_NAME}
            PRIVATE
            ${PROJECT_SOURCE_DIR}/include
            ${PROJECT_SOURCE_DIR}/src
    )
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()
