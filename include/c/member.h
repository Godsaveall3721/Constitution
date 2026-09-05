#ifndef CONSTITUTION_C_MEMBER_H
#define CONSTITUTION_C_MEMBER_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct constitution_member {
    char *name;
    char *role;
    unsigned age;
    char *constituency_name;
} member_t;

constitution_status_t member_init(member_t *member, const char *name, const char *role, unsigned age);
constitution_status_t member_assign_constituency(member_t *member, const char *constituency_name);
void member_reset(member_t *member);
constitution_status_t member_copy(member_t *destination, const member_t *source);

#ifdef __cplusplus
}
#endif

#endif
