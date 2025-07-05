//
// Created by Aki Abramowski on 04.07.25.
//

#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H
#include "error.h"

typedef void (*ErrorHandlerImpl) (void * context, const Error * error);

size_t format_error(const Error * error, char * buffer, size_t buffer_len);

#endif //ERROR_HANDLER_H
