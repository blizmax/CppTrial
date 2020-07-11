#pragma once

#include "Core/Thread/ThreadPool.h"
#include "Core/Queue.h"

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

    String GetName() const
    {
        return name;
    }

    /** 
     * Wait until the runnig task is done.
     */
    void Wait()
    {
        while (IsRunning())
        {
            Thread::YieldThis();
        }
    }

    /** 
     * Cancel the task if is in scheduler queue. 
     */
    void Cancel()
    {
        canceled = true;
    }

    static SPtr<AsyncTask> Create(const String &name, const ThreadFunc &func)
    {
        return Memory::MakeShared<AsyncTask>(name, func);
    }

private:
    friend class AsyncTaskScheduler;

    static constexpr uint32 READY = 0;
    static constexpr uint32 RUNNING = 1;
    static constexpr uint32 DONE = 2;

    String name;
    ThreadFunc worker;
    std::atomic_uint32_t state{READY};
    bool canceled = false;
};

class AsyncTaskScheduler
{
public:
    AsyncTaskScheduler(int32 initCapacity) : pool(initCapacity)
    {
    }

    ~AsyncTaskScheduler()
    {
        StopAll();
    }

    void AddTask(const SPtr<AsyncTask> &task)
    {
        CT_CHECK(task->IsReady() && !task->canceled && task->worker != nullptr);

        auto func = [=, this]() {
            SPtr<AsyncTask> runningTask = task;

            while (runningTask)
            {
                runningTask->state.store(AsyncTask::RUNNING);
                runningTask->worker();
                runningTask->state.store(AsyncTask::DONE);
                runningTask = nullptr;

                //Get first task in queue.
                {
                    std::unique_lock<std::mutex> lock(mutex);
                    while (!queuedTasks.IsEmpty())
                    {
                        if (queuedTasks.First()->canceled)
                        {
                            queuedTasks.Pop();
                        }
                        else
                        {
                            runningTask = queuedTasks.First();
                            queuedTasks.Pop();
                            break;
                        }
                    }
                }
            }
        };

        std::unique_lock<std::mutex> lock(mutex);

        if (pool.GetAvailableCount() > 0)
        {
            int32 id = pool.GetWorkingCount();
            pool.Run(String::Format(CT_TEXT("QueuedThread{0}"), id), func);
        }
        else
        {
            queuedTasks.Push(task);
        }
    }

    void StopAll()
    {
        pool.StopAll();
    }

    ThreadPool &GetThreadPool()
    {
        return pool;
    }

private:
    Queue<SPtr<AsyncTask>> queuedTasks;
    ThreadPool pool;
    std::mutex mutex;
};