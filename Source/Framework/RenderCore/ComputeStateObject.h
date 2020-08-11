#pragma once

#include "RenderCore/Program.h"
#include "RenderCore/RootSignature.h"

struct ComputeStateObjectDesc
{
    SPtr<ProgramKernel> programKernel;
    SPtr<RootSignature> rootSignature;

    bool operator==(const ComputeStateObjectDesc &other) const
    {
        if (programKernel != other.programKernel)
            return false;
        if (rootSignature != other.rootSignature)
            return false;
        return true;
    }

    bool operator!=(const ComputeStateObjectDesc &other) const
    {
        return !(*this == other);
    }
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