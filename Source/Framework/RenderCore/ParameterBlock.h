#pragma once


#include "RenderCore/RootSignature.h"
#include "RenderCore/Texture.h"
#include "RenderCore/Buffer.h"
#include "RenderCore/Sampler.h"
#include "RenderCore/Program.h"
#include "Core/HashMap.h"

namespace RenderCore
{

class CopyContext;

class ParameterBlock
{
public:
    ParameterBlock(const SPtr<ProgramReflection> &reflection);
    virtual ~ParameterBlock() = default;

    bool SetBuffer(const String &name, const SPtr<Buffer> &buffer);
    bool SetTexture(const String &name, const SPtr<Texture> &texture);
    bool SetSrv(const String &name, const SPtr<ResourceView> &srv);
    bool SetUav(const String &name, const SPtr<ResourceView> &uav);
    bool SetCbv(const String &name, const SPtr<ResourceView> &cbv);
    bool SetSampler(const String &name, const SPtr<Sampler> &sampler);

    static SPtr<ParameterBlock> Create(const SPtr<ProgramReflection> &reflection);

protected:
    bool SetResourceSrvUav(const Resource *resource, const ProgramReflection::BindingData &binding);
    bool PrepareDescriptorSets(CopyContext *ctx);

protected:
    SPtr<ProgramReflection> reflection;

    HashMap<int32, SPtr<ResourceView>> cbvs; //TODO Owns const buffers?

    HashMap<int32, SPtr<ResourceView>> srvs;
    HashMap<int32, SPtr<ResourceView>> uavs;
    HashMap<int32, SPtr<Sampler>> samplers;

};


}