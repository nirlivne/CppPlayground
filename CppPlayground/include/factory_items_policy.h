#ifndef __FACTORY_ITEMS_POLICY_H__
#define __FACTORY_ITEMS_POLICY_H__

#include "menu_item.h"
#include <string>
#include <vector>

class FactoryItemsPolicy {
public:
    using categoryName = std::string;
    using menuItemText = std::string;
    //using categorizedMenuItems = std::map<categoryName, std::vector<menuItemText>>;

protected:
    std::vector<MenuItem> ListItems() const;
    void InvokeItem(const MenuItem& item) const;
    ~FactoryItemsPolicy() = default;
};

//using FactoryConsoleMenu = Menu<FactoryItemsPolicy, ConsoleDisplayPolicy>;

#endif
