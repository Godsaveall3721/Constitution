#include "constituency.h"
#include "internal.h"

constitution_status_t constituency_init(constituency_t *constituency,
                                       const char *name,
                                       const char *label,
                                       const char *seat_name,
                                       constitution_constituency_kind_t kind,
                                       unsigned long long population,
                                       const char *parent_name)
{
    if (constituency == NULL || name == NULL) {
        return CONSTITUTION_STATUS_INVALID_ARGUMENT;
    }

    ((constituency_t *)constituency)->name = constitution_clone_text(name);
    ((constituency_t *)constituency)->label = constitution_clone_text(label);
    ((constituency_t *)constituency)->seat_name = constitution_clone_text(seat_name);
    ((constituency_t *)constituency)->parent_name = constitution_clone_text(parent_name);
    if (((constituency_t *)constituency)->name == NULL ||
        (((constituency_t *)constituency)->label == NULL && label != NULL) ||
        (((constituency_t *)constituency)->seat_name == NULL && seat_name != NULL) ||
        (((constituency_t *)constituency)->parent_name == NULL && parent_name != NULL)) {
        constituency_reset(constituency);
        return CONSTITUTION_STATUS_NO_MEMORY;
    }

    ((constituency_t *)constituency)->kind = kind;
    ((constituency_t *)constituency)->population = population;
    return CONSTITUTION_STATUS_OK;
}

void constituency_reset(constituency_t *constituency)
{
    if (constituency == NULL) {
        return;
    }

    constitution_release_text(&((constituency_t *)constituency)->name);
    constitution_release_text(&((constituency_t *)constituency)->label);
    constitution_release_text(&((constituency_t *)constituency)->seat_name);
    constitution_release_text(&((constituency_t *)constituency)->parent_name);
    ((constituency_t *)constituency)->population = 0;
}

constitution_status_t constituency_copy(constituency_t *destination, const constituency_t *source)
{
    if (destination == NULL || source == NULL) {
        return CONSTITUTION_STATUS_INVALID_ARGUMENT;
    }

    return constituency_init(destination, source->name, source->label, source->seat_name, source->kind, source->population, source->parent_name);
}

const char *constitution_constituency_kind_name(const constitution_constituency_kind_t kind)
{
    switch (kind) {
    case CONSTITUTION_CONSTITUENCY_KIND_NATION: return "共和国";
    case CONSTITUTION_CONSTITUENCY_KIND_REGION: return "大区";
    case CONSTITUTION_CONSTITUENCY_KIND_PROVINCE: return "省";
    case CONSTITUTION_CONSTITUENCY_KIND_METROPOLITAN_CITY: return "广域市";
    case CONSTITUTION_CONSTITUENCY_KIND_SUB_PREFECTURE: return "省区";
    case CONSTITUTION_CONSTITUENCY_KIND_COMMUNE: return "公社";
    case CONSTITUTION_CONSTITUENCY_KIND_ELECTORAL_DISTRICT: return "选区";
    default: return "其他行政区";
    }
}
