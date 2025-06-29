//
// Created by Aki Abramowski on 11.06.25.
//

#include "clon_json_writer.h"
#include "clon.h"
#include "unity.h"

#define TEST_ARG_PARSING(exp, input) do { \
    char buffer[512] = {0}; \
    Context context = make_json_writer_context(buffer, sizeof(buffer)); \
    TEST_ASSERT_FALSE(parse_arg(input, JSON_WRITER_IMPL, &context)); \
    TEST_ASSERT_EQUAL_STRING(exp, buffer); \
} while (0)

#define TEST_ARGS_PARSING(exp, ...) do { \
    const char * const args[] = {__VA_ARGS__}; \
    char buffer[512] = {0}; \
    Context context = make_json_writer_context(buffer, sizeof(buffer)); \
    TEST_ASSERT_FALSE(parse_args((const char **)args, sizeof(args) / sizeof(args[0]), JSON_WRITER_IMPL, &context)); \
    TEST_ASSERT_EQUAL_STRING(exp, buffer); \
} while (0)

void setUp(void) {
}

void tearDown(void) {
}

void test_arg_parsing() {
    TEST_ARG_PARSING("null", "null");
}

void test_args_parsing() {
    TEST_ARGS_PARSING("null", "null");
    TEST_ARGS_PARSING("[null,null]", "null", "null");
    TEST_ARGS_PARSING("[null,{}]", "null", "[=]");
    TEST_ARGS_PARSING("{\"key1\":\"value\",\"key2\":\"value\"}", "key1=value", "key2=value");
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_arg_parsing);
    RUN_TEST(test_args_parsing);
    return UNITY_END();
}
