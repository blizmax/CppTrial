#pragma once

#include "Core/General.h"

CT_SCOPE_BEGIN

namespace AlgoInternal
{
template <typename T, typename Compare>
SizeType LowerBound(T *ptr, SizeType count, const T &value, Compare compare)
{
    SizeType start = 0;
    SizeType size = count;
    while (size > 0)
    {
        SizeType remain = size % 2;
        size = size / 2;
        SizeType cur = start + size;
        start = compare(ptr[cur], value) ? (cur + remain) : start;
    }
    return start;
}
} // namespace AlgoInternal

namespace Algo
{
template <typename T, typename Compare>
SizeType BinarySearch(T *ptr, SizeType count, const T &value, Compare compare)
{
    SizeType index = AlgoInternal::LowerBound(ptr, count, value, compare);
    if (index < count)
    {
        if (!compare(value, ptr[index]))
        {
            return index;
        }
    }
    return INDEX_NONE;
}

} // namespace Algo

CT_SCOPE_END