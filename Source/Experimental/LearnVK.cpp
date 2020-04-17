#include "Core/String.h"
#include "Core/Exception.h"
#include "IO/FileHandle.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "RenderVulkan/VulkanContext.h"
#include "RenderVulkan/VulkanSwapChain.h"
#include "RenderVulkan/VulkanRenderPipeline.h"
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
SPtr<VulkanRenderPipeline> pipeline;

SPtr<Shader> CreateShader()
{
    ShaderCreateParams params;
    IO::FileHandle vertSrcFile(CT_TEXT("External/vulkan/vert.spv"));
    IO::FileHandle fragSrcFile(CT_TEXT("External/vulkan/frag.spv"));
    params.vertexSource = vertSrcFile.ReadBytes();
    params.fragmentSource = fragSrcFile.ReadBytes();
    return Shader::Create(params);
}

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


    //Swap chain
    VulkanSwapChainCreateParams swapChainParams;
    swapChainParams.width = WIDTH;
    swapChainParams.height = HEIGHT;
    swapChainParams.surface = windowData.surface;
    windowData.swapChain = VulkanSwapChain::Create();
    windowData.swapChain->Rebuild(swapChainParams);

    //TODO pass renderpass to pipeline
    //Render pipeline
    RenderPipelineCreateParams pipelineParams;
    BlendStateCreateParams blendParams;
    pipelineParams.blendState = BlendState::Create(blendParams);
    DepthStencilStateCreateParams depthStencilParams;
    pipelineParams.depthStencilState = DepthStencilState::Create(depthStencilParams);
    RasterizationStateCreateParams rasterizationParams;
    pipelineParams.rasterizationState = RasterizationState::Create(rasterizationParams);
    pipelineParams.shader = CreateShader();
    pipeline = std::static_pointer_cast<VulkanRenderPipeline>(RenderPipeline::Create(pipelineParams));

    context.SetRenderPipeline(pipeline);
}

void CleanupVulkan()
{
    auto &context = VulkanContext::Get();

    pipeline->Destroy();

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