//
// Created by Aki Abramowski on 26.05.25.
//

#ifndef ERROR_HELPER_H
#define ERROR_HELPER_H

#include "unity.h"
#include "error_handler.h"

#define ASSERT_NO_ERROR(f) if (!f) { \
    printf(""); \
    TEST_FAIL_MESSAGE("Unexpected error"); \
}

extern const ErrorHandlerImpl TEST_ERROR_HANDLER_IMPL;

#endif //ERROR_HELPER_H
