#ifndef CONSTITUTION_C_CONSTITUTION_H
#define CONSTITUTION_C_CONSTITUTION_H

#include "body.h"
#include "court.h"
#include "document.h"
#include "election.h"
#include "impeachment.h"
#include "law.h"
#include "constituency.h"
#include "office.h"
#include "procedure.h"
#include "result.h"
#include "registry.h"
#include "referendum.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct constitution_model {
    registry_t registry;
    constituency_t **constituencies;
    size_t constituency_count;
    size_t constituency_capacity;
    member_t **members;
    size_t member_count;
    size_t member_capacity;
    body_t **bodies;
    size_t body_count;
    size_t body_capacity;
    office_t **offices;
    size_t office_count;
    size_t office_capacity;
    procedure_t **procedures;
    size_t procedure_count;
    size_t procedure_capacity;
    document_t **documents;
    size_t document_count;
    size_t document_capacity;
    election_t **elections;
    size_t election_count;
    size_t election_capacity;
    referendum_t **referendums;
    size_t referendum_count;
    size_t referendum_capacity;
    impeachment_t **impeachments;
    size_t impeachment_count;
    size_t impeachment_capacity;
    law_t **laws;
    size_t law_count;
    size_t law_capacity;
    court_t **courts;
    size_t court_count;
    size_t court_capacity;
} constitution_t;

void constitution_init(constitution_t *constitution);
void constitution_reset(constitution_t *constitution);
constitution_result_t constitution_summary(constitution_t *constitution);

size_t constitution_count_members_by_role(constitution_t *constitution, const char *role_fragment);
size_t constitution_count_electoral_college_members(const constitution_t *constitution);

constituency_t *constitution_add_constituency(constitution_t *constitution, const char *name, const char *label, const char *seat_name, constitution_constituency_kind_t kind, unsigned long long population, const char *parent_name);
member_t *constitution_add_member(constitution_t *constitution, const char *name, const char *role, unsigned age, const char *constituency_name);
constitution_status_t constitution_load_config_directory(constitution_t *constitution, const char *directory);

body_t *constitution_add_body(constitution_t *constitution, const char *name, constitution_body_kind_t kind, size_t seat_total);
office_t *constitution_add_office(constitution_t *constitution, const char *name, constitution_office_kind_t kind);
procedure_t *constitution_add_procedure(constitution_t *constitution, const char *name, constitution_procedure_kind_t kind, const char *basis, constitution_status_t (*run)(void *context));
document_t *constitution_add_document(constitution_t *constitution, const char *name, constitution_document_kind_t kind, const char *basis, const char *content);
election_t *constitution_add_election(constitution_t *constitution, const char *name, constitution_election_kind_t kind, const char *basis, size_t seat_count, size_t voter_count);
referendum_t *constitution_add_referendum(constitution_t *constitution, const char *name, const char *basis, size_t electorate);
impeachment_t *constitution_add_impeachment(constitution_t *constitution, const char *name, const char *basis, const char *target);
law_t *constitution_add_law(constitution_t *constitution, const char *name, const char *basis, const char *content, const char *status_text);
court_t *constitution_add_court(constitution_t *constitution, const char *name, const char *basis, bool constitutional_review);

void constitution_print_summary(constitution_t *constitution);

#ifdef __cplusplus
}
#endif

#endif
