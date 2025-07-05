//
// Created by Aki Abramowski on 15.06.25.
//

#ifndef CLON_JSON_WRITER_H
#define CLON_JSON_WRITER_H

#include <stddef.h>

#include "writer.h"

typedef struct Context {
    char * buffer;
    size_t buffer_size;
    size_t pos;
    int pretty;
    int indent_size;
    size_t written;
    size_t indent;
} Context;

Context make_json_writer_context(char* buffer, size_t buffer_size);
Context make_pretty_json_writer_context(char* buffer, size_t buffer_size);
Context make_pretty_indent_json_writer_context(char* buffer, size_t buffer_size, int indent_size);

extern const WriterImpl JSON_WRITER_IMPL;

#endif //CLON_JSON_WRITER_H
