#pragma once

#include "Render/.Package.h"

struct VertexAttribute
{
    String name;
    VertexDataType dataType;
    uint32 size;
    uint32 offset;
    bool normalized;

    VertexAttribute(const String &name, VertexDataType dataType, bool normalized = false)
        : name(name), dataType(dataType), normalized(normalized), offset(0)
    {
        size = Render::GetVertexDataSize(dataType);
    }
};

class VertexBufferLayout
{
public:
    VertexBufferLayout() = default;

    VertexBufferLayout(std::initializer_list<VertexAttribute> initList) : attributes(initList)
    {
        uint32 offset = 0;
        for (auto &attr : attributes)
        {
            attr.offset = offset;
            offset += attr.size;
        }
        stride = offset;
    }

    uint32 GetStride() const
    {
        return stride;
    }

    const Array<VertexAttribute> &GetAttributes() const
    {
        return attributes;
    }

private:
    Array<VertexAttribute> attributes;
    uint32 stride = 0;
};

class VertexBuffer
{
public:
    virtual ~VertexBuffer() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
    virtual void SetData(float *vertices, uint32 size) = 0;

    virtual void SetLayout(const VertexBufferLayout &layout) = 0;
    virtual const VertexBufferLayout &GetLayout() const = 0;

    static SPtr<VertexBuffer> Create(float *vertices, uint32 size, GpuBufferUsage usage = GpuBufferUsage::Static);
};