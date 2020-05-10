#pragma once

#include "RenderCore/.Package.h"

namespace RenderCore
{

class Resource;

struct ResourceViewInfo
{
    uint32 firstElement = 0;
    uint32 elementCount = UINT32_MAX;

    uint32 mostDetailedMip = 0;
    uint32 mipLevels = UINT32_MAX;
    uint32 firstArraySlice = 0;
    uint32 arrayLayers = UINT32_MAX;

    ResourceViewInfo(uint32 firstElement, uint32 elementCount)
        : firstElement(firstElement), elementCount(elementCount)
    {
    }

    ResourceViewInfo(uint32 mostDetailedMip, uint32 mipLevels, uint32 firstArraySlice, uint32 arrayLayers)
        : mostDetailedMip(mostDetailedMip), mipLevels(mipLevels), firstArraySlice(firstArraySlice), arrayLayers(arrayLayers)
    {
    }
};

class ResourceView
{
public:
    virtual ~ResourceView() = default;

    const ResourceViewInfo &GetViewInfo() const
    {
        return viewInfo;
    }

    Resource *GetResource() const
    {
        return resource.lock().get();
    }

    //static SPtr<ResourceView> CreateSrv();

protected:
    ResourceViewInfo viewInfo;
    WPtr<Resource> resource;
};
}