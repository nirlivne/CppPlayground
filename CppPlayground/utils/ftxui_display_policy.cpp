#include "ftxui_display_policy.h"
#include "categories.h"

#include <functional> // for function
#include <iostream> // for basic_ostream::operator<<, operator<<, endl, basic_ostream, basic_ostream<>::__ostream_type, cout, ostream
#include <map>
#include <memory> // for allocator, shared_ptr, __shared_ptr_access
#include <string> // for char_traits, to_string, operator+, string, basic_string
#include <vector>

#include "ftxui/component/captured_mouse.hpp" // for ftxui
#include "ftxui/component/component.hpp" // for MenuEntry, Renderer, Vertical
#include "ftxui/component/component_base.hpp" // for ComponentBase
#include "ftxui/component/component_options.hpp" // for MenuEntryOption
#include "ftxui/component/screen_interactive.hpp" // for ScreenInteractive
#include "ftxui/dom/elements.hpp" // for operator|, Element, separator, text, hbox, size, frame, color, vbox, HEIGHT, LESS_THAN, bold, border, inverted
#include "ftxui/screen/color.hpp" // for Color, Color::Blue, Color::Cyan, Color::Green, Color::Red, Color::Yellow

// pimpl idiom
// break dependencies and free the user from
// including the above ftxui stuff
// https://www.fluentcpp.com/2017/09/22/make-pimpl-using-unique_ptr/

class FTXUI_DisplayPolicy::Impl {
public:
    using invokeFn_t = std::function<void(const MenuItem&)>;
    int ActivateMenu(const invokeFn_t& invokeFn);
    void GroupItemsByCategory(const std::vector<MenuItem>& menuItems);
    // const;
    void BuildMenu(const std::vector<MenuItem>& menuItems, const invokeFn_t& invokeFn);

private:
    ftxui::Component BuildMenuComponent();
    ftxui::Component BuildRenderer();

    int selected_item = 0;
    ftxui::ScreenInteractive screen = ftxui::ScreenInteractive::Fullscreen();
    ftxui::Component menu;
    ftxui::Component renderer;
    // int selected_item = 0;
    std::map<std::string, std::vector<MenuItem>> categoryMap;
    std::vector<std::reference_wrapper<const MenuItem>> orderedItems;
};

using namespace ftxui;

// Define a special style for some menu entry.
MenuEntryOption Colored(ftxui::Color c)
{
    MenuEntryOption option;
    option.transform = [c](EntryState state) {
        state.label = (state.active ? "> " : "  ") + state.label;
        Element e = text(state.label) | color(c);
        if (state.focused)
            e = e | inverted;
        if (state.active)
            e = e | bold;
        return e;
    };
    return option;
}

Component Wrap(std::string name, Component component)
{
    return Renderer(component, [name, component] {
        return hbox({
                   //text(name) | size(WIDTH, EQUAL, 20),
                   paragraph(name) | size(WIDTH, EQUAL, 20),
                   separator(),
                   component->Render() | xflex,
               })
            | xflex;
    });
}

std::vector<std::string> Banner()
{
    return {
        R"(   ____              ____  _                                             _ )",
        R"(  / ___| _     _    |  _ \| | __ _ _   _  __ _ _ __ ___  _   _ _ __   __| |)",
        R"( | |   _| |_ _| |_  | |_) | |/ _` | | | |/ _` | '__/ _ \| | | | '_ \ / _` |)",
        R"( | |__|_   _|_   _| |  __/| | (_| | |_| | (_| | | | (_) | |_| | | | | (_| |)",
        R"(  \____||_|   |_|   |_|   |_|\__,_|\__, |\__, |_|  \___/ \__,_|_| |_|\__,_|)",
        R"(                                   |___/ |___/                             )"
    };
}

void FTXUI_DisplayPolicy::Impl::GroupItemsByCategory(const std::vector<MenuItem>& menuItems)
{
    categoryMap.clear();
    orderedItems.clear();

    static MenuItem sep{ "-" };

    // Group by category
    for (const auto& item : menuItems) {
        categoryMap[item.mCategory].push_back(item);
    }

    // Keep items order
    for (const auto& categoryNode : categoryMap) {
        orderedItems.push_back(sep);
        for (const auto& item : categoryNode.second) {
            orderedItems.push_back(item);
        }
    }
}

ftxui::Component FTXUI_DisplayPolicy::Impl::BuildMenuComponent()
{
    size_t index = 1;

    auto menu = Container::Vertical({}, &selected_item);

    // Create menu items grouped by category
    for (const auto& categoryNode : categoryMap) {
        const auto& categoryName = categoryNode.first;

        bool first = true;
        for (const auto& item : categoryNode.second) {
            std::string item_text = /*" " + std::to_string(index) + ") " +*/ item.mText;

            if (first) {
                std::string cat_text = categoryName.empty() ? category_general : categoryName;
                menu->Add(Renderer([] { return separator(); }));
                menu->Add(Wrap(cat_text, MenuEntry(item_text)));
                first = false;
            }
            else {
                menu->Add(Wrap("", MenuEntry(item_text)));
            }

            index++;
        }
    }

    menu->Add(Renderer([] { return separator(); }));
    menu->Add(Wrap("", MenuEntry(" Q) quit", Colored(Color::Red))));

    return menu;
}

ftxui::Component FTXUI_DisplayPolicy::Impl::BuildRenderer()
{
    auto renderer = Renderer(menu, [&] {
        return vbox({
            // Generated using: https://patorjk.com/software/taag
            vbox(
                text(R"(   ____              ____  _                                             _ )"),
                text(R"(  / ___| _     _    |  _ \| | __ _ _   _  __ _ _ __ ___  _   _ _ __   __| |)"),
                text(R"( | |   _| |_ _| |_  | |_) | |/ _` | | | |/ _` | '__/ _ \| | | | '_ \ / _` |)"),
                text(R"( | |__|_   _|_   _| |  __/| | (_| | |_| | (_| | | | (_) | |_| | | | | (_| |)"),
                text(R"(  \____||_|   |_|   |_|   |_|\__,_|\__, |\__, |_|  \___/ \__,_|_| |_|\__,_|)"),
                text(R"(                                   |___/ |___/                             )")) 
            | size(HEIGHT, EQUAL, 7),

            //separator(),
            menu->Render() | frame /* | size(HEIGHT, LESS_THAN, 20)*/,
            })
            | border;
        });

    return renderer;
}

void FTXUI_DisplayPolicy::Impl::BuildMenu(const std::vector<MenuItem>& menuItems, const invokeFn_t& invokeFn)
{
    GroupItemsByCategory(menuItems);

    menu = BuildMenuComponent();
    renderer = BuildRenderer();

    renderer |= CatchEvent([&, invokeFn](Event event) {
        if (event == Event::Character('q')) {
            std::cout << "Quitting...\n\n";
            selected_item = orderedItems.size();
            screen.ExitLoopClosure()();
            return true;
        } else if (event == Event::Return) {
            std::cout << "Running " << selected_item << " ...\n\n ";
            // screen.ExitLoopClosure()();

            if (orderedItems.size() <= selected_item) { // quit
                screen.ExitLoopClosure()();
                return true;
            }

            screen.WithRestoredIO([&] {
                auto& item = orderedItems[selected_item];
                std::cout << "Running option " << selected_item << " - " << item.get().mKey << "\n\n";
                std::cout << std::string(70, '-') << "\n";
                invokeFn(item);
                std::cout << std::string(70, '-') << "\n";
                std::cout << "Hit enter to return to the menu\n";
                std::cout << std::string(70, '-') << "\n";

                std::string input;
                std::getline(std::cin, input);
            })();

            return true;
        }
        return false;
    });
}

int FTXUI_DisplayPolicy::Impl::ActivateMenu(const invokeFn_t& invokeFn)
{
    while (true) {
        screen.Loop(renderer);
        std::cout << std::string(70, '*') << "\n";
        std::cout << "we are out of the menu loop\n";
        std::cout << std::string(70, '*') << "\n";

        std::cout << "Selected element = " << selected_item << std::endl;

        if (selected_item < orderedItems.size()) {
            auto& item = orderedItems[selected_item];
            std::cout << "Running option " << selected_item << " - " << item.get().mKey << "\n\n";
            invokeFn(item);
        } else {
            std::cout << selected_item;
            break;
        }
        int i;
        std::cin >> i;
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

// pimpl idiom
FTXUI_DisplayPolicy::FTXUI_DisplayPolicy() : ui(new FTXUI_DisplayPolicy::Impl) {}
FTXUI_DisplayPolicy::~FTXUI_DisplayPolicy() = default;

void FTXUI_DisplayPolicy::BuildMenu(const std::vector<MenuItem>& menuItems, const invokeFn_t& invokeFn)
{
    ui->BuildMenu(menuItems, invokeFn);
}

int FTXUI_DisplayPolicy::ActivateMenu(const invokeFn_t& invokeFn)
{

    return ui->ActivateMenu(invokeFn);
}
