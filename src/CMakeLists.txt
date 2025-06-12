add_library(lexer STATIC lexer.c)
add_library(parser STATIC parser.c)
add_library(ast STATIC ast.c)
add_library(error STATIC error.c)
add_library(writer STATIC writer.c)
add_library(json_writer STATIC json_writer.c)

set(COMPONENT_LIBS
        ast
        error
        lexer
        parser
        writer
        json_writer
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
)

target_link_libraries(json_writer
        PUBLIC
        writer
        ast
)

target_link_libraries(parser
        PUBLIC
        lexer
        ast
        error
)

add_library(${PROJECT_NAME} STATIC library.c)
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
