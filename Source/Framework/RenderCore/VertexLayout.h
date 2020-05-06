#pragma once

#include "RenderCore/.Package.h"

namespace RenderCore
{
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
        size = GetVertexDataSize(dataType);
    }
};

class VertexBufferLayout
{
public:
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

    static SPtr<VertexBufferLayout> Create(std::initializer_list<VertexAttribute> initList)
    {
        return Memory::MakeShared<VertexBufferLayout>(initList);
    }

private:
    Array<VertexAttribute> attributes;
    uint32 stride = 0;
};

class VertexLayout
{
public:
    virtual ~VertexLayout() = default;

    virtual void AddBufferLayout(const SPtr<VertexBufferLayout> &layout) = 0;
    virtual const Array<SPtr<VertexBufferLayout>> &GetBufferLayouts() const = 0;

    static SPtr<VertexLayout> Create();
};

}