#ifndef __REGISTER_MENU_ITEMS__
#define __REGISTER_MENU_ITEMS__

#include "example_factory.h"
#include "macro_overload.h"

//template <std::size_t N>
//constexpr std::array<char, N + 1> remove_non_alphanum(const char (&s)[N])
//{
//    std::array<char, N + 1> res { '_' };
//    std::size_t i = 1;
//    for (auto c : s) {
//        res[i++] = ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) ? c : '_';
//    }
//    return res;
//}

// clang-format off
#define INNER_ADD_MENU_ITEM(name, brief, category, description)                                     \
    class name : public IExampleMethod, public RegisteredInFactory<name> {                          \
    public:                                                                                         \
        virtual void Run() override;                                                                \
        static std::unique_ptr<IExampleMethod> CreateMethod() { return std::make_unique<name>(); }  \
        static std::string GetFactoryName() { s_bRegistered; return #name; }                        \
        static std::string GetBrief() { return (brief); }                                           \
        static std::string GetDescription() { return (description); }                               \
        static std::string GetCategory() { return (category); }                                     \
    };                                                                                              \
    void name::Run()
// clang-format on

// macro overloading
#define ADD_MENU_ITEM(...) PP_MACRO_OVERLOAD(ADD_MENU_ITEM, __VA_ARGS__)
#define ADD_MENU_ITEM_1(name) INNER_ADD_MENU_ITEM(name, #name, "", "")
#define ADD_MENU_ITEM_2(name, brief) INNER_ADD_MENU_ITEM(name, brief, "", "")
#define ADD_MENU_ITEM_3(name, brief, category) INNER_ADD_MENU_ITEM(name, brief, category, "")
#define ADD_MENU_ITEM_4(name, brief, category, description) INNER_ADD_MENU_ITEM(name, brief, category, description)

#define UNIQUE_NAME( prefix ) JOIN( prefix, __COUNTER__ )
#define JOIN( symbol1, symbol2 ) _DO_JOIN( symbol1, symbol2 )
#define _DO_JOIN( symbol1, symbol2 ) symbol1##symbol2

#define TEST_ADD_MENU_ITEM_2(brief, category) \
    INNER_ADD_MENU_ITEM(UNIQUE_NAME(GENERATED_EXAMPLE__), brief, category, "")

#endif
