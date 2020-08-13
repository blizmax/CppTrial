#pragma once

#include "Core/Delegate.h"
#include "Core/Thread.h"
#include "Core/Time.h"
#include "Utils/.Package.h"

class Profiler
{
public:
    struct SessionData
    {
        String name;
        int64 startTime;
        float elapsedTime;
    };

    struct ScopeData
    {
        String name;
        int64 startTime;
        float elapsedTime;
    };

    class Scope
    {
    public:
        Scope(const String &name, Profiler &profiler)
            : profiler(profiler)
        {
            data.name = name;
            data.startTime = Time::NanoTime();

            profiler.scopeBeginEventHandler(data);
        }

        ~Scope()
        {
            data.elapsedTime = (Time::NanoTime() - data.startTime) / 1000000.0f;

            profiler.scopeEndEventHandler(data);
        }

    private:
        Profiler &profiler;
        ScopeData data;
    };

public:
    Delegate<void(const SessionData &)> sessionBeginEventHandler;
    Delegate<void(const SessionData &)> sessionEndEventHandler;
    Delegate<void(const ScopeData &)> scopeBeginEventHandler;
    Delegate<void(const ScopeData &)> scopeEndEventHandler;

    void BeginSession(const String &name)
    {
        std::unique_lock<std::shared_mutex> lock(mutex);

        BeginSessionUnlocked(name);
    }

    void EndSession()
    {
        std::unique_lock<std::shared_mutex> lock(mutex);

        EndSessionUnlocked();
    }

    Scope NewScope(const String &name)
    {
        std::unique_lock<std::shared_mutex> lock(mutex);

        if (!sessionOpen)
        {
            CT_LOG(Warning, CT_TEXT("No session open, begin a temp session."));
            BeginSessionUnlocked(CT_TEXT("Temp"));
        }

        lock.unlock();

        return Scope(name, *this);
    }

    bool IsSessionOpen() const
    {
        std::shared_lock<std::shared_mutex> lock(mutex);

        return sessionOpen;
    }

    SessionData GetSessionData() const
    {
        std::shared_lock<std::shared_mutex> lock(mutex);

        return sessionData;
    }

    static Profiler &GetGlobalProfiler()
    {
        return sGlobal;
    }

private:
    void BeginSessionUnlocked(const String &name)
    {
        EndSessionUnlocked();

        sessionOpen = true;

        sessionData.name = name;
        sessionData.startTime = Time::NanoTime();
        sessionData.elapsedTime = 0.0f;

        sessionBeginEventHandler(sessionData);
    }

    void EndSessionUnlocked()
    {
        if (sessionOpen)
        {
            sessionOpen = false;

            sessionData.elapsedTime = (Time::NanoTime() - sessionData.startTime) / 1000000.0f;

            sessionEndEventHandler(sessionData);
        }
    }

private:
    static Profiler sGlobal;

    bool sessionOpen = false;
    mutable std::shared_mutex mutex;
    SessionData sessionData;
};

inline Profiler Profiler::sGlobal;

#define CT_PROFILE_SESSION_BEGIN(name) ::Profiler::GetGlobalProfiler().BeginSession(name)
#define CT_PROFILE_SESSION_END() ::Profiler::GetGlobalProfiler().EndSession()
#define CT_PROFILE_SCOPE(name) auto scope##__LINE__ = ::Profiler::GetGlobalProfiler().NewScope(name)
#define CT_PROFILE_FUNCTION() CT_PROFILE_SCOPE(__func__)