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
        size = GetDataSize();
    }

    uint32 GetDataSize() const
    {
        switch (dataType)
        {
        case VertexDataType::Float1:
            return 4;
        case VertexDataType::Float2:
            return 4 * 2;
        case VertexDataType::Float3:
            return 4 * 3;
        case VertexDataType::Float4:
            return 4 * 4;
        case VertexDataType::Int1:
            return 4;
        case VertexDataType::Int2:
            return 4 * 2;
        case VertexDataType::Int3:
            return 4 * 3;
        case VertexDataType::Int4:
            return 4 * 4;
        }

        CT_EXCEPTION(Render, "Unknown data type!");
        return 0;
    }

    uint32 GetComponentCount() const
    {
        switch (dataType)
        {
        case VertexDataType::Float1:
            return 1;
        case VertexDataType::Float2:
            return 2;
        case VertexDataType::Float3:
            return 3;
        case VertexDataType::Float4:
            return 4;
        case VertexDataType::Int1:
            return 1;
        case VertexDataType::Int2:
            return 2;
        case VertexDataType::Int3:
            return 3;
        case VertexDataType::Int4:
            return 4;
        }

        CT_EXCEPTION(Render, "Unknown data type!");
        return 0;
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

    virtual void SetLayout(const VertexBufferLayout &layout) = 0;
    virtual const VertexBufferLayout &GetLayout() const = 0;

    static SPtr<VertexBuffer> Create(float *vertices, uint32 size);
};