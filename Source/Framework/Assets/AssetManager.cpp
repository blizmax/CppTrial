#include "Assets/AssetManager.h"
#include "IO/FileHandle.h"
#include "Render/ImageLoader.h"

AssetManager assetManager;
AssetManager *gAssetManager = &assetManager;

void AssetManager::Startup()
{
    //TODO load default assets
}

void AssetManager::Shutdown()
{
    UnloadAll();
}

void AssetManager::Tick()
{
}

bool AssetManager::IsExtensionSupported(const String &ext) const
{
    return GetImporter(ext) != nullptr;
}

const SPtr<AssetImporter> &AssetManager::GetImporter(const String &ext) const
{
    for (const auto &[k, v] : importers)
    {
        if (v->IsExtensionSupported(ext))
            return v;
    }
    return nullptr;
}

APtr<AssetObject> AssetManager::Import(const String &path, const SPtr<AssetImportSettings> &settings)
{
    IO::FileHandle file(path);
    if (!file.IsFile())
    {
        CT_LOG(Warning, CT_TEXT("Import path invalid, path: {0}."), path);
        return nullptr;
    }

    String ext = file.GetExtension();
    const auto &importer = GetImporter(ext);
    if (!importer)
    {
        CT_LOG(Warning, CT_TEXT("Unsupported file type, path: {0}, extension: {1}."), path, ext);
        return nullptr;
    }

    return importer->Import(path, settings);
}

APtr<AssetObject> AssetManager::ImportAsync(const String &path, const SPtr<AssetImportSettings> &settings)
{
    //TODO
}

APtr<AssetObject> AssetManager::Load(const String &path)
{
    //TODO
}

APtr<AssetObject> AssetManager::LoadAsync(const String &path)
{
    //TODO
}

void AssetManager::UnloadAll()
{
    //TODO
}

void AssetManager::UnloadAllUnused()
{
    //TODO
}
