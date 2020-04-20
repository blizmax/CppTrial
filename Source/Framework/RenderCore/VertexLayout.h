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

struct VertexLayoutCreateParams
{
    Array<VertexAttribute> attributes;
    uint32 stride = 0;

    VertexLayoutCreateParams() = default;

    VertexLayoutCreateParams(std::initializer_list<VertexAttribute> initList)
        : attributes(initList)
    {
        uint32 offset = 0;
        for (auto &attr : attributes)
        {
            attr.offset = offset;
            offset += attr.size;
        }
        stride = offset;
    }
};

class VertexLayout
{
public:
    virtual ~VertexLayout() = default;

    virtual uint32 GetStride() const = 0;
    virtual const Array<VertexAttribute> &GetAttributes() const = 0;

    static SPtr<VertexLayout> Create(const VertexLayoutCreateParams &params);
};

}