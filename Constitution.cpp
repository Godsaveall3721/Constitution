// 尽可能利用C++特性,不忘C语言特性
// C++ 版本允许范围为98,03,11,14,17

#include <iostream>
#include <random>
#include <memory>  // 现代C++智能指针
#include <cstdlib> // C模式的malloc/free
#include "Constitution.hpp"

extern "C" {
#include "election.h"
}

extern "C" long long add_asm (long long a, long long b);

namespace constitution {

    class Member;
    class ExecutiveBody;
    class Constituency;
    class ChamberOfDeputies;
    class Senate;
    class ConseilDelleMetropolitanCitta;
    class DepaertmentalConseil;
    class MunicipalCouncil;

    // 第一步：实现行政单位抽象层的构建
    // 共和国----大区-------省--------省区---公社
    //               \
    //                \----广域市-----------公社
    class AdministrativeUnit { // 行政区划： base
    public:
        std::string name;
        std::string capital;
        uint64_t population;
        std::vector<std::shared_ptr<Constituency>> constituencies; // 对应的选区
        AdministrativeUnit(const std::string& the_name, const std::string& the_capital, uint64_t pop = 0) : name(the_name),capital(the_capital), population(pop) {
            name = the_name;
            capital = the_capital;
        }
        virtual std::shared_ptr<ExecutiveBody> formExecutive() = 0; // 从议员中产生行政班底的虚函数
        virtual ~AdministrativeUnit() = default;
    };

    class Commune : public AdministrativeUnit { // 行政区划： 公社
    public:
        std::string maire; // 市长由委员会选举产生
        std::string Maire_est_egalement_Preseidente_of_Municipal_Council; // 市长兼公社委员会主席
        std::shared_ptr<MunicipalCouncil> council; // 公社委员会
        std::shared_ptr<ExecutiveBody> mairie; // 公社行政班底：市政厅
        const int termYears = 7; // 任期 7 年
        Commune(const std::string& name, const std::string& Subprefecture, const uint64_t pop) : AdministrativeUnit(name, Subprefecture, pop) {
            council = std::make_shared<MunicipalCouncil>();
        }

        // 实现虚函数：由委员会产生市政厅
        std::shared_ptr<ExecutiveBody> formExecutive() override {
            // mairie = council->formExecutiveBody(); // 权限委托给议会
            return mairie;
        }
    };

    class Arrondissement_of_Depaertment : public AdministrativeUnit { // 行政区划： 省区 (仅仅是省的下级，不是广域市的下级)
    public:
        std::string Sous_Prefet; // 副省长(内政部任命)
        std::vector<std::shared_ptr<Commune>> communes; // 下辖公社
        Arrondissement_of_Depaertment(const std::string& name, const std::string& Sous_prefecture, const uint64_t pop) : AdministrativeUnit(name, Sous_prefecture, pop) {} // 副省会(Sous-prefecture)

    };

    class Depaertment : public AdministrativeUnit { // 行政区划： 省
        std::string Prefet_of_Departement; // 省长(内政部任命)
        std::string Preseidente_of_Depaertmental_Conseil; // 省议会主席
        std::vector<std::shared_ptr<Arrondissement_of_Depaertment>> Arrondissement_of_Depaertments; // 下辖省区
        std::shared_ptr<DepaertmentalConseil> council; // 省议会
        Depaertment(const std::string& name, const std::string& Prefectures, const uint64_t pop) : AdministrativeUnit(name, Prefectures, pop) {
            council = std::make_shared<DepaertmentalConseil>();
        } // 省会(Prefectures)
    };

    class Metropolitan_Citta : public AdministrativeUnit { // 行政区划： 广域市
        std::string Metropolitan_Sindaco; // 广域市市长（相对多数制产生）
        std::string Preseidente_of_Conseil_Delle_Metropolitan_Citta; // 广域市议会主席
        std::vector<std::shared_ptr<Commune>> communes; // 下辖公社
        std::shared_ptr<ConseilDelleMetropolitanCitta> council; // 广域市议会
        Metropolitan_Citta(const std::string& name, const std::string& Central_City, const uint64_t pop) : AdministrativeUnit(name, Central_City, pop) {
            council = std::make_shared<ConseilDelleMetropolitanCitta>();
        } // 中心城市(Central City)
    };

    class Region : public AdministrativeUnit { // 行政区划： 大区
    public:
        std::string Prefet_of_Region; // 大区区长(内政部任命)
        std::string Preseidente_of_Regional_Conseil; // 大区议会主席
        std::vector<std::shared_ptr<Depaertment>> depaertments; // 下辖省
        std::vector<std::shared_ptr<Metropolitan_Citta>> metropolitan_cittas;  // 下辖广域市
        Region(const std::string& name, const std::string& Prefectures, const uint64_t pop) : AdministrativeUnit(name, Prefectures, pop) {}
    };

    class Republic : public AdministrativeUnit { // 行政区划： 共和国
    public:
        std::string President; // 总统
        std::string Prime_Minister; //  总理
        std::vector<std::shared_ptr<Region>> regions; // 下辖大区
        std::shared_ptr<Senate> senate;
        std::shared_ptr<ChamberOfDeputies> chamber;
        Republic() : AdministrativeUnit("ROE", "Vienna", 500000000ULL) {
            senate = std::make_shared<Senate>();
            chamber = std::make_shared<ChamberOfDeputies>();
        }

        std::shared_ptr<ExecutiveBody> formExecutive() override {
            std::cout << "Republic forming executive (placeholder).\n";
            return nullptr;
        }
    };

    // 第二步：完善各个抽象层的议会,以及选举制度
    // 第一级                        | 第二级           | 第三级                            |  第四级（省的辅助行政区）| 第五级
    // 国家议会（参议院/众议院，两院制）| 大区议会（一院制） | 省议会（一院制） 广域市议会（一院制） |  无                   | 公社的委员会(一院制)

    typedef enum class ElectionSystem {
        TwoRoundMajority,    // 二轮绝对多数制
        FreeList,            // 自由式名单制
        OpenList,            // 开放式名单制
        ClosedList,          // 封闭式名单制
        SingleTransferable,  // 单记让渡制
        RelativeMajority     // 相对多数制
    } ElectionSystem;

    class AbstractCouncil { // 议会：base
    public:
        std::string councilName;
        int termYears; // 任期
        ElectionSystem electionMethod; // 选举模式
        std::vector<std::shared_ptr<Member>> members; // 议员席位 现代vector + shared_ptr
        std::shared_ptr<Member> councilPresident;    // 议会主席
        AbstractCouncil(const std::string &name, const int term, const ElectionSystem method) : councilName(name), termYears(term), electionMethod(method) {}

        virtual std::shared_ptr<ExecutiveBody> runElection() = 0;
        virtual std::shared_ptr<ExecutiveBody> formExecutive() = 0;
        virtual ~AbstractCouncil() = default;

    };

    class ChamberOfDeputies : public AbstractCouncil { // 议会：共和国众议院
    public:
        ChamberOfDeputies() : AbstractCouncil("共和国众议院", 4, ElectionSystem::ClosedList) {} // 配合联立制
        std::shared_ptr<ExecutiveBody> runElection() override {
            /* 模拟 4年一届的选举 */
            /* 全国性封闭式名单制 + 小选区性二轮绝对多数制  联立制--席位不固定，党票瓜分比例，小选区胜者必进议会，没有达到比例党内候补进去 */

            return nullptr;
        }

        std::shared_ptr<ExecutiveBody> formExecutive() override {
            /* 组阁与总理信任投票（直选总统无需建设性投票，间选总统需要建设性投票）*/
            /* 选举议长 */
        }

    };

    class Senate : public AbstractCouncil { // 议会：共和国参议院
    public:
        Senate() : AbstractCouncil("共和国参议院", 6, ElectionSystem::FreeList) {} // 配合并立制
        std::shared_ptr<ExecutiveBody> runElection() override {
            /* 模拟 1/3 改选逻辑 */
            /* 历届非弹劾卸任总统为终身当然议员 (不参与改选) */
            /* 由公民通过二轮绝对多数制产生（每个大区7名） */
            /* 全国性自由式名单制 + 小选区性二轮绝对多数制 并立制--席位固定，60%是小选区专属，40%是党比例分配专属 */

            return nullptr;
        }
        std::shared_ptr<ExecutiveBody> formExecutive() override {
            /* 参议院在内阁模式下民选议员部分自己决定是否付出可能被解散改选的代价来参与组阁 */
            /* 选举议长 */
        }
    };

    class RegionalCouncil : public AbstractCouncil { // 议会：大区议会
    public:
        RegionalCouncil() : AbstractCouncil("大区议会", 4, ElectionSystem::FreeList) {}
        std::shared_ptr<ExecutiveBody> runElection() override {
            /* 模拟 4年一届自由名单制*/
            return nullptr;
        }
        std::shared_ptr<ExecutiveBody> formExecutive() override {
            /* 大区议会选举大区议会主席，大区委员会 */
            /* 中央钦定的大区区长 (与此议会无关) */
        }
    };

    class DepaertmentalConseil : public AbstractCouncil { // 议会：省议会
    public:
        DepaertmentalConseil() : AbstractCouncil("省议会", 5, ElectionSystem::OpenList) {}
        std::shared_ptr<ExecutiveBody> runElection() override {
            /* 模拟 5年一届的开放式名单制 */
            return nullptr;
        }
        std::shared_ptr<ExecutiveBody> formExecutive() override {
            /* 省议会选举省议会主席，省委员会 */
            /* 中央钦定的省长 (与此议会无关) */
        }
    };

    class ConseilDelleMetropolitanCitta : public AbstractCouncil { // 议会：广域市议会
    public:
        ConseilDelleMetropolitanCitta() : AbstractCouncil("广域市议会", 5, ElectionSystem::OpenList) {}
        std::shared_ptr<ExecutiveBody> runElection() override {
            /* 模拟 5年一届的开放式名单制 */
            return nullptr;
        }
        std::shared_ptr<ExecutiveBody> formExecutive() override {
            /* 广域市议会选举广域市议会主席，都会委员会 */
            /* 市民直选的广域市市长 (与此议会无关) */
        }
    };

    class MunicipalCouncil : public  AbstractCouncil { // 议会：公社委员会
    public:
        MunicipalCouncil() : AbstractCouncil("公社委员会", 7, ElectionSystem::SingleTransferable) {}
        std::shared_ptr<ExecutiveBody> runElection() override {
            /* 模拟 7年一届单记让渡制 */
            return nullptr;
        }
        std::shared_ptr<ExecutiveBody> formExecutive() override {
            /* 公社委员会选举市长兼公社委员会主席，市政厅 */
        }
    };

    // 第三步：完善选区
    class Constituency {
    public:
        std::string name;
        uint64_t voterCount;
        // 大区选举由行省(Province)选区单位组成，省由县(Canton)组成，众议院民选部分，参议院民选部分，直选总统由公社(Commune)组成
        Constituency(const std::string& n, const uint64_t& v) : name(n), voterCount(v) {}
    };

    // 第四步：完善行政班底/内阁/委员会
    class ExecutiveBody {
    public:
        std::shared_ptr<Member> leader; // 总理/主席/市长
        std::vector<std::shared_ptr<Member>> cabinetMembers; // 阁员/委员名单

        // virtual void setLeader(std::string name) { leader = name; }
        // virtual void addMember(std::string name) { cabinetMembers.push_back(name); }

        // 对应宪法：副署逻辑
        bool countersign(const std::string& order) {
            std::cout << "班底成员正在对命令 [" << order << "] 进行副署..." << std::endl;
            return true;
        }

        virtual ~ExecutiveBody() = default;
    };

    // 特殊化：共和国内阁
    class Cabinet : public ExecutiveBody {
    public:
        // 处理建设性不信任案逻辑
        bool handleNoConfidenceVote();
    };

    class Member {
    public:
        std::string name;
        // 回顾老C++ 不用std::shared_ptr<Constituency>，没有RAII机制兜底。new/delete
        Constituency* fromConstituency;
        bool isCabinetMember = false; // 是否已入阁
        Member(const std::string &n, Constituency* c) : name(n), fromConstituency(c) {}
        ~Member() {
            // 故意不delete fromConstituency（共享，不拥有所有权）
            // 如果拥有所有权，可在这里delete
        }
    };

}





int main() {
    std::cout << "ROE Constitution Simulation - Stage 1\n\n";

    constitution::Republic republic;

    // 测试创建几个选区
    for (int i = 1; i <= 5; ++i) {
        republic.constituencies.push_back(
            std::make_shared<constitution::Constituency>("Constituency " + std::to_string(i), 5000000ULL)
        );
    }

    std::cout << "Republic: " << republic.name << ", Capital: " << republic.capital
              << ", Population: " << republic.population << "\n";

    // 模拟议会初始化
    republic.chamber->runElection();
    republic.senate->runElection();

    // 模拟形成执行机构
    republic.formExecutive();

    std::cout << "\nStage 1 complete. No crash.\n";
    return 0;
}