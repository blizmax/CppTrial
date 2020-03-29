#pragma once

#include "Core/.Package.h"
#include "Core/Template.h"

namespace Container
{
template <typename K, typename V>
struct MapEntry
{
public:
    using KeyType = K;
    using ValueType = V;

    MapEntry() = default;
    MapEntry(const MapEntry &) = default;
    MapEntry(MapEntry &&) = default;
    MapEntry &operator=(const MapEntry &) = default;
    MapEntry &operator=(MapEntry &&) = default;
    ~MapEntry() = default;

    MapEntry(const K &k, const V &v) : key(k), value(v)
    {
    }

    MapEntry(const K &k, V &&v) : key(k), value(std::move(v))
    {
    }

    MapEntry(K &&k, const V &v) : key(std::move(k)), value(v)
    {
    }

    MapEntry(K &&k, V &&v) : key(std::move(k)), value(std::move(v))
    {
    }

    const K &Key() const
    {
        return key;
    }

    V &Value()
    {
        return value;
    }

    const V &Value() const
    {
        return value;
    }

    template <SizeType Index>
    decltype(auto) get()
    {
        if constexpr (Index == 0)
            return Key();
        else if constexpr (Index == 1)
            return Value();
    }

    template <SizeType Index>
    decltype(auto) get() const
    {
        if constexpr (Index == 0)
            return Key();
        else if constexpr (Index == 1)
            return Value();
    }

private:
    K key;
    V value;
};

template <typename T>
struct SetKeyTraits
{
    using KeyType = T;
    static const KeyType &GetKey(const T &value)
    {
        return value;
    }
};

template <typename T>
struct MapKeyTraits
{
    using KeyType = typename T::KeyType;
    static const KeyType &GetKey(const T &value)
    {
        return value.Key();
    }
};
}

namespace std
{
template <typename K, typename V>
struct tuple_size<Container::MapEntry<K, V>> : public integral_constant<SizeType, 2>
{
};

template <SizeType Index, typename K, typename V>
struct tuple_element<Index, Container::MapEntry<K, V>>;

template <typename K, typename V>
struct tuple_element<0, Container::MapEntry<K, V>>
{
    using type = typename TAddConst<K>::type;
};

template <typename K, typename V>
struct tuple_element<1, Container::MapEntry<K, V>>
{
    using type = V;
};
}