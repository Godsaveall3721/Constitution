#include "law.h"
#include "internal.h"

constitution_status_t law_init(law_t *law,
                               const char *name,
                               const char *basis,
                               const char *content,
                               const char *status_text)
{
    if (law == NULL || name == NULL) {
        return CONSTITUTION_STATUS_INVALID_ARGUMENT;
    }

    constitution_status_t status = document_init(&law->document,
                                                 name,
                                                 CONSTITUTION_DOCUMENT_KIND_BILL,
                                                 basis,
                                                 content);
    if (status != CONSTITUTION_STATUS_OK) {
        return status;
    }

    law->status_text = constitution_clone_text(status_text);
    if (law->status_text == NULL) {
        law_reset(law);
        return CONSTITUTION_STATUS_NO_MEMORY;
    }

    return CONSTITUTION_STATUS_OK;
}

void law_reset(law_t *law)
{
    if (law == NULL) {
        return;
    }

    document_reset(&law->document);
    constitution_release_text(&law->status_text);
}
