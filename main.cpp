// 尽可能利用C++特性,不忘C语言特性
// C++ 版本允许范围为98,03,11,14,17

#include <iostream>
#include <random>
#include <memory>  // 现代C++智能指针
#include <cstdlib> // C模式的malloc/free
#include "main.hpp"

extern "C" {
#include "election.h"
}

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
    class AdministrativeUnit {
    public:
        std::string name;
        std::string capital;
        uint64_t population;
        std::vector<std::shared_ptr<Constituency>> constituencies; // 对应的选区
        virtual std::shared_ptr<ExecutiveBody> formExecutive() = 0; // 从议员中产生行政班底的虚函数
        AdministrativeUnit(const std::string& the_name, const std::string& the_capital) : name(the_name),capital(the_capital) {
            name = the_name;
            capital = the_capital;
        }
        virtual ~AdministrativeUnit() = default;
    };

    class Commune : public AdministrativeUnit {
    public:
        std::string maire; // 市长由委员会选举产生
        std::string Maire_est_egalement_Preseidente_of_Municipal_Council; // 市长兼公社委员会主席
        std::shared_ptr<MunicipalCouncil> council; // 公社委员会
        std::shared_ptr<ExecutiveBody> mairie; // 公社行政班底：市政厅
        const int termYears = 7; // 任期 7 年
        Commune(const std::string& name, const std::string& Subprefecture) : AdministrativeUnit(name, Subprefecture) {
            council = std::make_shared<MunicipalCouncil>();
        }

        // 实现虚函数：由委员会产生市政厅
        std::shared_ptr<ExecutiveBody> formExecutive() override {
            // mairie = council->formExecutiveBody(); // 权限委托给议会
            return mairie;
        }
    };

    class Arrondissement_of_Depaertment : public AdministrativeUnit {
    public:
        std::string Sous_Prefet; // 副省长(内政部任命)
        std::vector<std::shared_ptr<Commune>> communes; // 下辖公社
        Arrondissement_of_Depaertment(const std::string& name, const std::string& Sous_prefecture) : AdministrativeUnit(name, Sous_prefecture) {} // 副省会(Sous-prefecture)

    };

    class Depaertment : public AdministrativeUnit {
        std::string Prefet_of_Departement; // 省长(内政部任命)
        std::string Preseidente_of_Depaertmental_Conseil; // 省议会主席
        std::vector<std::shared_ptr<Arrondissement_of_Depaertment>> Arrondissement_of_Depaertments; // 下辖省区
        std::shared_ptr<DepaertmentalConseil> council; // 省议会
        Depaertment(const std::string& name, const std::string& Prefectures) : AdministrativeUnit(name, Prefectures) {
            council = std::make_shared<DepaertmentalConseil>();
        } // 省会(Prefectures)
    };

    class Metropolitan_Citta : public AdministrativeUnit {
        std::string Metropolitan_Sindaco; // 广域市市长（相对多数制产生）
        std::string Preseidente_of_Conseil_Delle_Metropolitan_Citta; // 广域市议会主席
        std::vector<std::shared_ptr<Commune>> communes; // 下辖公社
        std::shared_ptr<ConseilDelleMetropolitanCitta> council; // 广域市议会
        Metropolitan_Citta(const std::string& name, const std::string& Central_City) : AdministrativeUnit(name, Central_City) {
            council = std::make_shared<ConseilDelleMetropolitanCitta>();
        } // 中心城市(Central City)
    };

    class Region : public AdministrativeUnit {
    public:
        std::string Prefet_of_Region; // 大区区长(内政部任命)
        std::string Preseidente_of_Regional_Conseil; // 大区议会主席
        std::vector<std::shared_ptr<Depaertment>> depaertments; // 下辖省
        std::vector<std::shared_ptr<Metropolitan_Citta>> metropolitan_cittas;  // 下辖广域市
        Region(const std::string& name, const std::string& Prefectures) : AdministrativeUnit(name, Prefectures) {}
    };

    class Republic : public AdministrativeUnit {
    public:
        std::string President; // 总统
        std::string Prime_Minister; //  总理
        std::vector<std::shared_ptr<Region>> regions; // 下辖大区
        std::shared_ptr<Senate> senate;
        std::shared_ptr<ChamberOfDeputies> chamber;
        Republic() : AdministrativeUnit("ROE", "Vienna") {
            senate = std::make_shared<Senate>();
            chamber = std::make_shared<ChamberOfDeputies>();
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

    class AbstractCouncil {
    public:
        std::string councilName;
        int termYears; // 任期
        ElectionSystem electionMethod; // 选举模式
        std::vector<std::shared_ptr<Member>> members; // 议员席位
        std::shared_ptr<Member> councilPresident;    // 议会主席
        AbstractCouncil(const std::string &name, const int term, const ElectionSystem method) : councilName(name), termYears(term), electionMethod(method) {}

        virtual void runElection() = 0;
        //virtual void formExecutiveBody()  = 0;
        virtual ~AbstractCouncil() = default;

    };

    class ChamberOfDeputies : public AbstractCouncil {
    public:
        ChamberOfDeputies() : AbstractCouncil("共和国众议院", 4, ElectionSystem::ClosedList) {} // 配合联立制
        void runElection() override { /* 模拟 4年一届的联立制选举 */ }
    };

    class Senate : public AbstractCouncil {
    public:
        Senate() : AbstractCouncil("共和国参议院", 6, ElectionSystem::FreeList) {} // 配合并立制
        void runElection() override { /* 模拟 1/3 改选逻辑 */}
    };

    class RegionalCouncil : public AbstractCouncil {
    public:
        RegionalCouncil() : AbstractCouncil("大区议会", 4, ElectionSystem::FreeList) {}
        void runElection() override { /* 模拟 4年一届自由名单制*/ }
    };

    class DepaertmentalConseil : public AbstractCouncil {
    public:
        DepaertmentalConseil() : AbstractCouncil("省议会", 5, ElectionSystem::OpenList) {}
        void runElection() override { /* 模拟 5年一届的开放式名单制 */ }
    };

    class ConseilDelleMetropolitanCitta : public AbstractCouncil {
    public:
        ConseilDelleMetropolitanCitta() : AbstractCouncil("广域市议会", 5, ElectionSystem::OpenList) {}
        void runElection() override { /* 模拟 5年一届的开放式名单制 */ }
    };

    class MunicipalCouncil : public  AbstractCouncil {
    public:
        MunicipalCouncil() : AbstractCouncil("公社委员会", 7, ElectionSystem::SingleTransferable) {}
        void runElection() override { /* 模拟 7年一届单记让渡制 */}

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

    // 特殊化：内阁
    class Cabinet : public ExecutiveBody {
    public:
        // 处理建设性不信任案逻辑
        bool handleNoConfidenceVote();
    };

    class Member {
    public:
        std::string name;
        std::shared_ptr<Constituency> fromConstituency;
        bool isCabinetMember = false; // 是否已入阁
        Member(const std::string &n, const std::shared_ptr<Constituency>& c) : name(n), fromConstituency(c) {}
    };

}





int main() {


}