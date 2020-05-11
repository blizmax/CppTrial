#pragma once

#include "RenderCore/Resource.h"

namespace RenderCore
{
class Buffer : public Resource
{
public:
    Buffer(uint32 size, ResourceBindFlags bindFlags, CpuAccess access)
        : Resource(ResourceType::Buffer, bindFlags, size)
    {
    }

    virtual ~Buffer() = default;

    virtual void *Map(BufferMapType mapType) = 0;
    virtual void Unmap() = 0;
    virtual void SetBlob(const void *data, uint32 offset, uint32 size) = 0;

    virtual SPtr<ResourceView> GetSrv() override
    {
        return GetSrv(0);
    }

    virtual SPtr<ResourceView> GetUav() override
    {
        return GetUav(0);
    }

    void ClearViews()
    {
        srvs.Clear();
        uavs.Clear();
        cbv = nullptr;
    }

    SPtr<ResourceView> GetCbv()
    {
        if(!cbv)
            cbv = ResourceView::CreateCbv(weakThis.lock());
        return cbv;
    }

    SPtr<ResourceView> GetSrv(uint32 first, uint32 count = UINT32_MAX)
    {
        auto viewInfo = ResourceViewInfo(first, count);
        for(const auto & e : srvs)
        {
            if(e->GetViewInfo() == viewInfo)
                return e;
        }
        auto newView = ResourceView::CreateSrv(weakThis.lock(), first, count);
        srvs.Add(newView);
        return newView;
    }

    SPtr<ResourceView> GetUav(uint32 first, uint32 count = UINT32_MAX)
    {
        auto viewInfo = ResourceViewInfo(first, count);
        for(const auto & e : uavs)
        {
            if(e->GetViewInfo() == viewInfo)
                return e;
        }
        auto newView = ResourceView::CreateUav(weakThis.lock(), first, count);
        uavs.Add(newView);
        return newView;
    }

    CpuAccess GetCpuAccess() const
    {
        return cpuAccess;
    }

    uint32 GetElementCount() const
    {
        return elementCount;
    }

    uint32 GetStructSize() const
    {
        return structSize;
    }

    ResourceFormat GetResourceFormat() const
    {
        return format;
    }

    bool IsTyped() const
    {
        return format != ResourceFormat::Unknown;
    }

    bool IsStructured() const
    {
        return structSize != 0;
    }

    static SPtr<Buffer> Create(uint32 size, ResourceBindFlags bindFlags = ResourceBind::ShaderResource | ResourceBind::UnorderedAccess, CpuAccess access = CpuAccess::None, const void *data = nullptr);
    
    static SPtr<Buffer> CreateTyped(ResourceFormat format, uint32 count, ResourceBindFlags bindFlags = ResourceBind::ShaderResource | ResourceBind::UnorderedAccess, CpuAccess access = CpuAccess::None, const void *data = nullptr)
    {
        uint32 size = count * GetResourceFormatBytes(format);
        auto buffer = Create(size, bindFlags, access, data);
        buffer->format = format;
        buffer->elementCount = count;
        return buffer;
    }

    static SPtr<Buffer> CreateStructured(uint32 structSize, uint32 count, ResourceBindFlags bindFlags = ResourceBind::ShaderResource | ResourceBind::UnorderedAccess, CpuAccess access = CpuAccess::None, const void *data = nullptr)
    {
        uint32 size = structSize * count;
        auto buffer = Create(size, bindFlags, access, data);
        buffer->elementCount = count;
        buffer->structSize = structSize;
        return buffer;
    }

protected:
    CpuAccess cpuAccess;
    uint32 elementCount = 0;
    uint32 structSize = 0;
    ResourceFormat format = ResourceFormat::Unknown;
    Array<SPtr<ResourceView>> srvs;
    Array<SPtr<ResourceView>> uavs;
    SPtr<ResourceView> cbv;
    WPtr<Buffer> weakThis;
};
}