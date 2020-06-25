#pragma once

#include "Assets/AssetImporter.h"
#include "RenderCore/Texture.h"

class TextureImportSettings : public ImportSettings
{
public:
    bool flipY = false;

    static SPtr<TextureImportSettings> Create()
    {
        return Memory::MakeShared<TextureImportSettings>();
    }
};

class TextureImporter : public AssetImporter<Texture>
{
public:
    virtual APtr<Texture> Import(const String &path, const SPtr<ImportSettings> &settings) override;
};