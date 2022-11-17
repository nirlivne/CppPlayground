#include "register_items.h"
#include "categories.h"

// Based on:
// https://akrzemi1.wordpress.com/2017/07/12/your-own-error-code/
// https://akrzemi1.wordpress.com/2017/08/12/your-own-error-condition/
// https://akrzemi1.wordpress.com/2017/09/04/using-error-codes-effectively/


//////////////////////////////////////////////////////////////////////////////
// module1.h
//////////////////////////////////////////////////////////////////////////////

# include <system_error>

enum class EErrorModule1
{
    // no 0
    Success = 0,
    SomeError1 = 1,
    ConnectionError = 30,
    ResourceError,
    Timeout,
};

namespace std
{
    template <>
    struct is_error_code_enum<EErrorModule1> : true_type {};
}

std::error_code make_error_code(EErrorModule1);

//////////////////////////////////////////////////////////////////////////////
// module1.cpp
//////////////////////////////////////////////////////////////////////////////

//# include "module1.h"

namespace { // anonymous namespace

    struct ErrCategoryModule1 : std::error_category
    {
        const char* name() const noexcept override;
        std::string message(int ev) const override;
    };

    const char* ErrCategoryModule1::name() const noexcept
    {
        return "module1";
    }

    std::string ErrCategoryModule1::message(int ev) const
    {
        switch (static_cast<EErrorModule1>(ev))
        {
        case EErrorModule1::SomeError1:       return "some error 1";

        case EErrorModule1::ConnectionError:  return "could not connect to server";
        case EErrorModule1::ResourceError:    return "insufficient resources";
        case EErrorModule1::Timeout:          return "processing timed out";

        default:
            return "(unrecognized error)";
        }
    }

    const ErrCategoryModule1 theModule1ErrCategory{};

} // anonymous namespace

std::error_code make_error_code(EErrorModule1 e)
{
    return { static_cast<int>(e), theModule1ErrCategory };
}

//////////////////////////////////////////////////////////////////////////////
// module2.h
//////////////////////////////////////////////////////////////////////////////

# include <system_error>

enum class EErrorModule2
{
    // no 0
    Success     = 0,
    SomeError1  = 1,
    FileError   = 20,
    DeviceError = 30,
    Timeout     = 40,
};

namespace std
{
    template <>
    struct is_error_code_enum<EErrorModule2> : true_type {};
}

std::error_code make_error_code(EErrorModule2);

//////////////////////////////////////////////////////////////////////////////
// module2.cpp
//////////////////////////////////////////////////////////////////////////////

// # include "module2.h"

namespace { // anonymous namespace

    struct ErrCategoryModule2 : std::error_category
    {
        const char* name() const noexcept override;
        std::string message(int ev) const override;
    };

    const char* ErrCategoryModule2::name() const noexcept
    {
        return "module2";
    }

    std::string ErrCategoryModule2::message(int ev) const
    {
        switch (static_cast<EErrorModule2>(ev))
        {
        case EErrorModule2::SomeError1:  return "some error 1";

        case EErrorModule2::FileError:   return "could not open file";
        case EErrorModule2::DeviceError: return "device error";
        case EErrorModule2::Timeout:     return "processing timed out";

        default:
            return "(unrecognized error)";
        }
    }

    const ErrCategoryModule2 theModule2ErrCategory{};

} // anonymous namespace

std::error_code make_error_code(EErrorModule2 e)
{
    return { static_cast<int>(e), theModule2ErrCategory };
}


//////////////////////////////////////////////////////////////////////////////
// fail_source.h
//////////////////////////////////////////////////////////////////////////////

//#include "module1.h"
//#include "module2.h"

enum class FailureSource
{
    // no 0
    FileSystemError = 1,
    DeviceError,
    NetworkError,
    ResourceError,
    Timeout,
};

namespace std
{
    template <>
    struct is_error_condition_enum<FailureSource> : true_type {};
}

std::error_condition make_error_condition(FailureSource e);

//////////////////////////////////////////////////////////////////////////////
// fail_source.cpp
//////////////////////////////////////////////////////////////////////////////

//#include "fail_source.h"

namespace {

    class FailureSourceCategory : public std::error_category
    {
    public:
        const char* name() const noexcept override;
        std::string message(int ev) const override;
        bool equivalent(
            const std::error_code& code,
            int condition) const noexcept override;
    };

    const char* FailureSourceCategory::name() const noexcept
    {
        return "failure-source";
    }

    std::string FailureSourceCategory::message(int ev) const
    {
        switch (static_cast<FailureSource>(ev))
        {
        case FailureSource::FileSystemError: return "file system error";
        case FailureSource::DeviceError:     return "device error";
        case FailureSource::ResourceError:   return "resource error";
        case FailureSource::Timeout:         return "timeout error";

        default: return "(unrecognized condition)";
        }
    }

    bool FailureSourceCategory::equivalent(
        const std::error_code& ec,
        int cond) const noexcept
    {
        const std::error_category& Module1Cat = std::error_code{ EErrorModule1{} }.category();
        const std::error_category& Module2Cat = std::error_code{ EErrorModule2{} }.category();

        switch (static_cast<FailureSource>(cond))
        {
        case FailureSource::FileSystemError: 
            return (ec == EErrorModule2::FileError);

        case FailureSource::ResourceError: 
            return (ec == EErrorModule1::ResourceError);

        case FailureSource::DeviceError:
            return (ec == EErrorModule2::DeviceError);

        case FailureSource::Timeout:
            if (ec == EErrorModule1::Timeout ||
                ec == EErrorModule2::Timeout)
                return true;

        default:
            return false;
        }
    }

    const FailureSourceCategory theFailureSourceCategory{};

} // anonymous namespace

std::error_condition make_error_condition(FailureSource e)
{
    return { static_cast<int>(e), theFailureSourceCategory };
}

//////////////////////////////////////////////////////////////////////////////
// main.cpp
//////////////////////////////////////////////////////////////////////////////

# include <cassert>
# include <iostream>
//#include "module1.h"
//#include "module2.h"
//#include "fail_source.h"

#define TRUE_FALSE(cond) #cond << " " << ((cond) ? "true" : "false") << "\n"
#define EXPR_VALUE(expr) "" << #expr << ": " << expr << "\n"
#define FORMAT_EC(ec) "--------------------------------------\n" \
                   << EXPR_VALUE(ec) \
                   << EXPR_VALUE(ec.value()) \
                   << EXPR_VALUE(ec.message()) \
                   << EXPR_VALUE(ec.category().name()) \
                   << "---------------------------------------\n\n"

ADD_MENU_ITEM(std_error_code, "std::error_code usage", category_core_cpp) 
{
    // Some error in module1
    std::error_code ec = EErrorModule1::Success;
    std::cout << "std::error_code ec = EErrorModule1::Success\n";
    std::cout << TRUE_FALSE(ec);
    std::cout << FORMAT_EC(ec);

    std::error_code ec1 = EErrorModule1::SomeError1;

    std::cout << "std::error_code ec1 = EErrorModule1::SomeError1\n";
    std::cout << TRUE_FALSE(ec1);
    std::cout << TRUE_FALSE(ec1 == EErrorModule1::SomeError1);
    std::cout << TRUE_FALSE(ec1 != EErrorModule2::SomeError1);
    std::cout << FORMAT_EC(ec1);

    // Some error in module2
    std::error_code ec2 = EErrorModule2::Timeout;
    std::cout << "std::error_code ec2 = EErrorModule2::Timeout;\n";
    std::cout << TRUE_FALSE(ec2);
    std::cout << TRUE_FALSE(ec2 == EErrorModule2::Timeout);
    std::cout << TRUE_FALSE(ec2 != EErrorModule1::Timeout);
    std::cout << FORMAT_EC(ec2);

    // Error condition

    std::error_code ec3 = EErrorModule1::Timeout;
    std::cout << "std::error_code ec3 = EErrorModule1::Timeout\n";
    std::cout << TRUE_FALSE(ec3 == FailureSource::Timeout);
    std::cout << TRUE_FALSE(ec3 == EErrorModule1::Timeout);
    std::cout << TRUE_FALSE(ec3 != EErrorModule2::Timeout);
    std::cout << FORMAT_EC(ec3);
}