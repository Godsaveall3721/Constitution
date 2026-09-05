#ifndef CONSTITUTION_C_DOCUMENT_H
#define CONSTITUTION_C_DOCUMENT_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct constitution_document {
    char *name;
    constitution_document_kind_t kind;
    char *basis;
    char *content;
} document_t;

constitution_status_t document_init(document_t *document,
                                    const char *name,
                                    constitution_document_kind_t kind,
                                    const char *basis,
                                    const char *content);
void document_reset(document_t *document);

#ifdef __cplusplus
}
#endif

#endif
