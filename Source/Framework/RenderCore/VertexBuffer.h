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

    static SPtr<VertexBuffer> Create(const VertexBufferCreateParams &params);
};

}