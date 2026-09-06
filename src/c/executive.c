#include "executive.h"

#include <stdio.h>

constitution_executive_resolution_t constitution_resolve_executive_mode(double direct_bill_support_ratio, size_t electoral_college_members)
{
    constitution_executive_resolution_t resolution;
    resolution.direct_bill_support_ratio = direct_bill_support_ratio;
    resolution.direct_bill_passed = (direct_bill_support_ratio >= 0.5);
    resolution.electoral_college_members = electoral_college_members;

    if (resolution.direct_bill_passed) {
        resolution.mode = CONSTITUTION_EXECUTIVE_MODE_DIRECT;
        resolution.president_method = "公民直接通过二轮绝对多数制产生";
        resolution.government_head = "总统";
        resolution.cabinet_presidency = "总统主持内阁会议";
        resolution.executive_power = "总统与总理内阁共同行使";
        resolution.president_vote_rights = "总统对内阁会议议题拥有主持权，并对国防、外交与重大行政方针作出决定";
        resolution.prime_minister_role = "总理协助总统落实政令并指挥各行政部门";
        resolution.cabinet_note = "若最近议会选举中的附加案赞成票过半，则总统由直选产生并兼任政府首长";
        resolution.electoral_college_note = "不启动总统选举大会";
    } else {
        resolution.mode = CONSTITUTION_EXECUTIVE_MODE_INDIRECT;
        resolution.president_method = "总统选举大会通过排序复选制产生";
        resolution.government_head = "总理";
        resolution.cabinet_presidency = "总统主持内阁会议，但可委托总理主持内阁会议";
        resolution.executive_power = "总理内阁完全行使";
        resolution.president_vote_rights = "总统主持内阁会议但不参与表决";
        resolution.prime_minister_role = "总理为政府首长，行政实权由总理内阁完全行使";
        resolution.cabinet_note = "若附加案赞成票少于半数，则启动总统选举大会，按排序复选制产生总统";
        resolution.electoral_college_note = "总统选举大会由共和国、大区、省和广域市的全体议会议员组成";
    }

    return resolution;
}

const char *constitution_executive_mode_name(const constitution_executive_mode_t mode)
{
    switch (mode) {
        case CONSTITUTION_EXECUTIVE_MODE_DIRECT:
            return "直选总统制";
        case CONSTITUTION_EXECUTIVE_MODE_INDIRECT:
            return "间接总统制";
        default:
            return "未知模式";
    }
}

void constitution_print_executive_resolution(const constitution_executive_resolution_t *resolution)
{
    if (resolution == NULL) {
        return;
    }

    printf("[行政首长模式] %s\n", constitution_executive_mode_name(resolution->mode));
    printf("- 最近议会附加案赞成率：%.1f%%\n", resolution->direct_bill_support_ratio * 100.0);
    printf("- 结果：%s\n", resolution->direct_bill_passed ? "超过半数，启动直选总统路径" : "未过半数，启动总统选举大会路径");
    printf("- 总统产生方式：%s\n", resolution->president_method);
    printf("- 政府首长：%s\n", resolution->government_head);
    printf("- 内阁主持：%s\n", resolution->cabinet_presidency);
    printf("- 行政权：%s\n", resolution->executive_power);
    printf("- 总统权限：%s\n", resolution->president_vote_rights);
    printf("- 总理职能：%s\n", resolution->prime_minister_role);
    printf("- 内阁说明：%s\n", resolution->cabinet_note);
    printf("- 选举大会说明：%s\n", resolution->electoral_college_note);
    printf("- 总统选举大会成员：%zu\n", resolution->electoral_college_members);
}
