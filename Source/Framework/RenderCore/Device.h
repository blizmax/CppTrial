#pragma once

#include "RenderCore/Resource.h"
#include "RenderCore/RenderWindow.h"
#include "RenderCore/GpuFence.h"
#include "RenderCore/RenderContext.h"
#include "RenderCore/FrameBuffer.h"

namespace RenderCore
{

struct DeviceDesc
{
    ResourceFormat colorFormat = ResourceFormat::BGRA8UnormSrgb; //BGRAUnorm;
    ResourceFormat depthFormat = ResourceFormat::D32Float;
    bool vsync = true;
};

class Device
{
public:
    virtual ~Device() = default;

    const DeviceDesc &GetDesc() const
    {
        return desc;
    }

    SPtr<FrameBuffer> GetSwapChainFrameBuffer() const
    {
        return swapChainFrameBuffers[curBackBufferIndex];
    }

    RenderContext *GetRenderContext() const
    {
        return renderContext.get();
    }

    virtual void ResizeSwapChain(uint32 width, uint32 height) = 0;
    virtual void Present() = 0;
    virtual void FlushAndSync() = 0;

    static SPtr<Device> Create(RenderWindow *window, const DeviceDesc &desc);

protected:
    Device(RenderWindow *window, const DeviceDesc &desc)
        : window(window), desc(desc)
    {
    }

protected:
    RenderWindow *window;
    DeviceDesc desc;

    SPtr<GpuFence> frameFence;
    SPtr<RenderContext> renderContext;

    uint32 backBufferCount;
    Array<SPtr<FrameBuffer>> swapChainFrameBuffers;
    uint32 curBackBufferIndex = 0;
};
}