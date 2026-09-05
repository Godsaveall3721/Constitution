#ifndef CONSTITUTION_C_SEAT_H
#define CONSTITUTION_C_SEAT_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct constitution_seat {
    char *name;
    size_t total;
    size_t occupied;
} seat_t;

constitution_status_t seat_init(seat_t *seat, const char *name, size_t total);
void seat_reset(seat_t *seat);
constitution_status_t seat_assign(seat_t *seat, size_t count);

#ifdef __cplusplus
}
#endif

#endif
