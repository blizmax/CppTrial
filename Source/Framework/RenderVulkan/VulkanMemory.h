#pragma once

#include "RenderVulkan/.Package.h"
#include "Core/HashSet.h"

#include <vk_mem_alloc.h>

class IVulkanResource
{
public:
    virtual ~IVulkanResource() = default;
    virtual void Destroy() = 0;
};

template <typename T>
class VulkanResourceRegistry
{
public:
    void Cleanup()
    {
        for (T *e : resources)
            e->Destroy();
        resources.Clear();
    }

    void Register(T *ptr)
    {
        resources.Add(ptr);
    }

    void Remove(T *ptr)
    {
        if (resources.Remove(ptr))
            ptr->Destroy();
    }

private:
    HashSet<T *> resources;
};