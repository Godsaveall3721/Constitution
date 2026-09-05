#ifndef CONSTITUTION_C_TYPES_H
#define CONSTITUTION_C_TYPES_H

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum constitution_status {
    CONSTITUTION_STATUS_OK = 0,
    CONSTITUTION_STATUS_INVALID_ARGUMENT,
    CONSTITUTION_STATUS_NO_MEMORY,
    CONSTITUTION_STATUS_NOT_FOUND,
    CONSTITUTION_STATUS_ALREADY_EXISTS,
    CONSTITUTION_STATUS_EMPTY
} constitution_status_t;

typedef enum constitution_body_kind {
    CONSTITUTION_BODY_KIND_NATIONAL_ASSEMBLY,
    CONSTITUTION_BODY_KIND_PALACE,
    CONSTITUTION_BODY_KIND_REGIONAL_COUNCIL,
    CONSTITUTION_BODY_KIND_DEPARTMENTAL_COUNCIL,
    CONSTITUTION_BODY_KIND_METROPOLITAN_COUNCIL,
    CONSTITUTION_BODY_KIND_MUNICIPAL_COUNCIL,
    CONSTITUTION_BODY_KIND_COMMITTEE,
    CONSTITUTION_BODY_KIND_OTHER
} constitution_body_kind_t;

typedef enum constitution_office_kind {
    CONSTITUTION_OFFICE_KIND_PRESIDENT,
    CONSTITUTION_OFFICE_KIND_PRIME_MINISTER,
    CONSTITUTION_OFFICE_KIND_SPEAKER,
    CONSTITUTION_OFFICE_KIND_CHAIR,
    CONSTITUTION_OFFICE_KIND_MINISTER,
    CONSTITUTION_OFFICE_KIND_OTHER
} constitution_office_kind_t;

typedef enum constitution_procedure_kind {
    CONSTITUTION_PROCEDURE_KIND_ELECTION,
    CONSTITUTION_PROCEDURE_KIND_APPOINTMENT,
    CONSTITUTION_PROCEDURE_KIND_DISSOLUTION,
    CONSTITUTION_PROCEDURE_KIND_REFERENDUM,
    CONSTITUTION_PROCEDURE_KIND_IMPEACHMENT,
    CONSTITUTION_PROCEDURE_KIND_REVIEW,
    CONSTITUTION_PROCEDURE_KIND_EXPLANATION,
    CONSTITUTION_PROCEDURE_KIND_SUSPENSION,
    CONSTITUTION_PROCEDURE_KIND_OTHER
} constitution_procedure_kind_t;

typedef enum constitution_document_kind {
    CONSTITUTION_DOCUMENT_KIND_BILL,
    CONSTITUTION_DOCUMENT_KIND_AMENDMENT,
    CONSTITUTION_DOCUMENT_KIND_DECREE,
    CONSTITUTION_DOCUMENT_KIND_RESOLUTION,
    CONSTITUTION_DOCUMENT_KIND_TREATY,
    CONSTITUTION_DOCUMENT_KIND_ORDER,
    CONSTITUTION_DOCUMENT_KIND_OTHER
} constitution_document_kind_t;

typedef enum constitution_election_kind {
    CONSTITUTION_ELECTION_KIND_TWO_ROUND_MAJORITY,
    CONSTITUTION_ELECTION_KIND_FREE_LIST,
    CONSTITUTION_ELECTION_KIND_OPEN_LIST,
    CONSTITUTION_ELECTION_KIND_CLOSED_LIST,
    CONSTITUTION_ELECTION_KIND_SINGLE_TRANSFERABLE,
    CONSTITUTION_ELECTION_KIND_SORTED_RUNOFF,
    CONSTITUTION_ELECTION_KIND_OTHER
} constitution_election_kind_t;

char *constitution_clone_text(const char *text);
void constitution_release_text(char **text);

const char *constitution_body_kind_name(const constitution_body_kind_t kind);
const char *constitution_office_kind_name(const constitution_office_kind_t kind);
const char *constitution_procedure_kind_name(const constitution_procedure_kind_t kind);
const char *constitution_document_kind_name(const constitution_document_kind_t kind);
const char *constitution_election_kind_name(const constitution_election_kind_t kind);

#ifdef __cplusplus
}
#endif

#endif
