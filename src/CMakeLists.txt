add_library(lexer STATIC lexer.c)
target_include_directories(lexer PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(parser STATIC parser.c)
target_include_directories(parser PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(ast STATIC ast.c)
target_include_directories(ast PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(error STATIC error.c)
target_include_directories(error PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(${PROJECT_NAME} STATIC library.c)
target_link_libraries(${PROJECT_NAME}
        PUBLIC
        lexer
        parser
        ast
        error
)
