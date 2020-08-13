#pragma once

#include "RenderCore/Device.h"
#include "RenderCore/Sampler.h"
#include "RenderCore/Texture.h"
#include "RenderCore/VertexArray.h"

class RenderAPI
{
public:
    static void Init();
    static void Destroy();

    static Device *GetDevice();
    static Device *CreateDevice(RenderWindow *window, const DeviceDesc &desc);

    static SPtr<Texture> GetDefaultTexture2D();
    static SPtr<Sampler> GetDefaultSampler();
    static SPtr<VertexArray> GetDefaultQuadVertexArray();


private:
    static SPtr<Device> device;
};