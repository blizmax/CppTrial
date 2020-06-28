#include "Render/Importers/TextureImporter.h"
#include "IO/FileHandle.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ASSERT(x) do {CT_CHECK(x)} while(0)
#include <stb_image.h>

namespace
{
class ImporterImpl
{
public:
    SPtr<Texture> Import(const String &path, const SPtr<TextureImportSettings> &settings)
    {
        IO::FileHandle file(path);
        if (!file.IsFile())
        {
            CT_LOG(Error, "Load image failed, can not open file. Path: {0}.", path);
            return nullptr;
        }

        auto bytes = file.ReadBytes();
        stbi_set_flip_vertically_on_load(settings->flipY ? 1 : 0);

        ResourceFormat format = ResourceFormat::Unknown;
        int32 width, height, channels;
        if (!stbi_info_from_memory(bytes.GetData(), bytes.Count(), &width, &height, &channels))
        {
            CT_LOG(Error, "Load image failed, can not get image info. Path: {0}.", path);
            return nullptr;
        }

        // NOTE Always convert 3-elements image to 4-elements.
        int32 reqComp = (channels == 3) ? STBI_rgb_alpha : STBI_default;

        bool bHDR = stbi_is_hdr_from_memory(bytes.GetData(), bytes.Count());
        bool b16bits = !bHDR && stbi_is_16_bit_from_memory(bytes.GetData(), bytes.Count());
        void *data = nullptr;
        if (bHDR)
        {
            data = stbi_loadf_from_memory(bytes.GetData(), bytes.Count(), &width, &height, &channels, reqComp);

            if (channels == 4)
                format = ResourceFormat::RGBA32Float;
            else if (channels == 2)
                format = ResourceFormat::RG32Float;
            else if (channels == 1)
                format = ResourceFormat::R32Float;
        }
        else if (b16bits)
        {
            data = stbi_load_16_from_memory(bytes.GetData(), bytes.Count(), &width, &height, &channels, reqComp);

            if (channels == 4)
                format = ResourceFormat::RGBA16Unorm;
            else if (channels == 2)
                format = ResourceFormat::RG16Unorm;
            else if (channels == 1)
                format = ResourceFormat::R16Unorm;
        }
        else
        {
            data = stbi_load_from_memory(bytes.GetData(), bytes.Count(), &width, &height, &channels, reqComp);

            if (channels == 4)
                format = ResourceFormat::RGBA8Unorm;
            else if (channels == 2)
                format = ResourceFormat::RG8Unorm;
            else if (channels == 1)
                format = ResourceFormat::R8Unorm;
        }

        CT_CHECK(format != ResourceFormat::Unknown);
        if (settings->srgbFormat)
        {
            format = LinearToSrgbFormat(format);
        }

        if (!data)
        {
            CT_LOG(Error, "Load image failed, {0}. Path: {1}.", String(stbi_failure_reason()), path);
            return nullptr;
        }
        auto texture = Texture::Create2D(width, height, format, 1, settings->generateMips ? -1 : 1, data);
        stbi_image_free(data);
        return texture;
    }
};
}

APtr<Texture> TextureImporter::Import(const String &path, const SPtr<ImportSettings> &settings)
{
    APtr<Texture> result;

    ImporterImpl impl;
    auto texture = impl.Import(path, ImportSettings::As<TextureImportSettings>(settings));

    result.SetData(APtr<Texture>::InnerData::Create(texture));
    return result;
}