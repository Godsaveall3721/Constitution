#ifndef CONSTITUTION_C_BODY_H
#define CONSTITUTION_C_BODY_H

#include "member.h"
#include "seat.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct constitution_body {
    char *name;
    constitution_body_kind_t kind;
    seat_t seat;
    member_t *members;
    size_t member_count;
    size_t member_capacity;
} body_t;

constitution_status_t body_init(body_t *body, const char *name, constitution_body_kind_t kind, size_t seat_total);
void body_reset(body_t *body);
constitution_status_t body_add_member(body_t *body, const member_t *member);

#ifdef __cplusplus
}
#endif

#endif
