#ifndef CONSTITUTION_C_RESULT_H
#define CONSTITUTION_C_RESULT_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct constitution_result {
    constitution_status_t status;
    const char *message;
} constitution_result_t;

const constitution_result_t *constitution_ok(void);
const constitution_result_t *constitution_invalid_argument(void);

#ifdef __cplusplus
}
#endif

#endif
