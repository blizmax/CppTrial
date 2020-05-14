#include "Core/String.h"
#include "Core/Exception.h"
#include "IO/FileHandle.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "RenderVulkan/VulkanContext.h"
#include "RenderVulkan/VulkanSwapChain.h"
#include "RenderVulkan/VulkanRenderPipeline.h"
#include "RenderVulkan/VulkanSync.h"
#include "RenderVulkan/VulkanCommandBuffer.h"
//#include "RenderVulkan/VulkanVertexLayout.h"
#include "RenderCore/VertexLayout.h"

using namespace RenderCore;

const int32 WIDTH = 800;
const int32 HEIGHT = 600;

GLFWwindow *window;
SPtr<VertexLayout> vertexLayout;
SPtr<VulkanRenderPipelineState> pipelineState;
Array<SPtr<VulkanCommandBuffer>> commandBuffers;

SPtr<Shader> CreateShader()
{
    ShaderDesc desc;
    IO::FileHandle vertSrcFile(CT_TEXT("Assets/Shaders/LearnVK/shader.vert"));
    IO::FileHandle fragSrcFile(CT_TEXT("Assets/Shaders/LearnVK/shader.frag"));
    desc.vertexSource = vertSrcFile.ReadString();
    desc.fragmentSource = fragSrcFile.ReadString();
    return Shader::Create(desc);
}

void InitVulkan()
{
    auto &context = VulkanContext::Get();
    context.Init();

    // VkSurfaceKHR surface;
    // if (glfwCreateWindowSurface(context.GetInstanceHandle(), window, gVulkanAlloc, &surface) != VK_SUCCESS)
    //     CT_EXCEPTION(LearnVK, "Create surface failed.");
 
    // VkBool32 supportsPresent;
    // auto physicalDevice = context.GetDevice()->GetPhysicalDeviceHandle();
    // auto familyIndex = context.GetDevice()->GetGraphicsQueueFamilyIndex();
    // vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, familyIndex, surface, &supportsPresent);
    // if (!supportsPresent)
    //     CT_EXCEPTION(LearnVK, "Graphics queue cannot support presentation.");

    // context.SetSurfaceKHR(surface);

    // //Swap chain
    // VulkanSwapChainCreateParams swapChainParams;
    // swapChainParams.width = WIDTH;
    // swapChainParams.height = HEIGHT;
    // context.RecreateSwapChain(swapChainParams);

    // //Pipeline
    // RenderPipelineStateCreateParams pipelineStateParams;
    // BlendStateDesc blendDesc;
    // pipelineStateParams.blendState = BlendState::Create(blendDesc);
    // DepthStencilStateDesc depthStencilDesc;
    // pipelineStateParams.depthStencilState = DepthStencilState::Create(depthStencilDesc);
    // RasterizationStateDesc rasterizationDesc;
    // pipelineStateParams.rasterizationState = RasterizationState::Create(rasterizationDesc);
    // pipelineStateParams.shader = CreateShader();
    // pipelineState = std::static_pointer_cast<VulkanRenderPipelineState>(RenderPipelineState::Create(pipelineStateParams));

    // //VertexBuffer
    // float vertices[] = {
    //     0.0f, -0.5f, 1.0f, 0.0f, 0.0f,
    //     0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
    //     -0.5f, 0.5f, 0.0f, 0.0f, 1.0f
    // };
    // VertexBufferCreateParams vbParams;
    // vbParams.size = sizeof(vertices);
    // vbParams.vertexCount = 3;
    // vertexBuffer = VertexBuffer::Create(vbParams);
    // void *mapped = vertexBuffer->Map();
    // std::memcpy(mapped, vertices, vbParams.size);
    // vertexBuffer->Unmap();

    // //VertexLayout
    // auto vertexBufferLayout = VertexBufferLayout::Create({
    //     {CT_TEXT("VertexPosition"), VertexDataType::Float2},
    //     {CT_TEXT("VertexColor"), VertexDataType::Float3}
    // });
    // vertexLayout = VertexLayout::Create();
    // vertexLayout->AddBufferLayout(vertexBufferLayout);

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

    window = glfwCreateWindow(WIDTH, HEIGHT, "LearnVK", nullptr, nullptr);

    InitVulkan();

    auto &context = VulkanContext::Get();
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // auto &frameData = context.GetCurrentFrameData();

        // frameData.fence->Wait();
        // context.GetSwapChain()->AcquireBackBuffer();
        // frameData.fence->Reset();

        // context.SetCurrentFrameBuffer(context.GetSwapChain()->GetCurentBackBufferData().frameBuffer);
        // context.SetCurrentRenderPipelineState(pipelineState);

        // auto backBufferIndex = context.GetSwapChain()->GetCurrentBackBufferIndex();
        // if (commandBuffers.Count() <= backBufferIndex)
        // {
        //     auto commandBuffer = context.GetRenderCommandPool()->GetIdleBuffer();
        //     commandBuffer->Begin();
        //     commandBuffer->SetVertexLayout(vertexLayout);
        //     commandBuffer->SetVertexBuffers(0, &vertexBuffer, 1);
        //     commandBuffer->BeginRenderPass();
        //     commandBuffer->BindRenderPipeline();
        //     commandBuffer->SetViewport(0, 0, WIDTH, HEIGHT);
        //     commandBuffer->SetScissor(0, 0, WIDTH, HEIGHT);
        //     commandBuffer->Draw(0, 3, 1);
        //     commandBuffer->EndRenderPass();
        //     commandBuffer->End();

        //     commandBuffers.Add(commandBuffer);
        // }

        // const auto &commandBuffer = commandBuffers[backBufferIndex];
        // context.Submit(commandBuffer);
        // context.Present();
    }

    CleanupVulkan();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}