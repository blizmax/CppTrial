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

    virtual SPtr<ResourceView> GetSrv() override;
    virtual SPtr<ResourceView> GetUav() override;
    SPtr<ResourceView> GetCbv();

    virtual void *Map(BufferMapType mapType) = 0;
    virtual void Unmap() = 0;
    virtual void SetBlob(const void *data, uint32 offset, uint32 size) = 0;

    static SPtr<Buffer> Create(uint32 size, ResourceBindFlags bindFlags = ResourceBind::ShaderResource | ResourceBind::UnorderedAccess,
        CpuAccess access = CpuAccess::None, const void *data = nullptr);

protected:
    CpuAccess cpuAccess;
};
}