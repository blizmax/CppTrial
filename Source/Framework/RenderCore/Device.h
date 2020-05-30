#pragma once

#include "RenderCore/Resource.h"
#include "RenderCore/RenderWindow.h"
#include "RenderCore/GpuFence.h"
#include "RenderCore/RenderContext.h"
#include "RenderCore/FrameBuffer.h"
#include "Core/List.h"

namespace RenderCore
{

using ReleaseFunc = std::function<void(void)>;

class DeferredReleaser
{
public:
    struct ReleaseData
    {
        uint64 fenceValue;
        ReleaseFunc releaseFunc;
    };

    void AddReleaseData(const ReleaseFunc &func, uint64 fenceValue)
    {
        releaseDatas.Add({fenceValue, func});
    }

    void Release(uint64 fenceValue)
    {
        while(releaseDatas.Count() && releaseDatas.First().fenceValue <= fenceValue)
        {
            releaseDatas.First().releaseFunc();
            releaseDatas.RemoveFirst();
        }
    }

private:
    List<ReleaseData> releaseDatas;
};

struct DeviceDesc
{
    ResourceFormat colorFormat = ResourceFormat::BGRA8UnormSrgb; //BGRAUnorm;
    ResourceFormat depthFormat = ResourceFormat::D32Float;
    bool vsync = true;
};

class Device
{
public:
    Device(RenderWindow *window, const DeviceDesc &desc)
        : window(window), desc(desc)
    {
    }

    virtual ~Device() = default;

    virtual void ResizeSwapChain(uint32 width, uint32 height) = 0;
    virtual void Present() = 0;
    virtual void FlushAndSync() = 0;

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

    void Release(const ReleaseFunc &func)
    {
        deferredReleaser.AddReleaseData(func, frameFence->GetCpuValue());
    }

    static SPtr<Device> Create(RenderWindow *window, const DeviceDesc &desc);

protected:
    void ExecuteDeferredRelease()
    {
        deferredReleaser.Release(frameFence->GetGpuValue());
    }

protected:
    RenderWindow *window;
    DeviceDesc desc;

    SPtr<GpuFence> frameFence;
    DeferredReleaser deferredReleaser;
    SPtr<RenderContext> renderContext;

    uint32 backBufferCount;
    Array<SPtr<FrameBuffer>> swapChainFrameBuffers;
    uint32 curBackBufferIndex = 0;
};
}