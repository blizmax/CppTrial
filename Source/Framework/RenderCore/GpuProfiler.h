#pragma once

#include "RenderCore/GpuTimer.h"
#include "Utils/Profiler.h"

class GpuProfiler
{
public:
    using SessionData = Profiler::SessionData;

    struct Session
    {
        SessionData data;
        SPtr<GpuTimer> timer;
    };

    void BeginSession(const String &name)
    {
        //TODO
    }

    void EndSession()
    {
        //TODO
    }

    static GpuProfiler *GetCurrentFrameProfiler()
    {
        return sCurrent;
    }

    static void SetCurrentFrameProfiler(GpuProfiler *p)
    {
        sCurrent = p;
    } 

private:


    static GpuProfiler *sCurrent;
};

inline GpuProfiler *GpuProfiler::sCurrent = nullptr;

#define CT_GPU_PROFILE_SESSION_BEGIN(name) ::GpuProfiler::GetCurrentFrameProfiler()->BeginSession(name);
#define CT_GPU_PROFILE_SESSION_END() ::GpuProfiler::GetCurrentFrameProfiler()->EndSession();