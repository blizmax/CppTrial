#pragma once

#include "Core/General.h"

CT_SCOPE_BEGIN

namespace AlgoInternal
{
template <typename T, typename Compare>
size_t LowerBound(T *ptr, size_t count, const T &value, Compare compare)
{
    size_t start = 0;
    size_t size = count;
    while (size > 0)
    {
        size_t remain = size % 2;
        size = size / 2;
        size_t cur = start + size;
        start = compare(ptr[cur], value) ? (cur + remain) : start;
    }
    return start;
}
} // namespace AlgoInternal

namespace Algo
{
template <typename T, typename Compare>
size_t BinarySearch(T *ptr, size_t count, const T &value, Compare compare)
{
    size_t index = AlgoInternal::LowerBound(ptr, count, value, compare);
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