#pragma once

#include "Render/OpenGL/.Package.h"
#include "Render/VertexArray.h"

class GLVertexArray : public VertexArray
{
public:
    GLVertexArray();
    virtual ~GLVertexArray();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual void AddVertexBuffer(const SPtr<VertexBuffer> &buffer) override;
    virtual void SetIndexBuffer(const SPtr<IndexBuffer> &buffer) override;

    virtual const Array<SPtr<VertexBuffer>> &GetVertexBuffers() const override
    {
        return vertexBuffers;
    }

    virtual const SPtr<IndexBuffer> &GetIndexBuffer() const override
    {
        return indexBuffer;
    }

private:
    uint32 id = 0;
    uint32 vertexBufferIndex = 0;
    Array<SPtr<VertexBuffer>> vertexBuffers;
    SPtr<IndexBuffer> indexBuffer;
};