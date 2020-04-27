#pragma once

#include "Core/Thread/ThreadPool.h"

class AsyncTask
{
public:
    AsyncTask(const String &name, const ThreadFunc &func) : name(name), worker(func)
    {   
    }

    bool IsReady() const
    {
        return state == READY;
    }

    bool IsRunning() const
    {
        return state == RUNNING;
    }

    bool IsDone() const
    {
        return state == DONE;
    }

    void Wait()
    {
        //TODO
    }

    void Cancel()
    {
        state.store(CANCELED);
    }

    static SPtr<AsyncTask> Create(const String &name, const ThreadFunc &func)
    {
        return Memory::MakeShared<AsyncTask>(name, func);
    }

private:
    static constexpr uint32 READY = 0;
    static constexpr uint32 RUNNING = 1;
    static constexpr uint32 DONE = 2;
    static constexpr uint32 CANCELED = 3;

    String name;
    ThreadFunc worker;
    std::atomic_uint32_t state{READY};
};

class AsyncTaskScheduler
{
public:
    AsyncTaskScheduler(int32 capacity)
    {

    }

    void AddTask(const ThreadFunc &func)
    {
        AddTask(AsyncTask::Create(CT_TEXT("Temp"), func));
    }

    void AddTask(const SPtr<AsyncTask> &task)
    {
        auto &pool = ThreadPool::GetGlobal();

        auto func = [=]()
        {
            //TODO
        };

        std::unique_lock<std::mutex> lock(mutex);

        CT_CHECK(!task->IsRunning());

        //TODO force run thread

    }

private:
    Array<SPtr<AsyncTask>> tasks;
    std::mutex mutex;
};