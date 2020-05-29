#pragma once

#include "RenderCore/VertexLayout.h"
#include "RenderCore/Buffer.h"

namespace RenderCore
{
class VertexArray
{
public:
    virtual ~VertexArray() = default;

    void SetVertexLayout(const SPtr<VertexLayout> &layout)
    {
        vertexLayout = layout;
    }

    void AddVertexBuffer(const SPtr<Buffer> &buffer)
    {
        vertexBuffers.Add(buffer);
    }

    void SetIndexBuffer(const SPtr<Buffer> &buffer)
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

    const Array<SPtr<Buffer>> &GetVertexBuffers() const
    {
        return vertexBuffers;
    }

    const SPtr<Buffer> &GetIndexBuffer() const
    {
        return indexBuffer;
    }

    Topology GetTopology() const
    {
        return topology;
    }

    static SPtr<VertexArray> Create()
    {
        return Memory::MakeShared<VertexArray>();
    }

private:
    SPtr<VertexLayout> vertexLayout;
    Array<SPtr<Buffer>> vertexBuffers;
    SPtr<Buffer> indexBuffer;
    Topology topology = Topology::TriangleList;
};
}