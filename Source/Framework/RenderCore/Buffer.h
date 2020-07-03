#pragma once

#include "RenderCore/Resource.h"

class Buffer : public Resource
{
public:
    Buffer(uint32 size, ResourceBindFlags bindFlags, CpuAccess access)
        : Resource(ResourceType::Buffer, bindFlags, size), cpuAccess(access)
    {
    }

    virtual ~Buffer() = default;

    virtual void *Map(BufferMapType mapType) = 0;
    virtual void Unmap() = 0;
    virtual void SetBlob(const void *data, uint32 offset, uint32 dataSize) = 0;

    virtual ResourceState GetSubresourceState(int32 arraySlice, int32 mipLevel) const override
    {
        CT_LOG(Warning, CT_TEXT("Get subresouce state from buffer is invalid, return global state instead."));
        return stateData.state;
    }

    virtual void SetSubresourceState(int32 arraySlice, int32 mipLevel, ResourceState newState) const override
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

    virtual SPtr<Buffer> AsBuffer() override
    {
        return weakThis.lock();
    }

    void ClearViews();
    SPtr<ResourceView> GetCbv();
    SPtr<ResourceView> GetSrv(int32 first, int32 count = -1);
    SPtr<ResourceView> GetUav(int32 first, int32 count = -1);

    uint32 GetOffset() const
    {
        return offset;
    }

    CpuAccess GetCpuAccess() const
    {
        return cpuAccess;
    }

    int32 GetElementCount() const
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

    template <typename T>
    void SetElement(int32 index, const T &val)
    {
        CT_CHECK(index >= 0);
        SetBlob(&val, sizeof(T) * index, sizeof(T));
    }

    static SPtr<Buffer> Create(uint32 size, ResourceBindFlags bindFlags = ResourceBind::ShaderResource | ResourceBind::UnorderedAccess, CpuAccess access = CpuAccess::None, const void *data = nullptr);
    static SPtr<Buffer> CreateTyped(ResourceFormat format, int32 count, ResourceBindFlags bindFlags = ResourceBind::ShaderResource | ResourceBind::UnorderedAccess, CpuAccess access = CpuAccess::None, const void *data = nullptr);
    static SPtr<Buffer> CreateStructured(uint32 structSize, int32 count, ResourceBindFlags bindFlags = ResourceBind::ShaderResource | ResourceBind::UnorderedAccess, CpuAccess access = CpuAccess::None, const void *data = nullptr, bool createCounter = true);

protected:
    uint32 offset = 0;
    CpuAccess cpuAccess;
    int32 elementCount = 0;
    uint32 structSize = 0;
    ResourceFormat format = ResourceFormat::Unknown;

    Array<SPtr<ResourceView>> srvs;
    Array<SPtr<ResourceView>> uavs;
    SPtr<ResourceView> cbv;
    SPtr<Buffer> uavCounter;
    WPtr<Buffer> weakThis;
};