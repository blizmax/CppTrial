#pragma once

#include "RenderCore/.Package.h"

namespace RenderCore
{

struct IndexBufferCreateParams
{
    uint32 indexCount;
    uint32 size;
    GpuBufferUsage usage = GpuBufferUsage::Static;
};

class IndexBuffer
{
public:
    virtual ~IndexBuffer() = default;

    virtual void *Map() = 0;
    virtual void Unmap() = 0;
    virtual uint32 GetSize() const = 0;

    static SPtr<IndexBuffer> Create(const IndexBufferCreateParams &params);
};

}