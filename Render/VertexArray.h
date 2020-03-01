#pragma once

#include "Render/.Package.h"
#include "Render/VertexBuffer.h"
#include "Render/IndexBuffer.h"

class VertexArray
{
public:
    virtual ~VertexArray() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void AddVertexBuffer(const SPtr<VertexBuffer> &buffer) = 0;
    virtual void SetIndexBuffer(const SPtr<IndexBuffer> &buffer) = 0;
    virtual const Array<SPtr<VertexBuffer>> &GetVertexBuffers() const = 0;
    virtual const SPtr<IndexBuffer> &GetIndexBuffer() const = 0;

    static SPtr<VertexArray> Create();
};