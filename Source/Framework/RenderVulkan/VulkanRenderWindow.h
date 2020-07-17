#pragma once

#include "RenderCore/RenderWindow.h"
#include "RenderVulkan/.Package.h"

class VulkanRenderWindow : virtual public RenderWindow
{
public:
    virtual VkSurfaceKHR CreateSurface() = 0;
};