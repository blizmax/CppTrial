#pragma once

#include "Core/General.h"
#include <cstring>
#include <new>
#include <type_traits>

CT_SCOPE_BEGIN

namespace ThisScope
{

template <typename T>
CT_INLINE void construct(T *ptr)
{
    ::new ((void *)ptr) T();
}

template <typename T, typename V>
CT_INLINE void construct(T *ptr, const V &value)
{
    ::new ((void *)ptr) T(value);
}

template <typename T, typename... Args>
CT_INLINE void construct(T *ptr, Args &&... args)
{
    ::new ((void *)ptr) T(std::forward<Args>(args)...);
}

template <typename T>
CT_INLINE void destroy_private(T *ptr, std::true_type)
{
}

template <typename T>
CT_INLINE void destroy_private(T *ptr, std::false_type)
{
    if (ptr != nullptr)
        ptr->~T();
}

template <typename T>
CT_INLINE void destroy(T *ptr)
{
    destroy_private(ptr, std::is_trivially_destructible<T>{});
}

template <typename T>
CT_INLINE void copy_private(const T *src, size_t count, T *dst, std::true_type)
{
    if (count > 0)
    {
        std::memmove(dst, src, count * sizeof(T));
    }
}

template <typename T>
CT_INLINE void copy_private(const T *src, size_t count, T *dst, std::false_type)
{
    for (; count > 0; --count)
    {
        *dst = *src;
        ++src;
        ++dst;
    }
}

template <typename T>
CT_INLINE void copy(const T *src, size_t count, T *dst)
{
    copy_private(src, count, dst, std::is_trivially_copy_assignable<T>{});
}

template <typename T>
CT_INLINE void copy_backward_private(const T *src, size_t count, T *dst, std::true_type)
{
    if (count > 0)
    {
        std::memmove(dst - count + 1, src - count + 1, count * sizeof(T));
    }
}

template <typename T>
CT_INLINE void copy_backward_private(const T *src, size_t count, T *dst, std::false_type)
{
    for (; count > 0; --count)
    {
        *dst = *src;
        --src;
        --dst;
    }
}

template <typename T>
CT_INLINE void copy_backward(const T *src, size_t count, T *dst)
{
    copy_backward_private(src, count, dst, std::is_trivially_copy_assignable<T>{});
}

template <typename T>
CT_INLINE void move_private(T *src, size_t count, T *dst, std::true_type)
{
    if (count > 0)
    {
        std::memmove(dst, src, count * sizeof(T));
    }
}

template <typename T>
CT_INLINE void move_private(T *src, size_t count, T *dst, std::false_type)
{
    for (; count > 0; --count)
    {
        *dst = std::move(*src);
        ++src;
        ++dst;
    }
}

template <typename T>
CT_INLINE void move(T *src, size_t count, T *dst)
{
    move_private(src, count, dst, std::is_trivially_move_assignable<T>{});
}

template <typename T>
CT_INLINE void move_backward_private(T *src, size_t count, T *dst, std::true_type)
{
    if (count > 0)
    {
        std::memmove(dst - count + 1, src - count + 1, count * sizeof(T));
    }
}

template <typename T>
CT_INLINE void move_backward_private(T *src, size_t count, T *dst, std::false_type)
{
    for (; count > 0; --count)
    {
        *dst = std::move(*src);
        --src;
        --dst;
    }
}

template <typename T>
CT_INLINE void move_backward(T *src, size_t count, T *dst)
{
    move_backward_private(src, count, dst, std::is_trivially_move_assignable<T>{});
}

template <typename T>
CT_INLINE void fill(T *dst, size_t count, const T &value)
{
    for (; count > 0; --count)
    {
        *dst = value;
        ++dst;
    }
}

template <typename T>
CT_INLINE void uninitialized_copy_private(const T *src, size_t count, T *dst, std::true_type)
{
    if (count > 0)
    {
        std::memcpy(dst, src, count * sizeof(T));
    }
}

template <typename T>
CT_INLINE void uninitialized_copy_private(const T *src, size_t count, T *dst, std::false_type)
{
    for (; count > 0; --count)
    {
        construct(dst, *src);
        ++dst;
        ++src;
    }
}

template <typename T>
CT_INLINE void uninitialized_copy(const T *src, size_t count, T *dst)
{
    uninitialized_copy_private(src, count, dst, std::is_trivially_copy_assignable<T>{});
}

template <typename T>
CT_INLINE void uninitialized_fill_private(T *dst, size_t count, const T &value, std::true_type)
{
    for (; count > 0; --count)
    {
        *dst = value;
        ++dst;
    }
}

template <typename T>
CT_INLINE void uninitialized_fill_private(T *dst, size_t count, const T &value, std::false_type)
{
    for (; count > 0; --count)
    {
        construct(dst, value);
        ++dst;
    }
}

template <typename T>
CT_INLINE void uninitialized_fill(T *dst, size_t count, const T &value)
{
    uninitialized_fill_private(dst, count, value, std::is_trivially_copy_assignable<T>{});
}

template <typename T>
CT_INLINE void uninitialized_move_private(T *src, size_t count, T *dst, std::true_type)
{
    for (; count > 0; --count)
    {
        *dst = std::move(*src);
        ++src;
        ++dst;
    }
}

template <typename T>
CT_INLINE void uninitialized_move_private(T *src, size_t count, T *dst, std::false_type)
{
    for (; count > 0; --count)
    {
        construct(dst, std::move(*src));
        ++src;
        ++dst;
    }
}

template <typename T>
CT_INLINE void uninitialized_move(T *src, size_t count, T *dst)
{
    uninitialized_move_private(src, count, dst, std::is_trivially_move_assignable<T>{});
}

// reverse range [first, last)
template <typename T>
CT_INLINE void reverse(T *first, T *last)
{
    while (first < last)
    {
        --last;
        std::swap(*first, *last);
        ++first;
    }
}

} // namespace ThisScope

CT_SCOPE_END