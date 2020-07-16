#include "RenderCore/RenderAPI.h"
#include "RenderCore/Sampler.h"

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
