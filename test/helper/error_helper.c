//
// Created by Aki Abramowski on 26.05.25.
//

#include "error_helper.h"

static void handle_error(void * context, const Error * error) {
    char buffer[512] = {0};
    format_error(error, buffer, sizeof(buffer));
    printf("%s\n", buffer);
}

const ErrorHandlerImpl TEST_ERROR_HANDLER_IMPL = handle_error;
