#pragma once

#include "Core/General.h"
#include "Core/Functional.h"
#include <type_traits>
#include <functional>

CT_SCOPE_BEGIN

namespace Hash
{

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
    return hasher(value);
}

template <typename T>
CT_INLINE void HashCombine(uint32 &hash, const T &value)
{
    hash ^= HashValue(value) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
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