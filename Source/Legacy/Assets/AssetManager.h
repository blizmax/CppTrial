#pragma once

#include "Assets/.Package.h"
#include "Assets/AssetPtr.h"
#include "Assets/AssetImporter.h"
#include "Core/Thread.h"
#include "Core/HashMap.h"
#include "Utils/Module.h"
#include "Utils/UUID.h"

class AssetManager : public Module
{
public:
    void Startup() override;
    void Shutdown() override;
    void Tick() override;

    APtr<AssetObject> Import(const String &path, const SPtr<AssetImportSettings> &settings = nullptr);
    APtr<AssetObject> ImportAsync(const String &path, const SPtr<AssetImportSettings> &settings = nullptr);
    bool IsExtensionSupported(const String &ext) const;
    SPtr<AssetImporter> GetImporter(const String &ext) const;

    APtr<AssetObject> Load(const String &path);
    APtr<AssetObject> LoadAsync(const String &path);

    void UnloadAll();
    void UnloadAllUnused();

    template <typename T>
    void RegisterImporter(const SPtr<AssetImporter> &importer)
    {
        importers.Put(TypeIndexOf<T>(), importer);
    }

    template <typename T>
    SPtr<AssetImporter> GetImporter() const
    {
        auto ptr = importers.TryGet(TypeIndexOf<T>());
        return *ptr;
    }

    template <typename T>
    APtr<T> Import(const String &path, const SPtr<AssetImportSettings> &settings = nullptr)
    {
        auto ptr = Import(path, settings);
        return ptr.Cast<T>();
    }

    template <typename T>
    APtr<T> ImportAsync(const String &path, const SPtr<AssetImportSettings> &settings = nullptr)
    {
        auto ptr = ImportAsync(path, settings);
        return ptr.Cast<T>();
    }

    template <typename T>
    APtr<T> Load(const String &path)
    {
        auto ptr = Load(path);
        return ptr.Cast<T>();
    }

    template <typename T>
    APtr<T> LoadAsync(const String &path)
    {
        auto ptr = LoadAsync(path);
        return ptr.Cast<T>();
    }

    String GetName() const override
    {
        return CT_TEXT("AssetManager");
    }

private:
    std::mutex importMutex;
    HashMap<std::type_index, SPtr<AssetImporter>> importers;

    HashMap<UUID, APtr<AssetObject>> loadedAssets;
};

extern AssetManager *gAssetManager;