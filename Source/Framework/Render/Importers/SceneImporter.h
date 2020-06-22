#pragma once

#include "Assets/AssetImporter.h"
#include "Render/SceneBuilder.h"

class SceneImportSettings : public ImportSettings
{
public:    
};

class SceneImporter : public AssetImporter<Scene>
{
public:
    virtual APtr<Scene> Import(const String &path, const SPtr<ImportSettings> &settings) override;
};
