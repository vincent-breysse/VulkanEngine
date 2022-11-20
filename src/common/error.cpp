
#include <pch.hpp>
#include <gyn/common/error.hpp>

std::string Gyn::ToString(const CodeLocation& location)
{
    const char* layout = 
        "File : {}\n"
        "Function : {}\n"
        "Line : {}\n";

    return fmt::format(layout, location.file, location.function, location.line);
}

std::string Gyn::ToString(const CodeLocation& location, const char* message)
{
    return ToString(location) + message;
}

std::string Gyn::ToString(const CodeLocation& location, const std::string& message)
{
    return ToString(location) + message;
}

std::string ToString(const CodeLocation& location, const ea::string& message)
{
    return ToString(location, message.c_str());
}

void Gyn::Assert(bool condition, const CodeLocation& codeLocation, const char* stringizedTest, const char* message)
{
    if (!condition)
    {
        constexpr const char* Layout =
            "Assertion failed\n"
            "============================================================\n"
            "# [{}] is false\n"
            "# {}\n"
            "# File {}, Line {}\n"
            "# Function {}\n";

        constexpr const char* NullMsgLayout =
            "Assertion failed\n"
            "============================================================\n"
            "# [{}] is false\n"
            "# File {}, Line {}\n"
            "# Function {}\n";

        if (message)
        {
            fmt::print(Layout,
                stringizedTest,
                message,
                codeLocation.file,
                codeLocation.line,
                codeLocation.function);
        }
        else
        {
            fmt::print(NullMsgLayout,
                stringizedTest,
                codeLocation.file,
                codeLocation.line,
                codeLocation.function);
        }

        std::terminate();
    }
}

void Gyn::Assert(bool condition, const CodeLocation& codeLocation, const char* stringizedTest, const std::string& message)
{
    Assert(condition, codeLocation, stringizedTest, message.c_str());
}