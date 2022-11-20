#pragma once

#include <gyn/common/pch.hpp>
#include <gyn/common/fwd.hpp>
#include <gyn/common/config.hpp>

namespace Gyn
{
    struct CodeLocation
    {
        const char* file;
        const char* function;
        uint32_t line;
    };

    std::string ToString(const CodeLocation& location);

    std::string ToString(const CodeLocation& location, const char* message);

    std::string ToString(const CodeLocation& location, const std::string& message);

    std::string ToString(const CodeLocation& location, const ea::string& message);

    void Assert(bool condition, const CodeLocation& codeLocation, const char* stringizedTest, const char* message);

    void Assert(bool condition, const CodeLocation& codeLocation, const char* stringizedTest, const std::string& message);

#define GYN_CODE_LOCATION() (::Gyn::CodeLocation{ GYN_FILE, GYN_FUNC, GYN_LINE })

#define GYN_ENABLE_ASSERTS GYN_DEBUG
constexpr bool EnableAsserts = GYN_ENABLE_ASSERTS;

#if GYN_ENABLE_ASSERTS
#define GYN_ASSERT_MSG(condition, message) ::Gyn::Assert(!!(condition), GYN_CODE_LOCATION(), #condition, (message))
#else 
#define GYN_ASSERT_MSG(condition, message) (void(0))
#endif

#define GYN_ASSERT(condition) GYN_ASSERT_MSG(condition, nullptr)

#define GYN_THROW_MSG(message) (throw std::runtime_error(::Gyn::ToString(GYN_CODE_LOCATION(), message)))
#define GYN_THROW() GYN_THROW_MSG("")

#define GYN_THIS_CONST ( const_cast<::std::remove_cv_t<::std::remove_pointer_t<decltype(this)>> const*>(this) )

#ifdef GYN_MACROS_CONVENIENCE

#define CODE_LOCATION() GYN_CODE_LOCATION()

#define ASSERT_MSG(condition, message) GYN_ASSERT_MSG(condition, message)
#define ASSERT(condition) GYN_ASSERT(condition)

#define THROW_MSG(message) GYN_THROW_MSG(message)
#define THROW() GYN_THROW()

#define THIS_CONST GYN_THIS_CONST

#endif

}