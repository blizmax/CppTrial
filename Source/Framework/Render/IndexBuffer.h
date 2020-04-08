#pragma once

#include "Render/.Package.h"

class IndexBuffer
{
public:
    virtual ~IndexBuffer() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
    virtual void SetData(uint32 *indices, uint32 count) = 0;

    virtual uint32 GetCount() const = 0;

    static SPtr<IndexBuffer> Create(uint32 *indices, uint32 count, GpuBufferUsage usage = GpuBufferUsage::Static);
};