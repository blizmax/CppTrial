#include "Assets/AssetManager.h"
#include "Application/ThreadManager.h"

AssetManager assetManager;
AssetManager *gAssetManager = &assetManager;

void AssetManager::Startup()
{
}

void AssetManager::Shutdown()
{
}

void AssetManager::Tick()
{
    std::unique_lock<std::mutex> lock(assetSyncMutex);
    for (auto &task : assetSyncTasks)
    {
        task();
    }
    assetSyncTasks.Clear();
}

void AssetManager::RunMainthread(const Runnable &func)
{
    if (Thread::IsMainThread())
    {
        func();
    }
    else
    {
        std::unique_lock<std::mutex> lock(assetSyncMutex);
        assetSyncTasks.Add(func);
    }
}

void AssetManager::RunMultithread(const Runnable &func)
{
    gThreadManager->RunAsync(func);
}