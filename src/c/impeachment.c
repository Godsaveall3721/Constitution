#include "impeachment.h"
#include "internal.h"

constitution_status_t impeachment_init(impeachment_t *impeachment,
                                       const char *name,
                                       const char *basis,
                                       const char *target)
{
    if (impeachment == NULL || name == NULL) {
        return CONSTITUTION_STATUS_INVALID_ARGUMENT;
    }

    impeachment->name = constitution_clone_text(name);
    impeachment->basis = constitution_clone_text(basis);
    impeachment->target = constitution_clone_text(target);
    if (impeachment->name == NULL || impeachment->basis == NULL || impeachment->target == NULL) {
        impeachment_reset(impeachment);
        return CONSTITUTION_STATUS_NO_MEMORY;
    }

    return CONSTITUTION_STATUS_OK;
}

void impeachment_reset(impeachment_t *impeachment)
{
    if (impeachment == NULL) {
        return;
    }

    constitution_release_text(&impeachment->name);
    constitution_release_text(&impeachment->basis);
    constitution_release_text(&impeachment->target);
}
