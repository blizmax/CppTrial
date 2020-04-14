#pragma once

#include "RenderVulkan/.Package.h"

namespace RenderCore
{
class VulkanResource
{
public:
    virtual ~VulkanResource() = default;
    virtual void Destroy() = 0;
};

}