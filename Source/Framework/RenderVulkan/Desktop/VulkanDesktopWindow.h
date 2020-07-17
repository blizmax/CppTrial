#pragma once

#include "Application/Desktop/DesktopWindow.h"
#include "RenderVulkan/VulkanRenderWindow.h"

class VulkanDesktopWindow : public DesktopWindow, public VulkanRenderWindow
{
public:
    virtual VkSurfaceKHR CreateSurface() override;
};