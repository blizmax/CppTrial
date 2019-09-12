#pragma once

#include "General.h"
#include <new>
#include <type_traits>

CT_SCOPE_BEGIN

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

CT_SCOPE_END