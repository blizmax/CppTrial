#pragma once

#include "Core/.Package.h"
#include "Core/Template.h"
#include <cstring>
#include <memory>
#include <new>

template <typename T>
struct Deleter;

template <typename T>
using SPtr = std::shared_ptr<T>;
template <typename T>
using WPtr = std::weak_ptr<T>;
template <typename T, typename D = Deleter<T>>
using UPtr = std::unique_ptr<T, D>;

class MemoryTracker
{
public:
    static int32 GetAllocCount()
    {
        return allocCount;
    }

    static int32 GetFreeCount()
    {
        return freeCount;
    }

    static void IncAllocCount()
    {
        ++allocCount;
    }

    static void IncFreeCount()
    {
        ++freeCount;
    }

private:
    inline static thread_local int32 allocCount = 0;
    inline static thread_local int32 freeCount = 0;
};

class Memory
{
public:
    template <typename T>
    static CT_INLINE void Construct(T *ptr)
    {
        ::new ((void *)ptr) T();
    }

    template <typename T, typename V>
    static CT_INLINE void Construct(T *ptr, const V &value)
    {
        ::new ((void *)ptr) T(value);
    }

    template <typename T, typename... Args>
    static CT_INLINE void Construct(T *ptr, Args &&... args)
    {
        ::new ((void *)ptr) T(std::forward<Args>(args)...);
    }

    template <typename T>
    static CT_INLINE void Destroy(T *ptr)
    {
        if constexpr (!std::is_trivially_destructible_v<T>)
        {
            if (ptr != nullptr)
                ptr->~T();
        }
    }

    static CT_INLINE void *Alloc(SizeType size)
    {
#if CT_PROFILE
        MemoryTracker::IncAllocCount();
#endif
        return ::operator new(size);
    }

    static CT_INLINE void Free(void *ptr)
    {
#if CT_PROFILE
        MemoryTracker::IncFreeCount();
#endif
        ::operator delete(ptr);
    }

    template <typename T, typename... Args>
    static CT_INLINE T *New(Args... args)
    {
        T *ptr = static_cast<T *>(Alloc(sizeof(T)));
        Construct(ptr, std::forward<Args>(args)...);
        return ptr;
    }

    template <typename T>
    static CT_INLINE void Delete(T *ptr)
    {
        Destroy(ptr);
        Free(static_cast<void *>(ptr));
    }

    template <typename T>
    static CT_INLINE T *ArrayNew(SizeType count)
    {
        T *ptr = static_cast<T *>(Alloc(sizeof(T) * count));
        for (SizeType i = 0; i < count; ++i)
        {
            Construct(ptr + i);
        }
        return ptr;
    }

    template <typename T>
    static CT_INLINE void ArrayDelete(T *ptr, SizeType count)
    {
        for (SizeType i = 0; i < count; ++i)
        {
            Destory(ptr + i);
        }
        Free(ptr);
    }

    template <typename T>
    static CT_INLINE void Copy(T *dst, const T *src, SizeType count)
    {
        if constexpr (std::is_trivially_copy_assignable_v<T>)
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
    static CT_INLINE void CopyBackward(T *dst, const T *src, SizeType count)
    {
        if constexpr (std::is_trivially_copy_assignable_v<T>)
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
    static CT_INLINE void Move(T *dst, T *src, SizeType count)
    {
        if constexpr (std::is_trivially_move_assignable_v<T>)
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
    static CT_INLINE void MoveBackward(T *dst, T *src, SizeType count)
    {
        if constexpr (std::is_trivially_move_assignable_v<T>)
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
    static CT_INLINE void Fill(T *dst, SizeType count, const T &value)
    {
        for (; count > 0; --count, ++dst)
        {
            *dst = value;
        }
    }

    template <typename T>
    static CT_INLINE void UninitializedCopy(T *dst, const T *src, SizeType count)
    {
        if constexpr (std::is_trivially_copy_assignable_v<T>)
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
    static CT_INLINE void UninitializedFill(T *dst, SizeType count, const T &value)
    {
        if constexpr (std::is_trivially_copy_assignable_v<T>)
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
    static CT_INLINE void UninitializedMove(T *dst, T *src, SizeType count)
    {
        if constexpr (std::is_trivially_move_assignable_v<T>)
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
    static CT_INLINE void Reverse(T *first, T *last)
    {
        while (first < last)
        {
            --last;
            std::swap(*first, *last);
            ++first;
        }
    }

    template <typename T, typename... Args>
    static CT_INLINE SPtr<T> MakeShared(Args &&... args);

    template <typename T, typename... Args>
    static CT_INLINE UPtr<T> MakeUnique(Args &&... args);
};

template <typename T>
struct Deleter
{
    Deleter() = default;

    template <typename U>
    requires std::convertible_to<U *, T *>
    Deleter(const Deleter<U> &other)
    {
    }

    void operator()(T *ptr) const
    {
        Memory::Delete<T>(ptr);
    }
};

template <typename T, typename... Args>
CT_INLINE SPtr<T> Memory::MakeShared(Args &&... args)
{
    T *ptr = New<T>(std::forward<Args>(args)...);
    return SPtr<T>(ptr, Deleter<T>());
}

template <typename T, typename... Args>
CT_INLINE UPtr<T> Memory::MakeUnique(Args &&... args)
{
    T *ptr = New<T>(std::forward<Args>(args)...);
    return UPtr<T>(ptr, Deleter<T>());
}
