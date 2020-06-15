#pragma once

#include "RenderVulkan/VulkanRenderWindow.h"
#include "Application/Desktop/DesktopWindow.h"

using namespace RenderCore;

class VulkanDesktopWindow : public DesktopWindow, public VulkanRenderWindow
{
public:
    virtual VkSurfaceKHR CreateSurface() override;
};