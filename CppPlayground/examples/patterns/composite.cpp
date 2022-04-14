#include "register_items.h"
#include "categories.h"

#include <iostream>
#include <string>
#include <vector>
#include <memory>

//////////////////////////////////////////////////////////////////////////
// Composite is part of Structural Patterns 
// Structural Patterns deal with decoupling the interface and implementation 
// of classes and objects
// Composite pattern forms a tree structure of simple and composite objects 
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Pay attention that the Liskov substitution priciple is violated here (L in SOLID) 
// The GoF book specifically addresses this issue on page 167.
// 
// Although the Composite class implements the Add and Remove operations for 
// managing children, an important issue in the Composite pattern is which 
// classes declare these operations... Should we declare these operations in 
// the Component and make them meaningful for Leaf classes, or should we 
// declare and define them only in Composite ?
// 
// The decision involves a trade - off between safety and transparency :
// 
//     Defining the child management interface at the root of the class 
//     hierarchy gives you transparency, because you can treat all components 
//     uniformly.It costs you safety, however, because clients may try to do 
//     meaningless things like add and remove objects from leaves.
//     Defining child management in the Composite class gives you safety, 
//     because any attempt to add or remove objects from leaves will be 
//     caught at compile - time in a statically typed language like C++.
//     But you lose transparency, because leaves and composites have different 
//     interfaces.
//     We have emphasized transparency over safety in this pattern.
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Plantuml diagram
// www.plantuml.com/plantuml/png/jP9VIiD04CRVznJpPggcwdi9XVeY51Jg1IRDf5dOx8RTQQMeq8FevNeI9t_QRQduPWY4-RxP_Bv9PXeOFQzhevO6Gu2PqMHP7IfNDywA0EyAvBe1rv17riuceq71eaYEVGVHK-qsTAclY6ULDaMYM-tpZ1CqqpzpveJb7uX9dfrJ9dTv5hq8-zMEpC4IhdLCa3jcLuChExxgKsXRaTSyC6-7IG1jSGpVl_wrqW7aHYWew9MLnr87YGrS8SD6EuDCCjlY1PB5y_pfSTGUejZNU0fa6H3OOq4If8IS-8t8Gi2IUDjXnRIXGKzskLKaGXjUgIlmPBhl55INF5qtwKTsiXkgNnf8lLvLVEAe-5_ZeGiBzhkl-_7uUh_x1fUcMLi_j4LidaeSsT4V
//
// class Client
// 
//     class Component {
//     +operation()
//         + add(Component)
//         + remove(Component)
//         + getChild(int)
// }
// 
// class Composite {
//     +operation()
//         + add(Component)
//         + remove(Component)
//         + getChild(int)
// }
// 
// class Leaf {
//     +operation()
//         + <b>add(Component) < / b >
//         +<b>remove(Component) < / b >
//         +<b>getChild(int) < / b >
// }
// note bottom of Leaf
// Leaf inherit <b>add, removeand getChild< / b>
// This is a design decision that violates LSP(SOLID)
// The decision present a trade off between safetyand transparency
// end note
// 
// ' relations
// Leaf - up - | > Component
// Client - right > Component
// Composite - up - | > Component
// Composite  “0..*” o-- > “1” Component
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//The 'Component' Treenode
//////////////////////////////////////////////////////////////////////////

class Component
{
public:
    Component() = delete; // not allowed
    Component(std::string name) : mName(std::move(name)) {};

    // public interface 
    virtual void Add(std::unique_ptr<Component> d) = 0;
    virtual void Remove(Component* d) = 0;
    virtual void Display(int indent) = 0;

    std::string getName() { return mName; };
    virtual ~Component() {};

private:
    std::string mName;
};

//////////////////////////////////////////////////////////////////////////
// The 'Leaf' class
//////////////////////////////////////////////////////////////////////////

class Leaf : public Component
{
public:
    Leaf() = delete;
    Leaf(std::string name);
    void Display(int indent);

    // ** Liskov substitution principle violation
    // ** We have a trade off here - transparency over security

    void Add(std::unique_ptr<Component> d);  // should not use
    void Remove(Component* d);               // should not use

    virtual ~Leaf();;
};


//////////////////////////////////////////////////////////////////////////
// The 'Composite' class
//////////////////////////////////////////////////////////////////////////

class Composite : public Component
{
public:
    Composite() = delete; //not allowed

    Composite(std::string name);;

    void Add(std::unique_ptr<Component> d);
    void Remove(Component* d);
    void Display(int indent);
    virtual ~Composite();
private:
    std::vector<std::unique_ptr<Component>> elements_;
};


//////////////////////////////////////////////////////////////////////////
// Leaf implementation
//////////////////////////////////////////////////////////////////////////

Leaf::Leaf(std::string name) : Component(name)
{
    std::cout << "Leaf::ctor - " << name << " created" << std::endl;
}

void Leaf::Add(std::unique_ptr<Component> d)
{
    std::cout << "ERROR: Cannot add to a PrimitiveElement" << std::endl;
}

void Leaf::Remove(Component* d)
{
    std::cout << "ERROR: Cannot remove from a PrimitiveElement" << std::endl;
}

void Leaf::Display(int indent)
{
    std::string newStr(indent, '-');
    std::cout << newStr << " " << getName() << std::endl;
}

Leaf::~Leaf()
{

    std::cout << "Leaf::dtor - " << getName() << " destroyed" << std::endl;
}

//////////////////////////////////////////////////////////////////////////
// Composite implementation
//////////////////////////////////////////////////////////////////////////

Composite::Composite(std::string name) : Component(name)
{

    std::cout << "Composite::ctor - " << name << " created" << std::endl;
}

void Composite::Add(std::unique_ptr<Component> d)
{
    std::cout << "Component added to " << getName() << std::endl;
    elements_.push_back(std::move(d));
}

void Composite::Remove(Component* d)
{
    auto it = std::begin(elements_);
    while (it != elements_.end())
    {
        if (it->get() == d)
        {
            //delete d;
            elements_.erase(it);
            break;
        }
        ++it;
    }
}

void Composite::Display(int indent)
{
    std::string newStr(indent, '-');
    std::cout << newStr << "+ " << getName() << std::endl;
    auto it = elements_.begin();
    while (it != elements_.end())
    {
        (*it)->Display(indent + 2);
        ++it;
    }
}

Composite::~Composite()
{
    while (!elements_.empty())
    {
        auto it = elements_.begin();
        //delete *it;
        elements_.erase(it);
    }
    std::cout << "Composite::dtor - " << getName() << " destroyed" << std::endl;
}

//////////////////////////////////////////////////////////////////////////
//The Main method
//////////////////////////////////////////////////////////////////////////

ADD_MENU_ITEM(composite_pattern, "Composite design pattern", category_design)
{
    std::cout << "==== Creating composite tree ====\n\n";
    //Create a Tree Structure
    Composite soup("Soup");
    soup.Add(std::make_unique<Leaf>("Potato"));
    soup.Add(std::make_unique<Leaf>("Fresh Tomato"));
    soup.Add(std::make_unique<Leaf>("Carrot"));

    //Create a Branch
    auto tomatoSauce = std::make_unique<Composite>("Tomato Sauce");
    tomatoSauce->Add(std::make_unique<Leaf>("Tomato"));
    tomatoSauce->Add(std::make_unique<Leaf>("Garlic"));
    tomatoSauce->Add(std::make_unique<Leaf>("Olive oil"));
    soup.Add(std::move(tomatoSauce));

    //Add and remove a primitive elements
    auto salt = std::make_unique<Leaf>("Salt");
    soup.Add(std::move(salt));
    auto pepper = std::make_unique<Leaf>("Pepper");
    soup.Add(std::move(pepper));
    soup.Remove(salt.get());

    //Recursively display nodes
    std::cout << "\n\n==== Display composite tree ====\n\n";
    soup.Display(1);

    std::cout << "\n\n==== Done with composite tree ====\n\n";
}