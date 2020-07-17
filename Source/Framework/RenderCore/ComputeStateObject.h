#pragma once

#include "RenderCore/RootSignature.h"

struct ComputeStateObjectDesc
{
    SPtr<RootSignature> rootSignature;
};

class ComputeStateObject
{
public:
    virtual ~ComputeStateObject() = default;

    const ComputeStateObjectDesc &GetDesc() const
    {
        return desc;
    }

    static SPtr<ComputeStateObject> Create(const ComputeStateObjectDesc &desc);

protected:
    ComputeStateObject(const ComputeStateObjectDesc &desc)
        : desc(desc)
    {
    }

protected:
    ComputeStateObjectDesc desc;
};