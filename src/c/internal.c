#include "internal.h"

#include <stdlib.h>
#include <string.h>

char *constitution_strdup_local(const char *text)
{
    if (text == NULL) {
        return NULL;
    }

    size_t length = strlen(text) + 1;
    char *copy = (char *)malloc(length);
    if (copy == NULL) {
        return NULL;
    }

    memcpy(copy, text, length);
    return copy;
}

void constitution_free_text_local(char **text)
{
    if (text == NULL || *text == NULL) {
        return;
    }

    free(*text);
    *text = NULL;
}
