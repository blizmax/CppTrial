#pragma once

#include "Core/.Package.h"
#include "Core/String.h"
#include "Core/Time.h"
#include "Core/Delegate.h"
#include <iostream>

enum class LogLevel
{
    None = 0,
    Debug = 1,
    Info = 2,
    Warning = 3,
    Error = 4,
    Fatal = 5,
};

class Logger
{
public:
    static constexpr LogLevel DEFAULT_LEVEL = LogLevel::Debug;

    Logger() = delete;
    Logger(const Logger &) = delete;
    Logger(Logger &&) = delete;
    Logger &operator=(const Logger &) = delete;
    Logger &operator=(Logger &&) = delete;
    ~Logger() = default;

    explicit Logger(const String &tag, LogLevel lv = DEFAULT_LEVEL)
        : tag(tag), level(lv)
    {
        printHandler.On([](LogLevel l, const String &s) {
            std::wcout << s.CStr() << std::endl;
        });
    }

    void SetLevel(LogLevel lv)
    {
        level = lv;
    }

    LogLevel GetLevel() const
    {
        return level;
    }

    template <typename... Args>
    void Debug(const String &msg, Args &&... args) const
    {
        Log(LogLevel::Debug, msg, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void Info(const String &msg, Args &&... args) const
    {
        Log(LogLevel::Info, msg, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void Warning(const String &msg, Args &&... args) const
    {
        Log(LogLevel::Warning, msg, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void Error(const String &msg, Args &&... args) const
    {
        Log(LogLevel::Error, msg, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void Log(LogLevel lv, const String &msg, Args &&... args) const
    {
        if (CompareLevel(level, lv) <= 0)
        {
            Print(lv, msg, std::forward<Args>(args)...);
        }
    }

    static Logger &GetGlobalLogger()
    {
        return gLogger;
    }

private:
    auto CompareLevel(const LogLevel &lhs, const LogLevel &rhs) const
    {
        return static_cast<std::underlying_type<LogLevel>::type>(lhs) -
               static_cast<std::underlying_type<LogLevel>::type>(rhs);
    }

    template <typename... Args>
    void Print(const LogLevel &lv, const String &msg, Args &&... args) const
    {
        String prefix;
        switch (lv)
        {
        case LogLevel::Debug:
            prefix = CT_TEXT("D");
            break;
        case LogLevel::Info:
            prefix = CT_TEXT("I");
            break;
        case LogLevel::Warning:
            prefix = CT_TEXT("W");
            break;
        case LogLevel::Error:
            prefix = CT_TEXT("E");
            break;
        case LogLevel::Fatal:
            prefix = CT_TEXT("F");
            break;
        }

        String fmtMsg = String::Format(msg, std::forward<Args>(args)...);
        String fmtTime = Time::ToString(CT_TEXT("%Y-%m-%d %H:%M:%S"));

        printHandler(lv, String::Format(CT_TEXT("[{0}] <{1}>[{2}] {3}"), fmtTime, prefix, tag, fmtMsg));
    }

public:
    mutable Delegate<void(LogLevel, const String &)> printHandler;

private:
    static Logger gLogger;

    String tag;
    LogLevel level = DEFAULT_LEVEL;
};

inline Logger Logger::gLogger(CT_TEXT("Global"));

#define CT_LOG(lv, ...) Logger::GetGlobalLogger().Log(LogLevel::lv, __VA_ARGS__)
