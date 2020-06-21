#pragma once

#include "RenderCore/RootSignature.h"
#include "RenderCore/DescriptorSet.h"
#include "RenderCore/Texture.h"
#include "RenderCore/Buffer.h"
#include "RenderCore/Sampler.h"
#include "RenderCore/Program.h"
#include "Core/HashMap.h"

class ParameterBlock
{
public:
    ParameterBlock(const SPtr<ParameterBlockReflection> &reflection);
    virtual ~ParameterBlock() = default;

    ShaderVar GetRootVar() const;
    SPtr<Buffer> GetUnderlyingConstantBuffer() const;
    bool PrepareDescriptorSets(CopyContext *ctx);

    SPtr<Buffer> GetBuffer(const ShaderVarLocation &location) const;
    SPtr<Buffer> GetBuffer(const String &name) const;
    SPtr<Texture> GetTexture(const ShaderVarLocation &location) const;
    SPtr<Texture> GetTexture(const String &name) const;
    SPtr<ResourceView> GetSrv(const ShaderVarLocation &location) const;
    SPtr<ResourceView> GetUav(const ShaderVarLocation &location) const;
    SPtr<Sampler> GetSampler(const ShaderVarLocation &location) const;
    SPtr<Sampler> GetSampler(const String &name) const;
    SPtr<ParameterBlock> GetParameterBlock(const ShaderVarLocation &location) const;
    SPtr<ParameterBlock> GetParameterBlock(const String &name) const;

    bool SetBuffer(const ShaderVarLocation &location, const SPtr<Buffer> &buffer);
    bool SetBuffer(const String &name, const SPtr<Buffer> &buffer);
    bool SetTexture(const ShaderVarLocation &location, const SPtr<Texture> &texture);
    bool SetTexture(const String &name, const SPtr<Texture> &texture);
    bool SetSrv(const ShaderVarLocation &location, const SPtr<ResourceView> &srv);
    bool SetUav(const ShaderVarLocation &location, const SPtr<ResourceView> &uav);
    bool SetSampler(const ShaderVarLocation &location, const SPtr<Sampler> &sampler);
    bool SetSampler(const String &name, const SPtr<Sampler> &sampler);
    bool SetParameterBlock(const ShaderVarLocation &location, const SPtr<ParameterBlock> &block);
    bool SetParameterBlock(const String &name, const SPtr<ParameterBlock> &block);

    template <typename T>
    bool Set(const ShaderVarLocation &location, const T &val)
    {
        CT_CHECK(location.varType->IsData());
        //TODO Check val type
        uint8 *ptr = constBufferData.GetData() + location.byteOffset;
        *(T *)ptr = val;
        MarkUniformDataDirty();
        return true;
    }

    bool SetBlob(const void *data, uint32 offset, uint32 size);
    bool SetBlob(const ShaderVarLocation &location, const void *data, uint32 size);

    template <typename T>
    bool SetBlob(const ShaderVarLocation &location, const T &blob)
    {
        return SetBlob(location, &blob, sizeof(T));
    }

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
        return constBufferData.Count();
    }

    const Array<SPtr<DescriptorSet>> &GetDescriptorSets() const
    {
        return sets;
    }

    static SPtr<ParameterBlock> Create(const SPtr<ParameterBlockReflection> &reflection);

protected:
    int32 GetFlatIndex(const ShaderVarLocation &location) const;
    void CheckResourceLocation(const ShaderVarLocation &location) const;
    void CheckDescriptorType(const ShaderVarLocation &location, DescriptorType descriptorType) const;
    void CheckDescriptorSrv(const ShaderVarLocation &location, const ResourceView *view) const;
    void CheckDescriptorUav(const ShaderVarLocation &location, const ResourceView *view) const;
    bool IsBufferVarValid(const ShaderVar &var, const Buffer *buffer) const;
    bool IsTextureVarValid(const ShaderVar &var, const Texture *texture) const;
    bool IsSamplerVarValid(const ShaderVar &var, const Sampler *sampler) const;
    bool IsParameterBlockVarValid(const ShaderVar &var, const ParameterBlock *block) const;
    void MarkDescriptorSetDirty(const ShaderVarLocation &location);
    void MarkDescriptorSetDirty(int32 setIndex);
    void MarkUniformDataDirty();

    Resource *GetResourceSrvUavCommon(const ShaderVarLocation &location) const;
    bool SetResourceSrvUavCommon(const ShaderVarLocation &location, const SPtr<Resource> &resource);

    void CreateParameterBlocks(const ShaderVar &var);
    void UpdateConstantBuffer();
    bool PrepareResources(CopyContext *ctx);
    bool PrepareResourcesAndConstantBuffer(CopyContext *ctx);
    bool BindIntoDescriptorSet(int32 setIndex, const SPtr<DescriptorSet> &set);

protected:
    SPtr<ParameterBlockReflection> reflection;
    Array<SPtr<DescriptorSet>> sets;

    ParameterBlock *parent = nullptr;
    Array<SPtr<ParameterBlock>> parameterBlocks;
    Array<SPtr<ResourceView>> srvs;
    Array<SPtr<ResourceView>> uavs;
    Array<SPtr<Sampler>> samplers;

    Array<uint8> constBufferData;
    mutable SPtr<Buffer> constantBuffer;
    mutable bool constantBufferDirty = false;
};

class ShaderVar
{
public:
    ShaderVar() = default;

    explicit ShaderVar(ParameterBlock *block) : block(block)
    {
        if (block)
            location = ShaderVarLocation(block->GetElementType(), VarLocation());
    }

    ShaderVar(ParameterBlock *block, const ShaderVarLocation &location) : block(block), location(location)
    {
    }

    ShaderVar FindMember(const String &name) const;
    ShaderVar FindMember(int32 index) const;

    SPtr<Buffer> GetBuffer() const;
    SPtr<Texture> GetTexture() const;
    SPtr<ResourceView> GetSrv() const;
    SPtr<ResourceView> GetUav() const;
    SPtr<Sampler> GetSampler() const;
    SPtr<ParameterBlock> GetParameterBlock() const;
    bool SetBuffer(const SPtr<Buffer> &buffer) const;
    bool SetTexture(const SPtr<Texture> &texture) const;
    bool SetSrv(const SPtr<ResourceView> &srv) const;
    bool SetUav(const SPtr<ResourceView> &uav) const;
    bool SetSampler(const SPtr<Sampler> &sampler) const;
    bool SetParameterBlock(const SPtr<ParameterBlock> &block) const;

    template <typename T>
    bool Set(const T &val) const
    {
        return block->Set(location, val);
    }

    bool SetBlob(const void *data, uint32 size) const;

    template <typename T>
    bool SetBlob(const T &val) const
    {
        return SetBlob(&val, sizeof(T));
    }

    bool IsValid() const
    {
        return location.IsValid();
    }

    const SPtr<ReflectionType> &GetVarType() const
    {
        return location.varType;
    }

    const ShaderVarLocation &GetLocation() const
    {
        return location;
    }

    void operator=(const SPtr<Buffer> &buffer) const
    {
        SetBuffer(buffer);
    }

    void operator=(const SPtr<Texture> &texture) const
    {
        SetTexture(texture);
    }

    void operator=(const SPtr<Sampler> &sampler) const
    {
        SetSampler(sampler);
    }

    template <typename T>
    void operator=(const T &val) const
    {
        Set(val);
    }

    operator SPtr<Buffer>() const;
    operator SPtr<Texture>() const;
    operator SPtr<Sampler>() const;

    ShaderVar operator[](const String &name) const;
    ShaderVar operator[](int32 index) const;

private:
    ParameterBlock *block = nullptr;
    ShaderVarLocation location;
};