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

    static SPtr<Device> CreateDevice(RenderWindow *window, const DeviceDesc &desc);
};
}