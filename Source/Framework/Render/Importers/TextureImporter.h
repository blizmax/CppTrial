#pragma once

#include "Assets/AssetImporter.h"
#include "RenderCore/Texture.h"

class TextureImportSettings : public ImportSettings
{
public:    
};

class TextureImporter : public AssetImporter<Texture>
{
public:
    virtual APtr<Texture> Import(const String &path, const SPtr<ImportSettings> &settings) override;
};