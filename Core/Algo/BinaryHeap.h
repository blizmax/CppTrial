#pragma once

#include "Core/General.h"

CT_SCOPE_BEGIN

namespace AlgoInternal
{

CT_INLINE size_t LeftChildIndex(size_t index)
{
    return index * 2 + 1;
}

CT_INLINE size_t ParentIndex(size_t index)
{
    return (index - 1) / 2;
}

CT_INLINE bool IsLeaf(size_t index, size_t count)
{
    return LeftChildIndex(index) >= count;
}

template <typename T, typename Compare>
CT_INLINE void SiftDown(T *heap, size_t count, size_t index, Compare compare)
{
    while (!IsLeaf(index, count))
    {
        const size_t left = LeftChildIndex(index);
        const size_t right = left + 1;
        size_t max = left;
        if (right < count && compare(heap[left], heap[right]))
        {
            max = right;
        }
        if (!compare(heap[index], heap[max]))
        {
            break;
        }
        std::swap(heap[index], heap[max]);
        index = max;
    }
}

template <typename T, typename Compare>
CT_INLINE size_t SiftUp(T *heap, size_t count, size_t index, Compare compare)
{
    while (index > 0)
    {
        size_t parent = ParentIndex(index);
        if (!compare(heap[index], heap[parent]))
        {
            break;
        }
        std::swap(heap[index], heap[parent]);
        index = parent;
    }

    return index;
}

template <typename T, typename Compare>
void HeapSort(T *ptr, size_t count, Compare compare)
{
    if (count < 2)
        return;

    for (size_t index = ParentIndex(count - 1) + 1; index > 0;)
    {
        --index;
        SiftDown(ptr, count, index, compare);
    }

    for (size_t range = count - 1; range > 0; --range)
    {
        std::swap(ptr[0], ptr[range]);
        SiftDown(ptr, range, 0, compare);
    }
}

} // namespace AlgoInternal

CT_SCOPE_END