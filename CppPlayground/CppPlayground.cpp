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

#ifdef USE_FTXUI_MENU
    using FTXUIExamplesMenu = Menu<FactoryItemsPolicy, FTXUI_DisplayPolicy>;
    FTXUIExamplesMenu menu;
#else
    using ConsoleExamplesMenu = Menu<FactoryItemsPolicy, ConsoleDisplayPolicy>;
    ConsoleExamplesMenu menu;
    menu.SetTitle("C++ Examples");
    menu.SetPrompt("Which example would you like to run? ");
#endif

    auto result = menu.Show();
}
