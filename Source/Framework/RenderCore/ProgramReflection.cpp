#include "RenderCore/ProgramReflection.h"

namespace RenderCore
{

const RootSignatureDesc &ProgramReflection::GetRootSignatureDesc() const
{
    if (dirty)
    {
        dirty = false;
        for (int32 s = 0; s < setDatas.Count(); ++s)
        {
            DescriptorSetLayoutDesc desc;
            desc.setIndex = s;
            desc.visibility = setDatas[s].visibility;
            for (int32 i : setDatas[s].bindings)
            {
                const auto &b = bindingDatas[i];
                desc.elements.Add({b.name, b.descriptorType, b.binding, b.arrayLength});
            }
            rootSignatureDesc.layouts.Add((DescriptorSetLayout::Create(desc)));
        }
    }

    return rootSignatureDesc;
}

}