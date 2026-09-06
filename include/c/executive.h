#ifndef CONSTITUTION_C_EXECUTIVE_H
#define CONSTITUTION_C_EXECUTIVE_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum constitution_executive_mode {
    CONSTITUTION_EXECUTIVE_MODE_DIRECT = 0,
    CONSTITUTION_EXECUTIVE_MODE_INDIRECT
} constitution_executive_mode_t;

typedef struct constitution_executive_resolution {
    constitution_executive_mode_t mode;
    double direct_bill_support_ratio;
    bool direct_bill_passed;
    size_t electoral_college_members;
    const char *president_method;
    const char *government_head;
    const char *cabinet_presidency;
    const char *executive_power;
    const char *president_vote_rights;
    const char *prime_minister_role;
    const char *cabinet_note;
    const char *electoral_college_note;
} constitution_executive_resolution_t;

constitution_executive_resolution_t constitution_resolve_executive_mode(double direct_bill_support_ratio, size_t electoral_college_members);
const char *constitution_executive_mode_name(const constitution_executive_mode_t mode);
void constitution_print_executive_resolution(const constitution_executive_resolution_t *resolution);

#ifdef __cplusplus
}
#endif

#endif
