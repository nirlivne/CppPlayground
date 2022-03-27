#ifndef __FTXUI_DISPLAY_POLICY_H__
#define __FTXUI_DISPLAY_POLICY_H__

#include "menu_item.h"
#include <functional>


class FTXUI_DisplayPolicy {
public:
    using invokeFn_t = std::function<void(const MenuItem&)>;

protected:
    int ActivateMenu(const invokeFn_t& invokeFn); // const;
    void BuildMenu(const std::vector<MenuItem>& menuItems, const invokeFn_t& invokeFn);

    FTXUI_DisplayPolicy();
    ~FTXUI_DisplayPolicy();

private:
    class Impl;
    std::unique_ptr<Impl> ui;
};



#endif