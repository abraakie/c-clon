#ifndef C_CLON_LIBRARY_H
#define C_CLON_LIBRARY_H

#include <stddef.h>

#include "writer.h"

int parse_arg(const char * arg, WriterImplementation writer_impl, void * writer_context);
int parse_args(const char ** args, size_t args_count, WriterImplementation writer_impl, void * writer_context);

#endif // C_CLON_LIBRARY_H
