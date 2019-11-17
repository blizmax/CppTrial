#pragma once

#include "Core/.Package.h"
#include "Core/String.h"
#include <iostream>

enum class LogLevel
{
    None = 0,
    Error = 1,
    Warning = 2,
    Info = 3,
    Debug = 4
};

class Log
{
public:
    static constexpr LogLevel DEFAULT_LEVEL = LogLevel::Debug;

    Log() = delete;
    Log(const Log &) = delete;
    Log(Log &&) = delete;
    Log &operator=(const Log &) = delete;
    Log &operator=(Log &&) = delete;
    ~Log() = default;

    explicit Log(const String &inTag, LogLevel inLevel = DEFAULT_LEVEL)
        : tag(inTag), level(inLevel)
    {
    }

    void SetLevel(LogLevel newLevel)
    {
        level = newLevel;
    }

    LogLevel GetLevel() const
    {
        return level;
    }

    template <typename... Args>
    void Debug(const String &msg, Args &&... args) const
    {
        if (CompareLevel(level, LogLevel::Debug) >= 0)
        {
            Print(CT_TEXT("D"), msg, std::forward<Args>(args)...);
        }
    }

    template <typename... Args>
    void Info(const String &msg, Args &&... args) const
    {
        if (CompareLevel(level, LogLevel::Info) >= 0)
        {
            Print(CT_TEXT("I"), msg, std::forward<Args>(args)...);
        }
    }

    template <typename... Args>
    void Warning(const String &msg, Args &&... args) const
    {
        if (CompareLevel(level, LogLevel::Warning) >= 0)
        {
            Print(CT_TEXT("W"), msg, std::forward<Args>(args)...);
        }
    }

    template <typename... Args>
    void Error(const String &msg, Args &&... args) const
    {
        if (CompareLevel(level, LogLevel::Error) >= 0)
        {
            Print(CT_TEXT("E"), msg, std::forward<Args>(args)...);
        }
    }

private:
    auto CompareLevel(const LogLevel &lhs, const LogLevel &rhs) const
    {
        return static_cast<std::underlying_type<LogLevel>::type>(lhs) -
               static_cast<std::underlying_type<LogLevel>::type>(rhs);
    }

    template <typename... Args>
    void Print(const String &prefix, const String &msg, Args &&... args) const
    {
        String fmtMsg = String::Format(msg, std::forward<Args>(args)...);
        std::wcout << String::Format(CT_TEXT("<{0}>[{1}] {2}"), prefix, tag, fmtMsg).CStr() << std::endl;
    }

    String tag;
    LogLevel level = DEFAULT_LEVEL;
};