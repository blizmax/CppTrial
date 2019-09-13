#pragma once

#include "General.h"
#include "Construct.h"
#include <cstring>
#include <type_traits>

CT_SCOPE_BEGIN

template <typename T>
CT_INLINE void uninitialized_copy_private(T *src, size_t count, T *dst, std::true_type)
{
    std::memcpy(dst, src, count);
}

template <typename T>
CT_INLINE void uninitialized_copy_private(T *src, size_t count, T *dst, std::false_type)
{
    for (; count > 0; --count)
    {
        construct(dst, *src);
        ++dst;
        ++src;
    }
}

template <typename T>
CT_INLINE void uninitialized_copy(T *src, size_t count, T *dst)
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

CT_SCOPE_END