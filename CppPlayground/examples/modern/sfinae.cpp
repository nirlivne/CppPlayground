#include "register_items.h"
#include "categories.h"
#include <fmt/core.h>
#include <iomanip>
#include <iostream>
#include <type_traits>

// SFINAE - Substitution Failure Is Not An Error.

// How to Make SFINAE Pretty – Part 1: What SFINAE Brings to Code
// https://www.fluentcpp.com/2018/05/15/make-sfinae-pretty-1-what-value-sfinae-brings-to-code/

// CppCon 2017: Stephen Dewhurst “Modern C++ Interfaces..."
// https://www.youtube.com/watch?v=PFdWqa68LmA

// Substition failure example

struct A {
    using type = int;
};

//
template <typename T>
void f(T x, typename T::type y)
{
    // ...
}

// this will cause a substitusion failure for [T = A()]
// as other_type is not defined for A
// The compiler will simply ignore this piece of code

template <typename T>
void f(T x, typename T::other_type y)
{
    // ...
}

////////////////////////////////////////////////////////

template <bool Condition, typename T = void>
struct my_enable_if {
};

// specialize only for true ...
template <typename T>
struct my_enable_if<true, T> {
    typedef T type;
};

template <typename T>
class MyClass {
public:
    void f(T const& x);

    template <typename T_ = T>
    void f(T&& x,
        typename my_enable_if<!std::is_reference<T_>::value,
            std::nullptr_t>::type = nullptr);
};

ADD_MENU_ITEM(sfinae, "SFINAE", category_core_cpp) //, "", "SFINAE")
{
    f(A(), 42);
}