#include "Core/String.h"
#include "Core/Exception.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "RenderVulkan/VulkanRenderAPI.h"
using namespace RenderCore;

VulkanRenderAPI renderAPI;

const int32 WIDTH = 800;
const int32 HEIGHT = 600;

bool enableValidationLayers = true;

struct WindowData
{
    GLFWwindow *window;
    VkSurfaceKHR surface;
};
WindowData windowData;

void InitVulkan()
{
    renderAPI.Init();

    if (glfwCreateWindowSurface(renderAPI.GetInstanceHandle(), windowData.window, gVulkanAlloc, &windowData.surface) != VK_SUCCESS)
    {
        CT_EXCEPTION(LearnVK, "Create surface failed.");
    }
    VkBool32 supportsPresent;
    auto physicalDevice = renderAPI.GetDevice()->GetPhysicalDeviceHandle();
    auto familyIndex = renderAPI.GetDevice()->GetGraphicsQueueFamilyIndex();
    vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, familyIndex, windowData.surface, &supportsPresent);
    if (!supportsPresent)
    {
        CT_EXCEPTION(LearnVK, "Graphics queue cannot support presentation.");
    }
}

void CleanupVulkan()
{
    vkDestroySurfaceKHR(renderAPI.GetInstanceHandle(), windowData.surface, gVulkanAlloc);

    renderAPI.Destroy();
}

int main(int argc, char **argv)
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    windowData.window = glfwCreateWindow(WIDTH, HEIGHT, "LearnVK", nullptr, nullptr);

    InitVulkan();

    while (!glfwWindowShouldClose(windowData.window))
    {
        glfwPollEvents();
    }

    CleanupVulkan();

    glfwDestroyWindow(windowData.window);
    glfwTerminate();

    return 0;
}