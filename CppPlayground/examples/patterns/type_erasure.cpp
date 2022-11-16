#include "register_items.h"
#include "categories.h"
#include <iostream>
#include <memory>
#include <vector>

// Concrete classes
class Circle {
public:
    explicit Circle(double rad)
        : mRadius(rad)
    {
    }

    double getRadius() const noexcept { return mRadius; }
    void setRadius(double rad) noexcept { mRadius = rad; }
    // ... More operations as needed: getCenter ...

private:
    double mRadius;
    // ... More data members as needed
};
class Square {
public:
    explicit Square(double s)
        : mSide(s)
    {
    }

    double getSide() const noexcept { return mSide; }
    void setSide(double side) noexcept { mSide = side; }

private:
    double mSide;
};

// free functions encapsulate the implementation

void draw(const Circle& circle)
{
    std::cout << "Drawing a circle with radius " << circle.getRadius() << std::endl;
}
void serialize(const Circle& circle)
{
    std::cout << "Serializing a circle with radius " << circle.getRadius() << std::endl;
}
void scale(Circle& circle, double scale_factor)
{
    std::cout << "Scaling a circle with radius " << circle.getRadius() << std::endl;
    circle.setRadius(circle.getRadius() * scale_factor);
}
void draw(const Square& square)
{
    std::cout << "Drawing a square with side " << square.getSide() << std::endl;
}
void serialize(const Square& square)
{
    std::cout << "Serializing a square with side " << square.getSide() << std::endl;
}
void scale(Square& square, double scale_factor)
{
    std::cout << "Scaling a square with side " << square.getSide() << std::endl;
    square.setSide(square.getSide() * scale_factor);
}

class Shape;
template <typename T>
struct is_shape : public std::is_same<Shape, std::decay_t<T>> {};

template<typename T>
inline constexpr bool is_shape_v = is_shape<T>::value;

template<typename T>
using enable_if_not_at_shape = std::enable_if_t<!is_shape_v<T>>;

class Shape {
private:
    // #pattern External polymorphism pattern
    // #link https://www.dre.vanderbilt.edu/~schmidt/PDF/C++-EP.pdf

    struct ShapeConcept {
        virtual ~ShapeConcept() = default;

        virtual void draw() const = 0;
        virtual void serialize() const = 0;
        virtual void scale(double scale_factor) = 0;

        // #pattern The prototype design pattern
        virtual std::unique_ptr<ShapeConcept> clone() const = 0;
    };

    template <typename T>
    struct ShapeModel : ShapeConcept {
        ShapeModel(T&& value)
            : object { std::forward<T>(value) }
        {
        }

        // The prototype design pattern
        std::unique_ptr<ShapeConcept> clone() const override
        {
            return std::make_unique<ShapeModel>(*this);
        }

        void draw() const override
        {
            ::draw(object);
        }

        void serialize() const override
        {
            ::serialize(object);
        }

        void scale(double scale_factor) override
        {
            ::scale(object, scale_factor);
        }

        T object;
    };

    friend void serialize(const Shape& shape)
    {
        shape.pimpl->serialize();
    }
    friend void draw(const Shape& shape)
    {
        shape.pimpl->draw();
    }
    friend void scale(const Shape& shape, double scale_factor)
    {
        shape.pimpl->scale(scale_factor);
    }

    // #pattern The bridge design pattern
    std::unique_ptr<ShapeConcept> pimpl;

public:
    // A templated ctor creating a bridge

    //template <typename T, typename = std::enable_if_t<!IsShape<T>::value>>
    //template <typename T, typename = std::enable_if_t<!is_shape_v<T>>>
    template <typename T, typename = enable_if_not_at_shape <T>>
    Shape(T&& x)
        : pimpl { new ShapeModel<T>(std::forward<T>(x)) }
    {
        std::cout << "CTOR\n";
    }

    // Special member functions
    Shape(const Shape& other)
        : pimpl { other.pimpl->clone() }
    {
        std::cout << "Copy CTOR\n";
    }

    Shape& operator=(const Shape& other)
    {
        std::cout << "Assign\n";
        this->pimpl = other.pimpl->clone();
        return *this;
    }
    Shape(Shape&& other)
    {
        std::cout << "Move CTOR\n";
        std::swap(this->pimpl, other.pimpl);
    } //= default;
    Shape& operator=(Shape&& other)
    {
        std::cout << "Move Assign\n";
        std::swap(this->pimpl, other.pimpl);
        return *this;
    } //= default;
};

void drawAllShapes(const std::vector<Shape>& shapes)
{
    for (const auto& shape : shapes) {
        draw(shape);
    }
}

ADD_MENU_ITEM(type_erasure, "Type erasure", category_design)

// ADD_MENU_ITEM(
//     type_erasure,
//     "Type erasure example",
//     "Based on CppCon 2022 talk - type erasure by Klaus ...")
{
    using Shapes = std::vector<Shape>;

    std::cout << "== Creating a list of shapes\n";

    // Creating some shapes
    Shapes shapes;
    shapes.reserve(10); // prevent vector resize and redundant prints
    std::cout << "== Adding 3 shapes\n";
    shapes.emplace_back(Circle { 2.0 });
    shapes.emplace_back(Square { 1.5 });
    shapes.emplace_back(Circle { 4.2 });

    // Drawing all shapes
    std::cout << "== Drawing a list of shapes\n";
    drawAllShapes(shapes);

    // Copying and scaling all shapes
    std::cout << "== Copying a list of shapes\n";
    auto shapes_scaled = shapes; // copy shapes, here clone() is used

    for (auto& shape : shapes_scaled)
        scale(shape, 10);

    std::cout << "== Drawing the copied list of shapes\n";
    drawAllShapes(shapes_scaled); // scaled by factor 10
    std::cout << "== Drawing the original list of shapes\n";
    drawAllShapes(shapes); // left untouched

    std::cout << "== Create new shape\n";
    Shape circle1(Circle { 2.0 });
    draw(circle1);

    // Move
    std::cout << "== Moving the shape\n";
    Shape circle2 { std::move(circle1) };
    draw(circle2);

    // Copy
    std::cout << "== Copying the shape\n";
    Shape circle3 { circle2 };
    draw(circle3);

    // Assign
    std::cout << "== Assigning a shape\n";
    circle1 = circle2;
    draw(circle1);

    // Move Assign
    std::cout << "== Move assigning a shape\n";
    circle1 = std::move(circle3);
    draw(circle1);
}
