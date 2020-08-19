#pragma once

#include "Assets/AssetExporter.h"
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

    void RunMultithread(Runnable func);
    void RunMainthread(Runnable func);

    template <typename T>
    void RegisterImporter(IAssetImporter *importer)
    {
        importers.Put(TypeIndexOf<T>(), importer);
    }

    template <typename T>
    void RegisterExporter(IAssetExporter *exporter)
    {
        exporters.Put(TypeIndexOf<T>(), exporter);
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
    AssetExporter<T> *GetExporter() const
    {
        auto ptr = exporters.TryGet(TypeIndexOf<T>());
        if (!ptr)
            return nullptr;
        return dynamic_cast<AssetExporter<T> *>(*ptr);
    }

    template <typename T>
    APtr<T> Import(const String &path, const SPtr<ImportSettings> &settings = nullptr)
    {
        auto importer = GetImporter<T>();
        if (importer)
            return importer->Import(path, settings);
        return APtr<T>();
    }

    template <typename T>
    void Export(const SPtr<T> &asset, const String &path, const SPtr<ExportSettings> &settings = nullptr)
    {
        auto exporter = GetExporter<T>();
        if (exporter)
            exporter->Export(asset, path, settings);
    }

    String GetName() const override
    {
        return CT_TEXT("AssetManager");
    }

private:
    Array<Runnable> assetSyncTasks;
    std::mutex assetSyncMutex;

    HashMap<std::type_index, IAssetImporter *> importers;
    HashMap<std::type_index, IAssetExporter *> exporters;
};

extern AssetManager *gAssetManager;