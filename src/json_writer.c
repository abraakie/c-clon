//
// Created by Aki Abramowski on 04.06.25.
//

#include "json_writer.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>

Context make_json_writer_context(char *buffer, const size_t buffer_size) {
    return (Context){buffer, buffer_size, 0, 0, 0, 0, 0, 0, 1};
}

Context make_pretty_json_writer_context(char *buffer, const size_t buffer_size) {
    return (Context){buffer, buffer_size, 0, 1, 2, 0, 0, 0, 1};
}

Context make_pretty_json_writer_context_with_indent(char *buffer, const size_t buffer_size, const int indent) {
    return (Context){buffer, buffer_size, 0, 1, indent, 0, 0, 0, 1};
}

static void write_newline_if_needed(Context * context) {
    if (context->pretty && context->pos < context->buffer_size - 1) {
        context->buffer[context->pos++] = '\n';
    }
}

static void write_comma_if_needed(Context * context) {
    if (context->needs_comma && context->pos < context->buffer_size - 1) {
        context->buffer[context->pos++] = ',';
        write_newline_if_needed(context);
    }
}

static void write_indent_if_needed(Context * context) {
    for (int i = 0; i < context->indent && context->needs_indent && context->pos < context->buffer_size - 1; ++i) {
        context->buffer[context->pos++] = ' ';
    }
}

static void begin_indent(Context * context) {
    context->indent += context->indent_size;
}

static void end_indent(Context * context) {
    context->indent -= context->indent_size;
}

static void json_begin_object(void * ctx) {
    Context * context = ctx;
    write_comma_if_needed(context);
    write_indent_if_needed(context);
    if (context->pos >= context->buffer_size - 1) {
        return;
    }
    context->needs_comma = 0;
    context->needs_indent = 1;
    context->buffer[context->pos++] = '{';
    write_newline_if_needed(context);
    begin_indent(context);
}

static void json_end_object(void * ctx) {
    Context * context = ctx;
    write_newline_if_needed(context);
    end_indent(context);
    write_indent_if_needed(context);
    if (context->pos >= context->buffer_size - 1) {
        return;
    }
    context->needs_comma = 1;
    context->buffer[context->pos++] = '}';
}

static void json_begin_array(void * ctx) {
    Context * context = ctx;
    write_comma_if_needed(context);
    write_indent_if_needed(context);
    if (context->pos >= context->buffer_size - 1) {
        return;
    }
    context->needs_comma = 0;
    context->needs_indent = 1;
    context->buffer[context->pos++] = '[';
    write_newline_if_needed(context);
    begin_indent(context);
}

static void json_end_array(void * ctx) {
    Context * context = ctx;
    write_newline_if_needed(context);
    end_indent(context);
    write_indent_if_needed(context);
    if (context->pos >= context->buffer_size - 1) {
        return;
    }
    context->needs_comma = 1;
    context->buffer[context->pos++] = ']';
}

static void json_write_string(void * ctx, const char * string) {
    Context * context = ctx;
    write_comma_if_needed(context);
    write_indent_if_needed(context);
    if (context->pos >= context->buffer_size - 1) {
        return;
    }
    context->buffer[context->pos++] = '"';
    for (int i = 0; i < strlen(string) && context->pos < context->buffer_size - 1; ++i) {
        context->buffer[context->pos++] = string[i];
    }
    if (context->pos >= context->buffer_size - 1) {
        return;
    }
    context->buffer[context->pos++] = '"';
    context->needs_comma = 1;
    context->needs_indent = 1;
}

static void json_write_key(void * ctx, const char * key) {
    Context * context = ctx;
    write_comma_if_needed(context);
    write_indent_if_needed(context);
    if (context->pos >= context->buffer_size - 1) {
        return;
    }
    context->buffer[context->pos++] = '"';
    for (int i = 0; i < strlen(key) && context->pos < context->buffer_size - 1; ++i) {
        context->buffer[context->pos++] = key[i];
    }
    if (context->pos >= context->buffer_size - 1) {
        return;
    }
    context->buffer[context->pos++] = '"';
    if (context->pretty) {
        context->buffer[context->pos++] = ' ';
    }
    context->buffer[context->pos++] = ':';
    if (context->pretty) {
        context->buffer[context->pos++] = ' ';
    }
    context->needs_comma = 0;
    context->needs_indent = 0;
}

static void json_write_number(void * ctx, const Number number) {
    Context * context = ctx;
    write_comma_if_needed(context);
    write_indent_if_needed(context);
    if (context->pos >= context->buffer_size - 1) {
        return;
    }
    if (number.type == NUMBER_INT) {
        context->pos += snprintf(context->buffer + context->pos, context->buffer_size - context->pos, "%lld", number.i);
    } else {
        context->pos += snprintf(context->buffer + context->pos, context->buffer_size - context->pos, "%.17g", number.f);
    }
    context->needs_comma = 1;
    context->needs_indent = 1;
}

static void json_write_bool(void * ctx, const int bool_value) {
    Context * context = ctx;
    write_comma_if_needed(context);
    write_indent_if_needed(context);
    if (context->pos >= context->buffer_size - 1) {
        return;
    }
    if (bool_value) {
        context->pos += snprintf(context->buffer + context->pos, context->buffer_size - context->pos, "true");
    } else {
        context->pos += snprintf(context->buffer + context->pos, context->buffer_size - context->pos, "false");
    }
    context->needs_comma = 1;
    context->needs_indent = 1;
}

static void json_write_null(void * ctx) {
    Context * context = ctx;
    write_comma_if_needed(context);
    write_indent_if_needed(context);
    if (context->pos >= context->buffer_size - 1) {
        return;
    }
    context->pos += snprintf(context->buffer + context->pos, context->buffer_size - context->pos, "null");
    context->needs_comma = 1;
    context->needs_indent = 1;
}

const WriterImpl JSON_WRITER_IMPL = {
    .begin_object = json_begin_object,
    .end_object = json_end_object,
    .begin_array = json_begin_array,
    .end_array = json_end_array,
    .write_key = json_write_key,
    .write_string = json_write_string,
    .write_number = json_write_number,
    .write_bool = json_write_bool,
    .write_null = json_write_null,
};
