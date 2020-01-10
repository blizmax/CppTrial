#pragma once

#include "Core/.Package.h"

namespace AlgoInternal
{

CT_INLINE SizeType LeftChildIndex(SizeType index)
{
    return index * 2 + 1;
}

CT_INLINE SizeType ParentIndex(SizeType index)
{
    return (index - 1) / 2;
}

CT_INLINE bool IsLeaf(SizeType index, SizeType count)
{
    return LeftChildIndex(index) >= count;
}

template <typename T, typename Compare>
CT_INLINE void SiftDown(T *heap, SizeType count, SizeType index, Compare compare)
{
    while (!IsLeaf(index, count))
    {
        const SizeType left = LeftChildIndex(index);
        const SizeType right = left + 1;
        SizeType max = left;
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
CT_INLINE SizeType SiftUp(T *heap, SizeType count, SizeType index, Compare compare)
{
    while (index > 0)
    {
        SizeType parent = ParentIndex(index);
        if (compare(heap[index], heap[parent]))
        {
            break;
        }
        std::swap(heap[index], heap[parent]);
        index = parent;
    }

    return index;
}

template <typename T, typename Compare>
void HeapSort(T *ptr, SizeType count, Compare compare)
{
    if (count < 2)
        return;

    for (SizeType index = ParentIndex(count - 1) + 1; index > 0;)
    {
        --index;
        SiftDown(ptr, count, index, compare);
    }

    for (SizeType range = count - 1; range > 0; --range)
    {
        std::swap(ptr[0], ptr[range]);
        SiftDown(ptr, range, 0, compare);
    }
}

} // namespace AlgoInternal