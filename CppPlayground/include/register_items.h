#ifndef __REGISTER_MENU_ITEMS__
#define __REGISTER_MENU_ITEMS__

#include "example_factory.h"
#include "macro_overload.h"

// clang-format off
#define INNER_ADD_MENU_ITEM(name, brief, description,category)                                               \
    class name : public IExampleMethod, public RegisteredInFactory<name> {                          \
    public:                                                                                         \
        virtual void Run() override;                                                                \
        static std::unique_ptr<IExampleMethod> CreateMethod() { return std::make_unique<name>(); }  \
        static std::string GetFactoryName() { s_bRegistered; return #name; }                        \
        static std::string GetBrief() { return (brief); }                                           \
        static std::string GetDescription() { return (description); }                               \
        static std::string GetCategory() { return (category); }                               \
    };                                                                                              \
    void name::Run()
// clang-format on

// macro overloading
#define ADD_MENU_ITEM(...) PP_MACRO_OVERLOAD(ADD_MENU_ITEM, __VA_ARGS__)
#define ADD_MENU_ITEM_1(name) INNER_ADD_MENU_ITEM(name, #name, "", "")
#define ADD_MENU_ITEM_2(name, brief) INNER_ADD_MENU_ITEM(name, brief, "", "")
#define ADD_MENU_ITEM_3(name, brief, description) INNER_ADD_MENU_ITEM(name, brief, description, "")
#define ADD_MENU_ITEM_4(name, brief, description, category) INNER_ADD_MENU_ITEM(name, brief, description, category)

#endif
