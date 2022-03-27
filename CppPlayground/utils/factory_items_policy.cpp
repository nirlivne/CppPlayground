#include "factory_items_policy.h"
#include "example_factory.h"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <iomanip>

std::vector<MenuItem> FactoryItemsPolicy::ListItems() const
{
    std::vector<MenuItem> items;

    for (const auto& node : CExampleFactory::TheMap()) {
        items.emplace_back(
            node.first,
            node.second.mBrief,
            node.second.mDescription,
            node.second.mCategory);
    }

    return items;
}

void FactoryItemsPolicy::InvokeItem(const MenuItem& item) const
{
    auto it = CExampleFactory::TheMap().find(item.mKey);
    if (it != std::end(CExampleFactory::TheMap())) {
        it->second.mCreateFunc()->Run();
    }
    // else { runtime_error ... }
}
