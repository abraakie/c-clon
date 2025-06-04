//
// Created by Aki Abramowski on 26.05.25.
//

#ifndef ERROR_HELPER_H
#define ERROR_HELPER_H

#include "unity.h"

#define ASSERT_NO_ERROR(f) if (!f) { \
    printf(""); \
    TEST_FAIL_MESSAGE("Unexpected error"); \
}

#endif //ERROR_HELPER_H
