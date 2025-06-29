//
// Created by Aki Abramowski on 11.06.25.
//

#include <string.h>

#include "clon_json_writer.h"
#include "unity.h"
#include "helper/node_helper.h"

#define TEST_JSON_WRITING(exp, input) do { \
    Node * in = input; \
    char buf[512] = {0}; \
    Context context = make_json_writer_context(buf, sizeof(buf)); \
    JSON_WRITER_IMPL(&context, in); \
    free_node(in); \
    TEST_ASSERT_EQUAL_STRING(exp, buf); \
} while (0)

#define TEST_JSON_PRETTY_WRITING(exp, input, indent) do { \
    Node * in = input; \
    char buf[512] = {0}; \
    Context context = make_pretty_indent_json_writer_context(buf, sizeof(buf), indent); \
    JSON_WRITER_IMPL(&context, in); \
    free_node(in); \
    TEST_ASSERT_EQUAL_STRING(exp, buf); \
} while (0)

void setUp(void) {
}

void tearDown(void) {
}

void test_simple_json_value_writing() {
    TEST_JSON_WRITING("null", MAKE_NODE_NULL());
    TEST_JSON_WRITING("true", MAKE_NODE_BOOL(1));
    TEST_JSON_WRITING("false", MAKE_NODE_BOOL(0));
    TEST_JSON_WRITING("123", MAKE_NODE_NUMBER_I(123));
    TEST_JSON_WRITING("\"abc\"", MAKE_NODE_STRING("abc"));
    TEST_JSON_WRITING("[]", MAKE_NODE_ARRAY());
    TEST_JSON_WRITING("{}", MAKE_NODE_OBJECT());
}

void test_string_special_character_parsing() {
    TEST_JSON_WRITING("\"\\\"\"", MAKE_NODE_STRING("\""));
    TEST_JSON_WRITING("\"\\\\\"", MAKE_NODE_STRING("\\"));
    TEST_JSON_WRITING("\"\\/\"", MAKE_NODE_STRING("/"));
    TEST_JSON_WRITING("\"\\\b\"", MAKE_NODE_STRING("\b"));
    TEST_JSON_WRITING("\"\\\f\"", MAKE_NODE_STRING("\f"));
    TEST_JSON_WRITING("\"\\\n\"", MAKE_NODE_STRING("\n"));
    TEST_JSON_WRITING("\"\\\r\"", MAKE_NODE_STRING("\r"));
    TEST_JSON_WRITING("\"\\\t\"", MAKE_NODE_STRING("\t"));
}

void test_object_json_writing() {
    TEST_JSON_WRITING("{\"key\":\"value\"}", MAKE_NODE_OBJECT(MAKE_OBJECT_ENTRY("key", MAKE_NODE_STRING("value"))));
    TEST_JSON_WRITING("{\"key\":\"value\",\"key2\":\"value2\"}", MAKE_NODE_OBJECT(MAKE_OBJECT_ENTRY("key", MAKE_NODE_STRING("value")), MAKE_OBJECT_ENTRY("key2", MAKE_NODE_STRING("value2"))));
    TEST_JSON_WRITING("{\"key\":\"value\",\"key2\":{\"inner_key\":\"inner_value\"}}", MAKE_NODE_OBJECT(MAKE_OBJECT_ENTRY("key", MAKE_NODE_STRING("value")), MAKE_OBJECT_ENTRY("key2", MAKE_NODE_OBJECT(MAKE_OBJECT_ENTRY("inner_key", MAKE_NODE_STRING("inner_value"))))));
}

void test_array_json_writing() {
    TEST_JSON_WRITING("[\"a\"]", MAKE_NODE_ARRAY(MAKE_NODE_STRING("a")));
    TEST_JSON_WRITING("[\"a\",\"b\"]", MAKE_NODE_ARRAY(MAKE_NODE_STRING("a"), MAKE_NODE_STRING("b")));
    TEST_JSON_WRITING("[\"a\",[\"b\"],[[\"c\"]]]", MAKE_NODE_ARRAY(MAKE_NODE_STRING("a"), MAKE_NODE_ARRAY(MAKE_NODE_STRING("b")), MAKE_NODE_ARRAY(MAKE_NODE_ARRAY(MAKE_NODE_STRING("c")))));
}

void test_pretty_json_writing() {
    TEST_JSON_PRETTY_WRITING("[ ]", MAKE_NODE_ARRAY(), 4);
    TEST_JSON_PRETTY_WRITING("{ }", MAKE_NODE_OBJECT(), 4);
    TEST_JSON_PRETTY_WRITING(
        "[\n"
        "    \"a\"\n"
        "]"
        , MAKE_NODE_ARRAY(MAKE_NODE_STRING("a")), 4);
    TEST_JSON_PRETTY_WRITING(
        "[\n"
        "    \"a\",\n"
        "    \"b\"\n"
        "]"
        , MAKE_NODE_ARRAY(MAKE_NODE_STRING("a"), MAKE_NODE_STRING("b")), 4);
    TEST_JSON_PRETTY_WRITING(
        "[\n"
        "    \"a\",\n"
        "    [\n"
        "        \"b\"\n"
        "    ],\n"
        "    [\n"
        "        [\n"
        "            \"c\"\n"
        "        ]\n"
        "    ]\n"
        "]", MAKE_NODE_ARRAY(MAKE_NODE_STRING("a"), MAKE_NODE_ARRAY(MAKE_NODE_STRING("b")), MAKE_NODE_ARRAY(MAKE_NODE_ARRAY(MAKE_NODE_STRING("c")))), 4);
    TEST_JSON_PRETTY_WRITING(
        "{\n"
        "    \"key\" : \"value\"\n"
        "}", MAKE_NODE_OBJECT(MAKE_OBJECT_ENTRY("key", MAKE_NODE_STRING("value"))), 4);
    TEST_JSON_PRETTY_WRITING(
        "{\n"
        "    \"key\" : 1\n"
        "}", MAKE_NODE_OBJECT(MAKE_OBJECT_ENTRY("key", MAKE_NODE_NUMBER_I(1))), 4);
    TEST_JSON_PRETTY_WRITING(
        "{\n"
        "    \"key\" : true\n"
        "}", MAKE_NODE_OBJECT(MAKE_OBJECT_ENTRY("key", MAKE_NODE_BOOL(1))), 4);
    TEST_JSON_PRETTY_WRITING(
        "{\n"
        "    \"key\" : null\n"
        "}", MAKE_NODE_OBJECT(MAKE_OBJECT_ENTRY("key", MAKE_NODE_NULL())), 4);
    TEST_JSON_PRETTY_WRITING(
        "{\n"
        "    \"key\" : [\n"
        "        \"value\"\n"
        "    ],\n"
        "    \"key2\" : { },\n"
        "    \"key3\" : [ ]\n"
        "}", MAKE_NODE_OBJECT(MAKE_OBJECT_ENTRY("key", MAKE_NODE_ARRAY(MAKE_NODE_STRING("value"))), MAKE_OBJECT_ENTRY("key2", MAKE_NODE_OBJECT()), MAKE_OBJECT_ENTRY("key3", MAKE_NODE_ARRAY())), 4);
    TEST_JSON_PRETTY_WRITING(
        "{\n"
        "    \"key\" : {\n"
        "        \"inner_key\" : \"inner_value\"\n"
        "    }\n"
        "}", MAKE_NODE_OBJECT(MAKE_OBJECT_ENTRY("key", MAKE_NODE_OBJECT(MAKE_OBJECT_ENTRY("inner_key", MAKE_NODE_STRING("inner_value"))))), 4);
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_simple_json_value_writing);
    RUN_TEST(test_string_special_character_parsing);
    RUN_TEST(test_object_json_writing);
    RUN_TEST(test_array_json_writing);
    RUN_TEST(test_pretty_json_writing);
    return UNITY_END();
}
