#pragma once

#include "RenderCore/QueryPool.h"

class GpuTimer
{
public:
    virtual ~GpuTimer() = default;

    virtual void Start() = 0;
    virtual void Stop() = 0;
    virtual float GetElapsedMs() = 0;

    bool IsRunning() const
    {
        return running;
    }

    static SPtr<GpuTimer> Create();

protected:
    int32 startID;
    int32 endID;
    bool running = false;
    bool unfetched = false;
    float elapsedMs = 0.0f;
};