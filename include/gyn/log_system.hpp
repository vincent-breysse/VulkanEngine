#pragma once

#include <gyn/common.hpp>

namespace Gyn
{
    enum class LogType
    {
        Null,
        Info,
        Error,
    };

    struct Log
    {
        LogType type;
        std::string str;
    };

    class LogSystem
    {
    public:

        static void Init();

        static void LogInfo(const char* str);
        static void LogInfo(const std::string& str);

        static void LogError(const char* str);
        static void LogError(const std::string& str);

        static ea::span<const Log> GetLogs();

        static void Deinit();
    };
}