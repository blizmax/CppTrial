#include "RenderVulkan/VulkanRenderAPI.h"

namespace RenderCore
{

void VulkanRenderAPI::SetRenderPipeline(const SPtr<RenderPipeline> &pipeline)
{
    //TODO
}

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger)
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr)
    {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }
    else
    {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks *pAllocator)
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr)
    {
        func(instance, debugMessenger, pAllocator);
    }
}

VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData)
{
    CT_LOG(Debug, CT_TEXT("Validation layer: {0}"), String(pCallbackData->pMessage));
    return VK_FALSE;
}

void VulkanRenderAPI::Init()
{
#if CT_DEBUG
    enableValidationLayers = true;
#endif

    CreateInstance();
    CreateDebugger();
    CreateDevice();
}

void VulkanRenderAPI::Destroy()
{
    vkDestroyInstance(instance, gVulkanAlloc);

    if (enableValidationLayers)
        DestroyDebugUtilsMessengerEXT(instance, debugMessenger, gVulkanAlloc);
}

void VulkanRenderAPI::SetRenderPipeline(const SPtr<RenderPipeline> &pipeline)
{
}

void VulkanRenderAPI::GetExtensions(Array<const char8 *> &result)
{
    result.Clear();

    result.Add(VK_KHR_SURFACE_EXTENSION_NAME);

#if CT_PLATFORM_WIN32
    result.Add("VK_KHR_win32_surface");
#endif

    if (enableValidationLayers)
        result.Add(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
}

void VulkanRenderAPI::GetValidationLayers(Array<const char8 *> &result)
{
    result.Clear();

    if (enableValidationLayers)
        result.Add("VK_LAYER_LUNARG_standard_validation");
}

void VulkanRenderAPI::CreateInstance()
{
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext = nullptr;
    appInfo.pApplicationName = "CppTrial";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "CppTrialFramework";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    Array<const char8 *> tempArr;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.flags = 0;
    createInfo.pApplicationInfo = &appInfo;
    GetExtensions(tempArr);
    createInfo.enabledExtensionCount = tempArr.Count();
    createInfo.ppEnabledExtensionNames = tempArr.GetData();
    GetValidationLayers(tempArr);
    createInfo.enabledLayerCount = tempArr.Count();
    createInfo.ppEnabledLayerNames = tempArr.GetData();

    if (vkCreateInstance(&createInfo, gVulkanAlloc, &instance) != VK_SUCCESS)
    {
        CT_EXCEPTION(RenderCore, "Create instance failed.");
    }
}

void VulkanRenderAPI::CreateDebugger()
{
    if (!enableValidationLayers)
        return;

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.pNext = nullptr;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = DebugCallback;

    if (CreateDebugUtilsMessengerEXT(instance, &createInfo, gVulkanAlloc, &debugMessenger) != VK_SUCCESS)
    {
        CT_EXCEPTION(RenderCore, "Create debugger failed.");
    }
}

void VulkanRenderAPI::CreateDevice()
{
    Array<VkPhysicalDevice> devices;
    uint32 deviceCount = 0;

    if(vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr) == VK_SUCCESS && deviceCount > 0)
    {
        devices.AppendUninitialized(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.GetData());

        int32 selected = 0;
        if(deviceCount > 1)
        {
            for(int32 i = 0; i < devices.Count(); ++i)
            {
                VkPhysicalDeviceProperties props;
                vkGetPhysicalDeviceProperties(devices[i], &props);
                if(props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
                {
                    selected = i;
                    break;
                }
            }
        }

        device = VulkanDevice::Create(devices[selected]);
    }
    else
    {
        CT_EXCEPTION(RenderCore, "Enumerate physical devices failed.");
    }
}

}