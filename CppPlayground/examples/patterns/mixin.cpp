#include "register_items.h"
#include "categories.h"

#include <iostream>
using namespace std;

// https://stackoverflow.com/questions/18773367/what-are-mixins-as-a-concept

struct Number {
    using value_type = int;

    int n;
    void set(int v) { n = v; }
    int get() const { return n; }
};

template <typename BASE, typename T = typename BASE::value_type>
struct Undoable : public BASE {
    using value_type = T;
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
    using value_type = T;

    T after;
    void set(T v)
    {
        after = v;
        BASE::set(v);
    }
    void redo() { BASE::set(after); }
};

using ReUndoableNumber = Redoable<Undoable<Number>>;

void ReUndoableNumberExample()
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

// The result of this mixin is:
//  
//       ,------.
//       |Number|
//       |------|
//       |+get()|
//       |+set()|
//       `------'
//           |
//           |
//   ,--------------.
//   |UndoableNumber|   // using UndoableNumber = Undoable<Number>
//   |--------------|
//   |+undo()       |
//   `--------------'
//           |
//  ,----------------.
//  |ReUndoableNumber|  // using ReUndoableNumber = Redoable<Undoable<Number>>
//  |----------------|
//  |+redo()         |
//  `----------------'
//  
//  


template <typename BASE, typename T = typename BASE::value_type>
struct Printed : public BASE {

    void print() const
    {
        std::cout << "value = " << BASE::get() << "\n";
    }
};


template <typename BASE>
struct RepeatPrint : public BASE {
    explicit RepeatPrint(BASE const& printable) : BASE(printable) {}
    void times(unsigned int n) const
    {
        while (n-- > 0)
        {
            this->print();
            //this->print();

        }
    }
};

struct APrint
{
    void print() const { std::cout << "A\n"; }
};


template<typename Printable>
RepeatPrint<Printable> repeatPrint(Printable const& printable)
{
    return RepeatPrint<Printable>(printable);
}


void RepeatedNumberExample()
{
    Printed<Number> printedNumber;
    printedNumber.set(100);
    printedNumber.print();

    std::cout << "Repeat 10 times\n";
    RepeatPrint(printedNumber).times(10);

    std::cout << "Repeat 4 times\n";
    APrint aprint;
    RepeatPrint(aprint).times(4);
}
// ===================================================================================

ADD_MENU_ITEM(mixin, "Mixin classes example", category_design)
{
    std::cout << "Mixin example 1 - Number / UndoableNumber / ReUndoableNumber\n";
    ReUndoableNumberExample();
    std::cout << "\n\n";
    std::cout << "Mixin example 2 - Number / PrintedNumber / RepeatedPrintedNumber\n";
    RepeatedNumberExample();
}
