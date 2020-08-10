#pragma once

#include "Render/RenderGraph/.Package.h"

class RenderGraphResourceCache
{
public:
    void Reset();
    void RegisterExternalResource(const String &name, const SPtr<Resource> &resource);
    void RegisterField(const String &name, const RenderPassReflection::Field &field);
    void RegisterFieldAlias(const String &oriName, const String &alias, const RenderPassReflection::Field &field);
    SPtr<Resource> GetResource(const String &name) const;
    const RenderPassReflection::Field &GetResourceReflection(const String &name) const;
    void AllocateResources(const RenderGraph::ResourceProperties &defaultProps);

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