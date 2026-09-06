#include "constitution.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void constitution_reset_constituencies(const constitution_t *constitution)
{
    for (size_t index = 0; index < constitution->constituency_count; ++index) {
        constituency_reset(constitution->constituencies[index]);
        free(constitution->constituencies[index]);
    }
    free((void *)constitution->constituencies);
}

static void constitution_reset_members(const constitution_t *constitution)
{
    for (size_t index = 0; index < constitution->member_count; ++index) {
        member_reset(constitution->members[index]);
        free(constitution->members[index]);
    }
    free((void *)constitution->members);
}

static void constitution_reset_bodies(const constitution_t *constitution)
{
    for (size_t index = 0; index < constitution->body_count; ++index) {
        body_reset(constitution->bodies[index]);
        free(constitution->bodies[index]);
    }
    free((void *)constitution->bodies);
}

static void constitution_reset_offices(const constitution_t *constitution)
{
    for (size_t index = 0; index < constitution->office_count; ++index) {
        office_reset(constitution->offices[index]);
        free(constitution->offices[index]);
    }
    free((void *)constitution->offices);
}

static void constitution_reset_procedures(const constitution_t *constitution)
{
    for (size_t index = 0; index < constitution->procedure_count; ++index) {
        procedure_reset(constitution->procedures[index]);
        free(constitution->procedures[index]);
    }
    free((void *)constitution->procedures);
}

static void constitution_reset_documents(const constitution_t *constitution)
{
    for (size_t index = 0; index < constitution->document_count; ++index) {
        document_reset(constitution->documents[index]);
        free(constitution->documents[index]);
    }
    free((void *)constitution->documents);
}

static void constitution_reset_elections(const constitution_t *constitution)
{
    for (size_t index = 0; index < constitution->election_count; ++index) {
        election_reset(constitution->elections[index]);
        free(constitution->elections[index]);
    }
    free((void *)constitution->elections);
}

static void constitution_reset_referendums(const constitution_t *constitution)
{
    for (size_t index = 0; index < constitution->referendum_count; ++index) {
        referendum_reset(constitution->referendums[index]);
        free(constitution->referendums[index]);
    }
    free((void *)constitution->referendums);
}

static void constitution_reset_impeachments(const constitution_t *constitution)
{
    for (size_t index = 0; index < constitution->impeachment_count; ++index) {
        impeachment_reset(constitution->impeachments[index]);
        free(constitution->impeachments[index]);
    }
    free((void *)constitution->impeachments);
}

static void constitution_reset_laws(const constitution_t *constitution)
{
    for (size_t index = 0; index < constitution->law_count; ++index) {
        law_reset(constitution->laws[index]);
        free(constitution->laws[index]);
    }
    free((void *)constitution->laws);
}

static void constitution_reset_courts(const constitution_t *constitution)
{
    for (size_t index = 0; index < constitution->court_count; ++index) {
        court_reset(constitution->courts[index]);
        free(constitution->courts[index]);
    }
    free((void *)constitution->courts);
}

void constitution_init(constitution_t *constitution)
{
    if (constitution == NULL) {
        return;
    }

    registry_init((registry_t *)&constitution->registry);
    ((constitution_t *)constitution)->constituencies = NULL;
    ((constitution_t *)constitution)->constituency_count = 0;
    ((constitution_t *)constitution)->constituency_capacity = 0;
    ((constitution_t *)constitution)->members = NULL;
    ((constitution_t *)constitution)->member_count = 0;
    ((constitution_t *)constitution)->member_capacity = 0;
    ((constitution_t *)constitution)->bodies = NULL;
    ((constitution_t *)constitution)->body_count = 0;
    ((constitution_t *)constitution)->body_capacity = 0;
    ((constitution_t *)constitution)->offices = NULL;
    ((constitution_t *)constitution)->office_count = 0;
    ((constitution_t *)constitution)->office_capacity = 0;
    ((constitution_t *)constitution)->procedures = NULL;
    ((constitution_t *)constitution)->procedure_count = 0;
    ((constitution_t *)constitution)->procedure_capacity = 0;
    ((constitution_t *)constitution)->documents = NULL;
    ((constitution_t *)constitution)->document_count = 0;
    ((constitution_t *)constitution)->document_capacity = 0;
    ((constitution_t *)constitution)->elections = NULL;
    ((constitution_t *)constitution)->election_count = 0;
    ((constitution_t *)constitution)->election_capacity = 0;
    ((constitution_t *)constitution)->referendums = NULL;
    ((constitution_t *)constitution)->referendum_count = 0;
    ((constitution_t *)constitution)->referendum_capacity = 0;
    ((constitution_t *)constitution)->impeachments = NULL;
    ((constitution_t *)constitution)->impeachment_count = 0;
    ((constitution_t *)constitution)->impeachment_capacity = 0;
    ((constitution_t *)constitution)->laws = NULL;
    ((constitution_t *)constitution)->law_count = 0;
    ((constitution_t *)constitution)->law_capacity = 0;
    ((constitution_t *)constitution)->courts = NULL;
    ((constitution_t *)constitution)->court_count = 0;
    ((constitution_t *)constitution)->court_capacity = 0;
}

void constitution_reset(constitution_t *constitution)
{
    if (constitution == NULL) {
        return;
    }

    registry_reset((registry_t *)&constitution->registry);
    constitution_reset_constituencies(constitution);
    constitution_reset_members(constitution);
    constitution_reset_bodies(constitution);
    constitution_reset_offices(constitution);
    constitution_reset_procedures(constitution);
    constitution_reset_documents(constitution);
    constitution_reset_elections(constitution);
    constitution_reset_referendums(constitution);
    constitution_reset_impeachments(constitution);
    constitution_reset_laws(constitution);
    constitution_reset_courts(constitution);

    ((constitution_t *)constitution)->constituencies = NULL;
    ((constitution_t *)constitution)->members = NULL;
    ((constitution_t *)constitution)->bodies = NULL;
    ((constitution_t *)constitution)->offices = NULL;
    ((constitution_t *)constitution)->procedures = NULL;
    ((constitution_t *)constitution)->documents = NULL;
    ((constitution_t *)constitution)->elections = NULL;
    ((constitution_t *)constitution)->referendums = NULL;
    ((constitution_t *)constitution)->impeachments = NULL;
    ((constitution_t *)constitution)->laws = NULL;
    ((constitution_t *)constitution)->courts = NULL;

    ((constitution_t *)constitution)->constituency_count = 0;
    ((constitution_t *)constitution)->member_count = 0;
    ((constitution_t *)constitution)->body_count = 0;
    ((constitution_t *)constitution)->office_count = 0;
    ((constitution_t *)constitution)->procedure_count = 0;
    ((constitution_t *)constitution)->document_count = 0;
    ((constitution_t *)constitution)->election_count = 0;
    ((constitution_t *)constitution)->referendum_count = 0;
    ((constitution_t *)constitution)->impeachment_count = 0;
    ((constitution_t *)constitution)->law_count = 0;
    ((constitution_t *)constitution)->court_count = 0;
}

constitution_result_t constitution_summary(constitution_t *constitution)
{
    constitution_result_t result;
    result.status = CONSTITUTION_STATUS_OK;
    result.message = "《欧罗巴共和国宪法》：模型已装配";
    if (constitution == NULL) {
        result.status = CONSTITUTION_STATUS_INVALID_ARGUMENT;
        result.message = "《宪法》：模型为空";
    }
    return result;
}

static bool constitution_role_counts_for_college(const char *role_fragment)
{
    if (role_fragment == NULL) {
        return false;
    }

    return strstr(role_fragment, "议员") != NULL ||
           strstr(role_fragment, "参议员") != NULL ||
           strstr(role_fragment, "众议员") != NULL;
}

size_t constitution_count_members_by_role(constitution_t *constitution, const char *role_fragment)
{
    if (constitution == NULL || role_fragment == NULL) {
        return 0;
    }

    const constitution_t *model = (const constitution_t *)constitution;
    size_t count = 0;
    for (size_t index = 0; index < model->member_count; ++index) {
        member_t *member = model->members[index];
        if (member != NULL && member->role != NULL && constitution_role_counts_for_college(role_fragment)) {
            if (strstr(member->role, role_fragment) != NULL) {
                count++;
            }
        }
    }

    return count;
}

size_t constitution_count_electoral_college_members(const constitution_t *constitution)
{
    if (constitution == NULL) {
        return 0;
    }

    const constitution_t *model = (const constitution_t *)constitution;
    size_t count = 0;
    for (size_t index = 0; index < model->member_count; ++index) {
        member_t *member = model->members[index];
        if (member == NULL || member->role == NULL) {
            continue;
        }

        if (strstr(member->role, "议员") != NULL || strstr(member->role, "参议员") != NULL || strstr(member->role, "众议员") != NULL) {
            count++;
        }
    }

    return count;
}

static size_t constitution_count_constituencies_by_kind_impl(const constitution_t *constitution, constitution_constituency_kind_t kind)
{
    if (constitution == NULL) {
        return 0;
    }

    const constitution_t *model = (const constitution_t *)constitution;
    size_t count = 0;
    for (size_t index = 0; index < model->constituency_count; ++index) {
        constituency_t *item = model->constituencies[index];
        if (item != NULL && item->kind == kind) {
            ++count;
        }
    }
    return count;
}

static size_t constitution_count_bodies_by_kind_impl(const constitution_t *constitution, constitution_body_kind_t kind)
{
    if (constitution == NULL) {
        return 0;
    }

    const constitution_t *model = (const constitution_t *)constitution;
    size_t count = 0;
    for (size_t index = 0; index < model->body_count; ++index) {
        body_t *item = model->bodies[index];
        if (item != NULL && item->kind == kind) {
            ++count;
        }
    }
    return count;
}

static size_t constitution_count_offices_by_kind_impl(const constitution_t *constitution, constitution_office_kind_t kind)
{
    if (constitution == NULL) {
        return 0;
    }

    const constitution_t *model = (const constitution_t *)constitution;
    size_t count = 0;
    for (size_t index = 0; index < model->office_count; ++index) {
        office_t *item = model->offices[index];
        if (item != NULL && item->kind == kind) {
            ++count;
        }
    }
    return count;
}

static size_t constitution_count_procedures_by_kind_impl(const constitution_t *constitution, constitution_procedure_kind_t kind)
{
    if (constitution == NULL) {
        return 0;
    }

    const constitution_t *model = (const constitution_t *)constitution;
    size_t count = 0;
    for (size_t index = 0; index < model->procedure_count; ++index) {
        procedure_t *item = model->procedures[index];
        if (item != NULL && item->kind == kind) {
            ++count;
        }
    }
    return count;
}

static size_t constitution_count_documents_by_kind_impl(const constitution_t *constitution, constitution_document_kind_t kind)
{
    if (constitution == NULL) {
        return 0;
    }

    const constitution_t *model = (const constitution_t *)constitution;
    size_t count = 0;
    for (size_t index = 0; index < model->document_count; ++index) {
        document_t *item = model->documents[index];
        if (item != NULL && item->kind == kind) {
            ++count;
        }
    }
    return count;
}

static size_t constitution_count_elections_by_kind_impl(const constitution_t *constitution, constitution_election_kind_t kind)
{
    if (constitution == NULL) {
        return 0;
    }

    const constitution_t *model = (const constitution_t *)constitution;
    size_t count = 0;
    for (size_t index = 0; index < model->election_count; ++index) {
        election_t *item = model->elections[index];
        if (item != NULL && item->kind == kind) {
            ++count;
        }
    }
    return count;
}

size_t constitution_count_constituencies_by_kind(const constitution_t *constitution, constitution_constituency_kind_t kind)
{
    return constitution_count_constituencies_by_kind_impl(constitution, kind);
}

size_t constitution_count_bodies_by_kind(constitution_t *constitution, constitution_body_kind_t kind)
{
    return constitution_count_bodies_by_kind_impl(constitution, kind);
}

size_t constitution_count_offices_by_kind(constitution_t *constitution, constitution_office_kind_t kind)
{
    return constitution_count_offices_by_kind_impl(constitution, kind);
}

size_t constitution_count_procedures_by_kind(constitution_t *constitution, constitution_procedure_kind_t kind)
{
    return constitution_count_procedures_by_kind_impl(constitution, kind);
}

size_t constitution_count_documents_by_kind(constitution_t *constitution, constitution_document_kind_t kind)
{
    return constitution_count_documents_by_kind_impl(constitution, kind);
}

size_t constitution_count_elections_by_kind(constitution_t *constitution, constitution_election_kind_t kind)
{
    return constitution_count_elections_by_kind_impl(constitution, kind);
}

static void **constitution_grow_pointer_array(void **items, size_t count, size_t *capacity)
{
    size_t new_capacity = (*capacity == 0) ? 4 : (*capacity * 2);
    while (new_capacity < count) {
        new_capacity *= 2;
    }

    void **grown = (void **)realloc(items, new_capacity * sizeof(void *));
    if (grown != NULL) {
        *capacity = new_capacity;
    }
    return grown;
}

constituency_t *constitution_add_constituency(constitution_t *constitution, const char *name, const char *label, const char *seat_name, constitution_constituency_kind_t kind, unsigned long long population, const char *parent_name)
{
    if (constitution == NULL) {
        return NULL;
    }

    constitution_t *model = (constitution_t *)constitution;
    constituency_t **items = (constituency_t **)constitution_grow_pointer_array((void **)model->constituencies, model->constituency_count + 1, &model->constituency_capacity);
    if (items == NULL) {
        return NULL;
    }
    model->constituencies = items;

    constituency_t *constituency = (constituency_t *)calloc(1, sizeof(constituency_t));
    if (constituency == NULL) {
        return NULL;
    }
    if (constituency_init(constituency, name, label, seat_name, kind, population, parent_name) != CONSTITUTION_STATUS_OK) {
        free(constituency);
        return NULL;
    }

    model->constituencies[model->constituency_count++] = constituency;
    registry_register(&model->registry, name, constitution_constituency_kind_name(kind), constituency);
    return constituency;
}

member_t *constitution_add_member(constitution_t *constitution, const char *name, const char *role, unsigned age, const char *constituency_name)
{
    if (constitution == NULL) {
        return NULL;
    }

    constitution_t *model = (constitution_t *)constitution;
    member_t **items = (member_t **)constitution_grow_pointer_array((void **)model->members, model->member_count + 1, &model->member_capacity);
    if (items == NULL) {
        return NULL;
    }
    model->members = items;

    member_t *member = (member_t *)calloc(1, sizeof(member_t));
    if (member == NULL) {
        return NULL;
    }
    if (member_init(member, name, role, age) != CONSTITUTION_STATUS_OK) {
        free(member);
        return NULL;
    }
    member_assign_constituency(member, constituency_name);

    model->members[model->member_count++] = member;
    registry_register(&model->registry, name, role, member);
    return member;
}

body_t *constitution_add_body(constitution_t *constitution, const char *name, constitution_body_kind_t kind, size_t seat_total)
{
    if (constitution == NULL) {
        return NULL;
    }

    constitution_t *model = (constitution_t *)constitution;
    body_t **items = (body_t **)constitution_grow_pointer_array((void **)model->bodies, model->body_count + 1, &model->body_capacity);
    if (items == NULL) {
        return NULL;
    }
    model->bodies = items;

    body_t *body = (body_t *)calloc(1, sizeof(body_t));
    if (body == NULL) {
        return NULL;
    }
    if (body_init(body, name, kind, seat_total) != CONSTITUTION_STATUS_OK) {
        free(body);
        return NULL;
    }

    model->bodies[model->body_count++] = body;
    registry_register(&model->registry, name, constitution_body_kind_name(kind), body);
    return body;
}

office_t *constitution_add_office(constitution_t *constitution, const char *name, constitution_office_kind_t kind)
{
    if (constitution == NULL) {
        return NULL;
    }

    constitution_t *model = (constitution_t *)constitution;
    office_t **items = (office_t **)constitution_grow_pointer_array((void **)model->offices, model->office_count + 1, &model->office_capacity);
    if (items == NULL) {
        return NULL;
    }
    model->offices = items;

    office_t *office = (office_t *)calloc(1, sizeof(office_t));
    if (office == NULL) {
        return NULL;
    }
    if (office_init(office, name, kind) != CONSTITUTION_STATUS_OK) {
        free(office);
        return NULL;
    }

    model->offices[model->office_count++] = office;
    registry_register(&model->registry, name, constitution_office_kind_name(kind), office);
    return office;
}

procedure_t *constitution_add_procedure(constitution_t *constitution, const char *name, constitution_procedure_kind_t kind, const char *basis, constitution_status_t (*run)(void *context))
{
    if (constitution == NULL) {
        return NULL;
    }

    constitution_t *model = (constitution_t *)constitution;
    procedure_t **items = (procedure_t **)constitution_grow_pointer_array((void **)model->procedures, model->procedure_count + 1, &model->procedure_capacity);
    if (items == NULL) {
        return NULL;
    }
    model->procedures = items;

    procedure_t *procedure = (procedure_t *)calloc(1, sizeof(procedure_t));
    if (procedure == NULL) {
        return NULL;
    }
    if (procedure_init(procedure, name, kind, basis, run) != CONSTITUTION_STATUS_OK) {
        free(procedure);
        return NULL;
    }

    model->procedures[model->procedure_count++] = procedure;
    registry_register(&model->registry, name, constitution_procedure_kind_name(kind), procedure);
    return procedure;
}

document_t *constitution_add_document(constitution_t *constitution, const char *name, constitution_document_kind_t kind, const char *basis, const char *content)
{
    if (constitution == NULL) {
        return NULL;
    }

    constitution_t *model = (constitution_t *)constitution;
    document_t **items = (document_t **)constitution_grow_pointer_array((void **)model->documents, model->document_count + 1, &model->document_capacity);
    if (items == NULL) {
        return NULL;
    }
    model->documents = items;

    document_t *document = (document_t *)calloc(1, sizeof(document_t));
    if (document == NULL) {
        return NULL;
    }
    if (document_init(document, name, kind, basis, content) != CONSTITUTION_STATUS_OK) {
        free(document);
        return NULL;
    }

    model->documents[model->document_count++] = document;
    registry_register(&model->registry, name, constitution_document_kind_name(kind), document);
    return document;
}

election_t *constitution_add_election(constitution_t *constitution, const char *name, constitution_election_kind_t kind, const char *basis, size_t seat_count, size_t voter_count)
{
    if (constitution == NULL) {
        return NULL;
    }

    constitution_t *model = (constitution_t *)constitution;
    election_t **items = (election_t **)constitution_grow_pointer_array((void **)model->elections, model->election_count + 1, &model->election_capacity);
    if (items == NULL) {
        return NULL;
    }
    model->elections = items;

    election_t *election = (election_t *)calloc(1, sizeof(election_t));
    if (election == NULL) {
        return NULL;
    }
    if (election_init(election, name, kind, basis, seat_count, voter_count) != CONSTITUTION_STATUS_OK) {
        free(election);
        return NULL;
    }

    model->elections[model->election_count++] = election;
    registry_register(&model->registry, name, constitution_election_kind_name(kind), election);
    return election;
}

referendum_t *constitution_add_referendum(constitution_t *constitution, const char *name, const char *basis, size_t electorate)
{
    if (constitution == NULL) {
        return NULL;
    }

    constitution_t *model = (constitution_t *)constitution;
    referendum_t **items = (referendum_t **)constitution_grow_pointer_array((void **)model->referendums, model->referendum_count + 1, &model->referendum_capacity);
    if (items == NULL) {
        return NULL;
    }
    model->referendums = items;

    referendum_t *referendum = (referendum_t *)calloc(1, sizeof(referendum_t));
    if (referendum == NULL) {
        return NULL;
    }
    if (referendum_init(referendum, name, basis, electorate) != CONSTITUTION_STATUS_OK) {
        free(referendum);
        return NULL;
    }

    model->referendums[model->referendum_count++] = referendum;
    registry_register(&model->registry, name, "公民复决", referendum);
    return referendum;
}

impeachment_t *constitution_add_impeachment(constitution_t *constitution, const char *name, const char *basis, const char *target)
{
    if (constitution == NULL) {
        return NULL;
    }

    constitution_t *model = (constitution_t *)constitution;
    impeachment_t **items = (impeachment_t **)constitution_grow_pointer_array((void **)model->impeachments, model->impeachment_count + 1, &model->impeachment_capacity);
    if (items == NULL) {
        return NULL;
    }
    model->impeachments = items;

    impeachment_t *impeachment = (impeachment_t *)calloc(1, sizeof(impeachment_t));
    if (impeachment == NULL) {
        return NULL;
    }
    if (impeachment_init(impeachment, name, basis, target) != CONSTITUTION_STATUS_OK) {
        free(impeachment);
        return NULL;
    }

    model->impeachments[model->impeachment_count++] = impeachment;
    registry_register(&model->registry, name, "弹劾", impeachment);
    return impeachment;
}

law_t *constitution_add_law(constitution_t *constitution, const char *name, const char *basis, const char *content, const char *status_text)
{
    if (constitution == NULL) {
        return NULL;
    }

    constitution_t *model = (constitution_t *)constitution;
    law_t **items = (law_t **)constitution_grow_pointer_array((void **)model->laws, model->law_count + 1, &model->law_capacity);
    if (items == NULL) {
        return NULL;
    }
    model->laws = items;

    law_t *law = (law_t *)calloc(1, sizeof(law_t));
    if (law == NULL) {
        return NULL;
    }
    if (law_init(law, name, basis, content, status_text) != CONSTITUTION_STATUS_OK) {
        free(law);
        return NULL;
    }

    model->laws[model->law_count++] = law;
    registry_register(&model->registry, name, "法律", law);
    return law;
}

court_t *constitution_add_court(constitution_t *constitution, const char *name, const char *basis, bool constitutional_review)
{
    if (constitution == NULL) {
        return NULL;
    }

    constitution_t *model = (constitution_t *)constitution;
    court_t **items = (court_t **)constitution_grow_pointer_array((void **)model->courts, model->court_count + 1, &model->court_capacity);
    if (items == NULL) {
        return NULL;
    }
    model->courts = items;

    court_t *court = (court_t *)calloc(1, sizeof(court_t));
    if (court == NULL) {
        return NULL;
    }
    if (court_init(court, name, basis, constitutional_review) != CONSTITUTION_STATUS_OK) {
        free(court);
        return NULL;
    }

    model->courts[model->court_count++] = court;
    registry_register(&model->registry, name, "法院", court);
    return court;
}

void constitution_print_summary(constitution_t *constitution)
{
    if (constitution == NULL) {
        printf("《宪法》：模型为空\n");
        return;
    }

    printf("《欧罗巴共和国宪法》模型摘要\n");
    printf("- 行政区：%zu\n", constitution->constituency_count);
    printf("- 人名录：%zu\n", constitution->member_count);
    printf("- 机关注册数：%zu\n", constitution->registry.count);
    printf("- 合议体：%zu\n", constitution->body_count);
    printf("- 职务：%zu\n", constitution->office_count);
    printf("- 程序：%zu\n", constitution->procedure_count);
    printf("- 文书：%zu\n", constitution->document_count);
    printf("- 选举：%zu\n", constitution->election_count);
    printf("- 公民复决：%zu\n", constitution->referendum_count);
    printf("- 弹劾：%zu\n", constitution->impeachment_count);
    printf("- 法律：%zu\n", constitution->law_count);
    printf("- 法院：%zu\n", constitution->court_count);
}

void constitution_print_exhaustive_report(constitution_t *constitution)
{
    if (constitution == NULL) {
        printf("《宪法》：模型为空\n");
        return;
    }

    printf("\n【一、前言】\n");
    printf("- 共和体制：%s\n", constitution_count_constituencies_by_kind(constitution, CONSTITUTION_CONSTITUENCY_KIND_NATION) > 0 ? "已装配" : "未装配");
    printf("- 国家主权与公民复决：%zu 项复决对象，%zu 名人名录成员\n",
           constitution->referendum_count,
           constitution->member_count);
    printf("- 选举：%zu 个选举对象；政党配置来自 config/parties.csv\n", constitution->election_count);
    printf("- 议员构成：%zu 名具有议员/参议员/众议员身份的成员\n", constitution_count_electoral_college_members(constitution));

    printf("\n【二、总统】\n");
    printf("- 总统职务：%zu\n", constitution_count_offices_by_kind(constitution, CONSTITUTION_OFFICE_KIND_PRESIDENT));
    printf("- 总理职务：%zu\n", constitution_count_offices_by_kind(constitution, CONSTITUTION_OFFICE_KIND_PRIME_MINISTER));
    printf("- 总统/内阁相关文书：%zu\n", constitution_count_documents_by_kind(constitution, CONSTITUTION_DOCUMENT_KIND_DECREE) + constitution_count_documents_by_kind(constitution, CONSTITUTION_DOCUMENT_KIND_ORDER) + constitution_count_documents_by_kind(constitution, CONSTITUTION_DOCUMENT_KIND_RESOLUTION));
    printf("- 总统缺位、解散、复决、紧急命令等程序：%zu\n", constitution_count_procedures_by_kind(constitution, CONSTITUTION_PROCEDURE_KIND_APPOINTMENT) + constitution_count_procedures_by_kind(constitution, CONSTITUTION_PROCEDURE_KIND_DISSOLUTION) + constitution_count_procedures_by_kind(constitution, CONSTITUTION_PROCEDURE_KIND_REFERENDUM));

    printf("\n【三、总理的内阁和国家议会】\n");
    printf("- 国家议会合议体：%zu\n", constitution_count_bodies_by_kind(constitution, CONSTITUTION_BODY_KIND_NATIONAL_ASSEMBLY));
    printf("- 共和国议会：%zu\n", constitution_count_bodies_by_kind(constitution, CONSTITUTION_BODY_KIND_PALACE));
    printf("- 大区议会：%zu\n", constitution_count_bodies_by_kind(constitution, CONSTITUTION_BODY_KIND_REGIONAL_COUNCIL));
    printf("- 省议会：%zu\n", constitution_count_bodies_by_kind(constitution, CONSTITUTION_BODY_KIND_DEPARTMENTAL_COUNCIL));
    printf("- 广域市议会：%zu\n", constitution_count_bodies_by_kind(constitution, CONSTITUTION_BODY_KIND_METROPOLITAN_COUNCIL));
    printf("- 公社委员会：%zu\n", constitution_count_bodies_by_kind(constitution, CONSTITUTION_BODY_KIND_MUNICIPAL_COUNCIL));
    printf("- 委员会：%zu\n", constitution_count_bodies_by_kind(constitution, CONSTITUTION_BODY_KIND_COMMITTEE));
    printf("- 议事/信任/不信任/解散/立法程序：%zu\n", constitution_count_procedures_by_kind(constitution, CONSTITUTION_PROCEDURE_KIND_ELECTION) + constitution_count_procedures_by_kind(constitution, CONSTITUTION_PROCEDURE_KIND_IMPEACHMENT) + constitution_count_procedures_by_kind(constitution, CONSTITUTION_PROCEDURE_KIND_DISSOLUTION));
    printf("- 常规法案与修正案：%zu\n", constitution_count_documents_by_kind(constitution, CONSTITUTION_DOCUMENT_KIND_BILL) + constitution_count_documents_by_kind(constitution, CONSTITUTION_DOCUMENT_KIND_AMENDMENT) + constitution_count_documents_by_kind(constitution, CONSTITUTION_DOCUMENT_KIND_RESOLUTION));

    printf("\n【四、共和国司法】\n");
    printf("- 宪法法院：%zu\n", constitution->court_count);
    printf("- 违宪审查/解释/条款暂停程序：%zu\n", constitution_count_procedures_by_kind(constitution, CONSTITUTION_PROCEDURE_KIND_REVIEW) + constitution_count_procedures_by_kind(constitution, CONSTITUTION_PROCEDURE_KIND_EXPLANATION) + constitution_count_procedures_by_kind(constitution, CONSTITUTION_PROCEDURE_KIND_SUSPENSION));
    printf("- 法律：%zu\n", constitution->law_count);

    printf("\n【五、地方】\n");
    printf("- 共和国：%zu\n", constitution_count_constituencies_by_kind(constitution, CONSTITUTION_CONSTITUENCY_KIND_NATION));
    printf("- 大区：%zu\n", constitution_count_constituencies_by_kind(constitution, CONSTITUTION_CONSTITUENCY_KIND_REGION));
    printf("- 省：%zu\n", constitution_count_constituencies_by_kind(constitution, CONSTITUTION_CONSTITUENCY_KIND_PROVINCE));
    printf("- 广域市：%zu\n", constitution_count_constituencies_by_kind(constitution, CONSTITUTION_CONSTITUENCY_KIND_METROPOLITAN_CITY));
    printf("- 省区：%zu\n", constitution_count_constituencies_by_kind(constitution, CONSTITUTION_CONSTITUENCY_KIND_SUB_PREFECTURE));
    printf("- 公社：%zu\n", constitution_count_constituencies_by_kind(constitution, CONSTITUTION_CONSTITUENCY_KIND_COMMUNE));

    printf("\n【六、宪法的解释与条款暂停】\n");
    printf("- 宪法法院：%zu\n", constitution->court_count);
    printf("- 法律/条约/命令/决议相关文书：%zu\n",
           constitution_count_documents_by_kind(constitution, CONSTITUTION_DOCUMENT_KIND_BILL) +
           constitution_count_documents_by_kind(constitution, CONSTITUTION_DOCUMENT_KIND_AMENDMENT) +
           constitution_count_documents_by_kind(constitution, CONSTITUTION_DOCUMENT_KIND_DECREE) +
           constitution_count_documents_by_kind(constitution, CONSTITUTION_DOCUMENT_KIND_RESOLUTION) +
           constitution_count_documents_by_kind(constitution, CONSTITUTION_DOCUMENT_KIND_TREATY) +
           constitution_count_documents_by_kind(constitution, CONSTITUTION_DOCUMENT_KIND_ORDER));
    printf("- 章条外规则与普通立法接口：%zu\n", constitution->law_count + constitution->procedure_count);

    printf("\n【七、补充非宪法原文】\n");
    printf("- 选举席位和人口比例条目：%zu\n", constitution_count_elections_by_kind(constitution, CONSTITUTION_ELECTION_KIND_CLOSED_LIST) + constitution_count_elections_by_kind(constitution, CONSTITUTION_ELECTION_KIND_OPEN_LIST) + constitution_count_elections_by_kind(constitution, CONSTITUTION_ELECTION_KIND_FREE_LIST));

    printf("\n【配置目录】\n");
    printf("- 行政区条目：%zu\n", constitution->constituency_count);
    printf("- 人名条目：%zu\n", constitution->member_count);
    printf("- 政党条目：通过 config/parties.csv 载入\n");

    printf("\n【目录清单】\n");
    printf("- 行政区：\n");
    for (size_t index = 0; index < constitution->constituency_count; ++index) {
        constituency_t *item = constitution->constituencies[index];
        if (item != NULL) {
            printf("  - %s｜%s｜人口 %llu\n", item->name != NULL ? item->name : "(空)", constitution_constituency_kind_name(item->kind), item->population);
        }
    }

    printf("- 人名录：\n");
    for (size_t index = 0; index < constitution->member_count; ++index) {
        member_t *item = constitution->members[index];
        if (item != NULL) {
            printf("  - %s｜%s｜%u岁\n", item->name != NULL ? item->name : "(空)", item->role != NULL ? item->role : "(空)", item->age);
        }
    }

    printf("- 合议体：\n");
    for (size_t index = 0; index < constitution->body_count; ++index) {
        body_t *item = constitution->bodies[index];
        if (item != NULL) {
            printf("  - %s｜%s｜席位 %zu\n", item->name != NULL ? item->name : "(空)", constitution_body_kind_name(item->kind), item->seat.total);
        }
    }

    printf("- 职务：\n");
    for (size_t index = 0; index < constitution->office_count; ++index) {
        office_t *item = constitution->offices[index];
        if (item != NULL) {
            printf("  - %s｜%s\n", item->name != NULL ? item->name : "(空)", constitution_office_kind_name(item->kind));
        }
    }

    printf("- 程序：\n");
    for (size_t index = 0; index < constitution->procedure_count; ++index) {
        procedure_t *item = constitution->procedures[index];
        if (item != NULL) {
            printf("  - %s｜%s\n", item->name != NULL ? item->name : "(空)", constitution_procedure_kind_name(item->kind));
        }
    }

    printf("- 文书：\n");
    for (size_t index = 0; index < constitution->document_count; ++index) {
        document_t *item = constitution->documents[index];
        if (item != NULL) {
            printf("  - %s｜%s\n", item->name != NULL ? item->name : "(空)", constitution_document_kind_name(item->kind));
        }
    }

    printf("- 选举：\n");
    for (size_t index = 0; index < constitution->election_count; ++index) {
        election_t *item = constitution->elections[index];
        if (item != NULL) {
            printf("  - %s｜%s｜席位 %zu｜选民 %zu\n", item->name != NULL ? item->name : "(空)", constitution_election_kind_name(item->kind), item->seat_count, item->voter_count);
        }
    }

    printf("- 公民复决：\n");
    for (size_t index = 0; index < constitution->referendum_count; ++index) {
        referendum_t *item = constitution->referendums[index];
        if (item != NULL) {
            printf("  - %s\n", item->name != NULL ? item->name : "(空)");
        }
    }

    printf("- 弹劾：\n");
    for (size_t index = 0; index < constitution->impeachment_count; ++index) {
        impeachment_t *item = constitution->impeachments[index];
        if (item != NULL) {
            printf("  - %s\n", item->name != NULL ? item->name : "(空)");
        }
    }

    printf("- 法律：\n");
    for (size_t index = 0; index < constitution->law_count; ++index) {
        law_t *item = constitution->laws[index];
        if (item != NULL) {
            printf("  - %s｜%s\n",
                   item->document.name != NULL ? item->document.name : "(空)",
                   item->status_text != NULL ? item->status_text : "(空)");
        }
    }

    printf("- 法院：\n");
    for (size_t index = 0; index < constitution->court_count; ++index) {
        court_t *item = constitution->courts[index];
        if (item != NULL) {
            printf("  - %s\n", item->name != NULL ? item->name : "(空)");
        }
    }
}
