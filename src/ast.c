//
// Created by Aki Abramowski on 29.05.25.
//

#include "ast.h"

#include <stdlib.h>
#include <string.h>

Node * make_node(const NodeType type) {
    Node * node = calloc(1, sizeof(Node));
    if (!node) return NULL;
    node->type = type;
    return node;
}

ArrayEntry * make_array_entry(Node * value) {
    ArrayEntry * entry = calloc(1, sizeof(ArrayEntry));
    if (!entry) return NULL;
    entry->value = value;
    return entry;
}

ArrayEntry * append_array_entry(ArrayEntry * array_entries, Node * value) {
    if (array_entries == NULL) {
        return make_array_entry(value);
    }
    ArrayEntry * entry = array_entries;
    while (entry->next) {
        entry = entry->next;
    }
    entry->next = make_array_entry(value);
    return array_entries;
}

ArrayEntry * append_array_entry_entry(ArrayEntry * array_entries, ArrayEntry * entry) {
    if (array_entries == NULL) {
        return entry;
    }
    ArrayEntry * cur = array_entries;
    while (cur->next) {
        cur = cur->next;
    }
    cur->next = entry;
    return array_entries;
}

ObjectEntry * make_object_entry(char * key, Node * value) {
    ObjectEntry * entry = malloc(sizeof(ObjectEntry));
    if (!entry) return NULL;
    entry->key = key;
    entry->value = value;
    entry->next = NULL;
    return entry;
}

ObjectEntry * append_object_entry(ObjectEntry * object_entries, ObjectEntry * entry) {
    if (object_entries == NULL) {
        return entry;
    }
    ObjectEntry * cur = object_entries;
    while (cur->next) {
        cur = cur->next;
    }
    cur->next = entry;
    return object_entries;
}

int object_entry_contains_key(const ObjectEntry * object_entries, const char * key) {
    const ObjectEntry * entry = object_entries;
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            return 1;
        }
        entry = entry->next;
    }
    return 0;
}

ObjectEntry * object_entry_get_entry(ObjectEntry * object_entries, const char * key) {
    ObjectEntry * entry = object_entries;
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            return entry;
        }
        entry = entry->next;
    }
    return NULL;
}

static void free_object_entry(ObjectEntry * object_entry) {
    if (!object_entry) {
        return;
    }
    free_object_entry(object_entry->next);
    free(object_entry->key);
    free_node(object_entry->value);
    free(object_entry);
}

static void free_array_entry(ArrayEntry * array_entry) {
    if (!array_entry) {
        return;
    }
    free_array_entry(array_entry->next);
    free_node(array_entry->value);
    free(array_entry);
}

void free_node(Node * node) {
    if (!node) {
        return;
    }
    switch (node->type) {
        case NODE_OBJECT:
            free_object_entry(node->object_entries);
            break;
        case NODE_ARRAY:
            free_array_entry(node->array_entries);
            break;
        case NODE_STRING:
            free(node->string_value);
            break;
        case NODE_NUMBER:
        case NODE_BOOL:
        case NODE_NULL:
            break;
    }
    free(node);
}
