#pragma once


#include "RenderCore/RootSignature.h"
#include "RenderCore/DescriptorSet.h"
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
    ParameterBlock(const SPtr<ParameterBlockReflection> &reflection);
    virtual ~ParameterBlock() = default;

    bool SetBuffer(const String &name, const SPtr<Buffer> &buffer);
    bool SetTexture(const String &name, const SPtr<Texture> &texture);
    bool SetSrv(const String &name, const SPtr<ResourceView> &srv);
    bool SetUav(const String &name, const SPtr<ResourceView> &uav);
    bool SetCbv(const String &name, const SPtr<ResourceView> &cbv);
    bool SetSampler(const ShaderVarLocation &location, const SPtr<Sampler> &sampler);
    bool SetSampler(const String &name, const SPtr<Sampler> &sampler);

    const SPtr<ParameterBlockReflection> &GetReflection() const
    {
        return reflection;
    }

    const SPtr<ReflectionType> &GetElementType() const
    {
        return reflection->GetElementType();
    }

    uint32 GetSize() const
    {
        return data.Count();
    }

    static SPtr<ParameterBlock> Create(const SPtr<ParameterBlockReflection> &reflection);

protected:
    uint32 GetFlatIndex(const ShaderVarLocation &location) const;
    void CheckResourceLocation(const ShaderVarLocation &location) const;
    void CheckDescriptorType(const ShaderVarLocation &location, DescriptorType descriptorType) const;
    void MarkDescriptorSetDirty(const ShaderVarLocation &location);

    bool BindIntoDescriptorSet(uint32 setIndex);

    bool SetResourceSrvUav(const ShaderVarLocation &location, const Resource *resource);
    bool PrepareResources(CopyContext *ctx);
    bool PrepareDescriptorSets(CopyContext *ctx);

protected:
    SPtr<ParameterBlockReflection> reflection;
    Array<uint8> data;
    Array<SPtr<DescriptorSet>> sets;

    //Array<SPtr<ResourceView>> cbvs;
    Array<SPtr<ResourceView>> srvs;
    Array<SPtr<ResourceView>> uavs;
    Array<SPtr<Sampler>> samplers;
};


}