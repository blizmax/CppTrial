#pragma once

#include "RenderVulkan/VulkanCopyContext.h"
#include "RenderCore/ComputeContext.h"

namespace RenderCore
{
class VulkanComputeContext : public VulkanCopyContext, public ComputeContext
{
public:
    VulkanComputeContext(const SPtr<GpuQueue> &queue);
    virtual ~VulkanComputeContext();

    virtual void Flush(bool wait) override;

    virtual void Dispatch(ComputeState *state, ComputeVars *vars, uint32 sizeX, uint32 sizeY, uint32 sizeZ) override;
    virtual void DispatchIndirect(ComputeState *state, ComputeVars *vars, const Buffer *argBuffer, uint32 argBufferOffset) override;
    virtual void ClearUav(const ResourceView *uav, const Vector4&value) override;
    virtual void ClearUav(const ResourceView *uav, const UVector4 &value) override;
    virtual void ClearUavCounter(const Buffer *buffer, uint32 value) override;

protected:
    void ClearColorImage(const ResourceView *view, float v0, float v1, float v2, float v3);
    void ClearColorImage(const ResourceView *view, uint32 v0, uint32 v1, uint32 v2, uint32 v3);

private:
    bool PrepareForDispatch(ComputeState *state, ComputeVars *vars);
    
};

}