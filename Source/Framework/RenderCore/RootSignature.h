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
    virtual ~RootSignature() = default;

    const RootSignatureDesc &GetDesc() const
    {
        return desc;
    }

    SPtr<RootSignature> Create(const RootSignatureDesc &desc);

protected:
    RootSignature(const RootSignatureDesc &desc) : desc(desc)
    {
    }

    RootSignatureDesc desc;
};
}