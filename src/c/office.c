#include "office.h"
#include "internal.h"

constitution_status_t office_init(office_t *office, const char *name, constitution_office_kind_t kind)
{
    if (office == NULL || name == NULL) {
        return CONSTITUTION_STATUS_INVALID_ARGUMENT;
    }

    office->name = constitution_clone_text(name);
    if (office->name == NULL) {
        return CONSTITUTION_STATUS_NO_MEMORY;
    }

    office->kind = kind;
    office->has_holder = false;
    office->holder.name = NULL;
    office->holder.role = NULL;
    office->holder.age = 0;
    return CONSTITUTION_STATUS_OK;
}

void office_reset(office_t *office)
{
    if (office == NULL) {
        return;
    }

    member_reset(&office->holder);
    office->has_holder = false;
    constitution_release_text(&office->name);
}

constitution_status_t office_assign_holder(office_t *office, const member_t *member)
{
    if (office == NULL || member == NULL) {
        return CONSTITUTION_STATUS_INVALID_ARGUMENT;
    }

    member_reset(&office->holder);
    constitution_status_t status = member_copy(&office->holder, member);
    if (status != CONSTITUTION_STATUS_OK) {
        return status;
    }

    office->has_holder = true;
    return CONSTITUTION_STATUS_OK;
}
