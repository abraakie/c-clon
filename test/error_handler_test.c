//
// Created by Aki Abramowski on 05.07.25.
//

#include "unity.h"
#include "error_handler.h"

void setUp() {}
void tearDown() {}

static void test_syntax_error_formatting() {
    const char * input = "my_awesome_faulty!input";
    Error error;
    make_syntax_error(&error, input, &input[17], 1, 0, "My Error");
    const char * expected =
        "my_awesome_faulty!input\n"
        "                 !\n"
        "                 ^\n"
        "My Error";
    char actual[512] = {0};
    format_error(&error, actual, sizeof(actual));
    TEST_ASSERT_EQUAL_STRING(expected, actual);
}

static void test_syntax_error_formatting_eof_token() {
    const char * input = "my_awesome_faulty_input=";
    Error error;
    make_syntax_error(&error, input, &input[24], 0, 0, "My Missing Value Error");
    const char * expected =
        "my_awesome_faulty_input=\n"
        "                        ^\n"
        "My Missing Value Error";
    char actual[512] = {0};
    format_error(&error, actual, sizeof(actual));
    TEST_ASSERT_EQUAL_STRING(expected, actual);
}

static void test_memory_error_formatting() {
    Error error;
    make_memory_error(&error, "My Mem Err");
    const char * expected = "Memory allocation failed: My Mem Err";
    char actual[512] = {0};
    format_error(&error, actual, sizeof(actual));
    TEST_ASSERT_EQUAL_STRING(expected, actual);
}

static void test_buffer_overflow_error_formatting() {
    Error error;
    make_buffer_overflow_error(&error, "My Buffer Overflow Err");
    const char * expected = "Buffer overflow: My Buffer Overflow Err";
    char actual[512] = {0};
    format_error(&error, actual, sizeof(actual));
    TEST_ASSERT_EQUAL_STRING(expected, actual);
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_syntax_error_formatting);
    RUN_TEST(test_syntax_error_formatting_eof_token);
    RUN_TEST(test_memory_error_formatting);
    RUN_TEST(test_buffer_overflow_error_formatting);
    return UNITY_END();
}
