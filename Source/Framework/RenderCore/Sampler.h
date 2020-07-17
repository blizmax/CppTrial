#pragma once

#include "RenderCore/.Package.h"

struct SamplerDesc
{
    TextureFilter magFilter = TextureFilter::Linear;
    TextureFilter minFilter = TextureFilter::Linear;
    TextureFilter mipFilter = TextureFilter::Linear;
    uint32 maxAnisotropy = 1;
    float maxLod = 1000.0f;
    float minLod = -1000.0f;
    float lodBias = 0.0f;
    CompareOperation compareOp = CompareOperation::AlwaysPass;
    TextureWrap uWrap = TextureWrap::Repeat;
    TextureWrap vWrap = TextureWrap::Repeat;
    TextureWrap wWrap = TextureWrap::Repeat;
    Color borderColor = Color::BLACK;
};

class Sampler
{
public:
    Sampler(const SamplerDesc &desc)
        : desc(desc)
    {
    }

    virtual ~Sampler() = default;

    const SamplerDesc &GetDesc() const
    {
        return desc;
    }

    static SPtr<Sampler> Create(const SamplerDesc &desc);
    static SPtr<Sampler> GetDefault();

protected:
    SamplerDesc desc;
};