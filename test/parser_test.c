//
// Created by Aki Abramowski on 24.05.25.
//

#include "parser.h"

#include <stdlib.h>
#include <string.h>

#include "helper/token_helper.h"
#include "mock/mock_lexer.h"
#include "helper/node_helper.h"
#include "unity.h"

#define HANDLE_ERROR(error) do { \
    char err_msg[1024] = {0}; \
    size_t result = format_error(&error, err_msg, sizeof(err_msg)); \
    printf("%s\n", err_msg); \
    TEST_FAIL_MESSAGE("Parsing failed"); \
} while (0)

#define ASSERT_EOF_TOKEN if (token.type != TOK_EOF) { \
    printf("Expected EOF token, got %s\n", token_type_to_string(token.type)); \
    TEST_FAIL_MESSAGE("No EOF token"); \
}

#define TEST_VALUE_PARSING(exp, ...) do { \
    MAKE_MOCK_LEXER_INIT(__VA_ARGS__); \
    Node * expected = exp; \
    Node * actual = NULL; \
    if (parse_value(ctx_ptr, &token, &actual, &error)) { \
        free_node(actual); \
        free_node(expected); \
        HANDLE_ERROR(error); \
    } \
    ASSERT_NODES_EQUAL(expected, actual); \
    free_node(actual); \
    free_node(expected); \
    ASSERT_EOF_TOKEN; \
} while(0)

#define TEST_ARRAY_ENTRY_PARSING(exp, ...) do { \
    MAKE_MOCK_LEXER_INIT(__VA_ARGS__); \
    ArrayEntry * expected = exp; \
    ArrayEntry * actual = NULL; \
    if (parse_array_entry(ctx_ptr, &token, &actual, &error)) { \
        free_array_entry(actual); \
        free_array_entry(expected); \
        HANDLE_ERROR(error); \
    } \
    ASSERT_ARRAY_ENTRIES_EQUAL(expected, actual); \
    free_array_entry(actual); \
    free_array_entry(expected); \
    ASSERT_EOF_TOKEN; \
} while (0)

#define TEST_OBJECT_ENTRY_PARSING(exp, ...) do { \
    MAKE_MOCK_LEXER_INIT(__VA_ARGS__); \
    ObjectEntry * expected = exp; \
    ObjectEntry * actual = NULL; \
    if (parse_object_entry(ctx_ptr, &token, &actual, &error)) { \
        free_object_entry(actual); \
        free_object_entry(expected); \
        HANDLE_ERROR(error); \
    } \
    ASSERT_OBJECT_ENTRIES_EQUAL(expected, actual); \
    free_object_entry(actual); \
    free_object_entry(expected); \
    ASSERT_EOF_TOKEN; \
} while (0)

#define TEST_STRING_PARSING(exp, input) do { \
    MAKE_MOCK_LEXER_INIT(TOKEN_STRING(input)); \
    char * expected = exp; \
    char * actual = NULL; \
    if (parse_string(ctx_ptr, &token, &actual, &error)) { \
        HANDLE_ERROR(error); \
    } \
    ASSERT_STRING_EQUAL(expected, actual); \
    free(actual); \
    ASSERT_EOF_TOKEN; \
} while (0)

#define TEST_NUMBER_PARSING(exp, in) do { \
    MAKE_MOCK_LEXER_INIT(TOKEN_NUMBER(in)); \
    Number expected = exp; \
    Number actual; \
    if (parse_number(ctx_ptr, &token, &actual, &error)) { \
        HANDLE_ERROR(error); \
    } \
    ASSERT_NUMBER_EQUAL(expected, actual); \
    ASSERT_EOF_TOKEN; \
} while (0)

#define TEST_ARRAY_OR_OBJECT_PARSING(exp, ...) do { \
    MAKE_MOCK_LEXER_INIT(__VA_ARGS__); \
    Node * expected = exp; \
    Node * actual = make_node(NODE_ARRAY); \
    if (parse_array_or_object(ctx_ptr, &token, actual, &error)) { \
        free_node(actual); \
        free_node(expected); \
        HANDLE_ERROR(error); \
    } \
    ASSERT_NODES_EQUAL(expected, actual); \
    ASSERT_EOF_TOKEN; \
} while (0)

void setUp(void) {
}

void tearDown(void) {
}

void test_string_parsing() {
    TEST_STRING_PARSING("hello", "hello");
    TEST_STRING_PARSING("Hello \"World\"!", "Hello \\\"World\\\"!");
}

void test_number_parsing() {
    TEST_NUMBER_PARSING(NUMBER_I(0), "0");
    TEST_NUMBER_PARSING(NUMBER_I(123), "123");
    TEST_NUMBER_PARSING(NUMBER_I(-123), "-123");
    TEST_NUMBER_PARSING(NUMBER_I(+123), "+123");

    TEST_NUMBER_PARSING(NUMBER_F(-1.0), "-1.0");
    TEST_NUMBER_PARSING(NUMBER_F(123.456), "123.456");
    TEST_NUMBER_PARSING(NUMBER_F(12345600), "123.456e+5");
    TEST_NUMBER_PARSING(NUMBER_F(-1.5), "-15e-1");
    TEST_NUMBER_PARSING(NUMBER_F(-1.5), "-15e-1");
}

void test_object_entry_parsing() {
    TEST_OBJECT_ENTRY_PARSING(MAKE_OBJECT_ENTRY("key", MAKE_NODE_NULL()), TOKEN_STRING("key"), TOKEN_EQUAL, TOKEN_NULL);
    TEST_OBJECT_ENTRY_PARSING(MAKE_OBJECT_ENTRY("key_one", MAKE_NODE_OBJECT(MAKE_OBJECT_ENTRY("key_two", MAKE_NODE_NULL()))),
        TOKEN_STRING("key_one"), TOKEN_DOT, TOKEN_STRING("key_two"), TOKEN_EQUAL, TOKEN_NULL);
    TEST_OBJECT_ENTRY_PARSING(MAKE_OBJECT_ENTRY("key_one", MAKE_NODE_OBJECT(
        MAKE_OBJECT_ENTRY("key_two", MAKE_NODE_NULL()))),
        TOKEN_STRING("key_one"), TOKEN_LBRACKET, TOKEN_STRING("key_two"), TOKEN_RBRACKET, TOKEN_EQUAL, TOKEN_NULL);
    TEST_OBJECT_ENTRY_PARSING(
        MAKE_OBJECT_ENTRY("key", MAKE_NODE_OBJECT(
            MAKE_OBJECT_ENTRY("key", MAKE_NODE_OBJECT(
                MAKE_OBJECT_ENTRY("key", MAKE_NODE_NULL()))))),
        TOKEN_STRING("key"), TOKEN_DOT, TOKEN_STRING("key"), TOKEN_DOT, TOKEN_STRING("key"), TOKEN_EQUAL, TOKEN_NULL);
}

void test_array_entry_parsing() {
    TEST_ARRAY_ENTRY_PARSING(MAKE_ARRAY_ENTRIES(MAKE_NODE_NULL()), TOKEN_NULL);
}

void test_array_or_object_parsing() {
    TEST_ARRAY_OR_OBJECT_PARSING(MAKE_NODE_ARRAY(), TOKEN_LBRACKET, TOKEN_RBRACKET);
    TEST_ARRAY_OR_OBJECT_PARSING(MAKE_NODE_OBJECT(), TOKEN_LBRACKET, TOKEN_EQUAL, TOKEN_RBRACKET);
    TEST_ARRAY_OR_OBJECT_PARSING(MAKE_NODE_OBJECT(MAKE_OBJECT_ENTRY("null_1", MAKE_NODE_NULL()), MAKE_OBJECT_ENTRY("null_2", MAKE_NODE_NULL())), TOKEN_LBRACKET, TOKEN_STRING("null_1"), TOKEN_EQUAL, TOKEN_NULL, TOKEN_COMMA, TOKEN_STRING("null_2"), TOKEN_EQUAL, TOKEN_NULL, TOKEN_RBRACKET);
    TEST_ARRAY_OR_OBJECT_PARSING(MAKE_NODE_OBJECT(
        MAKE_OBJECT_ENTRY(
            "outer", MAKE_NODE_OBJECT(
                MAKE_OBJECT_ENTRY(
                    "inner_one", MAKE_NODE_NULL()
                ),
                MAKE_OBJECT_ENTRY(
                    "inner_two", MAKE_NODE_NULL()
                )))),
                TOKEN_LBRACKET, TOKEN_STRING("outer"), TOKEN_DOT, TOKEN_STRING("inner_one"), TOKEN_EQUAL, TOKEN_NULL, TOKEN_COMMA, TOKEN_STRING("outer"), TOKEN_LBRACKET, TOKEN_STRING("inner_two"), TOKEN_RBRACKET, TOKEN_EQUAL, TOKEN_NULL, TOKEN_RBRACKET);
}

void test_value_parsing() {
    TEST_VALUE_PARSING(MAKE_NODE_NULL(), TOKEN_NULL);
    TEST_VALUE_PARSING(MAKE_NODE_BOOL(1), TOKEN_TRUE);
    TEST_VALUE_PARSING(MAKE_NODE_BOOL(0), TOKEN_FALSE);
    TEST_VALUE_PARSING(MAKE_NODE_STRING("Hello World!"), TOKEN_STRING("Hello World!"));
    TEST_VALUE_PARSING(MAKE_NODE_NUMBER_F(12.3), TOKEN_NUMBER("12.3"));
    TEST_VALUE_PARSING(MAKE_NODE_NUMBER_I(123), TOKEN_NUMBER("123"));
    TEST_VALUE_PARSING(MAKE_NODE_ARRAY(), TOKEN_LBRACKET, TOKEN_RBRACKET);
    TEST_VALUE_PARSING(MAKE_NODE_OBJECT(), TOKEN_LBRACKET, TOKEN_EQUAL, TOKEN_RBRACKET);
    TEST_VALUE_PARSING(MAKE_NODE_ARRAY(MAKE_NODE_ARRAY()), TOKEN_LBRACKET, TOKEN_LBRACKET, TOKEN_RBRACKET, TOKEN_RBRACKET);
    TEST_VALUE_PARSING(MAKE_NODE_ARRAY(MAKE_NODE_OBJECT()), TOKEN_LBRACKET, TOKEN_LBRACKET, TOKEN_EQUAL, TOKEN_RBRACKET, TOKEN_RBRACKET);
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_string_parsing);
    RUN_TEST(test_number_parsing);
    RUN_TEST(test_object_entry_parsing);
    RUN_TEST(test_array_entry_parsing);
    RUN_TEST(test_array_or_object_parsing);
    RUN_TEST(test_value_parsing);
    return UNITY_END();
}
