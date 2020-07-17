#include "RenderCore/RenderAPI.h"
#include "RenderCore/Sampler.h"
#include "RenderCore/Texture.h"

SPtr<Device> RenderAPI::device;

Device *RenderAPI::GetDevice()
{
    return device.get();
}

Device *RenderAPI::CreateDevice(RenderWindow *window, const DeviceDesc &desc)
{
    CT_CHECK(!device);
    device = Device::Create(window, desc);
    return device.get();
}

//====================================================================

namespace
{

struct InternalCache
{
    SPtr<Sampler> defaultSampler;
    SPtr<Texture> defaultTexture2D;
};

InternalCache cache;

}

SPtr<Sampler> Sampler::GetDefault()
{
    if (!cache.defaultSampler)
    {
        cache.defaultSampler = Create({});
    }
    return cache.defaultSampler;
}

SPtr<Texture> Texture::GetDefault2D()
{
    if (!cache.defaultTexture2D)
    {
        uchar8 data[] = { 255, 255, 255, 255 };
        cache.defaultTexture2D = Create2D(1, 1, ResourceFormat::RGBA8Unorm, 1, 1, data);
    }
    return cache.defaultTexture2D;
}
