#include "RenderCore/ProgramReflection.h"

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

int32 ReflectionType::GetTotalElementCount() const
{
    int32 count = 1;
    const ReflectionType *rtype = this;
    while (auto arrayType = rtype->AsArray())
    {
        count *= arrayType->GetElementCount();
        rtype = arrayType->GetElementType().get();
    }
    return count;
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
    if (index >= 0 && index < members.Count())
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

    auto &bindingInfos = defaultBlockReflection->GetBindingInfos();
    auto &bindingRanges = defaultBlockReflection->GetElementType()->GetBindingRanges();

    for (auto &setInfo : defaultBlockReflection->setInfos)
    {
        if (setInfo.bindingIndices.Count() == 0)
            continue;

        DescriptorSetLayoutDesc desc;
        desc.setIndex = setInfo.set;
        desc.visibility = ShaderVisibility::All;

        int32 maxBinding = 0;
        for (auto i : setInfo.bindingIndices)
        {
            auto binding = bindingInfos[i].binding;
            if (binding > maxBinding)
                maxBinding = binding;
        }
        desc.elements.SetCount(maxBinding + 1);
        for (auto i : setInfo.bindingIndices)
        {
            auto &info = bindingInfos[i];
            auto &range = bindingRanges[i];
            desc.elements[info.binding] = { info.name, range.descriptorType, info.binding, range.count };
        }

        auto layout = DescriptorSetLayout::Create(desc);
        setInfo.layout = layout;
        rootSignatureDesc.layouts.Add(layout);
    }
}