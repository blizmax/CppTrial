#include "RenderCore/ParameterBlock.h"

namespace RenderCore
{

SPtr<ParameterBlock> ParameterBlock::Create(const SPtr<ProgramReflection> &reflection)
{
    return Memory::MakeShared<ParameterBlock>(reflection);
}

ParameterBlock::ParameterBlock(const SPtr<ProgramReflection> &reflection)
    : reflection(reflection)
{

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
    const auto &bindingData = reflection->GetBindingData(name);
    if (bindingData.descriptorType != DescriptorType::Sampler)
        return false;
    
    
}

bool ParameterBlock::SetResourceSrvUav(const Resource *resource, const ProgramReflection::BindingData &binding)
{

}

bool ParameterBlock::PrepareDescriptorSets(CopyContext *ctx)
{

}

}