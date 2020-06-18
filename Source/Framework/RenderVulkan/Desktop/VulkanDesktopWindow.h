#pragma once

#include "RenderVulkan/VulkanRenderWindow.h"
#include "Application/Desktop/DesktopWindow.h"

class VulkanDesktopWindow : public DesktopWindow, public VulkanRenderWindow
{
public:
    virtual VkSurfaceKHR CreateSurface() override;
};