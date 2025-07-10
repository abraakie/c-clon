//
// Created by Aki Abramowski on 25.05.25.
//

#include "node_helper.h"

#include <string.h>

int array_entries_equals(ArrayEntry * a, ArrayEntry * b) {
    while (a && b) {
        if (!node_equals(a->value, b->value)) {
            return 0;
        }
        a = a->next;
        b = b->next;
    }
    return !a && !b;
}

int object_entries_equals(ObjectEntry * a, ObjectEntry * b) {
    while (a && b) {
        if (strcmp(a->key, b->key) != 0 || !node_equals(a->value, b->value)) {
            return 0;
        }
        a = a->next;
        b = b->next;
    }
    return !a && !b;
}

int node_equals(const Node * a, const Node * b) {
    if (a == NULL || b == NULL) {
        return 0;
    }
    if (a->type != b->type) {
        return 0;
    }
    switch (a->type) {
        case NODE_NULL: {
            return 1;
        }
        case NODE_NUMBER: {
            return a->number_value.type == b->number_value.type && a->number_value.i == b->number_value.i;
        }
        case NODE_STRING: {
            return strcmp(a->string_value, b->string_value) == 0;
        }
        case NODE_BOOL: {
            return a->boolean_value == b->boolean_value;
        }
        case NODE_ARRAY: {
            return array_entries_equals(a->array_entries, b->array_entries);
        }
        case NODE_OBJECT: {
            return object_entries_equals(a->object_entries, b->object_entries);
        }
        default: {
            return 0;
        }
    }
}

size_t object_entries_write(const ObjectEntry *entry, char *buffer, const size_t buffer_size) {
    size_t written = 0;
    while (entry && written < buffer_size - 1) {
        written += snprintf(buffer + written, buffer_size - written, "\"%s\":", entry->key);
        written += node_write(entry->value, buffer + written, buffer_size - written);
        if (entry->next) {
            written += snprintf(buffer + written, buffer_size - written, ",");
        }
        entry = entry->next;
    }
    return written;
}

size_t array_entries_write(const ArrayEntry *entry, char *buffer, const size_t buffer_size) {
    size_t written = 0;
    while (entry && written < buffer_size - 1) {
        written += node_write(entry->value, buffer + written, buffer_size - written);
        if (entry->next) {
            written += snprintf(buffer + written, buffer_size - written, ",");
        }
        entry = entry->next;
    }
    return written;
}

size_t node_write(const Node *node, char *buffer, const size_t buffer_size) {
    if (!node || !buffer || buffer_size == 0) {
        return 0;
    }
    size_t written = 0;
    switch (node->type) {
        case NODE_NUMBER:
            if (node->number_value.type == NUMBER_INT) {
                written += snprintf(buffer + written, buffer_size - written, "%ld", node->number_value.i);
            } else {
                written += snprintf(buffer + written, buffer_size - written, "%g", node->number_value.f);
            }
            break;
        case NODE_STRING:
            written += snprintf(buffer + written, buffer_size - written, "\"%s\"", node->string_value);
            break;
        case NODE_BOOL:
            written += snprintf(buffer + written, buffer_size - written, "%s", node->boolean_value ? "true" : "false");
            break;
        case NODE_NULL:
            written += snprintf(buffer + written, buffer_size - written, "null");
            break;
        case NODE_ARRAY:
            written += snprintf(buffer + written, buffer_size - written, "[");
            written += array_entries_write(node->array_entries, buffer + written, buffer_size - written);
            written += snprintf(buffer + written, buffer_size - written, "]");
            break;
        case NODE_OBJECT:
            written += snprintf(buffer + written, buffer_size - written, "{");
            written += object_entries_write(node->object_entries, buffer + written, buffer_size - written);
            written += snprintf(buffer + written, buffer_size - written, "}");
            break;
        default:
            break;
    }
    return written;
}

