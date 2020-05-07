#pragma once

#include "RenderCore/.Package.h"

namespace RenderCore
{

class Resource
{
public:
    Resource(ResourceType type, uint32 size) : resourceType(type), size(size)
    {
    }

    virtual ~Resource() = default;

    uint32 GetSize() const
    {
        return size;
    }

    ResourceType GetResourceType() const
    {
        return resourceType;
    }

protected:
    ResourceType resourceType;
    uint32 size;

};
}