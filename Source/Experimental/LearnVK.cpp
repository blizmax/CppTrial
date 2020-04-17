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
};

WindowData windowData;
SPtr<VulkanRenderPipelineState> pipelineState;

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

    VkSurfaceKHR surface;
    if (glfwCreateWindowSurface(context.GetInstanceHandle(), windowData.window, gVulkanAlloc, &surface) != VK_SUCCESS)
        CT_EXCEPTION(LearnVK, "Create surface failed.");
 
    VkBool32 supportsPresent;
    auto physicalDevice = context.GetDevice()->GetPhysicalDeviceHandle();
    auto familyIndex = context.GetDevice()->GetGraphicsQueueFamilyIndex();
    vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, familyIndex, surface, &supportsPresent);
    if (!supportsPresent)
        CT_EXCEPTION(LearnVK, "Graphics queue cannot support presentation.");

    context.SetSurfaceKHR(surface);

    //Swap chain
    VulkanSwapChainCreateParams swapChainParams;
    swapChainParams.width = WIDTH;
    swapChainParams.height = HEIGHT;
    context.RecreateSwapChain(swapChainParams);


    RenderPipelineStateCreateParams pipelineStateParams;
    BlendStateCreateParams blendParams;
    pipelineStateParams.blendState = BlendState::Create(blendParams);
    DepthStencilStateCreateParams depthStencilParams;
    pipelineStateParams.depthStencilState = DepthStencilState::Create(depthStencilParams);
    RasterizationStateCreateParams rasterizationParams;
    pipelineStateParams.rasterizationState = RasterizationState::Create(rasterizationParams);
    pipelineStateParams.shader = CreateShader();
    pipelineState = std::static_pointer_cast<VulkanRenderPipelineState>(RenderPipelineState::Create(pipelineStateParams));

    context.SetRenderPipelineState(pipelineState);
}

void CleanupVulkan()
{
    auto &context = VulkanContext::Get();
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