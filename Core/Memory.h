#pragma once

#include "Core/.Package.h"
#include "Core/Template.h"
#include <cstring>
#include <new>

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
    if constexpr (TIsTriviallyDestructible<T>::value)
    {
    }
    else
    {
        if (ptr != nullptr)
            ptr->~T();
    }
}

CT_INLINE void *Alloc(SizeType size)
{
    return ::operator new(size);
}

CT_INLINE void Free(void *ptr)
{
    ::operator delete(ptr);
}

template <typename T, typename... Args>
CT_INLINE T *New(Args... args)
{
    T *ptr = static_cast<T *>(Alloc(sizeof(T)));
    Construct(ptr, std::forward<Args>(args)...);
    return ptr;
}

template <typename T>
CT_INLINE void Delete(T *ptr)
{
    Destroy(ptr);
    Free(static_cast<void *>(ptr));
}

template <typename T>
CT_INLINE T *ArrayNew(SizeType count)
{
    T *ptr = static_cast<T *>(Alloc(sizeof(T) * count));
    for (SizeType i = 0; i < count; ++i)
    {
        Construct(ptr + i);
    }
    return ptr;
}

template <typename T>
CT_INLINE void ArrayDelete(T *ptr, SizeType count)
{
    for (SizeType i = 0; i < count; ++i)
    {
        Destory(ptr + i);
    }
    Free(ptr);
}

template <typename T>
CT_INLINE void Copy(T *dst, const T *src, SizeType count)
{
    if constexpr (TIsTriviallyCopyAssignable<T>::value)
    {
        if (count > 0)
        {
            std::memmove(dst, src, count * sizeof(T));
        }
    }
    else
    {
        for (; count > 0; --count, ++dst, ++src)
        {
            *dst = *src;
        }
    }
}

template <typename T>
CT_INLINE void CopyBackward(T *dst, const T *src, SizeType count)
{
    if constexpr (TIsTriviallyCopyAssignable<T>::value)
    {
        if (count > 0)
        {
            std::memmove(dst - count + 1, src - count + 1, count * sizeof(T));
        }
    }
    else
    {
        for (; count > 0; --count, --dst, --src)
        {
            *dst = *src;
        }
    }
}

template <typename T>
CT_INLINE void Move(T *dst, T *src, SizeType count)
{
    if constexpr (TIsTriviallyMoveAssignable<T>::value)
    {
        if (count > 0)
        {
            std::memmove(dst, src, count * sizeof(T));
        }
    }
    else
    {
        for (; count > 0; --count, ++dst, ++src)
        {
            *dst = std::move(*src);
        }
    }
}

template <typename T>
CT_INLINE void MoveBackward(T *dst, T *src, SizeType count)
{
    if constexpr (TIsTriviallyMoveAssignable<T>::value)
    {
        if (count > 0)
        {
            std::memmove(dst - count + 1, src - count + 1, count * sizeof(T));
        }
    }
    else
    {
        for (; count > 0; --count, --dst, --src)
        {
            *dst = std::move(*src);
        }
    }
}

template <typename T>
CT_INLINE void Fill(T *dst, SizeType count, const T &value)
{
    for (; count > 0; --count, ++dst)
    {
        *dst = value;
    }
}

template <typename T>
CT_INLINE void UninitializedCopy(T *dst, const T *src, SizeType count)
{
    if constexpr (TIsTriviallyCopyAssignable<T>::value)
    {
        if (count > 0)
        {
            std::memcpy(dst, src, count * sizeof(T));
        }
    }
    else
    {
        for (; count > 0; --count, ++dst, ++src)
        {
            Construct(dst, *src);
        }
    }
}

template <typename T>
CT_INLINE void UninitializedFill(T *dst, SizeType count, const T &value)
{
    if constexpr (TIsTriviallyCopyAssignable<T>::value)
    {
        for (; count > 0; --count, ++dst)
        {
            *dst = value;
        }
    }
    else
    {
        for (; count > 0; --count, ++dst)
        {
            Construct(dst, value);
        }
    }
}

template <typename T>
CT_INLINE void UninitializedMove(T *dst, T *src, SizeType count)
{
    if constexpr (TIsTriviallyMoveAssignable<T>::value)
    {
        for (; count > 0; --count, ++dst, ++src)
        {
            *dst = std::move(*src);
        }
    }
    else
    {
        for (; count > 0; --count, ++dst, ++src)
        {
            Construct(dst, std::move(*src));
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