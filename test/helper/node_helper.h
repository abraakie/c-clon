//
// Created by Aki Abramowski on 25.05.25.
//

#ifndef NODE_H
#define NODE_H

#include "../../include/ast.h"

#include "unity.h"

#define NUMBER_F(n) ((Number){.type=NUMBER_FLOAT, .f=n})
#define NUMBER_I(n) ((Number){.type=NUMBER_INT, .i=n})

#define MAKE_NODE_NUMBER_F(num) ({ \
    Node * node = make_node(NODE_NUMBER); \
    node->number_value = NUMBER_F(num); \
    node; \
})

#define MAKE_NODE_NUMBER_I(num) ({ \
    Node * node = make_node(NODE_NUMBER); \
    node->number_value = NUMBER_I(num); \
    node; \
})

#define MAKE_NODE_STRING(s) ({ \
    Node *node = make_node(NODE_STRING); \
    node->string_value = strdup(s); \
    node; \
})

#define MAKE_NODE_BOOL(b) ({ \
    Node *node = make_node(NODE_BOOL); \
    node->boolean_value = (b); \
    node; \
})

#define MAKE_NODE_NULL() ({ \
    make_node(NODE_NULL); \
})

#define MAKE_ARRAY_ENTRIES(...) ({ \
    Node *entries[] = { __VA_ARGS__ }; \
    ArrayEntry * array_entries = NULL; \
    size_t count = sizeof(entries) / sizeof(entries[0]); \
    for (size_t i = 0; i < count; i++) { \
        array_entries = append_array_entry(array_entries, entries[i]); \
    } \
    array_entries; \
}) \

#define MAKE_NODE_ARRAY(...) ({ \
    Node *node = make_node(NODE_ARRAY); \
    node->array_entries = MAKE_ARRAY_ENTRIES(__VA_ARGS__); \
    node; \
})

#define MAKE_OBJECT_ENTRY(k, v) ({ \
    char *key = strdup(k); \
    make_object_entry(key, v); \
})

#define MAKE_OBJECT_ENTRIES(...) ({ \
    ObjectEntry * object_entries = NULL; \
    ObjectEntry *entries[] = { __VA_ARGS__ }; \
    size_t count = sizeof(entries) / sizeof(entries[0]); \
    for (size_t i = 0; i < count; i++) { \
        object_entries = append_object_entry(object_entries, entries[i]); \
    } \
    object_entries; \
})

#define MAKE_NODE_OBJECT(...) ({ \
    Node *node = make_node(NODE_OBJECT); \
    node->object_entries = NULL; \
    ObjectEntry *entries[] = { __VA_ARGS__ }; \
    size_t count = sizeof(entries) / sizeof(entries[0]); \
    for (size_t i = 0; i < count; i++) { \
        node->object_entries = append_object_entry(node->object_entries, entries[i]); \
    } \
    node; \
})

#define ASSERT_STRING_EQUAL(expected, actual) if (strcmp(expected, actual) != 0) { \
    printf("Expected: %s\nActual: %s\n", expected, actual); \
    free(actual); \
    TEST_FAIL_MESSAGE("Strings do not equal"); \
}

#define PRINT_NUMBER(prefix, num) if (num.type == NUMBER_INT) { \
    printf("%s: type=INT, i=%ld\n", prefix, num.i); \
} \
else { \
    printf("%s: type=FLOAT, i=%g\n", prefix, num.f); \
}

#define ASSERT_NUMBER_EQUAL(expected, actual) if (expected.type != actual.type || (expected.type == NUMBER_INT ? expected.i != actual.i : expected.f != actual.f)) { \
    PRINT_NUMBER("Expected", expected); \
    PRINT_NUMBER("Actual", actual); \
    TEST_FAIL_MESSAGE("Numbers do not equal"); \
}

#define ASSERT_ARRAY_ENTRIES_EQUAL(expected, actual) if (!array_entries_equals(expected, actual)) { \
    char exp_msg[512]; \
    array_entries_write(expected, exp_msg, sizeof(exp_msg)); \
    char act_msg[512]; \
    array_entries_write(actual, act_msg, sizeof(act_msg)); \
    free_array_entry(expected); \
    free_array_entry(actual); \
    printf("Expected: %s\nActual: %s\n", exp_msg, act_msg); \
    TEST_FAIL_MESSAGE("Array entries do not equal"); \
}

#define ASSERT_OBJECT_ENTRIES_EQUAL(expected, actual) if (!object_entries_equals(expected, actual)) { \
    char exp_msg[512]; \
    object_entries_write(expected, exp_msg, sizeof(exp_msg)); \
    char act_msg[512]; \
    object_entries_write(actual, act_msg, sizeof(act_msg)); \
    free_object_entry(expected); \
    free_object_entry(actual); \
    printf("Expected: %s\nActual: %s\n", exp_msg, act_msg); \
    TEST_FAIL_MESSAGE("Object entries do not equal"); \
}

#define ASSERT_NODES_EQUAL(expected, actual) if (!node_equals(expected, actual)) { \
    char exp_msg[512]; \
    node_write(expected, exp_msg, sizeof(exp_msg)); \
    char act_msg[512]; \
    node_write(actual, act_msg, sizeof(act_msg)); \
    free_node(expected); \
    free_node(actual); \
    printf("Expected: %s\nActual: %s\n", exp_msg, act_msg); \
    TEST_FAIL_MESSAGE("Nodes do not equal"); \
}

const char * number_type_to_string(NumberType type);

int array_entries_equals(ArrayEntry * a, ArrayEntry * b);
int object_entries_equals(ObjectEntry * a, ObjectEntry * b);
int node_equals(const Node * a, const Node * b);
size_t object_entries_write(const ObjectEntry *entry, char *buffer, size_t buffer_size);
size_t array_entries_write(const ArrayEntry *entry, char *buffer, size_t buffer_size);
size_t node_write(const Node *node, char *buffer, size_t buffer_size);
#endif //NODE_H
