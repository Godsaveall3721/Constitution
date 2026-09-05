#include "referendum.h"
#include "internal.h"

constitution_status_t referendum_init(referendum_t *referendum,
                                      const char *name,
                                      const char *basis,
                                      size_t electorate)
{
    if (referendum == NULL || name == NULL) {
        return CONSTITUTION_STATUS_INVALID_ARGUMENT;
    }

    referendum->name = constitution_clone_text(name);
    referendum->basis = constitution_clone_text(basis);
    if (referendum->name == NULL || referendum->basis == NULL) {
        referendum_reset(referendum);
        return CONSTITUTION_STATUS_NO_MEMORY;
    }

    referendum->electorate = electorate;
    return CONSTITUTION_STATUS_OK;
}

void referendum_reset(referendum_t *referendum)
{
    if (referendum == NULL) {
        return;
    }

    constitution_release_text(&referendum->name);
    constitution_release_text(&referendum->basis);
    referendum->electorate = 0;
}
