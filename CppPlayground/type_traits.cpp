#include "register_items.h"
#include <iostream>
#include <iomanip>
#include <type_traits>
#include <fmt/core.h>

template <typename T, typename U>
struct decay_equiv : std::is_same<typename std::decay<T>::type, U>::type {
};

#define CHECK_EQUIV(T, U)                        \
    std::cout << std::setw(24)                   \
              << "[std::decay(" << (#T) << ")] " \
              << " is_same as "                  \
              << "[" << (#U) << "] ? "           \
              << std::boolalpha << decay_equiv<T, U>::value << '\n';

ADD_MENU_ITEM(type_traits, "Type traits and stuff", "", "Type traits")
{
    std::cout << "-- std::decay\n";
    CHECK_EQUIV(int, int);
    CHECK_EQUIV(int&, int);
    CHECK_EQUIV(int&&, int);
    CHECK_EQUIV(const int&, int);
    CHECK_EQUIV(int[2], int*);
    CHECK_EQUIV(int(int), int (*)(int));
}