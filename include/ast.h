//
// Created by Aki Abramowski on 29.05.25.
//

#ifndef AST_H
#define AST_H
#include <stdint.h>

typedef enum {
    NUMBER_INT,
    NUMBER_FLOAT
} NumberType;

typedef struct Number {
    NumberType type;
    union {
        int64_t i;
        double f;
    };
} Number;

typedef struct ArrayEntry {
    struct Node *value;
    struct ArrayEntry *next;
} ArrayEntry;

typedef struct ObjectEntry {
    char *key;
    struct Node *value;
    struct ObjectEntry *next;
} ObjectEntry;

typedef enum {
    NODE_OBJECT,
    NODE_ARRAY,
    NODE_STRING,
    NODE_NUMBER,
    NODE_BOOL,
    NODE_NULL
} NodeType;

typedef struct Node {
    NodeType type;
    union {
        ObjectEntry * object_entries;
        ArrayEntry * array_entries;
        Number number_value;
        char * string_value;
        int boolean_value;
    };
} Node;

void free_array_entry(ArrayEntry * array_entry);
void free_object_entry(ObjectEntry * object_entry);
void free_node(Node * node);

Node * make_node(NodeType type);
ArrayEntry * make_array_entry(Node * value);
ArrayEntry * append_array_entry(ArrayEntry * array_entries, Node * value);
ArrayEntry * append_array_entry_entry(ArrayEntry * array_entries, ArrayEntry * entry);
ObjectEntry * make_object_entry(char * key, Node * value);
ObjectEntry * append_object_entry(ObjectEntry * object_entries, ObjectEntry * entry);
int object_entry_contains_key(const ObjectEntry * object_entries, const char * key);
ObjectEntry * object_entry_get_entry(ObjectEntry * object_entries, const char * key);

#endif //AST_H
