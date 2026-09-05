#include "election.h"
#include "internal.h"

#include <stdio.h>

constitution_status_t election_init(election_t *election,
                                    const char *name,
                                    constitution_election_kind_t kind,
                                    const char *basis,
                                    size_t seat_count,
                                    size_t voter_count)
{
    if (election == NULL || name == NULL) {
        return CONSTITUTION_STATUS_INVALID_ARGUMENT;
    }

    election->name = constitution_clone_text(name);
    election->basis = constitution_clone_text(basis);
    if (election->name == NULL || election->basis == NULL) {
        election_reset(election);
        return CONSTITUTION_STATUS_NO_MEMORY;
    }

    election->kind = kind;
    election->seat_count = seat_count;
    election->voter_count = voter_count;
    return CONSTITUTION_STATUS_OK;
}

void election_reset(election_t *election)
{
    if (election == NULL) {
        return;
    }

    constitution_release_text(&election->name);
    constitution_release_text(&election->basis);
    election->seat_count = 0;
    election->voter_count = 0;
}

constitution_status_t election_describe(const election_t *election)
{
    if (election == NULL || election->name == NULL) {
        return CONSTITUTION_STATUS_INVALID_ARGUMENT;
    }

    printf("[选举] %s | %s | 席位:%zu | 选民:%zu\n",
           election->name,
           constitution_election_kind_name(election->kind),
           election->seat_count,
           election->voter_count);
    return CONSTITUTION_STATUS_OK;
}
