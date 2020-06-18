#pragma once

#include "RenderVulkan/.Package.h"
#include "RenderCore/RenderWindow.h"

class VulkanRenderWindow : virtual public RenderWindow
{
public:
    virtual VkSurfaceKHR CreateSurface() = 0;
};