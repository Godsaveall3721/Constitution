#ifndef CONSTITUTION_C_REGISTRY_H
#define CONSTITUTION_C_REGISTRY_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct constitution_registry_entry {
    char *name;
    char *kind;
    void *object;
    struct constitution_registry_entry *next;
} registry_entry_t;

typedef struct constitution_registry {
    registry_entry_t *head;
    size_t count;
} registry_t;

void registry_init(registry_t *registry);
void registry_reset(registry_t *registry);
constitution_status_t registry_register(registry_t *registry, const char *name, const char *kind, void *object);
void *registry_find(const registry_t *registry, const char *name);
void registry_foreach(const registry_t *registry, void (*visitor)(const registry_entry_t *entry, void *context), void *context);

#ifdef __cplusplus
}
#endif

#endif
