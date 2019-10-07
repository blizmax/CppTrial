#pragma once

#include "Core/General.h"
#include <cstring>
#include <new>
#include <type_traits>

CT_SCOPE_BEGIN

namespace Memory
{

template <typename T>
CT_INLINE void Construct(T *ptr)
{
    ::new ((void *)ptr) T();
}

template <typename T, typename V>
CT_INLINE void Construct(T *ptr, const V &value)
{
    ::new ((void *)ptr) T(value);
}

template <typename T, typename... Args>
CT_INLINE void Construct(T *ptr, Args &&... args)
{
    ::new ((void *)ptr) T(std::forward<Args>(args)...);
}

template <typename T>
CT_INLINE void Destroy(T *ptr)
{
    if constexpr (std::is_trivially_destructible<T>::value)
    {
    }
    else
    {
        if (ptr != nullptr)
            ptr->~T();
    }
}

template <typename T>
CT_INLINE void Copy(const T *src, SizeType count, T *dst)
{
    if constexpr (std::is_trivially_copy_assignable<T>::value)
    {
        if (count > 0)
        {
            std::memmove(dst, src, count * sizeof(T));
        }
    }
    else
    {
        for (; count > 0; --count)
        {
            *dst = *src;
            ++src;
            ++dst;
        }
    }
}

template <typename T>
CT_INLINE void CopyBackward(const T *src, SizeType count, T *dst)
{
    if constexpr (std::is_trivially_copy_assignable<T>::value)
    {
        if (count > 0)
        {
            std::memmove(dst - count + 1, src - count + 1, count * sizeof(T));
        }
    }
    else
    {
        for (; count > 0; --count)
        {
            *dst = *src;
            --src;
            --dst;
        }
    }
}

template <typename T>
CT_INLINE void Move(T *src, SizeType count, T *dst)
{
    if constexpr (std::is_trivially_move_assignable<T>::value)
    {
        if (count > 0)
        {
            std::memmove(dst, src, count * sizeof(T));
        }
    }
    else
    {
        for (; count > 0; --count)
        {
            *dst = std::move(*src);
            ++src;
            ++dst;
        }
    }
}

template <typename T>
CT_INLINE void MoveBackward(T *src, SizeType count, T *dst)
{
    if constexpr (std::is_trivially_move_assignable<T>::value)
    {
        if (count > 0)
        {
            std::memmove(dst - count + 1, src - count + 1, count * sizeof(T));
        }
    }
    else
    {
        for (; count > 0; --count)
        {
            *dst = std::move(*src);
            --src;
            --dst;
        }
    }
}

template <typename T>
CT_INLINE void Fill(T *dst, SizeType count, const T &value)
{
    for (; count > 0; --count)
    {
        *dst = value;
        ++dst;
    }
}

template <typename T>
CT_INLINE void UninitializedCopy(const T *src, SizeType count, T *dst)
{
    if constexpr (std::is_trivially_copy_assignable<T>::value)
    {
        if (count > 0)
        {
            std::memcpy(dst, src, count * sizeof(T));
        }
    }
    else
    {
        for (; count > 0; --count)
        {
            Construct(dst, *src);
            ++dst;
            ++src;
        }
    }
}

template <typename T>
CT_INLINE void UninitializedFill(T *dst, SizeType count, const T &value)
{
    if constexpr (std::is_trivially_copy_assignable<T>::value)
    {
        for (; count > 0; --count)
        {
            *dst = value;
            ++dst;
        }
    }
    else
    {
        for (; count > 0; --count)
        {
            Construct(dst, value);
            ++dst;
        }
    }
}

template <typename T>
CT_INLINE void UninitializedMove(T *src, SizeType count, T *dst)
{
    if constexpr (std::is_trivially_move_assignable<T>::value)
    {
        for (; count > 0; --count)
        {
            *dst = std::move(*src);
            ++src;
            ++dst;
        }
    }
    else
    {
        for (; count > 0; --count)
        {
            Construct(dst, std::move(*src));
            ++src;
            ++dst;
        }
    }
}

// reverse range [first, last)
template <typename T>
CT_INLINE void Reverse(T *first, T *last)
{
    while (first < last)
    {
        --last;
        std::swap(*first, *last);
        ++first;
    }
}

} // namespace Memory

CT_SCOPE_END