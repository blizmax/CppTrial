#pragma once

#include "RenderCore/DescriptorSetLayout.h"

namespace RenderCore
{

struct RootSignatureDesc
{
    Array<SPtr<DescriptorSetLayout>> layouts;
};

class RootSignature
{
public:
    RootSignature(const RootSignatureDesc &desc) : desc(desc)
    {
    }

    virtual ~RootSignature() = default;

    const RootSignatureDesc &GetDesc() const
    {
        return desc;
    }

    static SPtr<RootSignature> Create(const RootSignatureDesc &desc);

protected:
    RootSignatureDesc desc;
};
}