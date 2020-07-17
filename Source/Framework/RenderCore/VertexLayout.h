#pragma once

#include "RenderCore/.Package.h"

class VertexBufferLayout
{
public:
    struct Element
    {
        String name;
        ResourceFormat format;
        uint32 size;
        uint32 offset;
        uint32 arrayLength;

        Element(const String &name, ResourceFormat format, uint32 arrayLength = 1)
            : name(name), format(format), arrayLength(arrayLength), offset(0)
        {
            size = GetResourceFormatBytes(format) * arrayLength;
        }
    };

    VertexBufferLayout(std::initializer_list<Element> initList, bool perInstanceData)
        : elements(initList), perInstanceData(perInstanceData)
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

    bool IsPerInstanceData() const
    {
        return perInstanceData;
    }

    const Array<Element> &GetElements() const
    {
        return elements;
    }

    static SPtr<VertexBufferLayout> Create(std::initializer_list<Element> initList, bool perInstanceData = false)
    {
        return Memory::MakeShared<VertexBufferLayout>(initList, perInstanceData);
    }

private:
    Array<Element> elements;
    uint32 stride = 0;
    bool perInstanceData = false;
};

class VertexLayout
{
public:
    virtual ~VertexLayout() = default;

    void AddBufferLayout(const SPtr<VertexBufferLayout> &layout)
    {
        bufferLayouts.Add(layout);
    }

    const Array<SPtr<VertexBufferLayout>> &GetBufferLayouts() const
    {
        return bufferLayouts;
    }

    static SPtr<VertexLayout> Create()
    {
        return Memory::MakeShared<VertexLayout>();
    }

private:
    Array<SPtr<VertexBufferLayout>> bufferLayouts;
};