#pragma once

#include "Render/.Package.h"
#include "RenderCore/Texture.h"

struct MaterialResources
{
    SPtr<Texture> baseTexture;
    SPtr<Texture> specularTexture;
    SPtr<Texture> emissiveTexture;
    SPtr<Texture> normalTexture;
    SPtr<Texture> occlusionTexture;
    SPtr<Sampler> samplerState;
};

class Material
{
public:
    Material() = default;

    void SetBaseTexture(const SPtr<Texture> &texture);
    void SetSpecularTexture(const SPtr<Texture> &texture);
    void SetEmissiveTexture(const SPtr<Texture> &texture);
    void SetNormalTexture(const SPtr<Texture> &texture);
    void SetOcclusionTexture(const SPtr<Texture> &texture);
    void SetSampler(const SPtr<Sampler> &sampler);

    void SetBaseColor(const Color &color);
    void SetSpecularColor(const Color &color);
    void SetEmissiveColor(const Color &color);
    void SetAlphaThreshold(float value);
    void SetIndexOfRefraction(float value);
    void SetSpecularTransmission(float value);
    void SetAlphaMode(int32 alphaMode);
    void SetDoubleSided(bool value);
    void SetShadingModel(int32 model);

    const SPtr<Texture> &GetBaseColorTexture() const
    {
        return resources.baseTexture;
    }

    const SPtr<Texture> &GetSpecularTexture() const
    {
        return resources.specularTexture;
    }

    const SPtr<Texture> &GetEmissiveTexture() const
    {
        return resources.emissiveTexture;
    }

    const SPtr<Texture> &GetNormalTexture() const
    {
        return resources.normalTexture;
    }

    const SPtr<Texture> &GetOcclusionTexture() const
    {
        return resources.occlusionTexture;
    }

    const SPtr<Sampler> GetSampler() const
    {
        return resources.samplerState;
    }

    Color GetBaseColor() const
    {
        return data.base;
    }

    Color GetSpecularColor() const
    {
        return data.specular;
    }

    Color GetEmissiveColor() const
    {
        return data.emissive;
    }

    float GetAlphaThreshold() const
    {
        return data.alphaThreshold;
    }

    float GetIndexOfRefraction() const
    {
        return data.IoR;
    }

    float GetSpecularTransmission() const
    {
        return data.specularTransmission;
    }

    bool IsDoubleSided() const
    {
        return GetMaterialBit(data.flags, CT_MAT_DOUBLE_SIDED) > 0;
    }

    int32 GetShadingModel() const
    {
        return GetMaterialShadingModel(data.flags);
    }

    int32 GetFlags() const
    {
        return data.flags;
    }

    const MaterialData &GetData() const
    {
        return data;
    }

    const MaterialResources &GetResources() const
    {
        return resources;
    }

    const String &GetName() const
    {
        return name;
    }

    void SetName(const String &newName)
    {
        name = newName;
    }

    static SPtr<Material> Create();

private:
    void SetFlags(int32 flags);
    void UpdateBaseColorType();
    void UpdateSpecularType();
    void UpdateEmissiveType();
    void UpdateOcclusonType();
    void UpdateNormalType();

private:
    String name;
    MaterialData data{};
    MaterialResources resources{};
};