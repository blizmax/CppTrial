#include "Render/Importers/TextureImporter.h"
#include "Assets/AssetManager.h"
#include "IO/FileHandle.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ASSERT(x) \
    do                 \
    {                  \
        CT_CHECK(x)    \
    } while (0)
#include <stb_image.h>

#define TINYDDSLOADER_IMPLEMENTATION
#include <tinyddsloader.h>
using namespace tinyddsloader;

namespace
{
class ImporterImpl
{
public:
    ImporterImpl(const APtr<Texture> &asset, const SPtr<TextureImportSettings> &settings)
        : asset(asset), settings(settings)
    {
    }

    ResourceFormat GetResourceFormat(const DDSFile &dds)
    {
        switch (dds.GetFormat())
        {
        case DDSFile::DXGIFormat::R32G8X24_Typeless:
        case DDSFile::DXGIFormat::R16G16B16A16_Typeless:
        case DDSFile::DXGIFormat::R32G32B32_Typeless:
        case DDSFile::DXGIFormat::R32G32B32A32_Typeless:
        case DDSFile::DXGIFormat::R16G16B16A16_SNorm:
        case DDSFile::DXGIFormat::R32G32_Typeless:
        case DDSFile::DXGIFormat::R32_Float_X8X24_Typeless:
        case DDSFile::DXGIFormat::X32_Typeless_G8X24_UInt:
        case DDSFile::DXGIFormat::R10G10B10A2_Typeless:
        case DDSFile::DXGIFormat::Y416:
        case DDSFile::DXGIFormat::Y210:
        case DDSFile::DXGIFormat::Y216:
        case DDSFile::DXGIFormat::R8G8B8A8_Typeless:
        case DDSFile::DXGIFormat::R16G16_Typeless:
        case DDSFile::DXGIFormat::R32_Typeless:
        case DDSFile::DXGIFormat::R24G8_Typeless:
        case DDSFile::DXGIFormat::R24_UNorm_X8_Typeless:
        case DDSFile::DXGIFormat::X24_Typeless_G8_UInt:
        case DDSFile::DXGIFormat::R8G8_B8G8_UNorm:
        case DDSFile::DXGIFormat::G8R8_G8B8_UNorm:
        case DDSFile::DXGIFormat::R10G10B10_XR_BIAS_A2_UNorm:
        case DDSFile::DXGIFormat::B8G8R8A8_Typeless:
        case DDSFile::DXGIFormat::B8G8R8X8_Typeless:
        case DDSFile::DXGIFormat::AYUV:
        case DDSFile::DXGIFormat::Y410:
        case DDSFile::DXGIFormat::YUY2:
        case DDSFile::DXGIFormat::P010:
        case DDSFile::DXGIFormat::P016:
        case DDSFile::DXGIFormat::R8G8_Typeless:
        case DDSFile::DXGIFormat::R16_Typeless:
        case DDSFile::DXGIFormat::A8P8:
        case DDSFile::DXGIFormat::B4G4R4A4_UNorm:
        case DDSFile::DXGIFormat::NV12:
        case DDSFile::DXGIFormat::YUV420_OPAQUE:
        case DDSFile::DXGIFormat::NV11:
        case DDSFile::DXGIFormat::R8_Typeless:
        case DDSFile::DXGIFormat::AI44:
        case DDSFile::DXGIFormat::IA44:
        case DDSFile::DXGIFormat::P8:
        case DDSFile::DXGIFormat::R1_UNorm:
        case DDSFile::DXGIFormat::BC1_Typeless:
        case DDSFile::DXGIFormat::BC4_Typeless:
        case DDSFile::DXGIFormat::BC2_Typeless:
        case DDSFile::DXGIFormat::BC3_Typeless:
        case DDSFile::DXGIFormat::BC5_Typeless:
        case DDSFile::DXGIFormat::BC6H_Typeless:
        case DDSFile::DXGIFormat::BC7_Typeless:
            return ResourceFormat::Unknown;
        case DDSFile::DXGIFormat::R32G32B32A32_Float:
            return ResourceFormat::RGBA32Float;
        case DDSFile::DXGIFormat::R32G32B32A32_UInt:
            return ResourceFormat::RGBA32UInt;
        case DDSFile::DXGIFormat::R32G32B32A32_SInt:
            return ResourceFormat::RGBA32Int;
        case DDSFile::DXGIFormat::R32G32B32_Float:
            return ResourceFormat::RGB32Float;
        case DDSFile::DXGIFormat::R32G32B32_UInt:
            return ResourceFormat::RGB32UInt;
        case DDSFile::DXGIFormat::R32G32B32_SInt:
            return ResourceFormat::RGB32Int;
        case DDSFile::DXGIFormat::R16G16B16A16_Float:
            return ResourceFormat::RGBA16Float;
        case DDSFile::DXGIFormat::R16G16B16A16_UNorm:
            return ResourceFormat::RGBA16Unorm;
        case DDSFile::DXGIFormat::R16G16B16A16_UInt:
            return ResourceFormat::RGBA16UInt;
        case DDSFile::DXGIFormat::R16G16B16A16_SInt:
            return ResourceFormat::RGBA16Int;
        case DDSFile::DXGIFormat::R32G32_Float:
            return ResourceFormat::RG32Float;
        case DDSFile::DXGIFormat::R32G32_UInt:
            return ResourceFormat::RG32UInt;
        case DDSFile::DXGIFormat::R32G32_SInt:
            return ResourceFormat::RG32Int;
        case DDSFile::DXGIFormat::D32_Float_S8X24_UInt:
            return ResourceFormat::D32FloatS8X24;
        case DDSFile::DXGIFormat::R10G10B10A2_UNorm:
            return ResourceFormat::RGB10A2Unorm;
        case DDSFile::DXGIFormat::R10G10B10A2_UInt:
            return ResourceFormat::RGB10A2UInt;
        case DDSFile::DXGIFormat::R11G11B10_Float:
            return ResourceFormat::R11G11B10Float;
        case DDSFile::DXGIFormat::R8G8B8A8_UNorm:
            return ResourceFormat::RGBA8Unorm;
        case DDSFile::DXGIFormat::R8G8B8A8_UNorm_SRGB:
            return ResourceFormat::RGBA8UnormSrgb;
        case DDSFile::DXGIFormat::R8G8B8A8_UInt:
            return ResourceFormat::RGBA8UInt;
        case DDSFile::DXGIFormat::R8G8B8A8_SNorm:
            return ResourceFormat::RGBA8Snorm;
        case DDSFile::DXGIFormat::R8G8B8A8_SInt:
            return ResourceFormat::RGBA8Int;
        case DDSFile::DXGIFormat::R16G16_Float:
            return ResourceFormat::RG16Float;
        case DDSFile::DXGIFormat::R16G16_UNorm:
            return ResourceFormat::RG16Unorm;
        case DDSFile::DXGIFormat::R16G16_UInt:
            return ResourceFormat::RG16UInt;
        case DDSFile::DXGIFormat::R16G16_SNorm:
            return ResourceFormat::RG16Snorm;
        case DDSFile::DXGIFormat::R16G16_SInt:
            return ResourceFormat::RG16Int;
        case DDSFile::DXGIFormat::D32_Float:
            return ResourceFormat::D32Float;
        case DDSFile::DXGIFormat::R32_Float:
            return ResourceFormat::R32Float;
        case DDSFile::DXGIFormat::R32_UInt:
            return ResourceFormat::R32UInt;
        case DDSFile::DXGIFormat::R32_SInt:
            return ResourceFormat::R32Int;
        case DDSFile::DXGIFormat::D24_UNorm_S8_UInt:
            return ResourceFormat::D24UnormS8;
        case DDSFile::DXGIFormat::R9G9B9E5_SHAREDEXP:
            return ResourceFormat::RGB9E5Float;
        case DDSFile::DXGIFormat::B8G8R8A8_UNorm:
            return ResourceFormat::BGRA8Unorm;
        case DDSFile::DXGIFormat::B8G8R8X8_UNorm:
            return ResourceFormat::BGRX8Unorm;
        case DDSFile::DXGIFormat::B8G8R8A8_UNorm_SRGB:
            return ResourceFormat::BGRA8UnormSrgb;
        case DDSFile::DXGIFormat::B8G8R8X8_UNorm_SRGB:
            return ResourceFormat::BGRX8UnormSrgb;
        case DDSFile::DXGIFormat::R8G8_UNorm:
            return ResourceFormat::RG8Unorm;
        case DDSFile::DXGIFormat::R8G8_UInt:
            return ResourceFormat::RG8UInt;
        case DDSFile::DXGIFormat::R8G8_SNorm:
            return ResourceFormat::RG8Snorm;
        case DDSFile::DXGIFormat::R8G8_SInt:
            return ResourceFormat::RG8Int;
        case DDSFile::DXGIFormat::R16_Float:
            return ResourceFormat::R16Float;
        case DDSFile::DXGIFormat::D16_UNorm:
            return ResourceFormat::D16Unorm;
        case DDSFile::DXGIFormat::R16_UNorm:
            return ResourceFormat::R16Unorm;
        case DDSFile::DXGIFormat::R16_UInt:
            return ResourceFormat::R16UInt;
        case DDSFile::DXGIFormat::R16_SNorm:
            return ResourceFormat::R16Snorm;
        case DDSFile::DXGIFormat::R16_SInt:
            return ResourceFormat::R16Int;
        case DDSFile::DXGIFormat::B5G6R5_UNorm:
            return ResourceFormat::R5G6B5Unorm;
        case DDSFile::DXGIFormat::B5G5R5A1_UNorm:
            return ResourceFormat::RGB5A1Unorm;
        case DDSFile::DXGIFormat::R8_UNorm:
            return ResourceFormat::R8Unorm;
        case DDSFile::DXGIFormat::R8_UInt:
            return ResourceFormat::R8UInt;
        case DDSFile::DXGIFormat::R8_SNorm:
            return ResourceFormat::R8Snorm;
        case DDSFile::DXGIFormat::R8_SInt:
            return ResourceFormat::R8Int;
        case DDSFile::DXGIFormat::A8_UNorm:
            return ResourceFormat::Alpha8Unorm;
        case DDSFile::DXGIFormat::BC1_UNorm:
            return ResourceFormat::BC1Unorm;
        case DDSFile::DXGIFormat::BC1_UNorm_SRGB:
            return ResourceFormat::BC1UnormSrgb;
        case DDSFile::DXGIFormat::BC4_UNorm:
            return ResourceFormat::BC4Unorm;
        case DDSFile::DXGIFormat::BC4_SNorm:
            return ResourceFormat::BC4Snorm;
        case DDSFile::DXGIFormat::BC2_UNorm:
            return ResourceFormat::BC2Unorm;
        case DDSFile::DXGIFormat::BC2_UNorm_SRGB:
            return ResourceFormat::BC2UnormSrgb;
        case DDSFile::DXGIFormat::BC3_UNorm:
            return ResourceFormat::BC3Unorm;
        case DDSFile::DXGIFormat::BC3_UNorm_SRGB:
            return ResourceFormat::BC3UnormSrgb;
        case DDSFile::DXGIFormat::BC5_UNorm:
            return ResourceFormat::BC5Unorm;
        case DDSFile::DXGIFormat::BC5_SNorm:
            return ResourceFormat::BC5Snorm;
        case DDSFile::DXGIFormat::BC6H_SF16:
            return ResourceFormat::BC6HS16;
        case DDSFile::DXGIFormat::BC6H_UF16:
            return ResourceFormat::BC6HU16;
        case DDSFile::DXGIFormat::BC7_UNorm:
            return ResourceFormat::BC7Unorm;
        case DDSFile::DXGIFormat::BC7_UNorm_SRGB:
            return ResourceFormat::BC7UnormSrgb;
        default:
            return ResourceFormat::Unknown;
        }
    }

    void CreateFromDDSFile(Array<uint8> bytes)
    {
        DDSFile dds;
        auto ret = dds.Load(bytes.GetData(), bytes.Count());

        if (tinyddsloader::Result::Success != ret)
        {
            CT_LOG(Error, "Load dds image failed. Path: {0}, errorCode: {1}.", path, static_cast<std::underlying_type_t<tinyddsloader::Result>>(ret));
            return;
        }

        ResourceFormat format = GetResourceFormat(dds);
        CT_CHECK(format != ResourceFormat::Unknown);
        if (format == ResourceFormat::Unknown)
        {
            CT_LOG(Error, "Load dds image failed, Unknown resource format. Path: {0}.", path);
            return;
        }
        if (settings->srgbFormat)
        {
            format = LinearToSrgbFormat(format);
        }

        int32 mipLevels = -1;
        if (settings->generateMips == false || IsCompressedFormat(format))
        {
            // int32 count = dds.GetMipCount();
            mipLevels = 1;
        }

        if (settings->flipY)
        {
            dds.Flip();
        }

        gAssetManager->RunMainthread([asset = this->asset, dds = std::move(dds), format, mipLevels]() mutable {
            int32 width = dds.GetWidth();
            int32 height = dds.GetHeight();
            int32 depth = dds.GetDepth();
            int32 arrayLayers = dds.GetArraySize();
            auto dimension = dds.GetTextureDimension();
            auto imageData = dds.GetImageData();
            if (dimension == DDSFile::TextureDimension::Texture1D)
            {
                asset.GetData()->ptr = Texture::Create1D(width, format, arrayLayers, mipLevels, imageData->m_mem);
            }
            else if (dimension == DDSFile::TextureDimension::Texture2D)
            {
                if (dds.IsCubemap())
                {
                    //TODO arrayLayers /= 6 ?
                    asset.GetData()->ptr = Texture::CreateCube(width, height, format, arrayLayers, mipLevels, imageData->m_mem);
                }
                else
                {
                    asset.GetData()->ptr = Texture::Create2D(width, height, format, arrayLayers, mipLevels, imageData->m_mem);
                }
            }
            else if (dimension == DDSFile::TextureDimension::Texture3D)
            {
                asset.GetData()->ptr = Texture::Create3D(width, height, depth, format, mipLevels, imageData->m_mem);
            }
        });
    }

    void CreateFromStbi(Array<uint8> bytes)
    {
        stbi_set_flip_vertically_on_load(settings->flipY ? 1 : 0);

        ResourceFormat format = ResourceFormat::Unknown;
        int32 width, height, channels;
        if (!stbi_info_from_memory(bytes.GetData(), bytes.Count(), &width, &height, &channels))
        {
            CT_LOG(Error, "Load image failed, can not get image info. Path: {0}.", path);
            return;
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
            {
                format = ResourceFormat::RGBA32Float;
            }
            else if (channels == 2)
            {
                format = ResourceFormat::RG32Float;
            }
            else if (channels == 1)
            {
                format = ResourceFormat::R32Float;
            }
            else if (channels == 3)
            {
                format = ResourceFormat::RGBA32Float;
            }
        }
        else if (b16bits)
        {
            data = stbi_load_16_from_memory(bytes.GetData(), bytes.Count(), &width, &height, &channels, reqComp);

            if (channels == 4)
            {
                format = ResourceFormat::RGBA16Unorm;
            }
            else if (channels == 2)
            {
                format = ResourceFormat::RG16Unorm;
            }
            else if (channels == 1)
            {
                format = ResourceFormat::R16Unorm;
            }
            else if (channels == 3)
            {
                format = ResourceFormat::RGBA16Unorm;
            }
        }
        else
        {
            data = stbi_load_from_memory(bytes.GetData(), bytes.Count(), &width, &height, &channels, reqComp);

            if (channels == 4)
            {
                format = ResourceFormat::RGBA8Unorm;
            }
            else if (channels == 2)
            {
                format = ResourceFormat::RG8Unorm;
            }
            else if (channels == 1)
            {
                format = ResourceFormat::R8Unorm;
            }
            else if (channels == 3)
            {
                format = ResourceFormat::RGBA8Unorm;
            }
        }

        if (!data)
        {
            CT_LOG(Error, "Load image failed. Path: {0}, reason: {1}", path, String(stbi_failure_reason()));
            return;
        }

        if (format == ResourceFormat::Unknown)
        {
            CT_LOG(Error, "Load image failed, Unknown resource format. Path: {0}, channels:{1}.", path, channels);
            stbi_image_free(data);
            return;
        }
        if (settings->srgbFormat)
        {
            format = LinearToSrgbFormat(format);
        }

        int32 mipLevels = settings->generateMips ? -1 : 1;
        gAssetManager->RunMainthread([asset = this->asset, width, height, format, mipLevels, data]() mutable {
            asset.GetData()->ptr = Texture::Create2D(width, height, format, 1, mipLevels, data);
            stbi_image_free(data);
        });
    }

    void Import(const String &path)
    {
        IO::FileHandle file(path);
        if (!file.IsFile())
        {
            CT_LOG(Error, "Load image failed, can not open file. Path: {0}.", path);
            return;
        }

        this->path = path;

        String ext = file.GetExtension();
        auto bytes = file.ReadBytes();
        if (ext == CT_TEXT(".dds"))
        {
            CreateFromDDSFile(std::move(bytes));
        }
        else
        {
            CreateFromStbi(std::move(bytes));
        }
    }

    void ImportFromMemory(Array<uint8> bytes)
    {
        CreateFromStbi(std::move(bytes));
    }

private:
    SPtr<TextureImportSettings> settings;
    APtr<Texture> asset;
    String path;
};

}

APtr<Texture> TextureImporter::Import(const String &path, const SPtr<ImportSettings> &settings)
{
    APtr<Texture> result;
    result.NewData();

    gAssetManager->RunMultithread([=]() {
        ImporterImpl impl(result, ImportSettings::As<TextureImportSettings>(settings));
        impl.Import(path);
    });

    return result;
}

APtr<Texture> TextureImporter::ImportFromMemory(Array<uint8> data, const SPtr<ImportSettings> &settings)
{
    APtr<Texture> result;
    result.NewData();

    gAssetManager->RunMultithread([=]() {
        ImporterImpl impl(result, ImportSettings::As<TextureImportSettings>(settings));
        impl.ImportFromMemory(std::move(data));
    });

    return result;
}