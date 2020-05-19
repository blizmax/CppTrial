#include "RenderVulkan/VulkanDevice.h"
#include "RenderVulkan/VulkanRenderWindow.h"
#include "RenderVulkan/VulkanSync.h"

namespace RenderCore
{
SPtr<Device> Device::Create(RenderWindow *window, const DeviceDesc &desc)
{
    return Memory::MakeShared<VulkanDevice>(window, desc);
}

VulkanDevice::VulkanDevice(RenderWindow *window, const DeviceDesc &desc)
    : Device(window, desc)
{
    InitPhysicalDevice();
    CreateLogicalDevice();
    CreateSurface();
    CreateSwapChain();
    CreateVmaAllocator();

    for (int32 i = 0; i < backBufferCount; ++i)
    {
        presentFences.Add(VulkanFence::Create(true));
    }

    frameFence = GpuFence::Create();
    renderContext = RenderContext::Create();
    renderContext->Flush();

    UpdateBackBuffers(window->GetWidth(), window->GetHeight(), desc.colorFormat, desc.depthFormat);
    curBackBufferIndex = GetCurrentBackBufferIndex();
}

VulkanDevice::~VulkanDevice()
{
    renderContext->Flush(true);

    //TODO release vulkan resources

    swapChainFrameBuffers.Clear();
    presentFences.Clear();

    vmaDestroyAllocator(allocator);
    vkDestroySwapchainKHR(logicalDevice, swapChain, gVulkanAlloc);
    vkDestroySurfaceKHR(gVulkanInstance, surface, gVulkanAlloc);
    vkDestroyDevice(logicalDevice, gVulkanAlloc);
}

void VulkanDevice::ResizeSwapChain(uint32 width, uint32 height)
{
    renderContext->Flush(true);

    swapChainFrameBuffers.Clear();
    vkDestroySwapchainKHR(logicalDevice, swapChain, gVulkanAlloc);
    CreateSwapChain();
    UpdateBackBuffers(width, height, desc.colorFormat, desc.depthFormat);
}

void VulkanDevice::Present()
{
    //TODO
}

void VulkanDevice::FlushAndSync()
{
    //TODO
}

void VulkanDevice::InitPhysicalDevice()
{
    Array<VkPhysicalDevice> devices;
    uint32 deviceCount = 0;

    if (vkEnumeratePhysicalDevices(gVulkanInstance, &deviceCount, nullptr) == VK_SUCCESS && deviceCount > 0)
    {
        devices.AppendUninitialized(deviceCount);
        vkEnumeratePhysicalDevices(gVulkanInstance, &deviceCount, devices.GetData());

        int32 selected = 0;
        if (deviceCount > 1)
        {
            for (int32 i = 0; i < devices.Count(); ++i)
            {
                VkPhysicalDeviceProperties props;
                vkGetPhysicalDeviceProperties(devices[i], &props);
                if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
                {
                    selected = i;
                    break;
                }
            }
        }
        physicalDevice = devices[selected];
    }
    else
        CT_EXCEPTION(RenderCore, "Enumerate physical devices failed.");
}

void VulkanDevice::CreateLogicalDevice()
{
    vkGetPhysicalDeviceProperties(physicalDevice, &properties);
    vkGetPhysicalDeviceFeatures(physicalDevice, &features);
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

    uint32 queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
    Array<VkQueueFamilyProperties> queueFamilies;
    queueFamilies.AppendUninitialized(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.GetData());

    Array<VkDeviceQueueCreateInfo> queueCreateInfos;
    auto DetermineQueue = [this](Array<VkDeviceQueueCreateInfo> &arr, GpuQueueType q, uint32 i) {
        if (queueDatas[(int32)q].familyIndex != UINT32_MAX)
            return false;

        queueDatas[(int32)q].familyIndex = i;
        queueDatas[(int32)q].queues.Add(nullptr);
        const float priority = 1.0f;
        VkDeviceQueueCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        info.queueFamilyIndex = i;
        info.queueCount = 1;
        info.pQueuePriorities = &priority;
        arr.Add(info);
        return true;
    };
    for (int32 i = 0; i < queueFamilies.Count(); ++i)
    {
        if (queueFamilies[i].queueCount == 0)
            continue;
        auto flags = queueFamilies[i].queueFlags;
        if ((flags & VK_QUEUE_GRAPHICS_BIT) != 0 && DetermineQueue(queueCreateInfos, GpuQueueType::Graphics, i))
            continue;
        if ((flags & VK_QUEUE_COMPUTE_BIT) != 0 && DetermineQueue(queueCreateInfos, GpuQueueType::Compute, i))
            continue;
        if ((flags & VK_QUEUE_TRANSFER_BIT) != 0 && DetermineQueue(queueCreateInfos, GpuQueueType::Transfer, i))
            continue;
    }

    Array<const char8 *> extensions;
    extensions.Add(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

    uint32 extensionCount;
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);
    Array<VkExtensionProperties> availableExtensions;
    availableExtensions.AppendUninitialized(extensionCount);
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.GetData());
    for (const auto &e : extensions)
    {
        bool match = false;
        for (const auto &p : availableExtensions)
        {
            if (std::strcmp(p.extensionName, e) == 0)
            {
                match = true;
                break;
            }
        }
        if (!match)
            CT_EXCEPTION(RenderCore, "Device extension unsupported.");
    }

    // Create logic device
    VkDeviceCreateInfo deviceInfo = {};
    deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceInfo.queueCreateInfoCount = queueCreateInfos.Count();
    deviceInfo.pQueueCreateInfos = queueCreateInfos.GetData();
    deviceInfo.pEnabledFeatures = &features;
    deviceInfo.enabledExtensionCount = extensions.Count();
    deviceInfo.ppEnabledExtensionNames = extensions.GetData();
    deviceInfo.enabledLayerCount = 0;
    deviceInfo.ppEnabledLayerNames = nullptr;

    if (vkCreateDevice(physicalDevice, &deviceInfo, gVulkanAlloc, &logicalDevice) != VK_SUCCESS)
        CT_EXCEPTION(RenderCore, "Create logic device failed.");

    for (int32 i = 0; i < (int32)GpuQueueType::Count; ++i)
    {
        auto &queueData = queueDatas[i];
        for (int32 i = 0; i < queueData.queues.Count(); ++i)
        {
            VkQueue queue;
            vkGetDeviceQueue(logicalDevice, queueData.familyIndex, i, &queue);
            queueData.queues[i] = GpuQueue::Create((GpuQueueType)i, queue);
        }
    }
}

void VulkanDevice::CreateSurface()
{
    auto vkWindow = static_cast<VulkanRenderWindow *>(window);
    surface = vkWindow->CreateSurface();
}

void VulkanDevice::CreateSwapChain()
{
    uint32 width = window->GetWidth();
    uint32 height = window->GetHeight();

    VkSurfaceCapabilitiesKHR surfaceCaps;
    if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCaps) != VK_SUCCESS)
        CT_EXCEPTION(RenderCore, "Get surface capabilities failed.");

    Array<VkSurfaceFormatKHR> formats;
    uint32 formatCount;
    if (vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr) != VK_SUCCESS)
        CT_EXCEPTION(RenderCore, "Get surface formats failed.");
    formats.AppendUninitialized(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, formats.GetData());

    Array<VkPresentModeKHR> presentModes;
    uint32 presentModeCount;
    if (vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr) != VK_SUCCESS)
        CT_EXCEPTION(RenderCore, "Get surface present modes failed.");
    presentModes.AppendUninitialized(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes.GetData());

    if (formatCount == 0 || presentModeCount == 0)
        CT_EXCEPTION(RenderCore, "No suitable format or present mode.");

    VkSurfaceFormatKHR surfaceFormat;
    {
        bool found = false;
        VkSurfaceFormatKHR desired = {ToVkResourceFormat(desc.colorFormat), VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
        for (const auto &e : formats)
        {
            if (e.format == desired.format && e.colorSpace == desired.colorSpace)
            {
                surfaceFormat = e;
                found = true;
                break;
            }
        }
        if (!found)
            CT_EXCEPTION(RenderCore, "Found swap chain format failed.");
    }

    VkPresentModeKHR presentMode;
    {
        presentMode = VK_PRESENT_MODE_FIFO_KHR;
        if (!desc.vsync)
        {
            for (int32 i = 0; i < presentModes.Count(); ++i)
            {
                if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
                {
                    presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
                    break;
                }
                if (presentModes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR)
                {
                    presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
                    break;
                }
                if (presentModes[i] == VK_PRESENT_MODE_FIFO_RELAXED_KHR)
                {
                    presentMode = VK_PRESENT_MODE_FIFO_RELAXED_KHR;
                }
            }
        }
    }

    VkExtent2D extent;
    {
        if (surfaceCaps.currentExtent.width != UINT32_MAX)
        {
            extent = surfaceCaps.currentExtent;
        }
        else
        {
            extent.width = width;
            extent.height = height;
        }
    }

    uint32 imageCount = surfaceCaps.minImageCount;

    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.queueFamilyIndexCount = 0;
    createInfo.pQueueFamilyIndices = nullptr;
    createInfo.preTransform = surfaceCaps.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(logicalDevice, &createInfo, gVulkanAlloc, &swapChain) != VK_SUCCESS)
        CT_EXCEPTION(RenderCore, "Create swap chain failed.");
    if (vkGetSwapchainImagesKHR(logicalDevice, swapChain, &backBufferCount, nullptr) != VK_SUCCESS)
        CT_EXCEPTION(RenderCore, "Get swap chain images failed.");
}

void VulkanDevice::CreateVmaAllocator()
{
    VmaAllocatorCreateInfo allocatorInfo = {};
    allocatorInfo.physicalDevice = physicalDevice;
    allocatorInfo.device = logicalDevice;
    allocatorInfo.pAllocationCallbacks = gVulkanAlloc;
    vmaCreateAllocator(&allocatorInfo, &allocator);
}

void VulkanDevice::UpdateBackBuffers(uint32 width, uint32 height, ResourceFormat colorFormat, ResourceFormat depthFormat)
{
    Array<VkImage> images;
    images.AppendUninitialized(backBufferCount);
    vkGetSwapchainImagesKHR(logicalDevice, swapChain, &backBufferCount, images.GetData());

    for(int32 i = 0; i < backBufferCount; ++i)
    {
        SPtr<Texture> colorTexture = Texture::Create2D(width, height, colorFormat, images[i], ResourceBind::RenderTarget);
        SPtr<Texture> depthStencilTexture = nullptr;
        if(depthFormat != ResourceFormat::Unknown)
            depthStencilTexture = Texture::Create2D(width, height, depthFormat, 1, 1, nullptr, ResourceBind::DepthStencil);
        swapChainFrameBuffers.Add(FrameBuffer::Create({colorTexture}, depthStencilTexture));
    }
}

uint32 VulkanDevice::GetCurrentBackBufferIndex()
{
    auto &fence = presentFences[curPresentFenceIndex];
    fence->Wait();
    curPresentFenceIndex = (curPresentFenceIndex + 1) % presentFences.Count();
    auto &nextFence = presentFences[curPresentFenceIndex];
    nextFence->Reset();
    uint32 imageIndex;
    VkResult result = vkAcquireNextImageKHR(logicalDevice, swapChain, UINT64_MAX, VK_NULL_HANDLE, nextFence->GetHandle(), &imageIndex);
    CT_CHECK(result == VK_SUCCESS || result == VK_SUBOPTIMAL_KHR);
    return imageIndex;
}
}