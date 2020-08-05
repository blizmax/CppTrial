#include "Render/RenderGraph/RenderGraphResourceCache.h"

SPtr<RenderGraphResourceCache> RenderGraphResourceCache::Create()
{
    return Memory::MakeShared<RenderGraphResourceCache>();
}

void RenderGraphResourceCache::RegisterExternalResource(const String &name, const SPtr<Resource> &resource)
{
    externalResources.Put(name, resource);
}

void RenderGraphResourceCache::RegisterField(const String &name, const RenderPassReflection::Field &field)
{
    if (nameToIndex.Contains(name))
    {
        CT_LOG(Error, CT_TEXT("RenderGraphResourceCache::RegisterField() try to register an existed field, name: {0}."), name);
        return;
    }

    int32 newIndex = resourceDatas.Count();
    nameToIndex.Put(name, newIndex);
    bool resolveBindFlags = field.GetResourceBindFlags() == ResourceBind::None;

    resourceDatas.Add({
        .resource = nullptr,
        .resolveBindFlags = resolveBindFlags
    });
}

SPtr<Resource> RenderGraphResourceCache::GetResource(const String &name) const
{
    auto pRes = externalResources.TryGet(name);
    if (pRes)
        return *pRes;
    
    auto pIdx = nameToIndex.TryGet(name);
    if (pIdx)
        return resourceDatas[*pIdx].resource;

    return nullptr;
}