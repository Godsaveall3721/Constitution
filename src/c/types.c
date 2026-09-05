#include "types.h"
#include "internal.h"

const char *constitution_body_kind_name(const constitution_body_kind_t kind)
{
    switch (kind) {
    case CONSTITUTION_BODY_KIND_NATIONAL_ASSEMBLY: return "国家议会";
    case CONSTITUTION_BODY_KIND_PALACE: return "共和国议会";
    case CONSTITUTION_BODY_KIND_REGIONAL_COUNCIL: return "大区议会";
    case CONSTITUTION_BODY_KIND_DEPARTMENTAL_COUNCIL: return "省议会";
    case CONSTITUTION_BODY_KIND_METROPOLITAN_COUNCIL: return "广域市议会";
    case CONSTITUTION_BODY_KIND_MUNICIPAL_COUNCIL: return "公社委员会";
    case CONSTITUTION_BODY_KIND_COMMITTEE: return "委员会";
    default: return "其他合议体";
    }
}

const char *constitution_office_kind_name(const constitution_office_kind_t kind)
{
    switch (kind) {
    case CONSTITUTION_OFFICE_KIND_PRESIDENT: return "总统";
    case CONSTITUTION_OFFICE_KIND_PRIME_MINISTER: return "总理";
    case CONSTITUTION_OFFICE_KIND_SPEAKER: return "议长";
    case CONSTITUTION_OFFICE_KIND_CHAIR: return "主席";
    case CONSTITUTION_OFFICE_KIND_MINISTER: return "阁员";
    default: return "其他职务";
    }
}

const char *constitution_procedure_kind_name(const constitution_procedure_kind_t kind)
{
    switch (kind) {
    case CONSTITUTION_PROCEDURE_KIND_ELECTION: return "选举";
    case CONSTITUTION_PROCEDURE_KIND_APPOINTMENT: return "任命";
    case CONSTITUTION_PROCEDURE_KIND_DISSOLUTION: return "解散";
    case CONSTITUTION_PROCEDURE_KIND_REFERENDUM: return "公民复决";
    case CONSTITUTION_PROCEDURE_KIND_IMPEACHMENT: return "弹劾";
    case CONSTITUTION_PROCEDURE_KIND_REVIEW: return "审查";
    case CONSTITUTION_PROCEDURE_KIND_EXPLANATION: return "解释";
    case CONSTITUTION_PROCEDURE_KIND_SUSPENSION: return "条款暂停";
    default: return "其他程序";
    }
}

const char *constitution_document_kind_name(const constitution_document_kind_t kind)
{
    switch (kind) {
    case CONSTITUTION_DOCUMENT_KIND_BILL: return "法案";
    case CONSTITUTION_DOCUMENT_KIND_AMENDMENT: return "修正案";
    case CONSTITUTION_DOCUMENT_KIND_DECREE: return "命令";
    case CONSTITUTION_DOCUMENT_KIND_RESOLUTION: return "决议";
    case CONSTITUTION_DOCUMENT_KIND_TREATY: return "条约";
    case CONSTITUTION_DOCUMENT_KIND_ORDER: return "政令";
    default: return "其他文书";
    }
}

const char *constitution_election_kind_name(const constitution_election_kind_t kind)
{
    switch (kind) {
    case CONSTITUTION_ELECTION_KIND_TWO_ROUND_MAJORITY: return "二轮绝对多数制";
    case CONSTITUTION_ELECTION_KIND_FREE_LIST: return "自由式名单制";
    case CONSTITUTION_ELECTION_KIND_OPEN_LIST: return "开放式名单制";
    case CONSTITUTION_ELECTION_KIND_CLOSED_LIST: return "封闭式名单制";
    case CONSTITUTION_ELECTION_KIND_SINGLE_TRANSFERABLE: return "单记让渡制";
    case CONSTITUTION_ELECTION_KIND_SORTED_RUNOFF: return "排序复选制";
    default: return "其他选举制";
    }
}

char *constitution_clone_text(const char *text)
{
    return constitution_strdup_local(text);
}

void constitution_release_text(char **text)
{
    constitution_free_text_local(text);
}
