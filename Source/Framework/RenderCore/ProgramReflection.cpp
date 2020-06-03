#include "RenderCore/ProgramReflection.h"

namespace RenderCore
{

DescriptorSetLayoutDesc ProgramReflection::GetDescriptorSetLayoutDesc(int32 setIndex) const
{
    DescriptorSetLayoutDesc desc;
    desc.setIndex = setIndex;
    desc.visibility = setDatas[setIndex].visibility;
    for (int32 i : setDatas[setIndex].bindings)
    {
        const auto &b = bindingDatas[i];
        desc.elements.Add({b.name, b.descriptorType, b.binding, b.arrayLength});
    }
    return desc;
}

}