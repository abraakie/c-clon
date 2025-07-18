add_library(lexer STATIC lexer.c)
add_library(parser STATIC parser.c)
add_library(ast STATIC ast.c)
add_library(error STATIC error.c)
add_library(writer STATIC writer.c)
add_library(json_writer STATIC json_writer.c)
add_library(error_handler STATIC error_handler.c)
add_library(utf8_util STATIC utf8_util.c)

set(COMPONENT_LIBS
        ast
        error
        lexer
        parser
        writer
        json_writer
        error_handler
        utf8_util
)

foreach(lib ${COMPONENT_LIBS})
    target_include_directories(${lib}
            PUBLIC
            ${PROJECT_SOURCE_DIR}/include
            PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}
    )
endforeach()

target_link_libraries(lexer
        PUBLIC
        error
        utf8_util
)

target_link_libraries(parser
        PUBLIC
        lexer
        ast
        error
)

target_link_libraries(json_writer
        PUBLIC
        writer
)

target_link_libraries(error_handler
        PUBLIC
        error
        utf8_util
)

add_library(${PROJECT_NAME} STATIC clon.c)
target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(${PROJECT_NAME}
        PUBLIC
        ${COMPONENT_LIBS}
        m
)
