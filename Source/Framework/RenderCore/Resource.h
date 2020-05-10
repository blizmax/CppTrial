#pragma once

#include "RenderCore/ResourceView.h"

namespace RenderCore
{

class Resource
{
public:
    Resource(ResourceType type, ResourceBindFlags bindFlags, uint32 size)
        : resourceType(type), bindFlags(bindFlags), size(size)
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

    ResourceBindFlags GetBindFlags() const
    {
        return bindFlags;
    }

    virtual SPtr<ResourceView> GetSrv() = 0;
    virtual SPtr<ResourceView> GetUav() = 0;

protected:
    ResourceType resourceType;
    ResourceBindFlags bindFlags = 0;
    uint32 size = 0;

};
}