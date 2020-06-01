#include "Core/String.h"
#include "Core/Exception.h"
#include "IO/FileHandle.h"

#include "RenderCore/RenderAPI.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "RenderVulkan/VulkanRenderWindow.h"

using namespace RenderCore;

const int32 WIDTH = 800;
const int32 HEIGHT = 600;

class VkWindow : public VulkanRenderWindow
{
public:
    GLFWwindow *window;

    VkWindow()
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        window = glfwCreateWindow(WIDTH, HEIGHT, "LearnVK", nullptr, nullptr);
    }

    ~VkWindow()
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    virtual uint32 GetPositionX() const override
    {
        int32 x, y;
        glfwGetWindowPos(window, &x, &y);
        return x;
    }

    virtual uint32 GetPositionY() const override
    {   
        int32 x, y;
        glfwGetWindowPos(window, &x, &y);
        return y;
    }

    virtual uint32 GetWidth() const override
    {
        int32 w, h;
        glfwGetWindowSize(window, &w, &h);
        return w;
    }

    virtual uint32 GetHeight() const override
    {
        int32 w, h;
        glfwGetWindowSize(window, &w, &h);
        return h;
    }

    virtual VkSurfaceKHR CreateSurface() override
    {
        VkSurfaceKHR surface;
        if (glfwCreateWindowSurface(gVulkanInstance, window, gVulkanAlloc, &surface) != VK_SUCCESS)
            CT_EXCEPTION(LearnVK, "Create surface failed.");
        return surface;
    }

    bool Run()
    {
        glfwPollEvents();

        if(glfwWindowShouldClose(window))
            return false;
        return true;
    }

};

class Renderer
{
public:
    void Render(RenderContext *ctx, const SPtr<FrameBuffer> &fbo)
    {

    }
};

SPtr<VkWindow> window;
SPtr<Device> device;
SPtr<Renderer> renderer;
SPtr<FrameBuffer> targetFbo;

// SPtr<Shader> CreateShader()
// {
//     ShaderDesc desc;
//     IO::FileHandle vertSrcFile(CT_TEXT("Assets/Shaders/LearnVK/shader.vert"));
//     IO::FileHandle fragSrcFile(CT_TEXT("Assets/Shaders/LearnVK/shader.frag"));
//     desc.vertexSource = vertSrcFile.ReadString();
//     desc.fragmentSource = fragSrcFile.ReadString();
//     return Shader::Create(desc);
// }

SPtr<VkWindow> CreateWindow()
{
    return Memory::MakeShared<VkWindow>();
}

SPtr<Renderer> CreateRenderer()
{
    return Memory::MakeShared<Renderer>();
}

// void InitVulkan()
// {
//     auto &context = VulkanContext::Get();
//     context.Init();

//     // VkSurfaceKHR surface;
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

//}

int main(int argc, char **argv)
{
    window = CreateWindow();

    RenderAPI::Init();
    device = RenderAPI::CreateDevice(window.get(), DeviceDesc());
    
    auto backBufferFbo = device->GetSwapChainFrameBuffer();
    targetFbo = FrameBuffer::Create2D(backBufferFbo->GetWidth(), backBufferFbo->GetHeight(), backBufferFbo->GetDesc());

    renderer = CreateRenderer();

    while(window->Run())
    {
        auto renderContext = device->GetRenderContext();
        auto swapChainFbo = device->GetSwapChainFrameBuffer();

        renderer->Render(renderContext, targetFbo);
        renderContext->CopyResource(swapChainFbo->GetColorTexture(0).get(), targetFbo->GetColorTexture(0).get());
        device->Present();
    }

    return 0;
}