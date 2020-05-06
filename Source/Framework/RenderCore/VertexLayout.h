#pragma once

#include "RenderCore/.Package.h"

namespace RenderCore
{
class VertexBufferLayout
{
public:
    struct Element
    {
        String name;
        VertexDataType dataType;
        uint32 size;
        uint32 offset;
        bool normalized;

        Element(const String &name, VertexDataType dataType, bool normalized = false)
            : name(name), dataType(dataType), normalized(normalized), offset(0)
        {
            size = GetVertexDataSize(dataType);
        }
    };

    VertexBufferLayout(std::initializer_list<Element> initList) : elements(initList)
    {
        uint32 offset = 0;
        for (auto &e : elements)
        {
            e.offset = offset;
            offset += e.size;
        }
        stride = offset;
    }

    uint32 GetStride() const
    {
        return stride;
    }

    const Array<Element> &GetElements() const
    {
        return elements;
    }

    static SPtr<VertexBufferLayout> Create(std::initializer_list<Element> initList)
    {
        return Memory::MakeShared<VertexBufferLayout>(initList);
    }

private:
    Array<Element> elements;
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