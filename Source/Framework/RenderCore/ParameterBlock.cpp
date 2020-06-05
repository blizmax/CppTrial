#include "RenderCore/ParameterBlock.h"
#include "RenderCore/RenderAPI.h"

namespace RenderCore
{

SPtr<ParameterBlock> ParameterBlock::Create(const SPtr<ProgramReflection> &reflection)
{
    return Memory::MakeShared<ParameterBlock>(reflection);
}

ParameterBlock::ParameterBlock(const SPtr<ProgramReflection> &reflection)
    : reflection(reflection)
{
    sets.SetCount(reflection->GetDescriptorSetCount());
}

bool ParameterBlock::SetBuffer(const String &name, const SPtr<Buffer> &buffer)
{
    const auto &bindingData = reflection->GetBindingData(name);

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

bool ParameterBlock::SetSampler(const String &name, const SPtr<Sampler> &sampler)
{
    CT_CHECK(sampler);

    const auto &bindingData = reflection->GetBindingData(name);
    if (bindingData.descriptorType != DescriptorType::Sampler)
        return false;
    
    auto ptr = samplers.TryGet(bindingData.slot);
    if (ptr && *ptr == sampler)
        return true;

    samplers.Put(bindingData.slot, sampler);
    MarkDescriptorSetDirty(bindingData.set); 
    return true;
}

void ParameterBlock::MarkDescriptorSetDirty(uint32 setIndex)
{
    sets[setIndex] = nullptr;
}

bool ParameterBlock::BindIntoDescriptorSet(uint32 setIndex)
{
    const auto &set = sets[setIndex];

    for(int32 slot : reflection->GetBindingSlots(setIndex))
    {
        const auto &bindingData = reflection->GetBindingData(slot);
        uint32 binding = bindingData.binding;
        auto descriptorType = bindingData.descriptorType;

        // switch (descriptorType)
        // {
        // case DescriptorType::Cbv:
        //     {
        //         set->SetCbv( [slot]
        //     }
        // }
    }
}

bool ParameterBlock::SetResourceSrvUav(const Resource *resource, const ProgramReflection::BindingData &binding)
{

}

bool ParameterBlock::PrepareResources(CopyContext *ctx)
{

}

bool ParameterBlock::PrepareDescriptorSets(CopyContext *ctx)
{
    if(!PrepareResources(ctx))
        return false;

    for(int32 i = 0; i < reflection->GetDescriptorSetCount(); ++i)
    {
        if (!sets[i])
        {
            sets[i] = DescriptorSet::Create(RenderAPI::GetDevice()->GetGpuDescriptorPool(), reflection->GetDescriptorSetLayout(i));
            BindIntoDescriptorSet(i);
        }
    }

    return true;
}

}