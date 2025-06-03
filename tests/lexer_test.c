//
// Created by Aki Abramowski on 23.05.25.
//

#include "unity.h"
#include "lexer.h"
#include "helper/token_helper.h"
#include <string.h>

#define ASSERT_TOKEN_MESSAGE(expected, actual, base_massage) do { \
    char tok_message[1024]; \
    snprintf(tok_message, sizeof(tok_message), \
        "%s\nToken mismatch:\n" \
        "  Expected: type=%s, length=%zu, content='%.*s'\n" \
        "  Actual:   type=%s, length=%zu, content='%.*s'", \
        base_massage, \
        token_type_to_string(expected.type), expected.length, (int)expected.length, expected.start, \
        token_type_to_string(actual.type), actual.length, (int)actual.length, actual.start); \
    TEST_ASSERT_EQUAL_INT_MESSAGE(expected.type, actual.type, tok_message); \
    TEST_ASSERT_EQUAL_size_t_MESSAGE(expected.length, actual.length, tok_message); \
    TEST_ASSERT_EQUAL_STRING_LEN_MESSAGE(expected.start, actual.start, actual.length, tok_message); \
} while (0)

#define ASSERT_TOKEN_SEQUENCE(input_str, ...) do { \
    LxrContext ctx = MAKE_LEXER(input_str); \
    Token actual; \
    Error error; \
    Token expected[] = {__VA_ARGS__}; \
    size_t token_count = sizeof(expected) / sizeof(expected[0]); \
    char message[512]; \
    for(size_t i = 0; i < token_count; ++i) { \
        if (next_token(&ctx, &actual, &error)) { \
            snprintf(message, sizeof(message), \
                "\nToken error at index %zu\n" \
                "Input: \"%s\"\n" \
                "Position: %zu\n" \
                "Error: %s\n" \
                "Current char: '%c'", \
                i, ctx.input, error.pos, \
                error.message ? error.message : "Unknown error", \
                ctx.input[ctx.pos]); \
            TEST_FAIL_MESSAGE(message); \
        } \
        snprintf(message, sizeof(message), \
            "\nFailed at token index %zu\n" \
            "Input: \"%s\"\n" \
            "Position: %zu\n" \
            "Current char: '%c'", \
            i, ctx.input, ctx.pos, ctx.input[ctx.pos]); \
        ASSERT_TOKEN_MESSAGE(expected[i], actual, message); \
    } \
    if (next_token(&ctx, &actual, &error)) { \
        snprintf(message, sizeof(message), \
            "\nToken error at EOF check\n" \
            "Input: \"%s\"\n" \
            "Position: %zu\n" \
            "Error: %s", \
            ctx.input, error.pos, \
            error.message ? error.message : "Unknown error"); \
        TEST_FAIL_MESSAGE(message); \
    } \
    ASSERT_TOKEN_MESSAGE(TOKEN(TOK_EOF, ""), actual, "Expected EOF at the end of input"); \
} while(0)

void setUp(void) {
}

void tearDown(void) {
}

void test_single_tokens() {
    ASSERT_TOKEN_SEQUENCE("=", TOKEN_EQUAL);
    ASSERT_TOKEN_SEQUENCE("[", TOKEN_LBRACKET);
    ASSERT_TOKEN_SEQUENCE("]", TOKEN_RBRACKET);
    ASSERT_TOKEN_SEQUENCE(",", TOKEN_COMMA);
    ASSERT_TOKEN_SEQUENCE(".", TOKEN_DOT);
    ASSERT_TOKEN_SEQUENCE("");
}

void test_number_tokens() {
    ASSERT_TOKEN_SEQUENCE("1.67e+5", TOKEN_NUMBER("1.67e+5"));
    ASSERT_TOKEN_SEQUENCE("572452454531454364513452345", TOKEN_NUMBER("572452454531454364513452345"));
}

void test_string_tokens() {
    ASSERT_TOKEN_SEQUENCE("hello", TOKEN_STRING("hello"));
    ASSERT_TOKEN_SEQUENCE("\"true\"", TOKEN_STRING("true"));
    ASSERT_TOKEN_SEQUENCE("\"hello\"", TOKEN_STRING("hello"));
    ASSERT_TOKEN_SEQUENCE("\"hello \\\"World\\\"!\"", TOKEN_STRING("hello \\\"World\\\"!"));
}

void test_literal_tokens() {
    ASSERT_TOKEN_SEQUENCE("true", TOKEN_TRUE);
    ASSERT_TOKEN_SEQUENCE("false", TOKEN_FALSE);
    ASSERT_TOKEN_SEQUENCE("null", TOKEN_NULL);
}

void test_complex_tokens() {
    ASSERT_TOKEN_SEQUENCE("[1,\"hello\"]",
        TOKEN_LBRACKET,
        TOKEN_NUMBER("1"),
        TOKEN_COMMA,
        TOKEN_STRING("hello"),
        TOKEN_RBRACKET);
    ASSERT_TOKEN_SEQUENCE("[key=value]",
        TOKEN_LBRACKET,
        TOKEN_STRING("key"),
        TOKEN_EQUAL,
        TOKEN_STRING("value"),
        TOKEN_RBRACKET);
    ASSERT_TOKEN_SEQUENCE("][5'hi'=,",
        TOKEN_RBRACKET,
        TOKEN_LBRACKET,
        TOKEN_NUMBER("5"),
        TOKEN_STRING("hi"),
        TOKEN_EQUAL,
        TOKEN_COMMA);
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_single_tokens);
    RUN_TEST(test_number_tokens);
    RUN_TEST(test_string_tokens);
    RUN_TEST(test_literal_tokens);
    RUN_TEST(test_complex_tokens);
    return UNITY_END();
}
