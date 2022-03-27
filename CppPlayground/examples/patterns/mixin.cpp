#include "register_items.h"

#include <iostream>
using namespace std;

// https://stackoverflow.com/questions/18773367/what-are-mixins-as-a-concept

struct Number {
    typedef int value_type;

    int n;
    void set(int v) { n = v; }
    int get() const { return n; }
};

template <typename BASE, typename T = typename BASE::value_type>
struct Undoable : public BASE {
    typedef T value_type;
    T before;
    void set(T v)
    {
        before = BASE::get();
        BASE::set(v);
    }
    void undo() { BASE::set(before); }
};
using UndoableNumber = Undoable<Number>;

template <typename BASE, typename T = typename BASE::value_type>
struct Redoable : public BASE {
    typedef T value_type;
    T after;
    void set(T v)
    {
        after = v;
        BASE::set(v);
    }
    void redo() { BASE::set(after); }
};

using ReUndoableNumber = Redoable<Undoable<Number>>;

ADD_MENU_ITEM(mixin, "Mixin classes example", "", "pattern")
{
    std::cout << "Creating ReUndoableNumber (a mixing of UndoableNumber (a mixing of Number))\n";
    ReUndoableNumber mynum;

    std::cout << "Setting 42, setting 84\n";
    mynum.set(42);
    mynum.set(84);
    std::cout << "get? " << mynum.get() << '\n'; // 84

    std::cout << "undo, ";
    mynum.undo();
    std::cout << "get? " << mynum.get() << '\n'; // 42

    std::cout << "redo, ";
    mynum.redo();
    std::cout << "get? " << mynum.get() << '\n'; // 84
}
