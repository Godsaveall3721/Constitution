#ifndef CONSTITUTION_C_REFERENDUM_H
#define CONSTITUTION_C_REFERENDUM_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct constitution_referendum {
    char *name;
    char *basis;
    size_t electorate;
} referendum_t;

constitution_status_t referendum_init(referendum_t *referendum,
                                      const char *name,
                                      const char *basis,
                                      size_t electorate);
void referendum_reset(referendum_t *referendum);

#ifdef __cplusplus
}
#endif

#endif
