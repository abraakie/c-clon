//
// Created by Aki Abramowski on 04.06.25.
//

#ifndef JSON_WRITER_H
#define JSON_WRITER_H

#include <stddef.h>

#include "writer.h"

typedef struct Context {
    char * buffer;
    size_t buffer_size;
    size_t pos;
    int pretty;
    int indent_size;
    size_t written;
    int indent;
    int needs_comma;
    int needs_indent;
} Context;

extern const WriterImpl JSON_WRITER_IMPL;

Context make_json_writer_context(char * buffer, size_t buffer_size);
Context make_pretty_json_writer_context(char * buffer, size_t buffer_size);
Context make_pretty_json_writer_context_with_indent(char * buffer, size_t buffer_size, int indent);

#endif //JSON_WRITER_H
