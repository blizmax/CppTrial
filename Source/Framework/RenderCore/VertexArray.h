#pragma once

#include "RenderCore/Buffer.h"
#include "RenderCore/VertexLayout.h"

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

    const SPtr<Buffer> &GetVertexBuffer(int32 index) const
    {
        return vertexBuffers[index];
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

    ResourceFormat GetIndexBufferFormat() const
    {
        return indexBufferFormat;
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
    ResourceFormat indexBufferFormat = ResourceFormat::R32UInt;
};