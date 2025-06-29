#include "clon.h"

#include <stddef.h>
#include <stdlib.h>

#include "lexer.h"
#include "parser.h"

int parse_arg(const char * arg, const WriterImplementation writer_impl, void * writer_context) {
    LxrContext ctx = {arg, 0};
    Token token;
    Error error;
    if (next_token(&ctx, &token, &error)) {
        return ERROR;
    }
    Node * node = NULL;
    if (parse_value(&ctx, &token, &node, &error)) {
        return ERROR;
    }
    writer_impl(writer_context, node);
    free_node(node);
    return SUCCESS;
}

int parse_args(const char ** args, const size_t args_count, const WriterImplementation writer_impl, void * writer_context) {
    LxrContext ctx = {args[0], 0};
    Token token;
    Error error;
    if (next_token(&ctx, &token, &error)) {
        return ERROR;
    }
    Node * node = make_node(NODE_ARRAY);
    if (parse_first_array_or_object_entry(&ctx, &token, node, &error)) {
        return ERROR;
    }
    for (size_t i = 1; i < args_count; ++i) {
        ctx = (LxrContext) {args[i], 0};
        if (next_token(&ctx, &token, &error)) {
            return ERROR;
        }
        if (node->type == NODE_ARRAY) {
            if (parse_array_entry(&ctx, &token, &node->array_entries, &error)) {
                return ERROR;
            }
        } else {
            if (parse_object_entry(&ctx, &token, &node->object_entries, &error)) {
                return ERROR;
            }
        }
    }
    if (node->type == NODE_ARRAY && args_count == 1) {
        Node * tmp = node->array_entries->value;
        free(node);
        node = tmp;
    }
    writer_impl(writer_context, node);
    free_node(node);
    return SUCCESS;
}
