#pragma once

#include "RenderCore/RenderAPI.h"

namespace RenderCore
{
class VulkanRenderAPI : public RenderAPI
{
public:
    virtual void Init() override;
    virtual void Destroy() override;

    virtual void SetViewport(int32 x, int32 y, uint32 width, uint32 height) override;
    virtual void SetScissor(int32 x, int32 y, uint32 width, uint32 height) override;
    virtual void SetStencilRef(uint32 ref) override;

    virtual void SetRenderPipeline(const SPtr<RenderPipelineState> &pipeline) override;
    virtual void SetVertexBuffers(uint32 startIndex, SPtr<VertexBuffer> *buffers, uint32 count) override;
    virtual void SetIndexBuffer(const SPtr<IndexBuffer> &buffer) override;

};   
}