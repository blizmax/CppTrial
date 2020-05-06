#pragma once

#include "RenderCore/VertexLayout.h"
#include "RenderCore/VertexBuffer.h"
#include "RenderCore/IndexBuffer.h"

namespace RenderCore
{
class VertexArray
{
public:
    VertexArray();
    virtual ~VertexArray() = default;

    void SetVertexLayout(const SPtr<VertexLayout> &layout)
    {
        vertexLayout = layout;
    }

    void AddVertexBuffer(const SPtr<VertexBuffer> &buffer)
    {
        vertexBuffers.Add(buffer);
    }

    void SetIndexBuffer(const SPtr<IndexBuffer> &buffer)
    {
        indexBuffer = buffer;
    }

    void SetTopology(Topology t)
    {
        topology = t;
    }

    const SPtr<VertexLayout> &GetVertexLayout() const
    {
        return vertexLayout;
    }

    const Array<SPtr<VertexBuffer>> &GetVertexBuffers() const
    {
        return vertexBuffers;
    }

    const SPtr<IndexBuffer> &GetIndexBuffer() const
    {
        return indexBuffer;
    }

    Topology GetTopology() const
    {
        return topology;
    }

    static SPtr<VertexArray> Create();

private:
    SPtr<VertexLayout> vertexLayout;
    Array<SPtr<VertexBuffer>> vertexBuffers;
    SPtr<IndexBuffer> indexBuffer;
    Topology topology = Topology::TriangleList;
};
}