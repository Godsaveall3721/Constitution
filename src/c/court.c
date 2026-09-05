#include "court.h"
#include "internal.h"

constitution_status_t court_init(court_t *court, const char *name, const char *basis, bool constitutional_review)
{
    if (court == NULL || name == NULL) {
        return CONSTITUTION_STATUS_INVALID_ARGUMENT;
    }

    court->name = constitution_clone_text(name);
    court->basis = constitution_clone_text(basis);
    if (court->name == NULL || court->basis == NULL) {
        court_reset(court);
        return CONSTITUTION_STATUS_NO_MEMORY;
    }

    court->constitutional_review = constitutional_review;
    return CONSTITUTION_STATUS_OK;
}

void court_reset(court_t *court)
{
    if (court == NULL) {
        return;
    }

    constitution_release_text(&court->name);
    constitution_release_text(&court->basis);
    court->constitutional_review = false;
}
