#include "Render/RenderManager.h"
#include "Application/Application.h"

RenderManager renderManager;
RenderManager *gRenderManager = &renderManager;

void RenderManager::Startup()
{
    RenderAPI::Init();
    RenderAPI::CreateDevice(&gApp->GetWindow(), DeviceDesc());

    auto device = RenderAPI::GetDevice();
    auto swapChainFrameBuffer = device->GetSwapChainFrameBuffer();
    targetFrameBuffer = FrameBuffer::Create2D(swapChainFrameBuffer->GetWidth(), swapChainFrameBuffer->GetHeight(), swapChainFrameBuffer->GetDesc());
}

void RenderManager::Shutdown()
{
    targetFrameBuffer.reset();

    RenderAPI::Destroy();
}

void RenderManager::Tick()
{
    auto ctx = GetRenderContext();
    ctx->ClearFrameBuffer(targetFrameBuffer.get(), Color::BLACK, 1.0f, 0);
}

void RenderManager::Present()
{
    auto device = RenderAPI::GetDevice();
    auto ctx = device->GetRenderContext();
    auto swapChainFrameBuffer = device->GetSwapChainFrameBuffer();
    ctx->CopyResource(swapChainFrameBuffer->GetColorTexture(0).get(), targetFrameBuffer->GetColorTexture(0).get());
    device->Present();
}

RenderContext *RenderManager::GetRenderContext() const
{
    auto device = RenderAPI::GetDevice();
    return device->GetRenderContext();
}
