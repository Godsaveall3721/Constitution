#include "member.h"
#include "internal.h"

constitution_status_t member_init(member_t *member, const char *name, const char *role, unsigned age)
{
    if (member == NULL || name == NULL || role == NULL) {
        return CONSTITUTION_STATUS_INVALID_ARGUMENT;
    }

    member->name = constitution_clone_text(name);
    member->role = constitution_clone_text(role);
    member->constituency_name = NULL;
    if (member->name == NULL || member->role == NULL) {
        member_reset(member);
        return CONSTITUTION_STATUS_NO_MEMORY;
    }

    member->age = age;
    return CONSTITUTION_STATUS_OK;
}

constitution_status_t member_assign_constituency(member_t *member, const char *constituency_name)
{
    if (member == NULL) {
        return CONSTITUTION_STATUS_INVALID_ARGUMENT;
    }

    constitution_release_text(&member->constituency_name);
    member->constituency_name = constitution_clone_text(constituency_name);
    if (constituency_name != NULL && member->constituency_name == NULL) {
        return CONSTITUTION_STATUS_NO_MEMORY;
    }

    return CONSTITUTION_STATUS_OK;
}

void member_reset(member_t *member)
{
    if (member == NULL) {
        return;
    }

    constitution_release_text(&member->name);
    constitution_release_text(&member->role);
    constitution_release_text(&member->constituency_name);
    member->age = 0;
}

constitution_status_t member_copy(member_t *destination, const member_t *source)
{
    if (destination == NULL || source == NULL) {
        return CONSTITUTION_STATUS_INVALID_ARGUMENT;
    }

    return member_init(destination, source->name, source->role, source->age);
}
