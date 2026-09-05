#ifndef CONSTITUTION_C_ELECTION_H
#define CONSTITUTION_C_ELECTION_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct constitution_election {
    char *name;
    constitution_election_kind_t kind;
    char *basis;
    size_t seat_count;
    size_t voter_count;
} election_t;

constitution_status_t election_init(election_t *election,
                                    const char *name,
                                    constitution_election_kind_t kind,
                                    const char *basis,
                                    size_t seat_count,
                                    size_t voter_count);
void election_reset(election_t *election);
constitution_status_t election_describe(const election_t *election);

#ifdef __cplusplus
}
#endif

#endif
