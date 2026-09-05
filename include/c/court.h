#ifndef CONSTITUTION_C_COURT_H
#define CONSTITUTION_C_COURT_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct constitution_court {
    char *name;
    char *basis;
    bool constitutional_review;
} court_t;

constitution_status_t court_init(court_t *court, const char *name, const char *basis, bool constitutional_review);
void court_reset(court_t *court);

#ifdef __cplusplus
}
#endif

#endif
