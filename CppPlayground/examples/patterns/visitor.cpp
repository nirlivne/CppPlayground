#include "register_items.h"
#include <iostream>
#include <variant>
#include <vector>

// https://www.cppstories.com/2018/09/visit-variants/

struct Car {
    void drive() { std::cout << "Driving car\n"; }
};
struct Juice {
    void drink() { std::cout << "Drinking juice\n"; }
};
struct Burger {
    void eat() { std::cout << "Eating burger\n"; }
};
struct Shirt {
    void wear() { std::cout << "Wearing shirt\n"; }
};

template <class... Ts>
struct overload : Ts... {
    using Ts::operator()...;
};
// template<class... Ts> overload(Ts...)->overload<Ts...>; // line not needed in C++20...

auto createVisitor()
{
    return overload {
        // Duck typing is an advantage here
        [](Car& e) { e.drive(); },
        [](Juice& e) { e.drink(); },
        [](Burger& e) { e.eat(); },
        [](Shirt& e) { e.wear(); },
        //[](auto& e) { std::cout << "other...\n"; }
    };
};

ADD_MENU_ITEM(visitor1, "visitor pattern using std::variant", "", "pattern")
{
    std::variant<Car, Juice, Burger, Shirt> package;

    package = Car();
    std::visit(createVisitor(), package);

    package = Burger();
    std::visit(createVisitor(), package);
}

// https://gieseanw.wordpress.com/2017/05/03/a-true-heterogeneous-container-in-c/
template <class... T>
struct VariantContainer {
    template <class V>
    void visit(V&& visitor)
    {
        for (auto& object : objects) {
            std::visit(visitor, object);
        }
    }
    using value_type = std::variant<T...>;
    std::vector<value_type> objects;
};

ADD_MENU_ITEM(visitor2, "visitor example using container or std::variant", "", "pattern")
{
    VariantContainer<Juice, Burger, Shirt> container;
    container.objects.push_back(Shirt());
    container.objects.push_back(Burger());
    container.objects.push_back(Juice());

    container.visit(createVisitor());
}
