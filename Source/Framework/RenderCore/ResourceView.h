#pragma once

#include "RenderCore/.Package.h"

namespace RenderCore
{

class Resource;
class Buffer;
class Texture;

struct ResourceViewInfo
{
    int32 firstElement = 0;
    int32 elementCount = -1;

    int32 mostDetailedMip = 0;
    int32 mipLevels = -1;
    int32 firstArraySlice = 0;
    int32 arrayLayers = -1;

    ResourceViewInfo() = default;

    ResourceViewInfo(int32 firstElement, int32 elementCount)
        : firstElement(firstElement), elementCount(elementCount)
    {
    }

    ResourceViewInfo(int32 mostDetailedMip, int32 mipLevels, int32 firstArraySlice, int32 arrayLayers)
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
    ResourceView(const WPtr<Resource> &resource, int32 firstElement, int32 elementCount)
        :resource(resource), viewInfo(firstElement, elementCount)
    {
    }

    ResourceView(const WPtr<Resource> &resource, int32 mostDetailedMip, int32 mipLevels, int32 firstArraySlice, int32 arrayLayers)
        : resource(resource), viewInfo(mostDetailedMip, mipLevels, firstArraySlice, arrayLayers)
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

    static SPtr<ResourceView> CreateSrv(const SPtr<Buffer> &buffer, int32 first, int32 count);
    static SPtr<ResourceView> CreateUav(const SPtr<Buffer> &buffer, int32 first, int32 count);
    static SPtr<ResourceView> CreateCbv(const SPtr<Buffer> &buffer);

    static SPtr<ResourceView> CreateSrv(const SPtr<Texture> &texture, int32 mostDetailedMip, int32 mipLevels, int32 firstArraySlice, int32 arrayLayers);
    static SPtr<ResourceView> CreateUav(const SPtr<Texture> &texture, int32 mipLevel, int32 firstArraySlice, int32 arrayLayers);
    static SPtr<ResourceView> CreateRtv(const SPtr<Texture> &texture, int32 mipLevel, int32 firstArraySlice, int32 arrayLayers);
    static SPtr<ResourceView> CreateDsv(const SPtr<Texture> &texture, int32 mipLevel, int32 firstArraySlice, int32 arrayLayers);

protected:
    ResourceViewInfo viewInfo;
    WPtr<Resource> resource;
};

}