#include "RenderCore/ProgramReflection.h"

namespace RenderCore
{

const ReflectionDataType *ReflectionType::AsData() const
{
    return IsData() ? static_cast<const ReflectionDataType *>(this) : nullptr;
}

const ReflectionResourceType *ReflectionType::AsResource() const
{
    return IsResource() ? static_cast<const ReflectionResourceType *>(this) : nullptr;
}

const ReflectionArrayType *ReflectionType::AsArray() const
{
    return IsArray() ? static_cast<const ReflectionArrayType *>(this) : nullptr;
}

const ReflectionStructType *ReflectionType::AsStruct() const
{
    return IsStruct() ? static_cast<const ReflectionStructType *>(this) : nullptr;
}

const ReflectionType *ReflectionType::UnwrapArray() const
{
    const ReflectionType *rtype = this;
    while (auto arrayType = rtype->AsArray())
    {
        rtype = arrayType->GetElementType().get();
    }
    return rtype;
}

uint32 ReflectionType::GetTotalElementCount() const
{
    uint32 len = 1;
    const ReflectionType *rtype = this;
    while (auto arrayType = rtype->AsArray())
    {
        len *= arrayType->GetElementCount();
        rtype = arrayType->GetElementType().get();
    }
    return len;
}

SPtr<ReflectionVar> ReflectionType::FindMember(const String &name) const
{
    auto structType = AsStruct();
    if (structType)
        return structType->GetMember(name);
    return nullptr;
}

SPtr<ReflectionVar> ReflectionStructType::GetMember(int32 index) const
{
    if(index >= 0 && index < members.Count())
        return members[index];
    return nullptr;
}

SPtr<ReflectionVar> ReflectionStructType::GetMember(const String &name) const
{
    int32 index = GetMemberIndex(name);
    return GetMember(index);
}

int32 ReflectionStructType::GetMemberIndex(const String &name) const
{
    auto ptr = memberNames.TryGet(name);
    return ptr ? *ptr : -1;
}

void ProgramReflection::Finalize()
{
    CT_CHECK(!finalized);
    finalized = true;

    const auto &bindingInfos = defaultBlockReflection->GetBindingInfos();
    const auto &bindingRanges = defaultBlockReflection->GetElementType()->GetBindingRanges();
    CT_CHECK(bindingInfos.Count() == bindingRanges.Count());

    HashMap<uint32, Array<DescriptorSetLayoutDesc::Element>> setElements;
    for(int32 i = 0; i < bindingRanges.Count(); ++i)
    {
        const auto &info = bindingInfos[i];
        const auto &range = bindingRanges[i];
        if(!setElements.Contains(info.set))
            setElements.Put(info.set, {});
        setElements[info.set].Add({info.name, range.descriptorType, info.binding, range.count});
    }
    for(auto &[set, elements] : setElements)
    {
        DescriptorSetLayoutDesc desc;
        desc.setIndex = set;
        desc.visibility = ShaderVisibility::All;
        desc.elements = std::move(elements);

        rootSignatureDesc.layouts.Add(DescriptorSetLayout::Create(desc));
    }
}

}