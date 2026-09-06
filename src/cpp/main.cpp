#include "constitution.hpp"
#include "config_book.hpp"
#include "election_book.hpp"

extern "C" {
#include "executive.h"
#include "member.h"
}

#include <array>
#include <iostream>
#include <filesystem>
#include <algorithm>
#include <cstddef>
#include <system_error>
#include <string>
#include <vector>

namespace fs = std::filesystem;

static std::string resolve_config_directory()
{
    std::error_code error;
    const fs::path executable = fs::read_symlink("/proc/self/exe", error);
    if (!error) {
        const fs::path executable_directory = executable.parent_path();
        const fs::path candidates[] = {
            executable_directory / "../config",
            executable_directory / "config",
            executable_directory.parent_path() / "config",
        };

        for (const auto &candidate : candidates) {
            if (fs::exists(candidate, error) && fs::is_directory(candidate, error)) {
                return candidate.lexically_normal().string();
            }
        }
    }

    const fs::path current = fs::current_path();
    const fs::path candidates[] = {
        current / "config",
        current / "../config",
        current / "../Constitution/config",
    };

    for (const auto &candidate : candidates) {
        std::error_code error;
        if (fs::exists(candidate, error) && fs::is_directory(candidate, error)) {
            return candidate.lexically_normal().string();
        }
    }

    return "../config";
}

static constitution_status_t 试运行(void *context)
{
    (void)context;
    return CONSTITUTION_STATUS_OK;
}

static std::size_t 分配席次总数(const std::vector<double> &populationShares, std::size_t totalSeats, std::size_t index)
{
    std::vector<std::size_t> seats(populationShares.size(), 0);
    std::vector<double> remainders(populationShares.size(), 0.0);

    std::size_t allocated = 0;
    for (std::size_t current = 0; current < populationShares.size(); ++current) {
        const double exactSeats = populationShares[current] * static_cast<double>(totalSeats);
        const std::size_t roundedSeats = static_cast<std::size_t>(exactSeats);
        seats[current] = roundedSeats;
        remainders[current] = exactSeats - static_cast<double>(roundedSeats);
        allocated += roundedSeats;
    }

    while (allocated < totalSeats) {
        std::size_t bestIndex = 0;
        double bestRemainder = -1.0;
        for (std::size_t current = 0; current < remainders.size(); ++current) {
            if (remainders[current] > bestRemainder) {
                bestRemainder = remainders[current];
                bestIndex = current;
            }
        }
        ++seats[bestIndex];
        remainders[bestIndex] = 0.0;
        ++allocated;
    }

    return seats[index];
}

static std::size_t 统计精确职务(const constitution_t *model, const char *role)
{
    if (model == nullptr || role == nullptr) {
        return 0;
    }

    std::size_t count = 0;
    for (std::size_t index = 0; index < model->member_count; ++index) {
        member_t *item = model->members[index];
        if (item != nullptr && item->role != nullptr && std::string(item->role) == role) {
            ++count;
        }
    }
    return count;
}

static election_t *按名称查找选举(const constitution_t *model, const char *name)
{
    if (model == nullptr || name == nullptr) {
        return nullptr;
    }

    for (std::size_t index = 0; index < model->election_count; ++index) {
        election_t *item = model->elections[index];
        if (item != nullptr && item->name != nullptr && std::string(item->name) == name) {
            return item;
        }
    }

    return nullptr;
}

static const char *选举层级(const char *name)
{
    if (name == nullptr) {
        return "其他";
    }
    const std::string electionName(name);
    if (electionName.find("总统") != std::string::npos ||
        electionName.find("共和国议会") != std::string::npos ||
        electionName.find("参议院") != std::string::npos ||
        electionName.find("众议院") != std::string::npos) {
        return "共和国级";
    }
    if (electionName.find("大区") != std::string::npos) {
        return "大区级";
    }
    if (electionName.find("省议会") != std::string::npos || electionName == "省议会选举") {
        return "省级";
    }
    if (electionName.find("广域市") != std::string::npos) {
        return "广域市级";
    }
    if (electionName.find("公社") != std::string::npos) {
        return "公共级";
    }
    return "其他";
}

static void 打印选举分层(const constitution_t *model)
{
    if (model == nullptr) {
        return;
    }

    struct Section {
        const char *title;
        const char *kind;
    } sections[] = {
        {"共和国的选举", "共和国级"},
        {"大区的选举", "大区级"},
        {"省的选举", "省级"},
        {"广域市的选举", "广域市级"},
        {"公共的选举", "公共级"},
        {"其他选举", "其他"},
    };

    std::cout << "\n【选举分层】\n";
    for (const auto &section : sections) {
        bool hasItem = false;
        for (std::size_t index = 0; index < model->election_count; ++index) {
            election_t *item = model->elections[index];
            if (item == nullptr) {
                continue;
            }
            if (std::string(选举层级(item->name)) == section.kind) {
                hasItem = true;
                break;
            }
        }
        if (!hasItem) {
            continue;
        }

        std::cout << "- " << section.title << "：\n";
        for (std::size_t index = 0; index < model->election_count; ++index) {
            election_t *item = model->elections[index];
            if (item == nullptr) {
                continue;
            }
            if (std::string(选举层级(item->name)) != section.kind) {
                continue;
            }
            std::cout << "  - " << (item->name != nullptr ? item->name : "(空)")
                      << "｜制度：" << constitution_election_kind_name(item->kind)
                      << "｜依据：" << (item->basis != nullptr ? item->basis : "(空)")
                      << "｜席位 " << item->seat_count
                      << "｜选民 " << item->voter_count << "\n";
        }
    }
}

static void 打印两院结构(const constitution_t *model, std::size_t populationTotal)
{
    if (model == nullptr || populationTotal == 0) {
        return;
    }

    const std::size_t houseSeats = (populationTotal + 450000 - 1) / 450000;
    const std::size_t senateElectedSeats = (houseSeats + 3) / 4;
    const std::size_t regionalDirectSenateSeats = constitution_count_constituencies_by_kind(model, CONSTITUTION_CONSTITUENCY_KIND_REGION) * 7;
    const std::size_t senateIndirectSeats = senateElectedSeats > regionalDirectSenateSeats ? senateElectedSeats - regionalDirectSenateSeats : 0;
    const std::size_t lifetimeSenators = 统计精确职务(model, "终身当然议员");
    const std::size_t electedSenators = 统计精确职务(model, "参议员");
    const std::size_t houseMembers = 统计精确职务(model, "众议员");
    const std::size_t genericMembers = 统计精确职务(model, "议员");

    std::cout << "【两院结构】\n";
    std::cout << "- 众议院：法定席位 " << houseSeats << " 席，按每 450000 公民 1 席计算；当前名录登记众议员 " << houseMembers << " 名，普通议员 " << genericMembers << " 名。\n";
    std::cout << "- 参议院（选任）：法定选任席位 " << senateElectedSeats << " 席，按众议院席数的 1/4 维持；其中大区直选参议员席次 " << regionalDirectSenateSeats << " 席，间选席位 " << senateIndirectSeats << " 席；当前名录登记参议员 " << electedSenators << " 名。\n";
    std::cout << "- 参议院（终身当然）：当前名录登记终身当然议员 " << lifetimeSenators << " 名；历届非弹劾卸任国家元首自动进入参议院。\n";
    std::cout << "- 议会选举制度：众议院使用封闭式名单制 + 二轮绝对多数制的联立制；参议院使用自由式名单制 + 二轮绝对多数制的并立制。\n";
}

static void 装配宪法全貌(constitution_t *model)
{
    const std::size_t populationTotal = 550000000ULL;
    const std::size_t houseSeats = (populationTotal + 450000 - 1) / 450000;
    const std::size_t regionCount = constitution_count_constituencies_by_kind(model, CONSTITUTION_CONSTITUENCY_KIND_REGION);
    const std::size_t provinceCount = constitution_count_constituencies_by_kind(model, CONSTITUTION_CONSTITUENCY_KIND_PROVINCE);
    const std::size_t metropolitanCityCount = constitution_count_constituencies_by_kind(model, CONSTITUTION_CONSTITUENCY_KIND_METROPOLITAN_CITY);
    const std::size_t subPrefectureCount = constitution_count_constituencies_by_kind(model, CONSTITUTION_CONSTITUENCY_KIND_SUB_PREFECTURE);
    const std::size_t communeCount = constitution_count_constituencies_by_kind(model, CONSTITUTION_CONSTITUENCY_KIND_COMMUNE);
    const std::size_t directSenateSeats = regionCount * 7;
    const std::size_t indirectSenateSeats = (houseSeats + 3) / 4 > directSenateSeats ? ((houseSeats + 3) / 4) - directSenateSeats : 0;

    const struct {
        const char *name;
        constitution_body_kind_t kind;
        size_t seats;
    } bodies[] = {
        {"国家议会", CONSTITUTION_BODY_KIND_NATIONAL_ASSEMBLY, houseSeats + (houseSeats + 3) / 4},
        {"共和国议会", CONSTITUTION_BODY_KIND_PALACE, houseSeats},
        {"大区议会", CONSTITUTION_BODY_KIND_REGIONAL_COUNCIL, regionCount * 10},
        {"省议会", CONSTITUTION_BODY_KIND_DEPARTMENTAL_COUNCIL, 72},
        {"广域市议会", CONSTITUTION_BODY_KIND_METROPOLITAN_COUNCIL, 72},
        {"公社委员会", CONSTITUTION_BODY_KIND_MUNICIPAL_COUNCIL, 64},
        {"宪法委员会", CONSTITUTION_BODY_KIND_COMMITTEE, 8},
    };

    for (const auto &item : bodies) {
        constitution_add_body(model, item.name, item.kind, item.seats);
    }

    const struct {
        const char *name;
        constitution_office_kind_t kind;
    } offices[] = {
        {"总统", CONSTITUTION_OFFICE_KIND_PRESIDENT},
        {"总理", CONSTITUTION_OFFICE_KIND_PRIME_MINISTER},
        {"参议院议长", CONSTITUTION_OFFICE_KIND_SPEAKER},
        {"众议院议长", CONSTITUTION_OFFICE_KIND_SPEAKER},
        {"大区议会主席", CONSTITUTION_OFFICE_KIND_CHAIR},
        {"大区区长", CONSTITUTION_OFFICE_KIND_CHAIR},
        {"省议会主席", CONSTITUTION_OFFICE_KIND_CHAIR},
        {"省长", CONSTITUTION_OFFICE_KIND_CHAIR},
        {"广域市议会主席", CONSTITUTION_OFFICE_KIND_CHAIR},
        {"广域市市长", CONSTITUTION_OFFICE_KIND_CHAIR},
        {"公社委员会主席", CONSTITUTION_OFFICE_KIND_CHAIR},
        {"公社市长", CONSTITUTION_OFFICE_KIND_CHAIR},
        {"司法部长", CONSTITUTION_OFFICE_KIND_MINISTER},
        {"内政部长", CONSTITUTION_OFFICE_KIND_MINISTER},
        {"外长", CONSTITUTION_OFFICE_KIND_MINISTER},
        {"国防部长", CONSTITUTION_OFFICE_KIND_MINISTER},
        {"财政部长", CONSTITUTION_OFFICE_KIND_MINISTER},
        {"副总理", CONSTITUTION_OFFICE_KIND_OTHER},
        {"宪法法院主席", CONSTITUTION_OFFICE_KIND_OTHER},
        {"最高法院院长", CONSTITUTION_OFFICE_KIND_OTHER},
    };

    for (const auto &item : offices) {
        constitution_add_office(model, item.name, item.kind);
    }

    const struct {
        const char *name;
        constitution_procedure_kind_t kind;
        const char *basis;
    } procedures[] = {
        {"总统公民直选程序", CONSTITUTION_PROCEDURE_KIND_ELECTION, "二轮绝对多数制"},
        {"总统选举大会程序", CONSTITUTION_PROCEDURE_KIND_ELECTION, "排序复选制"},
        {"参议院选举程序", CONSTITUTION_PROCEDURE_KIND_ELECTION, "自由式名单制"},
        {"众议院选举程序", CONSTITUTION_PROCEDURE_KIND_ELECTION, "封闭式名单制"},
        {"大区议会选举程序", CONSTITUTION_PROCEDURE_KIND_ELECTION, "自由式名单制"},
        {"省议会选举程序", CONSTITUTION_PROCEDURE_KIND_ELECTION, "开放式名单制"},
        {"广域市议会选举程序", CONSTITUTION_PROCEDURE_KIND_ELECTION, "开放式名单制"},
        {"公社委员会选举程序", CONSTITUTION_PROCEDURE_KIND_ELECTION, "单记让渡制"},
        {"总理任命程序", CONSTITUTION_PROCEDURE_KIND_APPOINTMENT, "总统任命"},
        {"大区区长任免程序", CONSTITUTION_PROCEDURE_KIND_APPOINTMENT, "内政部会议决定"},
        {"省长任免程序", CONSTITUTION_PROCEDURE_KIND_APPOINTMENT, "内政部会议决定"},
        {"副省长任免程序", CONSTITUTION_PROCEDURE_KIND_APPOINTMENT, "内政部会议决定"},
        {"法官任命程序", CONSTITUTION_PROCEDURE_KIND_APPOINTMENT, "总统与政府内阁商议"},
        {"议会解散程序", CONSTITUTION_PROCEDURE_KIND_DISSOLUTION, "总理建议、总统命令"},
        {"公民复决程序", CONSTITUTION_PROCEDURE_KIND_REFERENDUM, "公民复决"},
        {"弹劾程序", CONSTITUTION_PROCEDURE_KIND_IMPEACHMENT, "议会与高级法庭"},
        {"违宪审查程序", CONSTITUTION_PROCEDURE_KIND_REVIEW, "宪法法院"},
        {"宪法解释程序", CONSTITUTION_PROCEDURE_KIND_EXPLANATION, "宪法法院"},
        {"条款暂停程序", CONSTITUTION_PROCEDURE_KIND_SUSPENSION, "宪法法院"},
        {"国民大会召开程序", CONSTITUTION_PROCEDURE_KIND_OTHER, "议会常会与特别会议"},
        {"预算审查程序", CONSTITUTION_PROCEDURE_KIND_OTHER, "议会"},
    };

    for (const auto &item : procedures) {
        constitution_add_procedure(model, item.name, item.kind, item.basis, 试运行);
    }

    const struct {
        const char *name;
        constitution_document_kind_t kind;
        const char *basis;
        const char *content;
    } documents[] = {
        {"序言", CONSTITUTION_DOCUMENT_KIND_RESOLUTION, "制宪权力", "我们，欧罗巴共和国的公民。"},
        {"I 前言总纲", CONSTITUTION_DOCUMENT_KIND_RESOLUTION, "国家基础", "不可分割、民主、社会服务、主权在民。"},
        {"II 总统总纲", CONSTITUTION_DOCUMENT_KIND_BILL, "总统条款", "直选、间选、任期、军队统帅、赦免、解散、复议、紧急命令。"},
        {"II 总统任期", CONSTITUTION_DOCUMENT_KIND_BILL, "总统条款 ii", "六年任期、可连任一届、错位十二个月。"},
        {"II 总统缺位", CONSTITUTION_DOCUMENT_KIND_BILL, "总统条款 vi", "代行、恢复、60 日内重启选举。"},
        {"II 总统副署", CONSTITUTION_DOCUMENT_KIND_BILL, "总统条款 viii", "除固有权力外须经副署。"},
        {"II 总统解散与复议", CONSTITUTION_DOCUMENT_KIND_BILL, "总统条款 ix-x", "任免总理、解散议会、提请复决、法案覆议。"},
        {"III 国家议会", CONSTITUTION_DOCUMENT_KIND_BILL, "议会条款", "参议院与众议院组成国家议会。"},
        {"III 两院议长", CONSTITUTION_DOCUMENT_KIND_BILL, "议会条款 ii", "两院选举本院议长及其他官员。"},
        {"III 议会召集", CONSTITUTION_DOCUMENT_KIND_BILL, "议会条款 iii", "总统召集、议会自召。"},
        {"III 参议院组成", CONSTITUTION_DOCUMENT_KIND_BILL, "议会条款 iv-vi", "自由式名单制、间选、改选顺序。"},
        {"III 众议院组成", CONSTITUTION_DOCUMENT_KIND_BILL, "议会条款 vii", "封闭式名单制、普选、四年任期。"},
        {"III 绝对双轨", CONSTITUTION_DOCUMENT_KIND_BILL, "议会条款 viii", "宪法修正、领土变动、宣战媾和、条约、选举法与地方制度法。"},
        {"III 预算与信任", CONSTITUTION_DOCUMENT_KIND_BILL, "议会条款 xii-xiv", "预算审查、信任投票、不信任案。"},
        {"III 解散与改选", CONSTITUTION_DOCUMENT_KIND_BILL, "议会条款 xiv, xxiv, xlvi", "解散、复选、改选期。"},
        {"III 常规法案", CONSTITUTION_DOCUMENT_KIND_BILL, "议会条款 xvi", "参议院可否决，众议院最终通过。"},
        {"III 议会调查", CONSTITUTION_DOCUMENT_KIND_RESOLUTION, "议会条款 xlix", "国政调查、证人、记录。"},
        {"III 薪资法律", CONSTITUTION_DOCUMENT_KIND_BILL, "议会条款 li", "影响薪资的法律延至下一届。"},
        {"III 终身当然议员", CONSTITUTION_DOCUMENT_KIND_RESOLUTION, "议会条款 lii", "历届卸任国家元首入参议院。"},
        {"III 内阁制模式", CONSTITUTION_DOCUMENT_KIND_RESOLUTION, "议会条款 liii", "直选/间选下的组阁与不信任规则。"},
        {"IV 司法总纲", CONSTITUTION_DOCUMENT_KIND_BILL, "司法章总则", "司法权归宪法法院、最高法院及各级法院。"},
        {"IV 宪法法院组成", CONSTITUTION_DOCUMENT_KIND_BILL, "司法条款 iv", "八人、半数改选、政府与两院任命。"},
        {"IV 选举监督", CONSTITUTION_DOCUMENT_KIND_BILL, "司法条款 vi-viii", "监督选举与公民投票。"},
        {"IV 修宪与审查", CONSTITUTION_DOCUMENT_KIND_AMENDMENT, "司法条款 ix-xi", "修宪、审查、委员会、二周裁决。"},
        {"IV 条约效力", CONSTITUTION_DOCUMENT_KIND_TREATY, "司法条款 xii", "条约公布后高于法律。"},
        {"V 单一制与地方", CONSTITUTION_DOCUMENT_KIND_RESOLUTION, "地方章总则", "共和国为单一制国家，地方权利系下授。"},
        {"V 大区制度", CONSTITUTION_DOCUMENT_KIND_BILL, "地方条款 ii-vi", "大区、首府、议会、委员会、区长。"},
        {"V 省制度", CONSTITUTION_DOCUMENT_KIND_BILL, "地方条款 vii-x", "省、会、议会、委员会、省长。"},
        {"V 广域市制度", CONSTITUTION_DOCUMENT_KIND_BILL, "地方条款 xi-xv", "广域市、中心城市、议会、委员会、市长。"},
        {"V 省区制度", CONSTITUTION_DOCUMENT_KIND_BILL, "地方条款 xvi-xvii", "省区与副省长。"},
        {"V 公社制度", CONSTITUTION_DOCUMENT_KIND_BILL, "地方条款 xviii-xxi", "公社、委员会、市长、市政厅。"},
        {"VI 不可修改", CONSTITUTION_DOCUMENT_KIND_AMENDMENT, "解释与条款暂停", "领土完整与共和体制不可修改。"},
        {"VI 地域清单", CONSTITUTION_DOCUMENT_KIND_RESOLUTION, "解释条款 iii-xi", "各大区及其下辖省与广域市。"},
        {"VII 普通立法补充", CONSTITUTION_DOCUMENT_KIND_BILL, "补充条款", "每名众议员代表 450000 公民。"},
    };

    for (const auto &item : documents) {
        constitution_add_document(model, item.name, item.kind, item.basis, item.content);
    }

    constitution_add_court(model, "共和国最高法院", "共和国司法权", false);
    constitution_add_court(model, "宪法法院", "违宪审查", true);

    const struct {
        const char *name;
        constitution_election_kind_t kind;
        const char *basis;
        size_t seat_count;
        size_t voter_count;
    } elections[] = {
        {"共和国众议院选举", CONSTITUTION_ELECTION_KIND_CLOSED_LIST, "封闭式名单制 + 二轮绝对多数制的联立制；选区对应 `config/electoral_districts.csv` 中共和国/大区/省/广域市/省区/公社层级", houseSeats, populationTotal},
        {"共和国参议院直选席次", CONSTITUTION_ELECTION_KIND_FREE_LIST, "自由式名单制 + 二轮绝对多数制的并立制；每个大区 7 席", directSenateSeats, 550000000ULL},
        {"共和国参议院间选席次", CONSTITUTION_ELECTION_KIND_FREE_LIST, "联合大区、省和广域市议会间选；并立制", indirectSenateSeats, 550000000ULL},
        {"总统选举大会", CONSTITUTION_ELECTION_KIND_SORTED_RUNOFF, "共和国、大区、省、广域市全体议会议员组成", 1, 550000000ULL},
        {"总统选举", CONSTITUTION_ELECTION_KIND_SORTED_RUNOFF, "公民直选；二轮绝对多数制", 1, 550000000ULL},
        {"大区议会选举", CONSTITUTION_ELECTION_KIND_FREE_LIST, "大区级自由式名单制", regionCount, populationTotal},
        {"省议会选举", CONSTITUTION_ELECTION_KIND_OPEN_LIST, "省级开放式名单制", provinceCount, populationTotal},
        {"广域市议会选举", CONSTITUTION_ELECTION_KIND_OPEN_LIST, "广域市级开放式名单制", metropolitanCityCount, populationTotal},
        {"省区议会选举", CONSTITUTION_ELECTION_KIND_OPEN_LIST, "省区层级选举单位", subPrefectureCount, populationTotal},
        {"公社委员会选举", CONSTITUTION_ELECTION_KIND_SINGLE_TRANSFERABLE, "公社级单记让渡制", communeCount, populationTotal},
    };

    for (const auto &item : elections) {
        constitution_add_election(model, item.name, item.kind, item.basis, item.seat_count, item.voter_count);
    }
}

int main()
{
    republic::Constitution constitution;
    constitution_t *model = constitution.native();

    const std::string configDirectory = resolve_config_directory();
    const std::size_t populationTotal = 550000000ULL;

    republic::cpp::ConfigBook configBook;
    configBook.load(configDirectory);

    constitution_load_config_directory(model, configDirectory.c_str());

    auto *共和国议会 = constitution_add_body(model, "共和国议会", CONSTITUTION_BODY_KIND_PALACE, 1223);
    auto *大区议会 = constitution_add_body(model, "大区议会", CONSTITUTION_BODY_KIND_REGIONAL_COUNCIL, 80);
    auto *总统 = constitution_add_office(model, "总统", CONSTITUTION_OFFICE_KIND_PRESIDENT);
    auto *总理 = constitution_add_office(model, "总理", CONSTITUTION_OFFICE_KIND_PRIME_MINISTER);
    auto *选举程序 = constitution_add_procedure(model, "共和国议会大选程序", CONSTITUTION_PROCEDURE_KIND_ELECTION, "依据众议院条款", 试运行);
    auto *总统法案 = constitution_add_document(model, "总统命令", CONSTITUTION_DOCUMENT_KIND_DECREE, "依据总统命令副署条款", "总统命令须经内阁相关阁员副署始生效力。");
    auto *公民复决 = constitution_add_referendum(model, "公民复决", "公民通过复决行使国家主权", 500000);
    auto *弹劾案 = constitution_add_impeachment(model, "弹劾案", "行政部门或司法部门官员之弹劾案", "相关官员");
    auto *法案 = constitution_add_law(model, "选举法", "选举法及地方制度法案", "普通立法内容", "普通立法处理");
    auto *法院 = constitution_add_court(model, "宪法法院", "依据宪法的解释与条款暂停", true);

    装配宪法全貌(model);

    member_t 议员{};
    member_init(&议员, "公民代表", "议员", 45);
    body_add_member(共和国议会, &议员);
    body_add_member(大区议会, &议员);
    office_assign_holder(总统, &议员);
    office_assign_holder(总理, &议员);

    struct RegionSeat {
        std::string name;
        unsigned long long population;
        std::size_t seats;
    };

    std::vector<RegionSeat> regions;
    for (const auto &district : configBook.electoralDistricts()) {
        if (district.kind == "大区") {
            regions.push_back({district.name, district.population, 0});
        }
    }

    std::size_t regionPopulationTotal = 0;
    for (const auto &region : regions) {
        regionPopulationTotal += static_cast<std::size_t>(region.population);
    }

    const std::size_t totalSeats = (populationTotal + 450000 - 1) / 450000;
    std::vector<double> populationShares;
    populationShares.reserve(regions.size());
    for (const auto &region : regions) {
        populationShares.push_back(static_cast<double>(region.population) / static_cast<double>(regionPopulationTotal));
    }

    for (std::size_t regionIndex = 0; regionIndex < regions.size(); ++regionIndex) {
        regions[regionIndex].seats = 分配席次总数(populationShares, totalSeats, regionIndex);
    }

    std::cout << "【共和国议会大选模拟】\n";
    election_t *共和国议会大选 = 按名称查找选举(model, "共和国众议院选举");
    election_describe(共和国议会大选);
    std::cout << "投票规则：封闭式名单制 + 二轮绝对多数制的联立制\n";
    std::cout << "共和国总人口：" << populationTotal << "\n";
    std::cout << "议席分配：\n";

    for (const auto &region : regions) {
        std::cout << "- " << region.name << "：" << region.seats << " 席，人口 " << region.population << "\n";
    }

    std::cout << "合计：" << totalSeats << " 席\n";
    std::cout << "\n";

    打印两院结构(model, populationTotal);
    std::cout << "\n";

    const auto executiveResolution = constitution_resolve_executive_mode(0.62, constitution_count_electoral_college_members(model));
    constitution_print_executive_resolution(&executiveResolution);
    std::cout << "\n";

    configBook.printElectoralDistrictGallery(std::cout);
    std::cout << "\n";
    configBook.printPartyGallery(std::cout);

    std::cout << "\n模拟结果：\n";

    republic::cpp::ElectionBook electionBook(totalSeats);
    std::vector<std::pair<std::string, double>> shares;
    const auto &parties = configBook.parties();
    for (const auto &party : parties) {
        shares.emplace_back(party.name, party.voteShare / 100.0);
    }
    electionBook.loadFromPartyShares(shares);
    electionBook.balanceSeats();
    electionBook.printSeatTable(std::cout);

    const std::size_t majorityThreshold = totalSeats / 2 + 1;
    std::cout << "\n多数门槛：" << majorityThreshold << " 席\n";
    const auto *largestBloc = electionBook.largestBloc();
    if (largestBloc == nullptr) {
        std::cout << "最大党团：无\n";
    } else if (largestBloc->seats >= majorityThreshold) {
        std::cout << "最大党团：" << largestBloc->name << "，已单独过半，无需联合组阁。\n";
    } else {
        std::cout << "最大党团：" << largestBloc->name << "，未过半，需要联合组阁。\n";

        std::vector<std::string> coalitionPartners;
        for (const auto &allocation : electionBook.allocations()) {
            if (allocation.name != largestBloc->name) {
                coalitionPartners.push_back(allocation.name);
            }
        }

        if (!coalitionPartners.empty()) {
            std::cout << "可能联合对象：";
            for (std::size_t index = 0; index < coalitionPartners.size(); ++index) {
                if (index != 0) {
                    std::cout << " + ";
                }
                std::cout << coalitionPartners[index];
            }
            std::cout << "\n";
        }
    }

    打印选举分层(model);

    std::cout << "\n【机关摘要】\n";
    constitution_print_summary(model);
    constitution_print_exhaustive_report(model);

    (void)选举程序;
    (void)总统法案;
    (void)公民复决;
    (void)弹劾案;
    (void)法案;
    (void)法院;

    member_reset(&议员);
    return 0;
}
