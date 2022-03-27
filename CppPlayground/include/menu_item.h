#ifndef __MENU_ITEM_H__
#define __MENU_ITEM_H__

#include <string>

struct MenuItem {
    // string view? references?
    std::string mKey;
    std::string mText;
    std::string mDescription;
    std::string mCategory;
};

#endif