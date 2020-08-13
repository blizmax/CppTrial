#pragma once

#include "Assets/AssetImporter.h"
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

    template <typename T>
    void RegisterImporter(IAssetImporter *importer)
    {
        importers.Put(TypeIndexOf<T>(), importer);
    }

    template <typename T>
    AssetImporter<T> *GetImporter() const
    {
        auto ptr = importers.TryGet(TypeIndexOf<T>());
        if (!ptr)
            return nullptr;
        return dynamic_cast<AssetImporter<T> *>(*ptr);
    }

    template <typename T>
    APtr<T> Import(const String &path, const SPtr<ImportSettings> &settings = nullptr)
    {
        auto importer = GetImporter<T>();
        if (importer)
            return importer->Import(path, settings);
        return APtr<T>();
    }

    String GetName() const override
    {
        return CT_TEXT("AssetManager");
    }

private:
    Array<Runnable> assetSyncTasks;
    std::mutex assetSyncMutex;

    HashMap<std::type_index, IAssetImporter *> importers;
};

extern AssetManager *gAssetManager;