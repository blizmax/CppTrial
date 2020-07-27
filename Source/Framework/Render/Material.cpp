#include "Render/Material.h"

SPtr<Material> Material::Create()
{
    return Memory::MakeShared<Material>();
}

void Material::SetFlags(int32 flags)
{
    if (data.flags != flags)
    {
        data.flags = flags;
    }
}

static int32 GetSampleMode(bool hasTexture, const Color &color)
{
    if (hasTexture)
        return CT_MAT_SAMPLE_MODE_TEXTURE;
    if (color.Luminance() == 0.0f)
        return CT_MAT_SAMPLE_MODE_NONE;
    return CT_MAT_SAMPLE_MODE_CONST;
}

void Material::UpdateBaseColorType()
{
    SetFlags(SetMaterialSampleMode(data.flags, CT_MAT_DIFFUSE, GetSampleMode(resources.baseTexture != nullptr, data.base)));
}

void Material::UpdateSpecularType()
{
    SetFlags(SetMaterialSampleMode(data.flags, CT_MAT_SPECULAR, GetSampleMode(resources.specularTexture != nullptr, data.specular)));
}

void Material::UpdateEmissiveType()
{
    SetFlags(SetMaterialSampleMode(data.flags, CT_MAT_EMISSIVE, GetSampleMode(resources.emissiveTexture != nullptr, data.emissive)));
}

void Material::UpdateOcclusonType()
{
    int32 sampleMode = resources.occlusionTexture != nullptr ? CT_MAT_SAMPLE_MODE_TEXTURE : CT_MAT_SAMPLE_MODE_NONE;
    SetFlags(SetMaterialSampleMode(data.flags, CT_MAT_OCCLUSION, sampleMode));
}

void Material::UpdateNormalType()
{
    int32 mode = CT_MAT_NORMAL_MODE_NONE;
    if (resources.normalTexture)
    {
        int32 channels = GetResourceFormatComponentCount(resources.normalTexture->GetResourceFormat());
        switch (channels)
        {
        case 2:
            mode = CT_MAT_NORMAL_MODE_RG;
            break;
        case 3:
        case 4:
            mode = CT_MAT_NORMAL_MODE_RGB;
            break;
        default:
            CT_LOG(Warning, CT_TEXT("Unsupported material normal map format."));
            break;
        }
    }
    SetFlags(SetMaterialNormalMode(data.flags, mode));
}

void Material::SetBaseTexture(const SPtr<Texture> &texture)
{
    if (resources.baseTexture != texture)
    {
        resources.baseTexture = texture;
        UpdateBaseColorType();

        // TODO alpha mode
    }
}

void Material::SetSpecularTexture(const SPtr<Texture> &texture)
{
    if (resources.specularTexture != texture)
    {
        resources.specularTexture = texture;
        UpdateSpecularType();
    }
}

void Material::SetEmissiveTexture(const SPtr<Texture> &texture)
{
    if (resources.emissiveTexture != texture)
    {
        resources.emissiveTexture = texture;
        UpdateEmissiveType();
    }
}

void Material::SetNormalTexture(const SPtr<Texture> &texture)
{
    if (resources.normalTexture != texture)
    {
        resources.normalTexture = texture;
        UpdateNormalType();
    }
}

void Material::SetOcclusionTexture(const SPtr<Texture> &texture)
{
    if (resources.occlusionTexture != texture)
    {
        resources.occlusionTexture = texture;
        UpdateOcclusonType();
    }
}

void Material::SetSampler(const SPtr<Sampler> &sampler)
{
    if (resources.samplerState != sampler)
    {
        resources.samplerState = sampler;
    }
}

void Material::SetBaseColor(const Color &color)
{
    if (data.base != color)
    {
        data.base = color;
        UpdateBaseColorType();
    }
}

void Material::SetSpecularColor(const Color &color)
{
    if (data.specular != color)
    {
        data.specular = color;
        UpdateSpecularType();
    }
}

void Material::SetEmissiveColor(const Color &color)
{
    if (data.emissive != color)
    {
        data.emissive = color;
        UpdateEmissiveType();
    }
}

void Material::SetAlphaThreshold(float value)
{
    if (data.alphaThreshold != value)
    {
        data.alphaThreshold = value;
    }
}

void Material::SetIndexOfRefraction(float value)
{
    if (data.IoR != value)
    {
        data.IoR = value;
    }
}

void Material::SetSpecularTransmission(float value)
{
    if (data.specularTransmission != value)
    {
        data.specularTransmission = value;
    }
}

void Material::SetAlphaMode(int32 alphaMode)
{
    //TODO
}

void Material::SetDoubleSided(bool value)
{
    SetFlags(SetMaterialBit(data.flags, CT_MAT_DOUBLE_SIDED, value));
}