#include "Render/Exporters/TextureExporter.h"
#include "Assets/AssetManager.h"
#include "IO/FileHandle.h"
#include "RenderCore/RenderAPI.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBIW_ASSERT(x) \
    do                  \
    {                   \
        CT_CHECK(x)     \
    } while (0)
#include <stb_image_write.h>

namespace
{

class ExporterImpl
{
public:
    ExporterImpl(const SPtr<Texture> &asset, const SPtr<TextureExportSettings> &settings)
        : asset(asset), settings(settings)
    {
    }

    bool CheckTexture() const
    {
        if (!asset)
        {
            CT_LOG(Warning, CT_TEXT("TextureExporter::Export() failed, texture is null."));
            return false;
        }
        if (asset->GetResourceType() != ResourceType::Texture2D)
        {
            CT_LOG(Warning, CT_TEXT("TextureExporter::Export() failed, texture must be Texture2D."));
            return false;
        }

        return true;
    }

    bool CheckExtension(const String &ext) const
    {
        if (ext != CT_TEXT(".png"))
        {
            CT_LOG(Warning, CT_TEXT("TextureExporter::Export() failed, only support png format now."));
            return false;
        }

        return true;
    }

    void WriteToFile(Array<uint8> data) const
    {
        // Thread safe?
        if (settings->flipY)
        {
            stbi_flip_vertically_on_write(true);
        }

        int32 w = asset->GetWidth();
        int32 h = asset->GetHeight();
        int32 n = GetResourceFormatComponentCount(asset->GetResourceFormat());

        if (stbi_write_png(CT_U8_CSTR(path), w, h, n, data.GetData(), 0) == 0)
        {
            CT_LOG(Error, CT_TEXT("TextureExporter::Export() failed, stbi_write_xxx() return 0."));
        }
    }

    void Export(const String &path)
    {
        if (!CheckTexture())
            return;

        this->path = path;

        IO::FileHandle file(path);
        String ext = file.GetExtension();
        if (!CheckExtension(ext))
            return;

        auto format = asset->GetResourceFormat();
        auto channels = GetResourceFormatComponentCount(format);
        auto componentType = GetResourceComponentType(format);

        ExporterImpl copy = *this;

        if (componentType == ResourceComponentType::Float && channels < 3)
        {
            //TODO Blit
            CT_EXCEPTION(Render, "Not implement.");
        }
        else
        {
            gAssetManager->RunMainthread([copy = std::move(copy)]() {
                auto mipLevel = copy.settings->mipLevel;
                auto arraySlice = copy.settings->arraySlice;
                auto subresource = copy.asset->GetSubresourceIndex(arraySlice, mipLevel);
                auto data = RenderAPI::GetDevice()->GetRenderContext()->ReadSubresource(copy.asset.get(), subresource);

                gAssetManager->RunMultithread([data = std::move(data), copy = std::move(copy)](){
                    copy.WriteToFile(std::move(data));
                });
            });
        }
    }

private:
    SPtr<TextureExportSettings> settings;
    SPtr<Texture> asset;
    String path;
};

}

void TextureExporter::Export(const SPtr<Texture> &asset, const String &path, const SPtr<ExportSettings> &settings)
{
    ExporterImpl impl(asset, ExportSettings::As<TextureExportSettings>(settings));
    impl.Export(path);
}