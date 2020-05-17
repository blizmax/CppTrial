#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "RenderVulkan/VulkanContext.h"
#include "RenderVulkan/Desktop/VulkanDesktopWindow.h"

Window *Window::Create(const WindowDesc &desc)
{
    Window *window = Memory::New<VulkanDesktopWindow>();
    window->CreateNativeWindow(desc);
    return window;
}

void Window::Init()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
}

VkSurfaceKHR VulkanDesktopWindow::CreateSurface()
{
    VkSurfaceKHR surface;
    if (glfwCreateWindowSurface(gVulkanContext->GetInstanceHandle(), window, gVulkanAlloc, &surface) != VK_SUCCESS)
        CT_EXCEPTION(LearnVK, "Create surface failed.");
    return surface;
}