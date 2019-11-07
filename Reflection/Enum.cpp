#include "Reflection/Enum.h"

namespace Reflection
{

SizeType Enum::GetElementSize() const
{
    return elements.Size();
}

bool Enum::IsValidIndex(SizeType index) const
{
    return index <= elements.Size();
}

bool Enum::IsValidName(const Name &name) const
{
    return GetIndexByName(name) != INDEX_NONE;
}

bool Enum::IsValidValue(int64 value) const
{
    return GetIndexByValue(value) != INDEX_NONE;
}

SizeType Enum::GetIndexByName(const Name &name) const
{
    for (SizeType i = 0; i < elements.Size(); ++i)
    {
        if (elements[i].name == name)
        {
            return i;
        }
    }
    return INDEX_NONE;
}

SizeType Enum::GetIndexByValue(int64 value) const
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

int64 Enum::GetValueByName(const Name &name) const
{
    SizeType index = GetIndexByName(name);
    CheckRange(index);
    return elements[index].value;
}

int64 Enum::GetValueByIndex(SizeType index) const
{
    CheckRange(index);
    return elements[index].value;
}

Name Enum::GetNameByValue(int64 value) const
{
    SizeType index = GetIndexByValue(value);
    CheckRange(index);
    return elements[index].name;
}

Name Enum::GetNameByIndex(SizeType index) const
{
    CheckRange(index);
    return elements[index].name;
}

void Enum::CheckRange(SizeType index) const
{
    CT_ASSERT(IsValidIndex(index));
}
} // namespace Reflection