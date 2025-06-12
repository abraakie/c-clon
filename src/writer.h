//
// Created by Aki Abramowski on 04.06.25.
//

#ifndef WRITER_H
#define WRITER_H
#include "ast.h"

typedef struct WriterImpl {
    void (*begin_object)(void * context);
    void (*end_object)(void * context);
    void (*begin_array)(void * context);
    void (*end_array)(void * context);
    void (*write_key)(void * context, const char * key);
    void (*write_string)(void * context, const char * string);
    void (*write_number)(void * context, Number number);
    void (*write_null)(void * context);
    void (*write_bool)(void * context, int value);
} WriterImpl;

typedef struct Writer {
    void * context;
    const WriterImpl * impl;
} Writer;

Writer make_writer(const WriterImpl * impl, void * context);
void write_ast(Writer * writer, const Node * node);

#endif //WRITER_H
