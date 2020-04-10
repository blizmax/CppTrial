#include "Application/ThreadManager.h"

ThreadManager threadManager;
ThreadManager *gThreadManager = &threadManager;

void ThreadManager::Startup()
{
}

void ThreadManager::Shutdown()
{
    pool.StopAll();
}

void ThreadManager::Tick()
{
}

void ThreadManager::Execute(const String &name, ThreadPool::WorkFunc func)
{
    pool.Execute(name, func);
}