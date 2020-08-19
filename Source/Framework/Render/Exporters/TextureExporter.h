#pragma once

#include "Assets/AssetExporter.h"
#include "RenderCore/Texture.h"

class TextureExportSettings : public ExportSettings
{
public:
    bool flipY = false;
    int32 mipLevel = 0;
    int32 arraySlice = 0;

    static SPtr<TextureExportSettings> Create()
    {
        return Memory::MakeShared<TextureExportSettings>();
    }
};

class TextureExporter : public AssetExporter<Texture>
{
public:
    virtual void Export(const SPtr<Texture> &asset, const String &path, const SPtr<ExportSettings> &settings) override;
};
