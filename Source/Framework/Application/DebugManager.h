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

    const ProfileEntry &GetCpuProfileRootEntry() const;

    virtual String GetName() const override
    {
        return CT_TEXT("DebugManager");
    }

private:
    struct ProfileData
    {
        Array<ProfileEntry> entries;
        HashMap<String, int32> nameToIndex;

        void Clear()
        {
            entries.Clear();
            nameToIndex.Clear();
        }

        void AddSession(const Profiler::SessionData &s)
        {
            auto ptr = nameToIndex.TryGet(s.name);
            int32 index = ptr ? *ptr : -1;
            if (index == -1)
            {
                ProfileEntry e;
                e.name = s.name;
                e.callNum = 1;
                e.elapsedMs = s.elapsedMs;
                entries.Add(e);
                nameToIndex.Put(s.name, entries.Count() - 1);
            }
            else
            {
                entries[index].elapsedMs += s.elapsedMs;
                entries[index].callNum++;
            }
        }
    };

    ProfileData cpuProfileData;
    ProfileEntry cpuProfileRootEntry;
};

extern DebugManager *gDebugManager;