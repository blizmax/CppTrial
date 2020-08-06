#pragma once

#include "Render/RenderGraph/.Package.h"
#include "Render/RenderGraph/RenderPassReflection.h"

class RenderGraphResourceCache
{
public:
    struct DefaultProperties
    {
        int32 width = 0;
        int32 height = 0;
        ResourceFormat format = ResourceFormat::Unknown;
    };

    void Reset();
    void RegisterExternalResource(const String &name, const SPtr<Resource> &resource);
    void RegisterField(const String &name, const RenderPassReflection::Field &field);
    SPtr<Resource> GetResource(const String &name) const;
    const RenderPassReflection::Field &GetResourceReflection(const String &name) const;
    void AllocateResources(const DefaultProperties &props);

    static SPtr<RenderGraphResourceCache> Create();

private:
    struct ResourceData
    {
        RenderPassReflection::Field field;
        SPtr<Resource> resource;
        bool resolveBindFlags;
    };

private:
    Array<ResourceData> resourceDatas;
    HashMap<String, int32> nameToIndex;
    HashMap<String, SPtr<Resource>> externalResources;
};