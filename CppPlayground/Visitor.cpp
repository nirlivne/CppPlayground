#include <iostream>
#include <variant>
#include <vector>
#include "Visitor.h"

    
// https://www.cppstories.com/2018/09/visit-variants/

struct Fluid { };
struct LightItem { };
struct HeavyItem { };
struct FragileItem { };

template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
template<class... Ts> overload(Ts...)->overload<Ts...>; // line not needed in C++20...



void visitor_example() {
    std::variant<Fluid, LightItem, HeavyItem, FragileItem> package;

    std::visit(overload{
        [](Fluid&) { std::cout << "fluid\n"; },
        [](LightItem&) { std::cout << "light item\n"; },
        [](HeavyItem&) { std::cout << "heavy item\n"; },
        [](FragileItem&) { std::cout << "fragile\n"; }
        }, package);
}

template<class... T>
struct VariantContainer
{
    template<typename V>
    void visit(V&& visitor)
    {
        for (auto& object : objects)
        {
            std::visit(visitor, object);
        }
    }

    using value_type = std::variant<T...>;
    std::vector<value_type> objects;
};

void visitor_example_container() {

    using MyContainer = VariantContainer<Fluid, LightItem, HeavyItem, FragileItem>;

    MyContainer container;
    container.objects.push_back(Fluid());
    container.objects.push_back(HeavyItem());

    container.visit(overload{
        [](Fluid&) { std::cout << "fluid\n"; },
        [](LightItem&) { std::cout << "light item\n"; },
        [](HeavyItem&) { std::cout << "heavy item\n"; },
        [](FragileItem&) { std::cout << "fragile\n"; }
        });
}
