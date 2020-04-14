#include "Core/String.h"
#include "Core/Exception.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "RenderVulkan/VulkanContext.h"
#include "RenderVulkan/VulkanSwapChain.h"
using namespace RenderCore;

const int32 WIDTH = 800;
const int32 HEIGHT = 600;

bool enableValidationLayers = true;

struct WindowData
{
    GLFWwindow *window;
    VkSurfaceKHR surface;
    SPtr<VulkanSwapChain> swapChain;
};
WindowData windowData;

void InitVulkan()
{
    auto &context = VulkanContext::Get();
    context.Init();

    if (glfwCreateWindowSurface(context.GetInstanceHandle(), windowData.window, gVulkanAlloc, &windowData.surface) != VK_SUCCESS)
        CT_EXCEPTION(LearnVK, "Create surface failed.");

    VkBool32 supportsPresent;
    auto physicalDevice = context.GetDevice()->GetPhysicalDeviceHandle();
    auto familyIndex = context.GetDevice()->GetGraphicsQueueFamilyIndex();
    vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, familyIndex, windowData.surface, &supportsPresent);
    if (!supportsPresent)
        CT_EXCEPTION(LearnVK, "Graphics queue cannot support presentation.");


    windowData.swapChain = VulkanSwapChain::Create();
    windowData.swapChain->Rebuild(windowData.surface, WIDTH, HEIGHT);
}

void CleanupVulkan()
{
    auto &context = VulkanContext::Get();

    windowData.swapChain->Destroy();
    vkDestroySurfaceKHR(context.GetInstanceHandle(), windowData.surface, gVulkanAlloc);

    context.Destroy();
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