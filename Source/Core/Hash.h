#pragma once

#include "Core/.Package.h"
#include "Core/Functional.h"
#include "Core/Template.h"

namespace Hash
{

template <typename T>
requires std::integral<T>
    CT_INLINE HashType HashValue(T value)
{
    if constexpr (sizeof(T) <= sizeof(HashType))
    {
        return static_cast<HashType>(value);
    }
    else
    {
        return static_cast<HashType>(value ^ (value >> 32));
    }
}

template <typename T>
requires std::is_enum_v<T>
    CT_INLINE HashType HashValue(T value)
{
    return HashValue((std::underlying_type_t<T>)value);
}

CT_INLINE HashType HashValue(float value)
{
    union
    {
        uint32 i;
        float f;
    } u;

    u.f = value;
    return u.i;
}

CT_INLINE HashType HashValue(double value)
{
    union
    {
        uint64 i;
        double d;
    } u;

    u.d = value;
    return HashValue(u.i);
}

template <OneKindOfChars T>
CT_INLINE HashType HashValue(const T *ptr)
{
    HashType seed = 131;
    HashType hash = 0;
    while (*ptr)
    {
        hash = hash * seed + (*ptr++);
    }
    return (hash & 0x7FFFFFFF);
}

template <typename T>
CT_INLINE HashType HashValue(T *ptr)
{
    return HashValue(reinterpret_cast<SizeType>(ptr));
}

template <typename T>
requires requires(const T &a)
{
    {
        a.HashCode()
    }
    ->std::same_as<HashType>;
}
CT_INLINE HashType HashValue(const T &value)
{
    return value.HashCode();
}

CT_INLINE HashType HashValue(const std::type_index &value)
{
    return static_cast<HashType>(value.hash_code());
}

template <typename T>
CT_INLINE void HashCombine(HashType &hash, const T &value)
{
    hash ^= HashValue(value) + 0x9E3779B9 + (hash << 6) + (hash >> 2);
}

}

template <typename T>
struct HashFunc
{
    HashType operator()(const T &value) const
    {
        return Hash::HashValue(value);
    }
};