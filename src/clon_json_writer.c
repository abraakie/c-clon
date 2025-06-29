//
// Created by Aki Abramowski on 15.06.25.
//

#include "clon_json_writer.h"

#include <stdio.h>
#include <string.h>
#include <secure/_string.h>

Context make_json_writer_context(char* buffer, const size_t buffer_size) {
    return (Context) {
        .buffer = buffer,
        .buffer_size = buffer_size,
        .pos = 0,
        .pretty = 0,
        .indent_size = 0,
        .indent = 0
    };
}

Context make_pretty_json_writer_context(char* buffer, const size_t buffer_size) {
    return (Context) {
        .buffer = buffer,
        .buffer_size = buffer_size,
        .pos = 0,
        .pretty = 1,
        .indent_size = 4,
        .indent = 0
    };
}

Context make_pretty_indent_json_writer_context(char* buffer, const size_t buffer_size, const int indent_size) {
    return (Context) {
        .buffer = buffer,
        .buffer_size = buffer_size,
        .pos = 0,
        .pretty = 1,
        .indent_size = indent_size,
        .indent = 0
    };
}

static void write_indent_if_necessary(Context * context) {
    if (context->pretty) {
        for (size_t i = 0; i < context->indent && context->pos < context->buffer_size - 1; ++i) {
            context->buffer[context->pos++] = ' ';
        }
    }
}

static void start_indent_if_necessary(Context * context) {
    if (context->pretty) {
        if (context->pos < context->buffer_size - 1) {
            context->buffer[context->pos++] = '\n';
        }
        context->indent += context->indent_size;
    }
}

static void end_indent_if_necessary(Context * context) {
    if (context->pretty) {
        if (context->pos < context->buffer_size - 1) {
            context->buffer[context->pos++] = '\n';
        }
        context->indent -= context->indent_size;
    }
}

static void write_string(Context * context, const char * string) {
    context->buffer[context->pos++] = '"';
    for (const char* str = string; *str && context->pos < context->buffer_size - 1; ++str) {
        if (*str == '"' || *str == '\\' || *str == '/' || *str == '\b' || *str == '\f' || *str == '\n' || *str == '\r' || *str == '\t') {
            context->buffer[context->pos++] = '\\';
        }
        context->buffer[context->pos++] = *str;
    }
    context->buffer[context->pos++] = '"';
}

static void write_node(Context * context, const Node * node) {
    if (context->pos >= context->buffer_size) {
        return;
    }
    switch (node->type) {
        case NODE_OBJECT:
            context->buffer[context->pos++] = '{';
            if (node->object_entries) {
                start_indent_if_necessary(context);
                for (const ObjectEntry* entry = node->object_entries; entry && context->pos < context->buffer_size - 1; entry = entry->next) {
                    write_indent_if_necessary(context);
                    write_string(context, entry->key);
                    if (context->pretty) {
                        context->buffer[context->pos++] = ' ';
                    }
                    context->buffer[context->pos++] = ':';
                    if (context->pretty) {
                        context->buffer[context->pos++] = ' ';
                    }
                    write_node(context, entry->value);
                    if (entry->next && context->pos < context->buffer_size - 1) {
                        context->buffer[context->pos++] = ',';
                        if (context->pretty) {
                            context->buffer[context->pos++] = '\n';
                        }
                    }
                }
                end_indent_if_necessary(context);
                write_indent_if_necessary(context);
            } else if (context->pretty && context->pos < context->buffer_size - 1) {
                context->buffer[context->pos++] = ' ';
            }
            context->buffer[context->pos++] = '}';
            break;
        case NODE_ARRAY:
            context->buffer[context->pos++] = '[';
            if (node->array_entries) {
                start_indent_if_necessary(context);
                for (const ArrayEntry* entry = node->array_entries; entry && context->pos < context->buffer_size - 1; entry = entry->next) {
                    if (context->pretty) {
                        for (int i = 0; i < context->indent; i++) {
                            context->buffer[context->pos++] = ' ';
                        }
                    }
                    write_node(context, entry->value);
                    if (entry->next && context->pos < context->buffer_size - 1) {
                        context->buffer[context->pos++] = ',';
                        if (context->pretty && context->pos < context->buffer_size - 1) {
                            context->buffer[context->pos++] = '\n';
                        }
                    }
                }
                end_indent_if_necessary(context);
                write_indent_if_necessary(context);
            } else if (context->pretty && context->pos < context->buffer_size - 1) {
                context->buffer[context->pos++] = ' ';
            }
            context->buffer[context->pos++] = ']';
            break;
        case NODE_STRING:
            write_string(context, node->string_value);
            break;
        case NODE_NUMBER:
            if (node->number_value.type == NUMBER_INT) {
                context->pos += snprintf(context->buffer + context->pos,
                                      context->buffer_size - context->pos,
                                      "%lld", node->number_value.i);
            } else {
                context->pos += snprintf(context->buffer + context->pos,
                                      context->buffer_size - context->pos,
                                      "%.17g", node->number_value.f);
            }
            break;
        case NODE_BOOL:
            if (node->boolean_value) {
                strcpy(context->buffer + context->pos, "true");
                context->pos += strlen("true");
            } else {
                strcpy(context->buffer + context->pos, "false");
                context->pos += strlen("false");
            }
            break;

        case NODE_NULL:
            strcpy(context->buffer + context->pos, "null");
            context->pos += strlen("null");
            break;
    }
}

void write_ast(void * ctx, const Node * node) {
    if (!ctx || !node) {
        return;
    }
    Context * context = ctx;
    write_node(context, node);
    if (context->pos < context->buffer_size) {
        context->buffer[context->pos] = '\0';
    }
}

const WriterImplementation JSON_WRITER_IMPL = write_ast;
