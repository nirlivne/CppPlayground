#if __has_include(<boost/range/combine.hpp>)
#include <boost/range/combine.hpp>
#define HAS_BOOSTT
#endif

#include "boost_examples.h"
#include "register_items.h"
#include <format>
#include <iostream>
#include <list>
#include <string>
#include <vector>

#ifdef HAS_BOOST

// Based on https://stackoverflow.com/a/8513803/999986
// Starting from Boost 1.56.0

ADD_MENU_ITEM(
    boost_loop_combine,
    "boost loop combine",
    "",
    "boost")
{
    std::vector<int> ints { 1, 2, 3 };
    double doubles[] = { 1.1, 2.2, 3.3 };
    std::list<std::string> strings { "a", "b", "c" };

    for (const auto& [x, y, z, w] : boost::combine(ints, doubles, strings, ints)) {
        std::cout << std::format("{:d} {:f} {} {:d}\n", x, y, z, w);
    }
}

#endif // HAS_BOOST
