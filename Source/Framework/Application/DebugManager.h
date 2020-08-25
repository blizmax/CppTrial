#pragma once

#include "Application/.Package.h"
#include "Core/HashMap.h"
#include "Core/Math.h"
#include "Utils/Module.h"

class DebugManager : public Module
{
public:
    DebugManager() = default;
    virtual ~DebugManager() = default;

    virtual void Startup() override;
    virtual void Shutdown() override;
    virtual void Tick() override;

    const Array<Profiler::SessionData> &GetCpuProfileSessions() const;
    void ResetProfile();

    virtual String GetName() const override
    {
        return CT_TEXT("DebugManager");
    }

private:
    struct ProfileData
    {
        Array<Profiler::SessionData> sessions;
        HashMap<String, int32> nameToIndex;

        void Clear()
        {
            sessions.Clear();
            nameToIndex.Clear();
        }

        void AddSession(const Profiler::SessionData &s)
        {
            auto ptr = nameToIndex.TryGet(s.name);
            int32 index = ptr ? *ptr : -1;
            if (index == -1)
            {
                sessions.Add(s);
                nameToIndex.Put(s.name, sessions.Count() - 1);
            }
            else
            {
                sessions[index].startTime = Math::Min(sessions[index].startTime, s.startTime);
                sessions[index].elapsedMs += s.elapsedMs;
            }
        }
    };

    ProfileData cpuProfile;
};

extern DebugManager *gDebugManager;