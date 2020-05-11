#pragma once

#include "RenderCore/.Package.h"

namespace RenderCore
{

class Resource;
class Buffer;
class Texture;

struct ResourceViewInfo
{
    uint32 firstElement = 0;
    uint32 elementCount = UINT32_MAX;

    uint32 mostDetailedMip = 0;
    uint32 mipLevels = 1;
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

    bool operator==(const ResourceViewInfo &other) const
    {
        return firstElement == other.firstElement
            && elementCount == other.elementCount
            && mostDetailedMip == other.mostDetailedMip
            && mipLevels == other.mipLevels
            && firstArraySlice == other.firstArraySlice
            && arrayLayers == other.arrayLayers;
    }

};

class ResourceView
{
public:
    ResourceView(const WPtr<Resource> &resource, uint32 firstElement, uint32 elementCount)
        :resource(resource), viewInfo(firstElement, elementCount)
    {
    }

    virtual ~ResourceView() = default;

    const ResourceViewInfo &GetViewInfo() const
    {
        return viewInfo;
    }

    Resource *GetResource() const
    {
        return resource.lock().get();
    }

    static SPtr<ResourceView> CreateSrv(const SPtr<Buffer> &buffer, uint32 first, uint32 count);
    static SPtr<ResourceView> CreateUav(const SPtr<Buffer> &buffer, uint32 first, uint32 count);
    static SPtr<ResourceView> CreateCbv(const SPtr<Buffer> &buffer);

protected:
    ResourceViewInfo viewInfo;
    WPtr<Resource> resource;
};

}