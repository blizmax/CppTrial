#pragma once

#include "Assets/AssetImporter.h"
#include "Render/SceneBuilder.h"

class SceneImporter : public AssetImporter
{
public:
    virtual bool IsExtensionSupported(const String &ext) const override;
    virtual APtr<AssetObject> Import(const String &path, const SPtr<AssetImportSettings> &settings) override;
};
