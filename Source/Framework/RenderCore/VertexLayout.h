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
        ResourceFormat format;
        uint32 size;
        uint32 offset;
        uint32 arrayLayers;

        Element(const String &name, ResourceFormat format, uint32 arrayLayers = 1)
            : name(name), format(format), arrayLayers(arrayLayers), offset(0)
        {
            size = GetResourceFormatBytes(format) * arrayLayers;
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