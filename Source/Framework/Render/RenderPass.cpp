#include "Render/RenderPass.h"

namespace RenderPassInternal
{

FieldVisibility MergeVisibility(FieldVisibility va, FieldVisibility vb)
{
    FieldVisibility result = va;

    if (va == FieldVisibility::Input)
    {
        switch (vb)
        {
        case FieldVisibility::Output:
        case FieldVisibility::InputOutput:
            result = FieldVisibility::InputOutput;
            break;
        default:
            break;
        }
    }
    else if (va == FieldVisibility::Output)
    {
        switch (vb)
        {
        case FieldVisibility::Input:
        case FieldVisibility::InputOutput:
            result = FieldVisibility::InputOutput;
            break;
        default:
            break;
        }
    }

    return result;
}

bool Field::IsValid() const
{
    if (sampleCount > 1 && mipLevels > 1)
    {
        CT_LOG(Error, CT_TEXT("RenderPassField is invalid, a multisampled field's mipLevels can not > 1."));
        return false;
    }
    if (IsInternal() && IsOptional())
    {
        CT_LOG(Error, CT_TEXT("RenderPassField is invalid, an internal field can not be optional."));
        return false;
    }

    return true;
}

Field &Field::Merge(const Field &other)
{
    CT_CHECK(!other.IsInternal());
    CT_CHECK(!IsInternal());

    bindFlags |= other.bindFlags;
    visibility = MergeVisibility(visibility, other.visibility);
    return *this;
}

Field &Field::ResourceType(FieldType type, int32 width, int32 height, int32 depth, int32 sampleCount, int32 mipLevels, int32 arrayLayers)
{
    switch (type)
    {
    case FieldType::RawBuffer:
        CT_CHECK(height == 0 && depth == 0 && sampleCount == 0 && mipLevels == 0 && arrayLayers == 0);
        break;
    case FieldType::Texture1D:
        CT_CHECK(height == 1 && depth == 1 && sampleCount == 1);
        break;
    case FieldType::Texture2D:
        CT_CHECK(depth == 1);
        break;
    case FieldType::Texture3D:
        CT_CHECK(sampleCount == 1 && mipLevels == 1);
        break;
    case FieldType::TextureCube:
        CT_CHECK(depth == 1 && mipLevels == 1);
        break;
    default:
        CT_CHECK(false);
        break;
    }

    this->fieldType = type;
    this->width = width;
    this->height = height;
    this->depth = depth;
    this->sampleCount = sampleCount;
    this->mipLevels = mipLevels;
    this->arrayLayers = arrayLayers;

    return *this;
}

}

RenderPassReflection::Field &RenderPassReflection::AddField(Field field)
{
    for (auto &f : fields)
    {
        if (f.GetName() == field.GetName())
        {
            auto va = f.GetVisibility();
            auto vb = field.GetVisibility();

            // Merge IO
            if (va != vb)
            {
                f.SetVisibility(MergeVisibility(va, vb));
            }
            return f;
        }
    }

    fields.Add(std::move(field));
    return fields.Last();
}

RenderPassReflection::Field &RenderPassReflection::AddField(const String &name, const String &desc, FieldVisibility visibility)
{
    return AddField(Field(name, desc, visibility));
}

RenderPassReflection::Field &RenderPassReflection::AddInput(const String &name, const String &desc)
{
    return AddField(name, desc, FieldVisibility::Input);
}

RenderPassReflection::Field &RenderPassReflection::AddOutput(const String &name, const String &desc)
{
    return AddField(name, desc, FieldVisibility::Output);
}

RenderPassReflection::Field &RenderPassReflection::AddInputOutput(const String &name, const String &desc)
{
    return AddField(name, desc, FieldVisibility::InputOutput);
}

RenderPassReflection::Field &RenderPassReflection::AddInternal(const String &name, const String &desc)
{
    return AddField(name, desc, FieldVisibility::Internal);
}

int32 RenderPassReflection::GetFieldIndex(const String &name) const
{
    for (int32 i = 0; i < fields.Count(); ++i)
    {
        if (fields[i].GetName() == name)
            return i;
    }
    return -1;
}

bool RenderPassReflection::operator==(const RenderPassReflection &other) const
{
    if (fields.Count() != other.fields.Count())
        return false;

    for (auto &f : fields)
    {
        int32 index = other.GetFieldIndex(f.GetName());
        if (index == -1)
            return false;
        if (f != other.fields[index])
            return false;
    }
    return true;
}