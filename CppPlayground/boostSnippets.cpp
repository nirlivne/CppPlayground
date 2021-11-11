#if __has_include(<boost/range/combine.hpp>)
#include <boost/range/combine.hpp>
#define HAS_BOOST
#endif

#include <vector>
#include <list>
#include <string>
#include <iostream>
#include <format>
#include "boostSnippets.h"

void boost_loop_combine() {
#ifndef HAS_BOOST
    std::cout << "boost is missing\n";
#else
    std::vector<int> ints{ 1, 2, 3 };
    double doubles[] = { 1.1, 2.2, 3.3 };
    std::list<std::string> strings{ "a", "b", "c" };

    for (auto [x, y, z, w] : boost::combine(ints, doubles, strings, ints)) 
    {
        std::cout << std::format("{:d} {:f} {} {:d}\n", x, y, z, w);
    }
#endif
}
