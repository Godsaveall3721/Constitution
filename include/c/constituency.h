#ifndef CONSTITUTION_C_CONSTITUENCY_H
#define CONSTITUTION_C_CONSTITUENCY_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum constitution_constituency_kind {
    CONSTITUTION_CONSTITUENCY_KIND_NATION,
    CONSTITUTION_CONSTITUENCY_KIND_REGION,
    CONSTITUTION_CONSTITUENCY_KIND_PROVINCE,
    CONSTITUTION_CONSTITUENCY_KIND_METROPOLITAN_CITY,
    CONSTITUTION_CONSTITUENCY_KIND_SUB_PREFECTURE,
    CONSTITUTION_CONSTITUENCY_KIND_COMMUNE,
    CONSTITUTION_CONSTITUENCY_KIND_OTHER
} constitution_constituency_kind_t;

typedef struct constitution_constituency {
    char *name;
    char *label;
    char *seat_name;
    constitution_constituency_kind_t kind;
    unsigned long long population;
    char *parent_name;
} constituency_t;

constitution_status_t constituency_init(constituency_t *constituency,
                                       const char *name,
                                       const char *label,
                                       const char *seat_name,
                                       constitution_constituency_kind_t kind,
                                       unsigned long long population,
                                       const char *parent_name);
void constituency_reset(constituency_t *constituency);
constitution_status_t constituency_copy(constituency_t *destination, const constituency_t *source);

const char *constitution_constituency_kind_name(const constitution_constituency_kind_t kind);

#ifdef __cplusplus
}
#endif

#endif
