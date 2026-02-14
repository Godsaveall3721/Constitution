// 尽可能利用C++特性,不忘C语言特性
// C++ 版本允许范围为98,03,11,14,17

#include <iostream>
#include <random>
#include <memory>
#include "main.hpp"

namespace constitution  {

uint8_t party = party_default;
uint8_t party_preference = party_preference_default;

std::default_random_engine engine(static_cast<unsigned>(time(nullptr)));
std::uniform_real_distribution<double> distribution(0.0, 1.0);

class the_Faction {
public:
    explicit the_Faction() { // explicit 拒绝隐式调用 auto x(elections_fun temp); 阻止传入ture自己生成临时对象，必须传入自己生成的对象

        this -> F = new double[party];

        // 计算过程用catch保护
        try {
            // 初步概率分配
            for (int i = 0; i < party; i++) {
                F[i] = distribution(engine);
            }

            F[party_preference] += distribution(engine) * 10;

            ReCalculation:
            for (int i = 0; i < party; i++) {
                temp += F[i];
            }
            for (int i = 0; i < party; i++) {
                F[i] = (F[i]/temp) * 100.00;
            }
            temp = 0.0;


            if (party % 2 == 1) {
                if (F[(party + 1 ) / 2] + F[((party + 1 ) / 2) - 2] + F[((party + 1 ) / 2) - 1] <= 95.00) {
                    F[(party + 1 ) / 2] += 0.01;
                    F[((party + 1 ) / 2) - 2] += 0.01;
                    F[((party + 1 ) / 2) - 1] += 0.01;
                    goto ReCalculation;
                }
            }else if (party % 2 == 0) {
                if (F[party / 2] + F[(party / 2) - 1] <= 95.00) {
                    F[party / 2] += 0.01;
                    F[(party / 2) - 1] += 0.01;
                    goto ReCalculation;
                }
            }else {
                throw std::logic_error("非法的政党数目");
            }

            // 优化偏好色谱
            if (party_preference != 0 && party_preference != party - 1 && F[party_preference] + F[party_preference + 1] + F[party_preference - 1] < 75.0 ) {
                F[party_preference] += 0.4;
                F[party_preference + 1] += 0.2;
                F[party_preference - 1] += 0.1;
                F[party_preference + 2] -= 0.2;
                goto ReCalculation;
            }
        } catch (...) {
        }

    };

    double* get_prt_fun() const {
        return this -> F;
    }
    virtual ~the_Faction() {
        delete[] this -> F;
        this -> F = nullptr;
    }

private:
    double temp = 0.0;
    double *F = nullptr; // 勿忘NULL的诞生，歧义之 int 0
};

// 模拟ROE宪法
class elections_fun : public the_Faction {
public:
    explicit elections_fun(const bool the_bool0) { // explicit 拒绝隐式调用 auto x(elections_fun temp); 阻止传入ture自己生成临时对象，必须传入自己生成的对象
        Directly_elected = the_bool0;
        has_vice_Prime_Minister = Indirectly_elected = !the_bool0;
        this -> F = get_prt_fun();
    }
    virtual bool do_elections() const {
        int8_t way = NULL;
        switch (Directly_elected) {
            case true:
                std::cout << "总统选举,直选模式" << std::endl;
                if (distribution(engine) <= 0.15) {
                    std::cout << "直选总统为无党籍" << std::endl; // 无党籍直接任命议会最大党成员出任总理,没有最大党就成立执政联盟
                    for (uint8_t i = 0;i < party; i++) {
                        std::cout << F[i] << "      ";
                    }
                    std::cout << std::endl;
                    auto temp = 0.0;
                    for (uint8_t i = 0;i < party - 1; i++) {
                        temp > base_function::vs1(F[i], F[i + 1]) ?  temp : temp = base_function::vs1(F[i], F[i + 1]);
                        if (i == party - 2) {
                            int temp2 = NULL;
                            for (uint8_t i = 0;i < party - 1; i++) {
                                (F[i] == base_function::vs2(F[i],F[i + 1]) && F[temp2] < F[i]) ?  temp2 = i : temp2;
                            }

                            if (party % 2 == 1) {
                                (temp2 > ((party + 1) / 2)) ? way = -1 : way = 1;
                            }else if (party % 2 == 0) {
                                (temp2 >= (party / 2)) ? way = -1 : way = 1;
                            }else{
                                throw std::logic_error("非法的政党数目");
                            }

                            std::cout << "晴雨表(右派到左派)：议会最大党" << temp2 << "号党派，总统潜在支持率为: " << temp << std::endl;
                                if (temp < 50.00) {
                                    std::cout << "直选总统所属党派在议会议席比例未超过50%" << std::endl; // 解决方法 1.政党执政联盟 2.依靠威望强行任命总理 3.认输放弃内政
                                        if (distribution(engine) > 0.05 ) { // 0.88的可能在议会内部解决
                                            if (distribution(engine) > 0.4) { // 0.6的可能政治光谱相近政党同意联盟
                                            std::cout << "直选总统所属政党与光谱相近政党结为执政联盟" << std::endl;
                                            std::cout << "联合政府由" << temp2 << "和" << temp2 + way << "号党派组成。支持率为: " << F[temp2] + F[temp2 + way] << std::endl;
                                            }else {
                                            std::cout << "直选总统所属政党与光谱相近政党选择与在野党结为执政联盟" << std::endl;
                                            std::cout << "联合政府由" << temp2 + 1 << "和" << temp2 + (2*way) << "号党派组成。支持率为: " << F[temp2 + way] + F[temp2 + (2*way)] << std::endl;
                                            }
                                        }else { // 总统强行任命自己党派成员出任总理
                                            // 如果被议会通过2次不信任动议就会下台,而且议会自己选出的总理，总统不得拒绝任命
                                            std::cout << "直选总统依靠其政治手腕任命所属政党派成员出任总理,政府由" << temp2 << "号党派组成。支持率为: " << F[temp2] << std::endl;
                                        }
                                }else {
                                    std::cout << "直选总统所属党派在议会议席比例未过50%,因此任命所属政党派成员出任总理,政府由" << temp2 << "号党派组成。支持率为: " << F[temp2] << std::endl;
                                }
                        }
                    }


                }else {
                    std::cout << "直选总统为有党籍" << std::endl;
                    for (uint8_t i = 0;i < party; i++) {
                        std::cout << F[i] << "      ";
                    }
                    std::cout << std::endl;
                    auto temp = 0.0;
                    for (uint8_t i = 0;i < party - 1; i++) {
                        temp > base_function::vs1(F[i], F[i + 1]) ?  temp : temp = base_function::vs1(F[i], F[i + 1]);
                        if (i == party - 2) {
                            int temp2 = NULL;
                            for (uint8_t i = 0;i < party - 1; i++) {
                                (F[i] == base_function::vs2(F[i],F[i + 1]) && F[temp2] < F[i]) ?  temp2 = i : temp2;
                            }

                            if (party % 2 == 1) {
                                (temp2 > ((party + 1) / 2)) ? way = -1 : way = 1;
                            }else if (party % 2 == 0) {
                                (temp2 >= (party / 2)) ? way = -1 : way = 1;
                            }else{
                                throw std::logic_error("非法的政党数目");
                            }

                            std::cout << "晴雨表(右派到左派)：直选总统属于" << temp2 << "号党派，总统潜在支持率为: " << temp << std::endl;
                                if (temp < 50.00) {
                                    std::cout << "直选总统所属党派在议会议席比例未超过50%" << std::endl; // 解决方法 1.政党执政联盟 2.依靠威望强行任命总理 3.认输放弃内政
                                        if (distribution(engine) > 0.05 ) { // 0.88的可能在议会内部解决
                                            if (distribution(engine) > 0.4) { // 0.6的可能政治光谱相近政党同意联盟
                                            std::cout << "直选总统所属政党与光谱相近政党结为执政联盟" << std::endl;
                                            std::cout << "联合政府由" << temp2 << "和" << temp2 + way << "号党派组成。支持率为: " << F[temp2] + F[temp2 + way] << std::endl;
                                            }else {
                                            std::cout << "直选总统所属政党与光谱相近政党选择与在野党结为执政联盟" << std::endl;
                                            std::cout << "联合政府由" << temp2 + 1 << "和" << temp2 + (2*way) << "号党派组成。支持率为: " << F[temp2 + way] + F[temp2 + (2*way)] << std::endl;
                                            }
                                        }else { // 总统强行任命自己党派成员出任总理
                                            // 如果被议会通过2次不信任动议就会下台,而且议会自己选出的总理，总统不得拒绝任命
                                            std::cout << "直选总统依靠其政治手腕任命所属政党派成员出任总理,政府由" << temp2 << "号党派组成。支持率为: " << F[temp2] << std::endl;
                                        }
                                }else {
                                    std::cout << "直选总统所属党派在议会议席比例未过50%,因此任命所属政党派成员出任总理,政府由" << temp2 << "号党派组成。支持率为: " << F[temp2] << std::endl;
                                }
                        }
                    }
                }
                std::cout << std::endl;
                return true;
            case false:
                std::cout << "总统选举,间选模式,晴雨表(右到左): " << std::endl;
                for (uint8_t i = 0;i < party; i++) {
                    std::cout << F[i] << "      ";
                }
                std::cout << std::endl;
                return false;
            default:
                break;
        }
        return NULL;
    }

    ~elections_fun() override {
            // delete[] this -> F; 并非this申请，不应由this来释放
            this -> F = nullptr;
    };

protected:
bool Directly_elected; // 直接
bool Indirectly_elected; // 间接
bool has_vice_Prime_Minister; // 副总理

private:
    double *F = nullptr; // 勿忘NULL的诞生，歧义之 int 0
};

    // 第一步：实现行政单位抽象层的构建
    // 共和国----大区-------省--------省区---公社
    //               \
    //                \----广域市-----------公社
    class AdministrativeUnit {
    public:
        std::string name;
        std::string capital;
        uint64_t population;
        AdministrativeUnit(const std::string& the_name, const std::string& the_capital) : name(the_name),capital(the_capital) {
            name = the_name;
            capital = the_capital;
        }
        virtual ~AdministrativeUnit() = default;
    };

    class Commune : public AdministrativeUnit {
    public:
        std::string Maire_est_egalement_Preseidente_of_Municipal_Council; // 市长兼公社委员会主席
        const int termYears = 7; // 任期 7 年
        Commune(const std::string& name, const std::string& Subprefecture) : AdministrativeUnit(name, Subprefecture) {}
    };

    class Arrondissement_of_Depaertment : public AdministrativeUnit {
    public:
        std::string Sous_Prefet; // 副省长
        std::vector<std::shared_ptr<Commune>> communes; // 下辖公社
        Arrondissement_of_Depaertment(const std::string& name, const std::string& Sous_prefecture) : AdministrativeUnit(name, Sous_prefecture) {} // 副省会(Sous-prefecture)

    };

    class Depaertment : public AdministrativeUnit {
        std::string Prefet_of_Departement; // 省长
        std::string Preseidente_of_Depaertmental_Conseil; // 省议会主席
        std::vector<std::shared_ptr<Arrondissement_of_Depaertment>> Arrondissement_of_Depaertments; // 下辖省区
        Depaertment(const std::string& name, const std::string& Prefectures) : AdministrativeUnit(name, Prefectures) {} // 省会(Prefectures)
    };

    class Metropolitan_Citta : public AdministrativeUnit {
        std::string Metropolitan_Sindaco; // 广域市市长
        std::string Preseidente_of_Conseil_Delle_Metropolitan_Citta; // 广域市议会主席
        std::vector<std::shared_ptr<Commune>> communes; // 下辖公社
        Metropolitan_Citta(const std::string& name, const std::string& Central_City) : AdministrativeUnit(name, Central_City) {} // 中心城市(Central City)
    };

    class Region : public AdministrativeUnit {
    public:
        std::string Prefet_of_Region; // 大区区长
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
        Republic(const std::string& name, const std::string capital) : AdministrativeUnit(name, capital) {}

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
        ElectionSystem electionMethod;

        AbstractCouncil(std::string name, int term, ElectionSystem method) : councilName(name), termYears(term), electionMethod(method) {}
        virtual ~AbstractCouncil() = default;
        virtual void runElection() = 0;

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

    class DepaertmentalCouncil : public AbstractCouncil {
    public:
        DepaertmentalCouncil(std::string levelName) : AbstractCouncil(levelName + "议会", 5, ElectionSystem::OpenList) {}
        void runElection() override { /* 模拟 5年一届的开放式名单制 */ }
    };

    class MunicipalCouncil : public  AbstractCouncil {
    public:
        MunicipalCouncil() : AbstractCouncil("公社委员会", 7, ElectionSystem::SingleTransferable) {}
        void runElection() override { /* 模拟 7年一届单记让渡制 */}

    };
}





int main() {
    // the_Faction faction;
    // double* arr = faction.get_prt_fun();
    // for (int i = 0; i < 5; i++) {
    //     std::cout << "Element " << i << ": " << arr[i] << std::endl;
    // }
    // int a = base_function::MyPlus<void>()(20,40);
    // base_function::MyPlus ss;
    // int b = ss(23,22);

    // constitution::elections_fun a1(true);
    //myfun::elections_fun a2(false);
    // a1.do_elections();

    //a2.do_elections();
    // return 0;

}