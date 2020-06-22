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

    String GetName() const override
    {
        return CT_TEXT("AssetManager");
    }
};

extern AssetManager *gAssetManager;