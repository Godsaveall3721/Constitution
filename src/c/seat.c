#include "seat.h"
#include "internal.h"

constitution_status_t seat_init(seat_t *seat, const char *name, size_t total)
{
    if (seat == NULL || name == NULL) {
        return CONSTITUTION_STATUS_INVALID_ARGUMENT;
    }

    seat->name = constitution_clone_text(name);
    if (seat->name == NULL) {
        return CONSTITUTION_STATUS_NO_MEMORY;
    }

    seat->total = total;
    seat->occupied = 0;
    return CONSTITUTION_STATUS_OK;
}

void seat_reset(seat_t *seat)
{
    if (seat == NULL) {
        return;
    }

    constitution_release_text(&seat->name);
    seat->total = 0;
    seat->occupied = 0;
}

constitution_status_t seat_assign(seat_t *seat, size_t count)
{
    if (seat == NULL) {
        return CONSTITUTION_STATUS_INVALID_ARGUMENT;
    }

    if (count > seat->total) {
        return CONSTITUTION_STATUS_INVALID_ARGUMENT;
    }

    seat->occupied = count;
    return CONSTITUTION_STATUS_OK;
}
