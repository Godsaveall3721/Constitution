#ifndef CONSTITUTION_C_CONFIG_H
#define CONSTITUTION_C_CONFIG_H

#include "constitution.h"

#ifdef __cplusplus
extern "C" {
#endif

constitution_status_t constitution_load_config_directory(constitution_t *constitution, const char *directory);

#ifdef __cplusplus
}
#endif

#endif
