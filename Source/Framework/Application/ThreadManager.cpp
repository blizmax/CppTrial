#include "Application/ThreadManager.h"

ThreadManager threadManager;
ThreadManager *gThreadManager = &threadManager;

void ThreadManager::Startup()
{
}

void ThreadManager::Shutdown()
{
    taskScheduler.StopAll();

    ThreadPool::GetGlobal().StopAll();
}

void ThreadManager::Tick()
{
}

ThreadPool::Handle ThreadManager::RunThread(const String &name, Runnable<> func)
{
    return ThreadPool::GetGlobal().Run(name, std::move(func));
}

void ThreadManager::RunAsync(const SPtr<AsyncTask> &task)
{
    taskScheduler.AddTask(task);
}