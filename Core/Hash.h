#pragma once

#include "Core/General.h"
#include "Core/Functional.h"
#include <type_traits>

CT_SCOPE_BEGIN

namespace Hash
{

#ifdef CT_USE_STD_HASH
#include <functional>

template <typename T>
struct HasHashCode
{
    template <typename U>
    static auto Check(U) -> typename std::decay<decltype(U::HashCode)>::type;
    static void Check(...);
    using Type = decltype(Check(std::declval<T>()));
    enum
    {
        Value = !std::is_void<Type>::value
    };
};

template <typename T>
CT_INLINE uint32 HashValue(const T &value, typename std::enable_if<std::is_class<T>::value && HasHashCode<T>::Value>::type * = nullptr)
{
    return value.HashCode();
}

template <typename T>
CT_INLINE uint32 HashValue(const T &value, typename std::enable_if<!std::is_class<T>::value || !HasHashCode<T>::Value>::type * = nullptr)
{
    static std::hash<T> hasher;
    return static_cast<uint32>(hasher(value));
}

#else

CT_INLINE uint32 HashValue(bool value)
{
    return static_cast<uint32>(value);
}

CT_INLINE uint32 HashValue(char8 value)
{
    return static_cast<uint32>(value);
}

CT_INLINE uint32 HashValue(char16 value)
{
    return static_cast<uint32>(value);
}

CT_INLINE uint32 HashValue(char32 value)
{
    return static_cast<uint32>(value);
}

CT_INLINE uint32 HashValue(wchar value)
{
    return static_cast<uint32>(value);
}

CT_INLINE uint32 HashValue(int8 value)
{
    return static_cast<uint32>(value);
}

CT_INLINE uint32 HashValue(int16 value)
{
    return static_cast<uint32>(value);
}

CT_INLINE uint32 HashValue(int32 value)
{
    return static_cast<uint32>(value);
}

CT_INLINE uint32 HashValue(int64 value)
{
    return static_cast<uint32>(value ^ (value >> 32));
}

CT_INLINE uint32 HashValue(uint8 value)
{
    return static_cast<uint32>(value);
}

CT_INLINE uint32 HashValue(uint16 value)
{
    return static_cast<uint32>(value);
}

CT_INLINE uint32 HashValue(uint32 value)
{
    return static_cast<uint32>(value);
}

CT_INLINE uint32 HashValue(uint64 value)
{
    return static_cast<uint32>(value ^ (value >> 32));
}

CT_INLINE uint32 HashValue(float value)
{
    union 
    {
        uint32 i;
        float f;
    } u;
    u.f = value;
    return u.i;
}

CT_INLINE uint32 HashValue(double value)
{
    union 
    {
        uint64 i;
        double d;
    } u;
    u.d = value;
    return HashValue(u.i);
}

//TODO Use enableif check is char type
template <typename CharT>
CT_INLINE uint32 HashValue(const CharT *ptr)
{
    uint32 seed = 131;
    uint32 hash = 0;
    while (*ptr)
    {
        hash = hash * seed + (*ptr++);
    }
    return (hash & 0x7FFFFFFF);
}

template <typename T>
CT_INLINE uint32 HashValue(T *ptr)
{
    return HashValue(reinterpret_cast<size_t>(ptr));
}

template <typename T>
CT_INLINE uint32 HashValue(const T &value)
{
    return value.HashCode();
}

#endif

template <typename T>
CT_INLINE void HashCombine(uint32 &hash, const T &value)
{
    hash ^= HashValue(value) + 0x9E3779B9 + (hash << 6) + (hash >> 2);
}

} // namespace Hash

template <typename T>
struct HashFunc
{
    uint32 operator()(const T &value) const
    {
        return Hash::HashValue(value);
    }
};