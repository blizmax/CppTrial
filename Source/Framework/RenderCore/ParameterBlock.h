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
class ShaderVar;

class ParameterBlock
{
public:
    ParameterBlock(const SPtr<ParameterBlockReflection> &reflection);
    virtual ~ParameterBlock() = default;

    SPtr<Buffer> GetBuffer(const ShaderVarLocation &location) const;
    SPtr<Buffer> GetBuffer(const String &name) const;
    SPtr<Texture> GetTexture(const ShaderVarLocation &location) const;
    SPtr<Texture> GetTexture(const String &name) const;
    SPtr<ResourceView> GetSrv(const ShaderVarLocation &location) const;
    SPtr<ResourceView> GetUav(const ShaderVarLocation &location) const;
    SPtr<Sampler> GetSampler(const ShaderVarLocation &location) const;
    SPtr<Sampler> GetSampler(const String &name) const;

    bool SetBuffer(const ShaderVarLocation &location, const SPtr<Buffer> &buffer);
    bool SetBuffer(const String &name, const SPtr<Buffer> &buffer);
    bool SetTexture(const ShaderVarLocation &location, const SPtr<Texture> &texture);
    bool SetTexture(const String &name, const SPtr<Texture> &texture);
    bool SetSrv(const ShaderVarLocation &location, const SPtr<ResourceView> &srv);
    bool SetUav(const ShaderVarLocation &location, const SPtr<ResourceView> &uav);
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
    void CheckDescriptorSrv(const ShaderVarLocation &location, const SPtr<ResourceView> &view) const;
    void CheckDescriptorUav(const ShaderVarLocation &location, const SPtr<ResourceView> &view) const;
    void MarkDescriptorSetDirty(const ShaderVarLocation &location);
    void MarkUniformDataDirty();

    bool BindIntoDescriptorSet(uint32 setIndex);

    SPtr<Resource> GetReourceSrvUavCommon(const ShaderVarLocation &location) const;
    bool SetResourceSrvUavCommon(const ShaderVarLocation &location, const SPtr<Resource> &resource);

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

class ShaderVar
{
public:
    ShaderVar(ParameterBlock *block) : block(block)
    {
    }

    ShaderVar FindMember(const String &name) const;
    ShaderVar FindMember(int32 index) const;

    SPtr<Buffer> GetBuffer() const;
    SPtr<Texture> GetTexture() const;
    SPtr<ResourceView> GetSrv() const;
    SPtr<ResourceView> GetUav() const;
    SPtr<Sampler> GetSampler() const;
    bool SetBuffer(const SPtr<Buffer> &buffer) const;
    bool SetTexture(const SPtr<Texture> &texture) const;
    bool SetSrv(const SPtr<ResourceView> &srv) const;
    bool SetUav(const SPtr<ResourceView> &uav) const;
    bool SetSampler(const SPtr<Sampler> &sampler) const;

    ShaderVar operator[](const String &name) const;
    ShaderVar operator[](int32 index) const;

private:
    ParameterBlock *block = nullptr;
    ShaderVarLocation location;
};

}