#ifndef __EXAMPLE_FACTORY__
#define __EXAMPLE_FACTORY__

#include <map>
#include <memory>
#include <string>

class IExampleMethod {
public:
    IExampleMethod() = default;
    virtual ~IExampleMethod() = default;
    virtual void Run() = 0;
};

struct ExampleMethodInfo {
    using TCreateMethod = std::unique_ptr<IExampleMethod> (*)();
    TCreateMethod mCreateFunc;

    std::string mBrief;
    std::string mDescription;
    std::string mCategory;
};

class CExampleFactory {
public:
    using MapType = std::map<std::string, ExampleMethodInfo>;

    CExampleFactory() = delete;
    static bool Register(const std::string& name, ExampleMethodInfo exampleInfo);
    static std::unique_ptr<IExampleMethod> Create(const std::string& name);
    // static void Show(const std::string& title = "Main menu", const std::string& prompt = "Enter your choice:");

    // private:
    static MapType & TheMap();
};

template <typename T>
class RegisteredInFactory {
protected:
    static bool s_bRegistered;
};

template <typename T>
bool RegisteredInFactory<T>::s_bRegistered
    = CExampleFactory::Register(T::GetFactoryName(), 
        { T::CreateMethod, T::GetBrief(), T::GetDescription(), T::GetCategory() });

#endif
