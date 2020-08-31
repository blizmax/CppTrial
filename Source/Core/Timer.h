#pragma once

#include "Core/Time.h"
#include "Core/String.h"

class Timer
{
public:
    Timer() = default;
    Timer(const Timer &) = default;
    Timer(Timer &&) = default;
    Timer &operator=(const Timer &) = default;
    Timer &operator=(Timer &&) = default;

    Timer(Runnable<Timer &> callback)
        : callback(std::move(callback))
    {
        Start();
    }

    Timer(const String &name, Runnable<Timer &> callback)
        : name(name), callback(std::move(callback))
    {
        Start();
    }

    ~Timer()
    {
        Stop();
    }

    void Start()
    {
        running = true;
        startTime = Time::NanoTime();
    }

    void Stop()
    {
        if (running)
        {
            running = false;
            endTime = Time::NanoTime();

            if (callback)
                callback(*this);
        }
    }

    bool IsRunning() const
    {
        return running;
    }

    int64 GetStartTime() const
    {
        return startTime;
    }

    int64 GetEndTime() const
    {
        return endTime;
    }

    float GetElapsedMs() const
    {
        return (endTime - startTime) / (float)Time::MILLI_TO_NANO;
    }

    const String &GetName() const
    {
        return name;
    }

private:
    String name;
    Runnable<Timer &> callback;
    int64 startTime;
    int64 endTime;
    bool running = false;
};