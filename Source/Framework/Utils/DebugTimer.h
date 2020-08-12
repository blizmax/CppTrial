#pragma once

#include "Utils/.Package.h"

class DebugTimer
{
public:
    DebugTimer(const String &name)
        : name(name)
    {
        startTime = Time::MilliTime();
    }

    ~DebugTimer()
    {
        Stop();
    }

    void Stop()
    {
        if (running)
        {
            running = false;
            float t = (Time::MilliTime() - startTime) / (float)Time::MILLIS_PER_SECOND;
            CT_LOG(Info, CT_TEXT("{0} total cost sec: {1}."), name, t);
        }
    }

private:
    String name;
    int64 startTime;
    bool running = true;
};