#pragma once

#include "RenderCore/RootSignature.h"

namespace RenderCore
{

class CopyContext;
class ComputeContext;
class RenderContext;

class ProgramVars
{
public:
    virtual ~ProgramVars() = default;

protected:
    bool PrepareDescriptorSets(CopyContext *ctx)
    {
        return true;
    }
};

class GraphicsVars : public ProgramVars
{
public:
    bool Apply(RenderContext *ctx, RootSignature *rootSignature)
    {
        return true;
    }

    static SPtr<GraphicsVars> Create();
};

class ComputeVars : public ProgramVars
{
public:
    //bool Apply(ComputeContext *ctx, RootSignature *rootSignature);
};

}