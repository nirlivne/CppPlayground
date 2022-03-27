#include "example_factory.h"
#include <algorithm>
#include <cctype>
#include <vector>

// Prevent static order issues between translation units
// And ensure map is initialized before call to Register
CExampleFactory::MapType & CExampleFactory::TheMap()
{
    static std::map<std::string, ExampleMethodInfo> _map;
    return _map;
}

bool CExampleFactory::Register(const std::string& name, ExampleMethodInfo exampleInfo)
{
    if (auto it = TheMap().find(name); it == TheMap().end()) {
        TheMap()[name] = exampleInfo;
        return true;
    }
    return false;
}

std::unique_ptr<IExampleMethod> CExampleFactory::Create(const std::string& name)
{
    if (auto it = TheMap().find(name); it != TheMap().end()) {
        return it->second.mCreateFunc(); // call the createFunc
    }

    return nullptr;
}
