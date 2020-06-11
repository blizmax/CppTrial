#include "RenderCore/ParameterBlock.h"
#include "RenderCore/RenderAPI.h"

namespace RenderCore
{

SPtr<ParameterBlock> ParameterBlock::Create(const SPtr<ParameterBlockReflection> &reflection)
{
    return Memory::MakeShared<ParameterBlock>(reflection);
}

ParameterBlock::ParameterBlock(const SPtr<ParameterBlockReflection> &reflection)
    : reflection(reflection)
{
    data.SetCount(reflection->GetElementType()->GetSize());
    sets.SetCount(reflection->GetSetInfoCount());

    auto structType = reflection->GetElementType()->AsStruct();
    srvs.SetCount(structType->srvCount);
    uavs.SetCount(structType->uavCount);
    samplers.SetCount(structType->samplerCount);

    //TODO
    //cbvCount
    //Create uniform buffer
}

uint32 ParameterBlock::GetFlatIndex(const ShaderVarLocation &location) const
{
    auto &elementType = GetElementType();
    auto &range = elementType->GetBindingRange(location.rangeIndex);
    return range.baseIndex + location.arrayIndex;
}

void ParameterBlock::CheckResourceLocation(const ShaderVarLocation &location) const
{
    auto &elementType = GetElementType();
    CT_CHECK(location.rangeIndex < elementType->GetBindingRangeCount());
    auto &range = elementType->GetBindingRange(location.rangeIndex);
    CT_CHECK(location.arrayIndex < range.count);
}

void ParameterBlock::CheckDescriptorType(const ShaderVarLocation &location, DescriptorType descriptorType) const
{
    auto &elementType = GetElementType();
    auto &range = elementType->GetBindingRange(location.rangeIndex);
    CT_CHECK(range.descriptorType == descriptorType);
}

void ParameterBlock::CheckDescriptorSrv(const ShaderVarLocation &location, const ResourceView *view) const
{
    auto &elementType = GetElementType();
    auto &range = elementType->GetBindingRange(location.rangeIndex);
    auto descriptorType = range.descriptorType;

    CT_CHECK(descriptorType == DescriptorType::TextureSrv || descriptorType == DescriptorType::RawBufferSrv
        || descriptorType == DescriptorType::TypedBufferSrv || descriptorType == DescriptorType::StructuredBufferSrv);
}

void ParameterBlock::CheckDescriptorUav(const ShaderVarLocation &location, const ResourceView *view) const
{
    auto &elementType = GetElementType();
    auto &range = elementType->GetBindingRange(location.rangeIndex);
    auto descriptorType = range.descriptorType;

    CT_CHECK(descriptorType == DescriptorType::TextureUav || descriptorType == DescriptorType::RawBufferUav
        || descriptorType == DescriptorType::TypedBufferUav || descriptorType == DescriptorType::StructuredBufferUav);
}

bool ParameterBlock::IsBufferVarValid(const ShaderVar &var, const Buffer *buffer) const
{
    //TODO
    return true;
}

bool ParameterBlock::IsTextureVarValid(const ShaderVar &var, const Texture *texture) const
{
    //TODO
    return true;
}

bool ParameterBlock::IsSamplerVarValid(const ShaderVar &var, const Sampler *sampler) const
{

}

void ParameterBlock::MarkDescriptorSetDirty(const ShaderVarLocation &location)
{
    auto setIndex = reflection->GetBindingInfo(location.rangeIndex).set;
    sets[setIndex] = nullptr;
}

void ParameterBlock::MarkUniformDataDirty()
{
    //TODO
}

bool ParameterBlock::BindIntoDescriptorSet(uint32 setIndex)
{
    const auto &set = sets[setIndex];

    //TODO
    // for(int32 slot : reflection->GetBindingSlots(setIndex))
    // {
    //     const auto &bindingData = reflection->GetBindingData(slot);
    //     uint32 binding = bindingData.binding;
    //     auto descriptorType = bindingData.descriptorType;

    //     // switch (descriptorType)
    //     // {
    //     // case DescriptorType::Cbv:
    //     //     {
    //     //         set->SetCbv( [slot]
    //     //     }
    //     // }
    // }
}

SPtr<Resource> ParameterBlock::GetReourceSrvUavCommon(const ShaderVarLocation &location) const
{

}

bool ParameterBlock::SetResourceSrvUavCommon(const ShaderVarLocation &location, const SPtr<Resource> &resource)
{

}

ShaderVar ParameterBlock::GetRootVar() const
{
    return ShaderVar(const_cast<ParameterBlock *>(this));
}

SPtr<Buffer> ParameterBlock::GetBuffer(const ShaderVarLocation &location) const
{
    return GetReourceSrvUavCommon(location)->AsBuffer();
}

SPtr<Buffer> ParameterBlock::GetBuffer(const String &name) const
{
    auto var = GetRootVar()[name];
    if (!IsBufferVarValid(var, nullptr))
        return nullptr;
    return var.GetBuffer();
}

SPtr<Texture> ParameterBlock::GetTexture(const ShaderVarLocation &location) const
{
    return GetReourceSrvUavCommon(location)->AsTexture();
}

SPtr<Texture> ParameterBlock::GetTexture(const String &name) const
{
    auto var = GetRootVar()[name];
    if (!IsTextureVarValid(var, nullptr))
        return nullptr;
    return var.GetTexture();
}

SPtr<ResourceView> ParameterBlock::GetSrv(const ShaderVarLocation &location) const
{
    CheckResourceLocation(location);
    CheckDescriptorSrv(location, nullptr);
    auto flatIndex = GetFlatIndex(location);
    return srvs[flatIndex];
}

SPtr<ResourceView> ParameterBlock::GetUav(const ShaderVarLocation &location) const
{
    CheckResourceLocation(location);
    CheckDescriptorSrv(location, nullptr);
    auto flatIndex = GetFlatIndex(location);
    return uavs[flatIndex];
}

SPtr<Sampler> ParameterBlock::GetSampler(const ShaderVarLocation &location) const
{
    CheckResourceLocation(location);
    CheckDescriptorType(location, DescriptorType::Sampler);
    auto flatIndex = GetFlatIndex(location);
    return samplers[flatIndex];
}

SPtr<Sampler> ParameterBlock::GetSampler(const String &name) const
{
    auto var = GetRootVar()[name];
    if (!IsSamplerVarValid(var, nullptr))
        return nullptr;
    return var.GetSampler();
}

SPtr<ParameterBlock> ParameterBlock::GetParameterBlock(const ShaderVarLocation &location) const
{

}

SPtr<ParameterBlock> ParameterBlock::GetParameterBlock(const String &name) const
{

}

bool ParameterBlock::SetBuffer(const ShaderVarLocation &location, const SPtr<Buffer> &buffer)
{
    return SetResourceSrvUavCommon(location, buffer);
}

bool ParameterBlock::SetBuffer(const String &name, const SPtr<Buffer> &buffer)
{

}

bool ParameterBlock::SetTexture(const ShaderVarLocation &location, const SPtr<Texture> &texture)
{
    return SetResourceSrvUavCommon(location, texture);
}

bool ParameterBlock::SetTexture(const String &name, const SPtr<Texture> &texture)
{

}

bool ParameterBlock::SetSrv(const ShaderVarLocation &location, const SPtr<ResourceView> &srv)
{
    CT_CHECK(srv);
    CheckResourceLocation(location);
    CheckDescriptorSrv(location, srv.get());

    auto flatIndex = GetFlatIndex(location);
    if (srvs[flatIndex] == srv)
        return true;

    srvs[flatIndex] = srv;
    MarkDescriptorSetDirty(location);
    return true;
}

bool ParameterBlock::SetUav(const ShaderVarLocation &location, const SPtr<ResourceView> &uav)
{
    CT_CHECK(uav);
    CheckResourceLocation(location);
    CheckDescriptorUav(location, uav.get());

    auto flatIndex = GetFlatIndex(location);
    if (uavs[flatIndex] == uav)
        return true;

    uavs[flatIndex] = uav;
    MarkDescriptorSetDirty(location);
    return true;
}

bool ParameterBlock::SetSampler(const ShaderVarLocation &location, const SPtr<Sampler> &sampler)
{
    CT_CHECK(sampler);
    CheckResourceLocation(location);
    CheckDescriptorType(location, DescriptorType::Sampler);

    auto flatIndex = GetFlatIndex(location);
    if (samplers[flatIndex] == sampler)
        return true;

    samplers[flatIndex] = sampler;
    MarkDescriptorSetDirty(location);
    return true;
}

bool ParameterBlock::SetSampler(const String &name, const SPtr<Sampler> &sampler)
{
}

bool ParameterBlock::SetParameterBlock(const ShaderVarLocation &location, const SPtr<ParameterBlock> &block)
{

}

bool ParameterBlock::SetParameterBlock(const String &name, const SPtr<ParameterBlock> &block)
{

}

bool ParameterBlock::PrepareResources(CopyContext *ctx)
{

}

bool ParameterBlock::PrepareDescriptorSets(CopyContext *ctx)
{
    if(!PrepareResources(ctx))
        return false;

    for(int32 i = 0; i < reflection->GetSetInfoCount(); ++i)
    {
        if(reflection->GetSetInfo(i).bindingIndices.Count() == 0)
            continue;

        if (!sets[i])
        {
            sets[i] = DescriptorSet::Create(RenderAPI::GetDevice()->GetGpuDescriptorPool(), reflection->GetDescriptorSetLayout(i));
            BindIntoDescriptorSet(i);
        }
    }

    return true;
}

ShaderVar ShaderVar::FindMember(const String &name) const
{
    if (!IsValid())
        return ShaderVar();
    
    auto varType = GetVarType();

    if (auto resourceType = varType->AsResource())
    {
        if (resourceType->GetShaderResourceType() == ShaderResourceType::ConstantBuffer)
        {
            return GetParameterBlock()->GetRootVar().FindMember(name);
        }
    }
    if (auto structType = varType->AsStruct())
    {
        auto member = structType->GetMember(name);
        if (member)
        {
            // TODO should use local offset instead of global offset? (memberLocation = location + member->GetLocation())
            auto memberLoc = ShaderVarLocation(member->GetReflectionType(), member->GetLocation());
            return ShaderVar(block, memberLoc);
        }
    }
    return ShaderVar();
}

ShaderVar ShaderVar::FindMember(int32 index) const
{
    if (!IsValid())
        return ShaderVar();
    
    auto varType = GetVarType();

    if (auto resourceType = varType->AsResource())
    {
        if (resourceType->GetShaderResourceType() == ShaderResourceType::ConstantBuffer)
        {
            return GetParameterBlock()->GetRootVar().FindMember(index);
        }
    }
    if (auto structType = varType->AsStruct())
    {
        auto member = structType->GetMember(index);
        if (member)
        {
            // TODO should use local offset instead of global offset? (memberLocation = location + member->GetLocation())
            auto memberLoc = ShaderVarLocation(member->GetReflectionType(), member->GetLocation());
            return ShaderVar(block, memberLoc);
        }
    }
    return ShaderVar();
}

SPtr<Buffer> ShaderVar::GetBuffer() const
{
    return block->GetBuffer(location);
}

SPtr<Texture> ShaderVar::GetTexture() const
{
    return block->GetTexture(location);
}

SPtr<ResourceView> ShaderVar::GetSrv() const
{
    return block->GetSrv(location);
}

SPtr<ResourceView> ShaderVar::GetUav() const
{
    return block->GetUav(location);
}

SPtr<Sampler> ShaderVar::GetSampler() const
{
    return block->GetSampler(location);
}

SPtr<ParameterBlock> ShaderVar::GetParameterBlock() const
{
    return block->GetParameterBlock(location);
}

bool ShaderVar::SetBuffer(const SPtr<Buffer> &buffer) const
{
    return block->SetBuffer(location, buffer);
}

bool ShaderVar::SetTexture(const SPtr<Texture> &texture) const
{
    return block->SetTexture(location, texture);
}

bool ShaderVar::SetSrv(const SPtr<ResourceView> &srv) const
{
    return block->SetSrv(location, srv);
}

bool ShaderVar::SetUav(const SPtr<ResourceView> &uav) const
{
    return block->SetUav(location, uav);
}

bool ShaderVar::SetSampler(const SPtr<Sampler> &sampler) const
{
    return block->SetSampler(location, sampler);
}

bool ShaderVar::SetParameterBlock(const SPtr<ParameterBlock> &block) const
{
    return block->SetParameterBlock(location, block);
}

}