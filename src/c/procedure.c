#include "procedure.h"
#include "internal.h"

constitution_status_t procedure_init(procedure_t *procedure,
                                     const char *name,
                                     constitution_procedure_kind_t kind,
                                     const char *basis,
                                     constitution_status_t (*run)(void *context))
{
    if (procedure == NULL || name == NULL) {
        return CONSTITUTION_STATUS_INVALID_ARGUMENT;
    }

    procedure->name = constitution_clone_text(name);
    procedure->basis = constitution_clone_text(basis);
    if (procedure->name == NULL || procedure->basis == NULL) {
        procedure_reset(procedure);
        return CONSTITUTION_STATUS_NO_MEMORY;
    }

    procedure->kind = kind;
    procedure->run = run;
    return CONSTITUTION_STATUS_OK;
}

void procedure_reset(procedure_t *procedure)
{
    if (procedure == NULL) {
        return;
    }

    constitution_release_text(&procedure->name);
    constitution_release_text(&procedure->basis);
    procedure->run = NULL;
}

constitution_status_t procedure_execute(procedure_t *procedure, void *context)
{
    if (procedure == NULL || procedure->run == NULL) {
        return CONSTITUTION_STATUS_INVALID_ARGUMENT;
    }

    return procedure->run(context);
}
