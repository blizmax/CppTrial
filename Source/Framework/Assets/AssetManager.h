#pragma once

#include "Assets/AssetPtr.h"
#include "Core/HashMap.h"
#include "Utils/Module.h"

class AssetManager : public Module
{
public:
    void Startup() override;
    void Shutdown() override;
    void Tick() override;

    void RunMultithread(const Runnable &func);
    void RunMainthread(const Runnable &func);

    String GetName() const override
    {
        return CT_TEXT("AssetManager");
    }

private:
    Array<Runnable> assetSyncTasks;
    std::mutex assetSyncMutex;
};

extern AssetManager *gAssetManager;