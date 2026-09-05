#include "body.h"
#include "internal.h"

#include <stdlib.h>
#include <string.h>

static constitution_status_t body_reserve(body_t *body, size_t desired_capacity)
{
    if (desired_capacity <= body->member_capacity) {
        return CONSTITUTION_STATUS_OK;
    }

    size_t new_capacity = body->member_capacity == 0 ? 4 : body->member_capacity * 2;
    while (new_capacity < desired_capacity) {
        new_capacity *= 2;
    }

    member_t *members = (member_t *)realloc(body->members, new_capacity * sizeof(member_t));
    if (members == NULL) {
        return CONSTITUTION_STATUS_NO_MEMORY;
    }

    body->members = members;
    body->member_capacity = new_capacity;
    return CONSTITUTION_STATUS_OK;
}

constitution_status_t body_init(body_t *body, const char *name, constitution_body_kind_t kind, size_t seat_total)
{
    if (body == NULL || name == NULL) {
        return CONSTITUTION_STATUS_INVALID_ARGUMENT;
    }

    body->name = constitution_clone_text(name);
    if (body->name == NULL) {
        return CONSTITUTION_STATUS_NO_MEMORY;
    }

    body->kind = kind;
    body->members = NULL;
    body->member_count = 0;
    body->member_capacity = 0;

    /* 参照宪法中“议会体、委员会、合议体”的共同结构，先给席位装上载体。 */
    constitution_status_t status = seat_init(&body->seat, name, seat_total);
    if (status != CONSTITUTION_STATUS_OK) {
        constitution_release_text(&body->name);
        return status;
    }

    return CONSTITUTION_STATUS_OK;
}

void body_reset(body_t *body)
{
    if (body == NULL) {
        return;
    }

    for (size_t index = 0; index < body->member_count; ++index) {
        member_reset(&body->members[index]);
    }
    free(body->members);
    body->members = NULL;
    body->member_count = 0;
    body->member_capacity = 0;

    seat_reset(&body->seat);
    constitution_release_text(&body->name);
}

constitution_status_t body_add_member(body_t *body, const member_t *member)
{
    if (body == NULL || member == NULL) {
        return CONSTITUTION_STATUS_INVALID_ARGUMENT;
    }

    constitution_status_t status = body_reserve(body, body->member_count + 1);
    if (status != CONSTITUTION_STATUS_OK) {
        return status;
    }

    status = member_copy(&body->members[body->member_count], member);
    if (status != CONSTITUTION_STATUS_OK) {
        return status;
    }

    body->member_count++;
    return CONSTITUTION_STATUS_OK;
}
