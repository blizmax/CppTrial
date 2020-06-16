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

    auto &window = gApp->GetWindow();
    window.windowResizedHandler.On([this](WindowResizedEvent &e) {
        if (e.width <= 0 || e.height <= 0) //minimize
            return;

        auto device = RenderAPI::GetDevice();
        device->ResizeSwapChain(e.width, e.height);
        auto swapChainFrameBuffer = device->GetSwapChainFrameBuffer();
        auto oldFbo = targetFrameBuffer;
        targetFrameBuffer = FrameBuffer::Create2D(swapChainFrameBuffer->GetWidth(), swapChainFrameBuffer->GetHeight(), swapChainFrameBuffer->GetDesc());
        device->GetRenderContext()->Blit(oldFbo->GetColorTexture(0)->GetSrv().get(), targetFrameBuffer->GetRtv(0).get());
    });
}

void RenderManager::Shutdown()
{
    targetFrameBuffer.reset();

    RenderAPI::Destroy();
}

void RenderManager::Tick()
{
}

void RenderManager::Present()
{
    // FIXME pause render rather than pause present when window minimized
    if (gApp->GetWindow().Minimized())
        return;

    auto device = RenderAPI::GetDevice();
    auto ctx = device->GetRenderContext();
    auto swapChainFrameBuffer = device->GetSwapChainFrameBuffer();
    ctx->CopyResource(swapChainFrameBuffer->GetColorTexture(0).get(), targetFrameBuffer->GetColorTexture(0).get());
    device->Present();

    ctx->ClearFrameBuffer(targetFrameBuffer.get(), Color(0.1f, 0.1f, 0.1f), 1.0f, 0);
}

RenderContext *RenderManager::GetRenderContext() const
{
    auto device = RenderAPI::GetDevice();
    return device->GetRenderContext();
}
