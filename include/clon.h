#ifndef C_CLON_LIBRARY_H
#define C_CLON_LIBRARY_H

#include <stddef.h>

#include "error_handler.h"
#include "writer.h"

int parse_arg(const char * arg, WriterImpl writer_impl, void * writer_context, ErrorHandlerImpl error_handler_impl, void * error_handler_context);
int parse_args(const char ** args, size_t args_count, WriterImpl writer_impl, void * writer_context, ErrorHandlerImpl error_handler_impl, void * error_handler_context);

#endif // C_CLON_LIBRARY_H
