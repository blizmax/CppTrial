#pragma once

#include "Core/General.h"
#include "Core/Functional.h"

CT_SCOPE_BEGIN

namespace AlgoInternal
{

template <typename T, typename Compare>
void BubbleSort(T *ptr, size_t count, Compare compare)
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
void SelectionSort(T *ptr, size_t count, Compare compare)
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
void IntroSortPrivate(T *ptr, size_t count, Compare compare)
{
    //TODO
}

template <typename T, typename Compare>
void StableSort(T *ptr, size_t count)
{
    //TODO
}

} // namespace AlgoInternal

namespace Algo
{
template <typename T, typename Compare>
void SimpleSort(T *ptr, size_t count, Compare compare)
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

template <typename T>
void SimpleSort(T *ptr, size_t count)
{
    SimpleSort(ptr, count, Less<T>());
}

template <typename T, typename Compare>
void QuickSortPrivate(T *ptr, size_t first, size_t last, Compare compare)
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

    size_t pos = upper - ptr;
    if (pos - 1 > first)
        QuickSortPrivate(ptr, first, pos - 1, compare);
    if (pos + 1 < last)
        QuickSortPrivate(ptr, pos + 1, last, compare);
}

template <typename T, typename Compare>
void QuickSort(T *ptr, size_t count, Compare compare)
{
    if (count < 2)
        return;

    QuickSortPrivate(ptr, 0, count - 1, compare);
}

template <typename T>
void QuickSort(T *ptr, size_t count)
{
    QuickSort(ptr, count, Less<T>());
}

} // namespace Algo

CT_SCOPE_END