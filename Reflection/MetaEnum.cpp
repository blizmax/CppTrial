#include "Reflection/MetaEnum.h"

CT_SCOPE_BEGIN

void MetaEnum::SetElements(Array<Element> &value)
{
    elements = value;
}

SizeType MetaEnum::GetElementSize() const
{
    return elements.Size();
}

bool MetaEnum::IsValidIndex(SizeType index) const
{
    return index <= elements.Size();
}

bool MetaEnum::IsValidName(const Name& name) const
{
    return GetIndexByName(name) != INDEX_NONE;
}

bool MetaEnum::IsValidValue(int64 value) const
{
    return GetIndexByValue(value) != INDEX_NONE;
}

SizeType MetaEnum::GetIndexByName(const Name &name) const
{
    for(SizeType i = 0; i < elements.Size(); ++i)
    {
        if(elements[i].name == name)
        {
            return i;
        }
    }
    return INDEX_NONE;
}

SizeType MetaEnum::GetIndexByValue(int64 value) const
{
    for (SizeType i = 0; i < elements.Size(); ++i)
    {
        if (elements[i].value == value)
        {
            return i;
        }
    }
    return INDEX_NONE;
}

int64 MetaEnum::GetValueByName(const Name& name) const
{
    SizeType index = GetIndexByName(name);
    CheckRange(index);
    return elements[index].value;
}

int64 MetaEnum::GetValueByIndex(SizeType index) const
{
    CheckRange(index);
    return elements[index].value;
}

Name MetaEnum::GetNameByValue(int64 value) const
{
    SizeType index = GetIndexByValue(value);
    CheckRange(index);
    return elements[index].name;
}

Name MetaEnum::GetNameByIndex(SizeType index) const
{
    CheckRange(index);
    return elements[index].name;
}

void MetaEnum::CheckRange(SizeType index) const
{
    CT_ASSERT(IsValidIndex(index));
}

CT_SCOPE_END