#pragma once

#include "RenderCore/.Package.h"

namespace RenderCore
{

struct VertexBufferCreateParams
{
    uint32 vertexCount;
    uint32 size;
    GpuBufferUsage usage = GpuBufferUsage::Static;
};

class VertexBuffer
{
public:
    virtual ~VertexBuffer() = default;

    virtual void *Map() = 0;
    virtual void Unmap() = 0;
    virtual uint32 GetSize() const = 0;
    virtual GpuBufferUsage GetUsage() const = 0;

    static SPtr<VertexBuffer> Create(const VertexBufferCreateParams &params);
};

}