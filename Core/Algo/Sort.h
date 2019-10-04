#pragma once

#include "Core/General.h"
#include "Core/Functional.h"
#include "Core/Algo/BinaryHeap.h"

CT_SCOPE_BEGIN

namespace AlgoInternal
{

template <typename T, typename Compare>
void BubbleSort(T *ptr, SizeType count, Compare compare)
{
    T *end = ptr + count - 1;
    while (end > ptr)
    {
        T *cursor;
        for (cursor = ptr; cursor < end; ++cursor)
        {
            if (compare(*(cursor + 1), *cursor))
            {
                std::swap(*cursor, *(cursor + 1));
            }
        }
        --end;
    }
}

template <typename T, typename Compare>
void SelectionSort(T *ptr, SizeType count, Compare compare)
{
    T *start = ptr;
    T *end = ptr + count - 1;
    while (start < end)
    {
        T *min, *cursor;
        for (min = start, cursor = start + 1; cursor <= end; ++cursor)
        {
            if (compare(*cursor, *min))
            {
                min = cursor;
            }
        }
        std::swap(*min, *start++);
    }
}

template <typename T, typename Compare>
void SimpleSort(T *ptr, SizeType count, Compare compare)
{
    T *end = ptr + count - 1;
    while (end > ptr)
    {
        T *max, *cursor;
        for (max = ptr, cursor = ptr + 1; cursor <= end; ++cursor)
        {
            if (compare(*max, *cursor))
            {
                max = cursor;
            }
        }
        std::swap(*max, *end--);
    }
}

template <typename T, typename Compare>
SizeType QuickSortPrivate(T *ptr, SizeType first, SizeType last, Compare compare)
{
    std::swap(ptr[first], ptr[(first + last) / 2]);
    T *lower = ptr + first + 1;
    T *upper = ptr + last;
    T *bound = ptr + first;
    while (lower <= upper)
    {
        while (compare(*lower, *bound))
            ++lower;
        while (compare(*bound, *upper))
            --upper;
        if (lower < upper)
            std::swap(*lower++, *upper--);
        else
            break;
    }
    std::swap(*upper, *bound);
    return upper - ptr;
}

template <typename T, typename Compare>
void QuickSort(T *ptr, SizeType first, SizeType last, Compare compare)
{
    SizeType pos = QuickSortPrivate(ptr, first, last, compare);
    if (pos > first + 1)
        QuickSort(ptr, first, pos - 1, compare);
    if (pos + 1 < last)
        QuickSort(ptr, pos + 1, last, compare);
}

template <typename T, typename Compare>
void IntroSort(T *ptr, SizeType count, Compare compare)
{
    if (count < 2)
        return;

    struct Stack
    {
        T *min;
        T *max;
        uint32 maxDepth;
    };

    constexpr auto CalcMaxDepth = [](SizeType n) -> uint32 {
        uint32 depth = 0;
        while (n > 0)
        {
            ++depth;
            n >>= 1;
        }
        return depth;
    };

    Stack sortStacks[64] = {{ptr, ptr + count - 1, CalcMaxDepth(count)}};
    Stack current;
    for (Stack *top = sortStacks; top >= sortStacks; --top)
    {
        current = *top;

    LOOP:
        SizeType num = current.max - current.min + 1;
        if (current.maxDepth == 0)
        {
            HeapSort(current.min, num, compare);
            continue;
        }
        if (num <= 8)
        {
            SimpleSort(current.min, num, compare);
        }
        else
        {
            SizeType pos = QuickSortPrivate(current.min, 0, num - 1, compare);
            --current.maxDepth;
            if (num - pos < pos)
            {
                if (pos > 1)
                {
                    top->min = current.min;
                    top->max = current.min + pos - 1;
                    top->maxDepth = current.maxDepth;
                    ++top;
                }
                if (pos + 2 < num)
                {
                    current.min = current.min + pos + 1;
                    goto LOOP;
                }
            }
            else
            {
                if (pos + 2 < num)
                {
                    top->min = current.min + pos + 1;
                    top->max = current.max;
                    top->maxDepth = current.maxDepth;
                    ++top;
                }
                if (pos > 1)
                {
                    current.max = current.min + pos - 1;
                    goto LOOP;
                }
            }
        }
    }
}

template <typename T, typename Compare>
void StableSort(T *ptr, SizeType count)
{
    //TODO
}

} // namespace AlgoInternal

namespace Algo
{
template <typename T, typename Compare>
void QuickSort(T *ptr, SizeType count, Compare compare)
{
    if (count < 2)
        return;

    AlgoInternal::QuickSort(ptr, 0, count - 1, compare);
}

template <typename T>
void QuickSort(T *ptr, SizeType count)
{
    QuickSort(ptr, count, Less<T>());
}

template <typename T, typename Compare>
void IntroSort(T *ptr, SizeType count, Compare compare)
{
    if (count < 2)
        return;

    AlgoInternal::IntroSort(ptr, count, compare);
}

template <typename T>
void IntroSort(T *ptr, SizeType count)
{
    IntroSort(ptr, count, Less<T>());
}

template <typename T, typename Compare>
void Sort(T *ptr, SizeType count, Compare compare)
{
    if (count < 2)
        return;

    AlgoInternal::IntroSort(ptr, count, compare);
}

template <typename T>
void Sort(T *ptr, SizeType count)
{
    Sort(ptr, count, Less<T>());
}

} // namespace Algo

CT_SCOPE_END