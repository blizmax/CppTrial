#pragma once

#include "RenderCore/.Package.h"

namespace RenderCore
{

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
    //Color borderColor;
};

class Sampler
{
public:
    virtual ~Sampler() = default;

    static SPtr<Sampler> Create(const SamplerDesc &desc);
};

}