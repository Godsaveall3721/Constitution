#ifndef CONSTITUTION_C_IMPEACHMENT_H
#define CONSTITUTION_C_IMPEACHMENT_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct constitution_impeachment {
    char *name;
    char *basis;
    char *target;
} impeachment_t;

constitution_status_t impeachment_init(impeachment_t *impeachment,
                                       const char *name,
                                       const char *basis,
                                       const char *target);
void impeachment_reset(impeachment_t *impeachment);

#ifdef __cplusplus
}
#endif

#endif
