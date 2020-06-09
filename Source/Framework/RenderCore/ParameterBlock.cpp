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

    srvs.SetCount(reflection->srvCount);
    uavs.SetCount(reflection->uavCount);
    samplers.SetCount(reflection->samplerCount);

    //TODO
    //cbvCount
    //Create uniform buffer
}

bool ParameterBlock::SetBuffer(const String &name, const SPtr<Buffer> &buffer)
{

}

bool ParameterBlock::SetTexture(const String &name, const SPtr<Texture> &texture)
{

}

bool ParameterBlock::SetSrv(const String &name, const SPtr<ResourceView> &srv)
{

}

bool ParameterBlock::SetUav(const String &name, const SPtr<ResourceView> &uav)
{

}

bool ParameterBlock::SetCbv(const String &name, const SPtr<ResourceView> &cbv)
{

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

void ParameterBlock::MarkDescriptorSetDirty(const ShaderVarLocation &location)
{
    auto setIndex = reflection->GetBindingInfo(location.rangeIndex).set;
    sets[setIndex] = nullptr;
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

bool ParameterBlock::SetResourceSrvUav(const ShaderVarLocation &location, const Resource *resource)
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

}