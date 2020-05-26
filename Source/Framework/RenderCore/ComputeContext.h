#pragma once

#include "RenderCore/CopyContext.h"
#include "RenderCore/ComputeState.h"
#include "RenderCore/ProgramVars.h"
#include "Math/Vector4.h"

namespace RenderCore
{

class ComputeContext : public CopyContext
{
public:
    virtual ~ComputeContext() = default;

    virtual void Dispatch(ComputeState *state, ComputeVars *vars, uint32 sizeX, uint32 sizeY, uint32 sizeZ) = 0;
    virtual void DispatchIndirect(ComputeState *state, ComputeVars *vars, const Buffer *argBuffer, uint32 argBufferOffset) = 0;
    virtual void ClearUav(const ResourceView *uav, const Vector4 &value) = 0;
    virtual void ClearUav(const ResourceView *uav, const UVector4 &value) = 0;
    virtual void ClearUavCounter(const Buffer *buffer, uint32 value) = 0;

    static SPtr<ComputeContext> Create(const SPtr<GpuQueue> &queue);
};

}