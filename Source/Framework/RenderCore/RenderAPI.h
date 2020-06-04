#pragma once

#include "RenderCore/.Package.h"
#include "RenderCore/Device.h"

namespace RenderCore
{
class RenderAPI
{
public:
    static void Init();
    static void Destroy();

    static Device *GetDevice();
    static Device *CreateDevice(RenderWindow *window, const DeviceDesc &desc);

private:
    static SPtr<Device> device;
};
}