#define GLFW_INCLUDE_VULKAN
#include "RenderVulkan/Desktop/VulkanDesktopWindow.h"
#include <GLFW/glfw3.h>

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
    if (glfwCreateWindowSurface(gVulkanInstance, window, gVulkanAlloc, &surface) != VK_SUCCESS)
        CT_EXCEPTION(LearnVK, "Create surface failed.");
    return surface;
}