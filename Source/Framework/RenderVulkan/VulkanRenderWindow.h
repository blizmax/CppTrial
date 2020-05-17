#pragma once

#include "RenderVulkan/.Package.h"
#include "RenderCore/RenderWindow.h"

namespace RenderCore
{

class VulkanRenderWindow : public RenderWindow
{
public:
    virtual VkSurfaceKHR CreateSurface() = 0;
};

}