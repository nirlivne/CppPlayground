#ifndef __MENU_H__
#define __MENU_H__

#include "example_factory.h"

// NOTE: public inheritance allow enriched policies (e.g. SetTitle)
//       private inheritance prevent Destructors of Policy Classes
//       See section 1.7 in Andrei Alexandrescu book, Modern C++ design

template <typename ItemsPolicy, typename DisplayPolicy>
class Menu : public ItemsPolicy, public DisplayPolicy {
public:
    int Show()
    {
        auto invoker = [this](const auto& item) { InvokeItem(item); };
        BuildMenu(ListItems(), invoker);
        return ActivateMenu(invoker);
    }

private:
    using DisplayPolicy::BuildMenu;
    using DisplayPolicy::ActivateMenu;
    using ItemsPolicy::InvokeItem;
    using ItemsPolicy::ListItems;
};

#endif
