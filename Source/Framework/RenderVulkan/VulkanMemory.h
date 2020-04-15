#pragma once

#include "RenderVulkan/.Package.h"

namespace RenderCore
{
class IVulkanResource
{
public:
    virtual ~IVulkanResource() = default;
    virtual void Destroy() = 0;
};

}