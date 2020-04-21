#pragma once

#include "RenderCore/RenderPipelineState.h"
#include "RenderCore/VertexBuffer.h"
#include "RenderCore/IndexBuffer.h"

namespace RenderCore
{
class RenderAPI
{
public:
    virtual ~RenderAPI() = default;

    virtual void Init() = 0;
    virtual void Destroy() = 0;

    virtual void SetViewport(int32 x, int32 y, uint32 width, uint32 height) = 0;
    virtual void SetScissor(int32 x, int32 y, uint32 width, uint32 height) = 0;
    virtual void SetStencilRef(uint32 ref) = 0;
    virtual void SetRenderPipeline(const SPtr<RenderPipelineState> &pipeline) = 0;
    virtual void SetVertexBuffers(uint32 startIndex, SPtr<VertexBuffer> *buffers, uint32 count) = 0;
    virtual void SetIndexBuffer(const SPtr<IndexBuffer> &buffer) = 0;

    static RenderAPI &Get();
};
}