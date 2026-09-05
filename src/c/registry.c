#include "registry.h"
#include "internal.h"

#include <stdlib.h>
#include <string.h>

void registry_init(registry_t *registry)
{
    if (registry == NULL) {
        return;
    }

    registry->head = NULL;
    registry->count = 0;
}

static void registry_entry_free(registry_entry_t *entry)
{
    if (entry == NULL) {
        return;
    }

    constitution_release_text(&entry->name);
    constitution_release_text(&entry->kind);
    free(entry);
}

void registry_reset(registry_t *registry)
{
    if (registry == NULL) {
        return;
    }

    registry_entry_t *current = registry->head;
    while (current != NULL) {
        registry_entry_t *next = current->next;
        registry_entry_free(current);
        current = next;
    }

    registry->head = NULL;
    registry->count = 0;
}

constitution_status_t registry_register(registry_t *registry, const char *name, const char *kind, void *object)
{
    if (registry == NULL || name == NULL || kind == NULL || object == NULL) {
        return CONSTITUTION_STATUS_INVALID_ARGUMENT;
    }

    registry_entry_t *entry = (registry_entry_t *)malloc(sizeof(registry_entry_t));
    if (entry == NULL) {
        return CONSTITUTION_STATUS_NO_MEMORY;
    }

    entry->name = constitution_clone_text(name);
    entry->kind = constitution_clone_text(kind);
    entry->object = object;
    entry->next = registry->head;
    if (entry->name == NULL || entry->kind == NULL) {
        registry_entry_free(entry);
        return CONSTITUTION_STATUS_NO_MEMORY;
    }

    registry->head = entry;
    registry->count++;
    return CONSTITUTION_STATUS_OK;
}

void *registry_find(const registry_t *registry, const char *name)
{
    if (registry == NULL || name == NULL) {
        return NULL;
    }

    for (registry_entry_t *current = registry->head; current != NULL; current = current->next) {
        if (current->name != NULL && strcmp(current->name, name) == 0) {
            return current->object;
        }
    }

    return NULL;
}

void registry_foreach(const registry_t *registry, void (*visitor)(const registry_entry_t *entry, void *context), void *context)
{
    if (registry == NULL || visitor == NULL) {
        return;
    }

    for (registry_entry_t *current = registry->head; current != NULL; current = current->next) {
        visitor(current, context);
    }
}
