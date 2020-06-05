#include "RenderCore/ProgramReflection.h"

namespace RenderCore
{

const RootSignatureDesc &ProgramReflection::GetRootSignatureDesc() const
{
    if (dirty)
    {
        dirty = false;
        for (int32 setIndex = 0; setIndex < setDatas.Count(); ++setIndex)
        {
            DescriptorSetLayoutDesc desc;
            desc.setIndex = setIndex;
            desc.visibility = setDatas[setIndex].visibility;
            for (int32 i : setDatas[setIndex].bindingSlots)
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