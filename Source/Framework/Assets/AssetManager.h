#pragma once

#include "Assets/.Package.h"
#include "Assets/AssetHandle.h"
#include "Utils/Module.h"
#include "Core/Any.h"

class AssetManager : public Module
{
public:
    void Startup() override;
    void Shutdown() override;
    void Tick() override;

    AssetHandle<AssetObject> Load(const String &path);
    AssetHandle<AssetObject> LoadAsync(const String &path);

    template <typename T>
    AssetHandle<T> Load(const String &path)
    {
        auto handle = Load(path);
        return handle.Cast<T>();
    }

    template <typename T>
    AssetHandle<T> LoadAsync(const String &path)
    {
        auto handle = LoadAsync(path);
        return handle.Cast<T>();
    }
};