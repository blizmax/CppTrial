#pragma once

#include "Core/.Package.h"
#include "Core/Memory.h"

template <typename T>
class Allocator
{
public:
    static T *Allocate();
    static T *Allocate(SizeType count);
    static void Deallocate(T *ptr);
    static void Deallocate(T *ptr, SizeType count);

    static void Construct(T *ptr);
    static void Construct(T *ptr, const T &value);
    static void Construct(T *ptr, T &&value);
    template <typename... Args>
    static void Construct(T *ptr, Args &&... args);
    static void Destroy(T *ptr);
    static void Destroy(T *ptr, SizeType count);
};

template <typename T>
CT_INLINE T *Allocator<T>::Allocate()
{
    return static_cast<T *>(::operator new(sizeof(T)));
}

template <typename T>
CT_INLINE T *Allocator<T>::Allocate(SizeType count)
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
CT_INLINE void Allocator<T>::Deallocate(T *ptr, SizeType count)
{
    Deallocate(ptr);
}

template <typename T>
CT_INLINE void Allocator<T>::Construct(T *ptr)
{
    Memory::Construct(ptr);
}

template <typename T>
CT_INLINE void Allocator<T>::Construct(T *ptr, const T &value)
{
    Memory::Construct(ptr, value);
}

template <typename T>
CT_INLINE void Allocator<T>::Construct(T *ptr, T &&value)
{
    Memory::Construct(ptr, std::move(value));
}

template <typename T>
template <typename... Args>
CT_INLINE void Allocator<T>::Construct(T *ptr, Args &&... args)
{
    Memory::Construct(ptr, std::forward<Args>(args)...);
}

template <typename T>
CT_INLINE void Allocator<T>::Destroy(T *ptr)
{
    Memory::Destroy(ptr);
}

template <typename T>
CT_INLINE void Allocator<T>::Destroy(T *ptr, SizeType count)
{
    for (; count > 0; --count)
    {
        Memory::Destroy(ptr);
        ++ptr;
    }
}

template <typename T>
using BasicAlloc = Allocator<T>;