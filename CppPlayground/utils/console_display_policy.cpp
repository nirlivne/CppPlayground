#include "console_display_policy.h"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <iomanip>

void ConsoleDisplayPolicy::PrintMenu() const
{
    PrintTitle();

    size_t index = 1;

    // Print menu items grouped by category
    for (const auto& categoryNode : categoryMap) {
        const auto& categoryName = categoryNode.first;
        std::cout
            << Indent(category_padding) << std::setw(category_width - category_padding)
            << (categoryName.empty() ? "General" : categoryName)
            << std::setw(0) << Indent(category_padding) 
            << "|" << Indent(category_padding);

        bool first = true;
        for (const auto& item : categoryNode.second) {
            if (!first) std::cout << Indent(14) << "|  ";
            first = false;
            std::cout << index << ") " << item.mText << "\n";
            index++;
        }
    }

    std::cout << "\nQ) Quit"
        << "\n\n";
}

std::string ConsoleDisplayPolicy::Indent(size_t cnt, char ch) const
{
    return std::string(cnt, ch);
}

void ConsoleDisplayPolicy::PrintTitle() const {
    const auto sep_size = category_width + category_padding;

    if (!mTitle.empty()) {
        std::cout << Indent(sep_size + mTitle.size() * 2 + category_padding, sep_char) << "\n";
        std::cout << Indent(sep_size + category_padding + 1) << mTitle << "\n";
        std::cout << Indent(sep_size, sep_char) << "+" << Indent(mTitle.size() * 2 + category_padding-1, sep_char) << "\n";
    }
}

void ConsoleDisplayPolicy::PrintPrompt() const {
    if (!mPrompt.empty()) {
        std::cout << mPrompt;
    }
}

void ConsoleDisplayPolicy::BuildMenu(const std::vector<MenuItem>& menuItems, const invokeFn_t & invokeFn)
{
    categoryMap.clear();
    orderedItems.clear();

    // Group by category
    for (const auto& item : menuItems) {
        categoryMap[item.mCategory].push_back(item);
    }

    // Keep items order
    for (const auto& categoryNode : categoryMap) {
        for (const auto& item : categoryNode.second) {
            orderedItems.push_back(item);
        }
    }
}

bool ConsoleDisplayPolicy::HandleUserInput(invokeFn_t invokeFn) const
{
    while (true) {

        PrintPrompt();

        std::string input;
        getline(std::cin, input);

        std::transform(input.begin(), input.end(), input.begin(), ::toupper);

        if (input == "Q") {
            return false;
            //should_quit = true;
            //break;
        }

        try {
            auto choice = std::stoul(input);
            if (choice > 0 && choice <= orderedItems.size()) {
                auto& item = orderedItems[choice - 1];
                std::cout << "Running option " << choice << " - " << item.get().mKey << "\n\n";
                invokeFn(item);
                // CExampleFactory::Create(name)->Run();
                std::cout << "\n";
                return true;
            }
        }
        catch (const std::exception&) {
            // invalid_argument
            // out_of_range
        }

        std::cout << "Unknown choice, try again..\n";
    }
}

int ConsoleDisplayPolicy::ActivateMenu(const invokeFn_t & invokeFn) const
{
    bool should_continue = true;

    do {
        PrintMenu();
        should_continue = HandleUserInput(invokeFn);

    } while (should_continue);

    return 0;
}
