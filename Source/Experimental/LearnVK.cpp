#include "Core/String.h"
#include "Core/Exception.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

const int32 WIDTH = 800;
const int32 HEIGHT = 600;

GLFWwindow *window;
VkInstance instance;

void CreateInstance()
{
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "LearnVK";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "WIP";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    uint32 glfwExtensionCount = 0;
    const char8**glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;

    createInfo.enabledLayerCount = 0;

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
    {
        CT_EXCEPTION(LearnVK, "Create instance error.");
    }

    CT_LOG(Info, CT_TEXT("Create instance done."));

    uint32 extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    Array<VkExtensionProperties> extensions;
    extensions.AppendUninitialized(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.GetData());
    for (const auto &extension : extensions)
    {
        CT_LOG(Info, CT_TEXT("Extension: {0}"), String(extension.extensionName));
    }
}

void InitVulkan()
{
    CreateInstance();
}

void CleanupVulkan()
{
    vkDestroyInstance(instance, nullptr);
}

int main(int argc, char **argv)
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(WIDTH, HEIGHT, "LearnVK", nullptr, nullptr);

    InitVulkan();

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    }

    CleanupVulkan();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}