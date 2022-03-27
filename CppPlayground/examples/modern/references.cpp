#include "register_items.h"
#include <iostream>

ADD_MENU_ITEM(references)
{
    int i1 = 10;
    int i2 = 20;
    bool b = true;

    int& i3 = b ? i1 : i2;
    std::cout << i1 << " " << i2 << " " << i3 << "\n";
    i3 = 100;
    std::cout << i1 << " " << i2 << " " << i3 << "\n";

    // reference_wrapper can be assigned and dereferenced
    std::reference_wrapper rw = i1;
    rw.get() = 1000;
    rw = i2;
    rw.get() = 900;
    std::cout << i1 << " " << i2 << " " << i3 << "\n";
    rw = i3;
    rw.get() = 800;
    std::cout << i1 << " " << i2 << " " << i3 << "\n";
}