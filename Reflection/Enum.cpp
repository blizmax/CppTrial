#include "Reflection/Enum.h"

namespace Reflection
{

int32 Enum::GetElementCount() const
{
    return elements.Count();
}

bool Enum::IsValidIndex(int32 index) const
{
    return index <= elements.Count();
}

bool Enum::IsValidName(const Name &name) const
{
    return GetIndexByName(name) != INDEX_NONE;
}

bool Enum::IsValidValue(int64 value) const
{
    return GetIndexByValue(value) != INDEX_NONE;
}

int32 Enum::GetIndexByName(const Name &name) const
{
    for (int32 i = 0; i < elements.Count(); ++i)
    {
        if (elements[i].name == name)
        {
            return i;
        }
    }
    return INDEX_NONE;
}

int32 Enum::GetIndexByValue(int64 value) const
{
    for (int32 i = 0; i < elements.Count(); ++i)
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
    int32 index = GetIndexByName(name);
    CheckRange(index);
    return elements[index].value;
}

int64 Enum::GetValueByIndex(int32 index) const
{
    CheckRange(index);
    return elements[index].value;
}

Name Enum::GetNameByValue(int64 value) const
{
    int32 index = GetIndexByValue(value);
    CheckRange(index);
    return elements[index].name;
}

Name Enum::GetNameByIndex(int32 index) const
{
    CheckRange(index);
    return elements[index].name;
}

void Enum::CheckRange(int32 index) const
{
    CT_CHECK(IsValidIndex(index));
}
} // namespace Reflection