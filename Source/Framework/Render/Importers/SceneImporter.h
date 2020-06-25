#pragma once

#include "Assets/AssetImporter.h"
#include "Render/SceneBuilder.h"

class SceneImportSettings : public ImportSettings
{
public:
    bool mergeMeshes = false;
    bool assumeLinearSpaceTextures = false;  // By default all textures are assumed in srgb space.

    static SPtr<SceneImportSettings> Create()
    {
        return Memory::MakeShared<SceneImportSettings>();
    }
};

class SceneImporter : public AssetImporter<Scene>
{
public:
    virtual APtr<Scene> Import(const String &path, const SPtr<ImportSettings> &settings) override;
};
