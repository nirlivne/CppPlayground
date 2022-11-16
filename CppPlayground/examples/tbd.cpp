#include "register_items.h"
#include "categories.h""
#include <fmt/core.h>

void not_yet_implemented()
{
    fmt::print("This example not yet implemented\n");
}


// Example I plan to add

ADD_MENU_ITEM(tbdmem1, "Memory and smart pointers", category_stl) { not_yet_implemented();  }
ADD_MENU_ITEM(tbdthr1, "Threads, Atomics and locks", category_stl) { not_yet_implemented(); }
ADD_MENU_ITEM(tbdthr2, "Threads, conditional variable", category_stl) { not_yet_implemented(); }
ADD_MENU_ITEM(tbdthr3, "Threads, promise and future", category_stl) { not_yet_implemented(); };

ADD_MENU_ITEM(tbdlmb, "Everything about lambdas", category_core_cpp) { not_yet_implemented(); };

ADD_MENU_ITEM(lib1, "{fmt}", category_libraries) { not_yet_implemented(); };
ADD_MENU_ITEM(lib2, "ftxui", category_libraries) { not_yet_implemented(); };
