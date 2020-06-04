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

class VkWindow;
class Renderer;

Device *device;
SPtr<VkWindow> window;
SPtr<Renderer> renderer;
SPtr<FrameBuffer> targetFbo;
SPtr<Program> program;

SPtr<Program> reflectionProgram;

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
    SPtr<GraphicsState> state;
    SPtr<VertexArray> vao;
    SPtr<RasterizationState> rasterizationState;
    SPtr<DepthStencilState> depthStencilState;
    SPtr<BlendState> blendState;

    Renderer()
    {
        state = GraphicsState::Create();

        auto vertexBufferLayout = VertexBufferLayout::Create({
            {CT_TEXT("VertexPosition"), ResourceFormat::RG32Float},
            {CT_TEXT("VertexColor"), ResourceFormat::RGB32Float}
        });
        auto vertexLayout = VertexLayout::Create();
        vertexLayout->AddBufferLayout(vertexBufferLayout);

        float vertices[] = {
            0.0f, -0.5f, 1.0f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.0f, 0.0f, 1.0f
        };
        auto vbo = Buffer::Create(sizeof(vertices), ResourceBind::Vertex, CpuAccess::None, vertices);

        vao = VertexArray::Create();
        vao->SetVertexLayout(vertexLayout);
        vao->AddVertexBuffer(vbo);

        {
            RasterizationStateDesc desc;
            rasterizationState = RasterizationState::Create(desc);
        }
        {
            DepthStencilStateDesc desc;
            depthStencilState = DepthStencilState::Create(desc);
        }
        {
            BlendStateDesc desc;
            desc.attachments.SetCount(1);
            blendState = BlendState::Create(desc);
        }

        state->SetVertexArray(vao);
        state->SetRasterizationState(rasterizationState);
        state->SetDepthStencilState(depthStencilState);
        state->SetBlendState(blendState);
    }

    void Render(RenderContext *ctx, const SPtr<FrameBuffer> &fbo)
    {
        state->SetFrameBuffer(fbo);
        state->SetProgram(program);

        ctx->ClearFrameBuffer(fbo.get(), Color::BLACK, 1.0f, 0);
        ctx->Draw(state.get(), nullptr, 3, 0);
    }
};

SPtr<Program> CreateProgram()
{
    ProgramDesc desc;
    IO::FileHandle vertSrcFile(CT_TEXT("Assets/Shaders/LearnVK/shader.vert"));
    IO::FileHandle fragSrcFile(CT_TEXT("Assets/Shaders/LearnVK/shader.frag"));
    desc.shaderDescs.Add({ShaderType::Vertex, vertSrcFile.ReadString()});
    desc.shaderDescs.Add({ShaderType::Pixel, fragSrcFile.ReadString()});
    return Program::Create(desc);
}

SPtr<VkWindow> CreateWindow()
{
    return Memory::MakeShared<VkWindow>();
}

SPtr<Renderer> CreateRenderer()
{
    return Memory::MakeShared<Renderer>();
}

int main(int argc, char **argv)
{
    window = CreateWindow();

    RenderAPI::Init();
    device = RenderAPI::CreateDevice(window.get(), DeviceDesc());

    auto backBufferFbo = device->GetSwapChainFrameBuffer();
    targetFbo = FrameBuffer::Create2D(backBufferFbo->GetWidth(), backBufferFbo->GetHeight(), backBufferFbo->GetDesc());

    program = CreateProgram();

    {
        ProgramDesc desc;
        IO::FileHandle fragSrcFile(CT_TEXT("Assets/Shaders/LearnVK/reflection.frag"));
        desc.shaderDescs.Add({ShaderType::Pixel, fragSrcFile.ReadString()});
        reflectionProgram = Program::Create(desc);
    }

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