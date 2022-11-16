#include "register_items.h"
#include "categories.h"
#include <iostream>
#include <variant>
#include <vector>

// 
// https://www.fluentcpp.com/2017/05/12/curiously-recurring-template-pattern/

template <class T>
struct Base
{
    void interface()
    {
        // ...
        static_cast<T*>(this)->implementation();
        // ...
    }

    static void static_func()
    {
        // ...
        T::static_sub_func();
        // ...
    }
};

struct Derived : Base<Derived>
{
    void implementation()         { std::cout << "Derived::implementation\n"; }
    static void static_sub_func() { std::cout << "Derived::static_sub_func\n"; };
};



ADD_MENU_ITEM(crtp, "Curiously Recurring Template Pattern (CRTP)", category_design) 
{
    // Static polymorphism
    Derived c;
    c.interface();
    Derived::static_func();
}
