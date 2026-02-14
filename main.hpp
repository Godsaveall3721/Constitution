//
// Created by x-pc on 2026/2/14.
//

#ifndef CONSTITUTION_MAIN_HPP
#define CONSTITUTION_MAIN_HPP

extern "C" long long add_64(long long a, long long b);

#define party_default 5
// Far_Right 0
// Right_Wing 1
// Centrist 2
// Left_Wing 3
// Far_Left 4
#define party_preference_default 1


namespace base_function {

    // 命名空间函数重载为原理的参数重载（局限，死板）
    inline int vs1(const int x, const int y) { return x>y ? x : y; }
    inline float vs1(const float x, const float y) { return x>y ? x : y; }
    inline double vs1(const double x, const double y) { return x>y ? x : y; }
    inline long double vs1(const long double x, const long double y) { return x>y ? x : y; }

    // 模板函数重载(入/出参数重载灵活多变)
    template<typename x, typename y>
    constexpr auto vs2(x&& a, y&& b) -> decltype(a + b) {
        return a>b ? a : b;
    }

    // my diy模板
    template<typename x = void>
    struct MyPlus;
    template<>
    struct MyPlus<void> {
        template<typename a, typename b>
        constexpr auto operator()(a&& x, b&& y) const
        -> decltype(std::forward<a>(x) + std::forward<b>(y))
        {
            return std::forward<a>(x) + std::forward<b>(y);
        }
    };

}




#endif //CONSTITUTION_MAIN_HPP