#pragma once


#include "RenderCore/RootSignature.h"
#include "RenderCore/Texture.h"
#include "RenderCore/Buffer.h"
#include "RenderCore/Sampler.h"

namespace RenderCore
{

class ParameterBlock
{
public:
    virtual ~ParameterBlock() = default;

    bool SetBuffer(const String &name, const SPtr<Buffer> &buffer);
    bool SetTexture(const String &name, const SPtr<Texture> &texture);
    bool SetSrv(const String &name, const SPtr<ResourceView> &srv);
    bool SetUav(const String &name, const SPtr<ResourceView> &uav);
    bool SetCbv(const String &name, const SPtr<ResourceView> &cbv);
    bool SetSampler(const String &name, const SPtr<Sampler> &sampler);

protected:
    bool PrepareDescriptorSets(CopyContext *ctx);

};


}