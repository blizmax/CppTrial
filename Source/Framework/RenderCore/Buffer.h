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
    virtual void SetBlob(const void *data, uint32 offset, uint32 dataSize) = 0;

    virtual ResourceState GetSubresourceState(uint32 arraySlice, uint32 mipLevel) const override
    {
        CT_LOG(Warning, CT_TEXT("Get subresouce state from buffer is invalid, return global state instead."));
        return stateData.state;
    }

    virtual void SetSubresourceState(uint32 arraySlice, uint32 mipLevel, ResourceState newState) const override
    {
        CT_LOG(Warning, CT_TEXT("Set subresouce state to buffer is invalid, do nothing."));
    }

    virtual SPtr<ResourceView> GetSrv() override
    {
        return GetSrv(0);
    }

    virtual SPtr<ResourceView> GetUav() override
    {
        return GetUav(0);
    }

    void ClearViews();
    SPtr<ResourceView> GetCbv();
    SPtr<ResourceView> GetSrv(uint32 first, uint32 count = UINT32_MAX);
    SPtr<ResourceView> GetUav(uint32 first, uint32 count = UINT32_MAX);

    uint32 GetOffset() const
    {
        return offset;
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

    const SPtr<Buffer> &GetUavCounter() const
    {
        return uavCounter;
    }

    static SPtr<Buffer> Create(uint32 size, ResourceBindFlags bindFlags = ResourceBind::ShaderResource | ResourceBind::UnorderedAccess, CpuAccess access = CpuAccess::None, const void *data = nullptr);
    static SPtr<Buffer> CreateTyped(ResourceFormat format, uint32 count, ResourceBindFlags bindFlags = ResourceBind::ShaderResource | ResourceBind::UnorderedAccess, CpuAccess access = CpuAccess::None, const void *data = nullptr);
    static SPtr<Buffer> CreateStructured(uint32 structSize, uint32 count, ResourceBindFlags bindFlags = ResourceBind::ShaderResource | ResourceBind::UnorderedAccess, CpuAccess access = CpuAccess::None, const void *data = nullptr, bool createCounter = true);

protected:
    uint32 offset = 0;
    CpuAccess cpuAccess;
    uint32 elementCount = 0;
    uint32 structSize = 0;
    ResourceFormat format = ResourceFormat::Unknown;

    Array<SPtr<ResourceView>> srvs;
    Array<SPtr<ResourceView>> uavs;
    SPtr<ResourceView> cbv;
    SPtr<Buffer> uavCounter;
    WPtr<Buffer> weakThis;
};
}