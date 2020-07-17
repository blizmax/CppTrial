#pragma once

#include "Application/.Package.h"
#include "Core/Thread.h"
#include "Utils/Module.h"

class ThreadManager : public Module
{
public:
    virtual void Startup() override;
    virtual void Shutdown() override;
    virtual void Tick() override;

    ThreadPool::Handle RunThread(const String &name, const ThreadFunc &func);

    void RunAsync(const ThreadFunc &func)
    {
        auto task = AsyncTask::Create(CT_TEXT("Temp"), func);
        RunAsync(task);
    }

    void RunAsync(const SPtr<AsyncTask> &task);

    String GetName() const override
    {
        return CT_TEXT("ThreadManager");
    }

private:
    AsyncTaskScheduler taskScheduler{ static_cast<int32>(Thread::HardwareConcurrency()) };
};

extern ThreadManager *gThreadManager;