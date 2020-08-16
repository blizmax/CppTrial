#pragma once

#include "Assets/AssetImporter.h"
#include "RenderCore/Texture.h"

class TextureImportSettings : public ImportSettings
{
public:
    bool flipY = false;
    bool generateMips = false;
    bool srgbFormat = false; // If use srgb format, input data will be gamma corrected.

    static SPtr<TextureImportSettings> Create()
    {
        return Memory::MakeShared<TextureImportSettings>();
    }
};

class TextureImporter : public AssetImporter<Texture>
{
public:
    virtual APtr<Texture> Import(const String &path, const SPtr<ImportSettings> &settings) override;
    APtr<Texture> ImportFromMemory(Array<uint8> bytes, const SPtr<ImportSettings> &settings);
};