#include "RenderVulkan/.Package.h"
#include "RenderCore/RenderAPI.h"
#include "RenderVulkan/VulkanDevice.h"

namespace RenderCore
{

VkAllocationCallbacks *gVulkanAlloc = nullptr;
VulkanDevice *gVulkanDevice = nullptr;
VkInstance gVulkanInstance = VK_NULL_HANDLE;
VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;
#if CT_DEBUG
bool debugLayerEnabled = true;
#else
bool debugLayerEnabled = false;
#endif

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger)
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr)
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    else
        return VK_ERROR_EXTENSION_NOT_PRESENT;
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks *pAllocator)
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr)
        func(instance, debugMessenger, pAllocator);
}

VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData)
{
    CT_LOG(Debug, CT_TEXT("Validation layer: {0}"), String(pCallbackData->pMessage));
    return VK_FALSE;
}

void RenderAPI::Init()
{
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "CppTrial";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "CppTrialFramework";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    Array<const char8 *> extensions;
    extensions.Add(VK_KHR_SURFACE_EXTENSION_NAME);
#if CT_PLATFORM_WIN32
    extensions.Add("VK_KHR_win32_surface");
#else
    extensions.Add("VK_KHR_xlib_surface"); //TODO
#endif
    if (debugLayerEnabled)
        extensions.Add(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    Array<const char8 *> validationLayers;
    if (debugLayerEnabled)
        validationLayers.Add("VK_LAYER_LUNARG_standard_validation");

    VkInstanceCreateInfo instanceInfo = {};
    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.pApplicationInfo = &appInfo;
    instanceInfo.enabledExtensionCount = extensions.Count();
    instanceInfo.ppEnabledExtensionNames = extensions.GetData();
    instanceInfo.enabledLayerCount = validationLayers.Count();
    instanceInfo.ppEnabledLayerNames = validationLayers.GetData();

    if (vkCreateInstance(&instanceInfo, gVulkanAlloc, &gVulkanInstance) != VK_SUCCESS)
        CT_EXCEPTION(RenderCore, "Create instance failed.");

    if (!debugLayerEnabled)
        return;
    VkDebugUtilsMessengerCreateInfoEXT messengerInfo = {};
    messengerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    messengerInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    messengerInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    messengerInfo.pfnUserCallback = DebugCallback;

    if (CreateDebugUtilsMessengerEXT(gVulkanInstance, &messengerInfo, gVulkanAlloc, &debugMessenger) != VK_SUCCESS)
        CT_EXCEPTION(RenderCore, "Create debugger failed.");
}

void RenderAPI::Destroy()
{
    if (debugMessenger != VK_NULL_HANDLE)
        DestroyDebugUtilsMessengerEXT(gVulkanInstance, debugMessenger, gVulkanAlloc);

    vkDestroyInstance(gVulkanInstance, gVulkanAlloc);
}

SPtr<Device> CreateDevice(RenderWindow *window, const DeviceDesc &desc)
{
    auto ptr = Device::Create(window, desc);
    gVulkanDevice = static_cast<VulkanDevice *>(ptr.get());
    return ptr;
}
}