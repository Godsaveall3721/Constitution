#include "constitution.hpp"
#include "config_book.hpp"
#include "election_book.hpp"

extern "C" {
#include "executive.h"
#include "member.h"
}

#include <iostream>
#include <filesystem>
#include <system_error>
#include <string>

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

int main()
{
    republic::Constitution constitution;
    constitution_t *model = constitution.native();

    const std::string configDirectory = resolve_config_directory();

    republic::cpp::ConfigBook configBook;
    configBook.load(configDirectory);

    constitution_load_config_directory(model, configDirectory.c_str());

    auto *共和国议会 = constitution_add_body(model, "共和国议会", CONSTITUTION_BODY_KIND_PALACE, 120);
    auto *大区议会 = constitution_add_body(model, "大区议会", CONSTITUTION_BODY_KIND_REGIONAL_COUNCIL, 80);
    auto *总统 = constitution_add_office(model, "总统", CONSTITUTION_OFFICE_KIND_PRESIDENT);
    auto *总理 = constitution_add_office(model, "总理", CONSTITUTION_OFFICE_KIND_PRIME_MINISTER);
    auto *选举程序 = constitution_add_procedure(model, "共和国议会大选程序", CONSTITUTION_PROCEDURE_KIND_ELECTION, "依据众议院条款", 试运行);
    auto *总统法案 = constitution_add_document(model, "总统命令", CONSTITUTION_DOCUMENT_KIND_DECREE, "依据总统命令副署条款", "总统命令须经内阁相关阁员副署始生效力。");
    auto *大选 = constitution_add_election(model, "共和国议会大选", CONSTITUTION_ELECTION_KIND_CLOSED_LIST, "众议院以封闭式名单制和二轮绝对多数制相配合的联立制选举产生", 120, 550000000ULL);
    auto *公民复决 = constitution_add_referendum(model, "公民复决", "公民通过复决行使国家主权", 500000);
    auto *弹劾案 = constitution_add_impeachment(model, "弹劾案", "行政部门或司法部门官员之弹劾案", "相关官员");
    auto *法案 = constitution_add_law(model, "选举法", "选举法及地方制度法案", "普通立法内容", "普通立法处理");
    auto *法院 = constitution_add_court(model, "宪法法院", "依据宪法的解释与条款暂停", true);

    member_t 议员{};
    member_init(&议员, "公民代表", "议员", 45);
    body_add_member(共和国议会, &议员);
    body_add_member(大区议会, &议员);
    office_assign_holder(总统, &议员);
    office_assign_holder(总理, &议员);

    struct RegionSeat {
        std::string name;
        unsigned long long population;
        size_t seats;
    };

    std::vector<RegionSeat> regions = {
        {"高卢-法兰西大区", 90000000ULL, 28},
        {"莱茵-日耳曼邦联大区", 88000000ULL, 27},
        {"奥匈-多瑙河大区", 76000000ULL, 24},
        {"波兰-立陶宛大区", 64000000ULL, 20},
        {"斯堪的纳维亚-卡尔马大区", 38000000ULL, 12},
        {"伊比利亚大区", 52000000ULL, 16},
        {"亚平宁-希腊大区", 74000000ULL, 23},
        {"联合王国-凯尔特大区", 68000000ULL, 20},
    };

    std::cout << "【共和国议会大选模拟】\n";
    election_describe(大选);
    std::cout << "投票规则：封闭式名单制 + 二轮绝对多数制的联立制\n";
    std::cout << "共和国总人口：550000000\n";
    std::cout << "议席分配：\n";

    size_t total_seats = 0;
    for (const auto &region : regions) {
        total_seats += region.seats;
        std::cout << "- " << region.name << "：" << region.seats << " 席，人口 " << region.population << "\n";
    }

    std::cout << "合计：" << total_seats << " 席\n";
    std::cout << "\n";

    configBook.printPartyGallery(std::cout);

    std::cout << "\n模拟结果：\n";

    republic::cpp::ElectionBook electionBook(total_seats);
    std::vector<std::pair<std::string, double>> shares;
    const auto &parties = configBook.parties();
    for (const auto &party : parties) {
        shares.emplace_back(party.name, party.voteShare / 100.0);
    }
    electionBook.loadFromPartyShares(shares);
    electionBook.balanceSeats();
    electionBook.printSeatTable(std::cout);

    std::cout << "\n多数门槛：61 席\n";
    const auto *largestBloc = electionBook.largestBloc();
    std::cout << "最大党团：" << (largestBloc == nullptr ? "无" : largestBloc->name) << "，需要联合组阁。\n";
    std::cout << "可能联合对象：社会民主联盟 + 绿党与公民联盟 + 自由复兴党。\n";

    std::cout << "\n【机关摘要】\n";
    constitution_print_summary(model);

    (void)选举程序;
    (void)总统法案;
    (void)公民复决;
    (void)弹劾案;
    (void)法案;
    (void)法院;

    member_reset(&议员);
    return 0;
}
