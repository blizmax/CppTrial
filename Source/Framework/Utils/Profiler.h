#pragma once

#include "Core/Delegate.h"
#include "Core/HashMap.h"
#include "Core/Thread.h"
#include "Core/Time.h"
#include "Utils/.Package.h"

struct ProfileEntry
{
    String name;
    int32 callNum = 0;
    float elapsedMs = 0.0f;

    Array<ProfileEntry> children;
};

struct ProfileEntryCollection
{
    int32 GetDepth() const
    {
        return callStack.Count();
    }

    int32 GetIndex() const
    {
        return callStack.Last();
    }

    ProfileEntry &GetCurrent()
    {
        ProfileEntry *e = &root;
        for (int32 d = 0; d < GetDepth(); ++d)
        {
            e = &(e->children[callStack[d]]);
        }
        return *e;
    }

    ProfileEntry &GetParent()
    {
        ProfileEntry *e = &root;
        for (int32 d = 0; d < GetDepth() - 1; ++d)
        {
            e = &(e->children[callStack[d]]);
        }
        return *e;
    }

    void AddEntry(const ProfileEntry &entry)
    {
        ProfileEntry &p = GetParent();
        p.children.Add(entry);
    }

    void Push(int32 index)
    {
        ProfileEntry &p = GetParent();
        CT_CHECK(index < p.children.Count());
        callStack.Add(index);
    }

    void Pop()
    {
        if (!callStack.IsEmpty())
        {
            callStack.RemoveLast();
        }
    }

    ProfileEntry root;
    Array<int32> callStack;
};

class Profiler
{
public:
    struct SessionData
    {
        String name;
        int64 startTime;
        float elapsedMs;
    };

    struct ScopeData
    {
        String name;
        int64 startTime;
        float elapsedMs;
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
            data.elapsedMs = (Time::NanoTime() - data.startTime) / (float)Time::MILLI_TO_NANO;

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
        sessionData.elapsedMs = 0.0f;

        sessionBeginEventHandler(sessionData);
    }

    void EndSessionUnlocked()
    {
        if (sessionOpen)
        {
            sessionOpen = false;

            sessionData.elapsedMs = (Time::NanoTime() - sessionData.startTime) / (float)Time::MILLI_TO_NANO;

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