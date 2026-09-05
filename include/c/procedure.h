#ifndef CONSTITUTION_C_PROCEDURE_H
#define CONSTITUTION_C_PROCEDURE_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct constitution_procedure {
    char *name;
    constitution_procedure_kind_t kind;
    char *basis;
    constitution_status_t (*run)(void *context);
} procedure_t;

constitution_status_t procedure_init(procedure_t *procedure,
                                     const char *name,
                                     constitution_procedure_kind_t kind,
                                     const char *basis,
                                     constitution_status_t (*run)(void *context));
void procedure_reset(procedure_t *procedure);
constitution_status_t procedure_execute(procedure_t *procedure, void *context);

#ifdef __cplusplus
}
#endif

#endif
