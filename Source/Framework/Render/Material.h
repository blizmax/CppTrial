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

struct MaterialData
{
    Color base{1.0f, 1.0f, 1.0f, 1.0f};     //RGB: Base color,     A: Transparency
    Color specular{0.0f, 0.0f, 0.0f, 0.0f}; //RGB: Specular,       A: Gloss
    Color emissive{0.0f, 0.0f, 0.0f, 1.0f}; //RGB: Emissive color, A: Factor

    float alphaThreshold = 0.5f;
    float ior = 1.5f;
    float _pad = 0.0f;
    float _pad1 = 0.0f;
};

class Material
{
public:
    Material(const String &name);

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
        return data.ior;
    }

    const String &GetName() const
    {
        return name;
    }

    const MaterialData &GetData() const
    {
        return data;
    }

    const MaterialResources &GetResources() const
    {
        return resources;
    }

    static SPtr<Material> Create(const String &name);

private:
    String name;
    MaterialData data;
    MaterialResources resources;
};