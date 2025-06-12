//
// Created by Aki Abramowski on 11.06.25.
//

#include "json_writer.h"
#include "library.h"
#include "unity.h"

void setUp(void) {
}

void tearDown(void) {
}

void test_arg_parsing() {
    char buffer[128];
    Context context = make_json_writer_context(buffer, sizeof(buffer));
    TEST_ASSERT_FALSE(parse_arg("value", &JSON_WRITER_IMPL, &context));
    TEST_ASSERT_EQUAL_STRING("\"value\"", buffer);
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_arg_parsing);
    return UNITY_END();
}
