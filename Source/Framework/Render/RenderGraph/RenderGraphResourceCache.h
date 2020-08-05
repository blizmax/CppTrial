#pragma once

#include "Render/RenderGraph/.Package.h"
#include "Render/RenderGraph/RenderPassReflection.h"

class RenderGraphResourceCache
{
public:
    void RegisterExternalResource(const String &name, const SPtr<Resource> &resource);
    void RegisterField(const String &name, const RenderPassReflection::Field &field);
    
    SPtr<Resource> GetResource(const String &name) const;

    static SPtr<RenderGraphResourceCache> Create();

private:
    struct ResourceData
    {
        SPtr<Resource> resource;
        bool resolveBindFlags;
    };

    Array<ResourceData> resourceDatas;
    HashMap<String, int32> nameToIndex;

    HashMap<String, SPtr<Resource>> externalResources;
};