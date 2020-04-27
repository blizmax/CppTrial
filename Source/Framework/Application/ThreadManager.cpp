#include "Application/ThreadManager.h"

ThreadManager threadManager;
ThreadManager *gThreadManager = &threadManager;

void ThreadManager::Startup()
{
}

void ThreadManager::Shutdown()
{
    ThreadPool::GetGlobal().StopAll();
}

void ThreadManager::Tick()
{
}

void ThreadManager::RunThread(const String &name, const ThreadFunc &func)
{
    ThreadPool::GetGlobal().Run(name, func);
}