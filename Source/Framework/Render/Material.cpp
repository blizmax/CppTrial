#include "Render/Material.h"

SPtr<Material> Material::Create(const String &name)
{
    return Memory::MakeShared<Material>(name);
}

Material::Material(const String &name) : name(name)
{   
}

void Material::SetBaseTexture(const SPtr<Texture> &texture)
{
    if (resources.baseTexture != texture)
    {
        resources.baseTexture = texture;
    }
}

void Material::SetSpecularTexture(const SPtr<Texture> &texture)
{
    if (resources.specularTexture != texture)
    {
        resources.specularTexture = texture;
    }
}

void Material::SetEmissiveTexture(const SPtr<Texture> &texture)
{
    if (resources.emissiveTexture != texture)
    {
        resources.emissiveTexture = texture;
    }
}

void Material::SetNormalTexture(const SPtr<Texture> &texture)
{
    if (resources.normalTexture != texture)
    {
        resources.normalTexture = texture;
    }
}

void Material::SetOcclusionTexture(const SPtr<Texture> &texture)
{
    if (resources.occlusionTexture != texture)
    {
        resources.occlusionTexture = texture;
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
    }
}

void Material::SetSpecularColor(const Color &color)
{
    if (data.specular != color)
    {
        data.specular = color;
    }
}

void Material::SetEmissiveColor(const Color &color)
{
    if (data.emissive != color)
    {
        data.emissive = color;
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
    if (data.ior != value)
    {
        data.ior = value;
    }
}