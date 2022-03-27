#include "examples/boost/boost_examples.h"
#include "examples/opencv/opencv_examples.h"
#include "register_items.h"
#include <iostream>

// menu 
#include "menu.h"
#include "ftxui_display_policy.h"
#include "console_display_policy.h"
#include "factory_items_policy.h"



// Example I plan to add
//ADD_MENU_ITEM(mem1, "Smart pointers", "", "memory") {}
//ADD_MENU_ITEM(thr1, "Atomics and locks", "", "threads") {}
//ADD_MENU_ITEM(thr2, "conditional variable", "", "threads") {}
//ADD_MENU_ITEM(thr3, "promise and future", "", "threads") {};
//ADD_MENU_ITEM(lmb, "Everything about lambdas", "", "Modern C++") {};

int main()
{
    using ConsoleExamplesMenu = Menu<FactoryItemsPolicy, ConsoleDisplayPolicy>;
    using FTXUIExamplesMenu = Menu<FactoryItemsPolicy, FTXUI_DisplayPolicy>;

    FTXUIExamplesMenu menu;

    //ConsoleExamplesMenu menu;
    //menu.SetTitle("C++ Examples");
    //menu.SetPrompt("Which example whould you like to run? ");

    auto result = menu.Show();
}
