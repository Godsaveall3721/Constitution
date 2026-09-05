#ifndef CONSTITUTION_C_LAW_H
#define CONSTITUTION_C_LAW_H

#include "document.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct constitution_law {
    document_t document;
    char *status_text;
} law_t;

constitution_status_t law_init(law_t *law,
                               const char *name,
                               const char *basis,
                               const char *content,
                               const char *status_text);
void law_reset(law_t *law);

#ifdef __cplusplus
}
#endif

#endif
