#pragma once

#include "Core/General.h"
#include "Core/Memory.h"

CT_SCOPE_BEGIN

template <typename T>
class Allocator
{
public:
    static T *Allocate();
    static T *Allocate(size_t count);
    static void Deallocate(T *ptr);
    static void Deallocate(T *ptr, size_t count);

    static void Construct(T *ptr);
    static void Construct(T *ptr, const T &value);
    static void Construct(T *ptr, T &&value);
    template <typename... Args>
    static void Construct(T *ptr, Args &&... args);
    static void Destroy(T *ptr);
    static void Destroy(T *ptr, size_t count);
};

template <typename T>
CT_INLINE T *Allocator<T>::Allocate()
{
    return static_cast<T *>(::operator new(sizeof(T)));
}

template <typename T>
CT_INLINE T *Allocator<T>::Allocate(size_t count)
{
    if (count == 0)
        return nullptr;
    return static_cast<T *>(::operator new(count * sizeof(T)));
}

template <typename T>
CT_INLINE void Allocator<T>::Deallocate(T *ptr)
{
    if (ptr == nullptr)
        return;
    ::operator delete(ptr);
}

template <typename T>
CT_INLINE void Allocator<T>::Deallocate(T *ptr, size_t count)
{
    Allocator<T>::Deallocate(ptr);
}

template <typename T>
CT_INLINE void Allocator<T>::Construct(T *ptr)
{
    ThisScope::construct(ptr);
}

template <typename T>
CT_INLINE void Allocator<T>::Construct(T *ptr, const T &value)
{
    ThisScope::construct(ptr, value);
}

template <typename T>
CT_INLINE void Allocator<T>::Construct(T *ptr, T &&value)
{
    ThisScope::construct(ptr, std::move(value));
}

template <typename T>
template <typename... Args>
CT_INLINE void Allocator<T>::Construct(T *ptr, Args &&... args)
{
    ThisScope::construct(ptr, std::forward<Args>(args)...);
}

template <typename T>
CT_INLINE void Allocator<T>::Destroy(T *ptr)
{
    ThisScope::destroy(ptr);
}

template <typename T>
CT_INLINE void Allocator<T>::Destroy(T *ptr, size_t count)
{
    for (; count > 0; --count)
    {
        ThisScope::destroy(ptr);
        ++ptr;
    }
}

CT_SCOPE_END