#include "RenderCore/Buffer.h"

namespace RenderCore
{

SPtr<Buffer> Buffer::CreateTyped(ResourceFormat format, uint32 count, ResourceBindFlags bindFlags, CpuAccess access, const void *data)
{
    uint32 size = count * GetResourceFormatBytes(format);
    auto buffer = Create(size, bindFlags, access, data);
    buffer->format = format;
    buffer->elementCount = count;
    return buffer;
}

SPtr<Buffer> Buffer::CreateStructured(uint32 structSize, uint32 count, ResourceBindFlags bindFlags, CpuAccess access, const void *data)
{
    uint32 size = structSize * count;
    auto buffer = Create(size, bindFlags, access, data);
    buffer->elementCount = count;
    buffer->structSize = structSize;
    return buffer;
}

Buffer::Buffer(uint32 size, ResourceBindFlags bindFlags, CpuAccess access)
    : Resource(ResourceType::Buffer, bindFlags, size)
{
}

void Buffer::ClearViews()
{
    srvs.Clear();
    uavs.Clear();
    cbv = nullptr;
}

SPtr<ResourceView> Buffer::GetCbv()
{
    if (!cbv)
        cbv = ResourceView::CreateCbv(weakThis.lock());
    return cbv;
}

SPtr<ResourceView> Buffer::GetSrv(uint32 first, uint32 count)
{
    auto viewInfo = ResourceViewInfo(first, count);
    for (const auto &e : srvs)
    {
        if (e->GetViewInfo() == viewInfo)
            return e;
    }
    auto newView = ResourceView::CreateSrv(weakThis.lock(), first, count);
    srvs.Add(newView);
    return newView;
}

SPtr<ResourceView> Buffer::GetUav(uint32 first, uint32 count)
{
    auto viewInfo = ResourceViewInfo(first, count);
    for (const auto &e : uavs)
    {
        if (e->GetViewInfo() == viewInfo)
            return e;
    }
    auto newView = ResourceView::CreateUav(weakThis.lock(), first, count);
    uavs.Add(newView);
    return newView;
}
}