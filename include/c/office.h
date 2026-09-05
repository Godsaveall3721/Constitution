#ifndef CONSTITUTION_C_OFFICE_H
#define CONSTITUTION_C_OFFICE_H

#include "member.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct constitution_office {
    char *name;
    constitution_office_kind_t kind;
    member_t holder;
    bool has_holder;
} office_t;

constitution_status_t office_init(office_t *office, const char *name, constitution_office_kind_t kind);
void office_reset(office_t *office);
constitution_status_t office_assign_holder(office_t *office, const member_t *member);

#ifdef __cplusplus
}
#endif

#endif
