
#include <pch.hpp>
#include <gyn/log_system.hpp>

namespace
{
    struct LogSystemData
    {
        ea::vector<Log> logs;
    };
}

static UniquePtr<LogSystemData> sLogSystem = nullptr;

void LogSystem::Init() 
{
    sLogSystem = NewUnique<LogSystemData>();
}

void LogSystem::LogInfo(const char* str)
{
    LogInfo(std::string(str));
}

void LogSystem::LogInfo(const std::string& str)
{
    Log log = {};
    log.str = str;
    log.type = LogType::Info;
    sLogSystem->logs.push_back(std::move(log));
}

void LogSystem::LogError(const char* str)
{
    LogError(std::string(str));
}

void LogSystem::LogError(const std::string& str)
{
    Log log = {};
    log.str = str;
    log.type = LogType::Error;
    sLogSystem->logs.push_back(std::move(log));
}

ea::span<const Log> LogSystem::GetLogs()
{
    return sLogSystem->logs;
}

void LogSystem::Deinit()
{
    sLogSystem = nullptr;
}