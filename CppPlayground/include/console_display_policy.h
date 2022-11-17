#ifndef __CONSOLE_DISPLAY_POLICY_H__
#define __CONSOLE_DISPLAY_POLICY_H__

#include "menu.h"
#include "menu_item.h"
#include <functional>
#include <string>
#include <vector>

class ConsoleDisplayPolicy {
public:
    using invokeFn_t = std::function<void(const MenuItem &)>;
protected:
    int ActivateMenu(const invokeFn_t & invokeFn) const;
    void BuildMenu(const std::vector<MenuItem>& menuItems, const invokeFn_t & invokeFn);
    ~ConsoleDisplayPolicy() = default;

private:
    void PrintTitle() const;
    void PrintMenu() const;
    void PrintPrompt() const;
    bool HandleUserInput(invokeFn_t invokeFn) const;
    std::string Indent(size_t cnt, char ch = ' ') const;
    
    const char sep_char = '-';
    const size_t indent_width = 25;
    const size_t category_width = 28;
    const size_t category_padding = 2;


public:
    inline void SetTitle(std::string title) { mTitle = std::move(title); }
    inline void SetPrompt(std::string prompt) { mPrompt = std::move(prompt); }

private:
    std::string mTitle{ "Menu" };
    std::string mPrompt;
    std::map<std::string, std::vector<MenuItem>> categoryMap;
    std::vector<std::reference_wrapper<const MenuItem>> orderedItems;

};

/*class FactoryItemsPolicy {
public:
    using categoryName = std::string;
    using menuItemText = std::string;
    using categorizedMenuItems = std::map<categoryName, std::vector<menuItemText>>;

protected:
    std::vector<MenuItem> ListItems() const;
    void InvokeItem(const MenuItem & item) const;
    ~FactoryItemsPolicy() = default;
};

using FactoryConsoleMenu = Menu<FactoryItemsPolicy, ConsoleDisplayPolicy>;
*/
#endif
