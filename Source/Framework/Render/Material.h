#pragma once

#include "Render/.Package.h"
#include "RenderCore/Texture.h"
#include "Assets/AssetPtr.h"

struct MaterialResources
{
    APtr<Texture> baseTexture;
    APtr<Texture> specularTexture;
    APtr<Texture> emissiveTexture;
    APtr<Texture> normalTexture;
    APtr<Texture> occlusionTexture;
    SPtr<Sampler> samplerState;
};

class Material
{
public:
    Material() = default;

    bool Update();

    void SetBaseTexture(const APtr<Texture> &texture);
    void SetSpecularTexture(const APtr<Texture> &texture);
    void SetEmissiveTexture(const APtr<Texture> &texture);
    void SetNormalTexture(const APtr<Texture> &texture);
    void SetOcclusionTexture(const APtr<Texture> &texture);
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

    SPtr<Texture> GetBaseColorTexture() const
    {
        return resources.baseTexture;
    }

    SPtr<Texture> GetSpecularTexture() const
    {
        return resources.specularTexture;
    }

    SPtr<Texture> GetEmissiveTexture() const
    {
        return resources.emissiveTexture;
    }

    SPtr<Texture> GetNormalTexture() const
    {
        return resources.normalTexture;
    }

    SPtr<Texture> GetOcclusionTexture() const
    {
        return resources.occlusionTexture;
    }

    SPtr<Sampler> GetSampler() const
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
    void MarkDirty();

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
    bool dirty = true;
    bool changedSinceLastUpdate = false;
};