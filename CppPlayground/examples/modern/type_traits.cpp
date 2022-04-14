#include "register_items.h"
#include "categories.h"
#include <iostream>
#include <iomanip>
#include <type_traits>
#include <fmt/core.h>

template <typename T, typename U>
struct decay_equiv : std::is_same<typename std::decay<T>::type, U>::type {
};

#define CHECK_EQUIV(T, U)                        \
    fmt::print("{:<24} is same as   {:<24} ? {}\n", "std::decay(" #T ")", (#U), decay_equiv<T, U>::value);

ADD_MENU_ITEM(type_traits, "Type traits and stuff", category_stl) // , "", "Type traits")
{
    fmt::print("== std::decay ==\n");

    CHECK_EQUIV(int, int);
    CHECK_EQUIV(int&, int);
    CHECK_EQUIV(int&&, int);
    CHECK_EQUIV(const int&, int);
    CHECK_EQUIV(int[2], int*);
    CHECK_EQUIV(int(int), int (*)(int));

    fmt::print("== std type traits ==\n");

    enum E {};
    class C {};
    int i;

    fmt::print("is_enum<E> = {}", std::is_enum<E>::value);
    fmt::print("is_object<C> = {}", std::is_object<E>::value);
    fmt::print("is_integral<i> = {}", std::is_integral<decltype(i)>::value);

    std::cout << std::is_enum<E>::value << '\n';

}