#include <iostream>
#include <map>
#include <memory>
#include <string>

class IExampleMethod {
public:
    IExampleMethod() = default;
    virtual ~IExampleMethod() = default;
    virtual void Run() = 0;
};

class ExampleMethodFactory {
public:
    using TCreateMethod = std::unique_ptr<IExampleMethod> (*)();

public:
    ExampleMethodFactory() = delete;
    static bool Register(const std::string name, TCreateMethod funcCreate);
    static std::unique_ptr<IExampleMethod> Create(const std::string& name);
    static void Print();

private:
    static std::map<std::string, TCreateMethod> s_methods;
};

std::map<std::string, ExampleMethodFactory::TCreateMethod> ExampleMethodFactory::s_methods;

bool ExampleMethodFactory::Register(const std::string name, TCreateMethod funcCreate)
{
    if (auto it = s_methods.find(name); it == s_methods.end()) {
        s_methods[name] = funcCreate;
        return true;
    }
    return false;
}

std::unique_ptr<IExampleMethod>
ExampleMethodFactory::Create(const std::string& name)
{
    if (auto it = s_methods.find(name); it != s_methods.end())
        return it->second(); // call the createFunc

    return nullptr;
}

void ExampleMethodFactory::Print()
{
    for (auto& example : s_methods) {
        std::cout << example.first << std::endl;
    }
}

template <typename T>
class RegisteredInFactory {
protected:
    static bool s_bRegistered;
};

template <typename T>
bool RegisteredInFactory<T>::s_bRegistered = ExampleMethodFactory::Register(T::GetFactoryName(), T::CreateMethod);

// clang-format off
#define EXAMPLE(name)                                                                               \
    class name : public IExampleMethod, public RegisteredInFactory<name> {                          \
    public:                                                                                         \
        virtual void Run() override;                                                                \
        static std::unique_ptr<IExampleMethod> CreateMethod() { return std::make_unique<name>(); }  \
        static std::string GetFactoryName() { s_bRegistered; return #name; }                        \
    };                                                                                              \
    void name::Run()
// clang-format on


EXAMPLE(MyExample1) { std::cout << "Running MyExample1"; }
EXAMPLE(MyExample2) { std::cout << "Running MyExample2"; }
EXAMPLE(MyExample3) { std::cout << "Running MyExample3"; }

int main()
{
    ExampleMethodFactory::Print();
    ExampleMethodFactory::Create("MyExample3")->Run();
}
