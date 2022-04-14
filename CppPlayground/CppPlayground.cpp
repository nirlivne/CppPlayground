#include "examples/boost/boost_examples.h"
#include "examples/opencv/opencv_examples.h"
#include "register_items.h"
#include <iostream>

// menu 
#include "menu.h"
#include "ftxui_display_policy.h"
#include "console_display_policy.h"
#include "factory_items_policy.h"

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
