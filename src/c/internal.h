#ifndef CONSTITUTION_C_INTERNAL_H
#define CONSTITUTION_C_INTERNAL_H

#include <stddef.h>

char *constitution_strdup_local(const char *text);
void constitution_free_text_local(char **text);

#endif
