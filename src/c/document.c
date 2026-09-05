#include "document.h"
#include "internal.h"

constitution_status_t document_init(document_t *document,
                                    const char *name,
                                    constitution_document_kind_t kind,
                                    const char *basis,
                                    const char *content)
{
    if (document == NULL || name == NULL) {
        return CONSTITUTION_STATUS_INVALID_ARGUMENT;
    }

    document->name = constitution_clone_text(name);
    document->basis = constitution_clone_text(basis);
    document->content = constitution_clone_text(content);
    if (document->name == NULL || document->basis == NULL || document->content == NULL) {
        document_reset(document);
        return CONSTITUTION_STATUS_NO_MEMORY;
    }

    document->kind = kind;
    return CONSTITUTION_STATUS_OK;
}

void document_reset(document_t *document)
{
    if (document == NULL) {
        return;
    }

    constitution_release_text(&document->name);
    constitution_release_text(&document->basis);
    constitution_release_text(&document->content);
}
