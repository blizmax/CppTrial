#pragma once

#include "Core/.Package.h"
#include "Core/Container/SkipList.h"
#include "Core/Container/AVLTree.h"

template <typename Key,
          typename Value,
          typename Comparer = CompareTo<Key>,
          template <typename T> class Alloc = Allocator>
class SortedMap
{
public:
    using EntryType = Container::MapEntry<Key, Value>;

    SortedMap() = default;
    SortedMap(const SortedMap &) = default;
    SortedMap(SortedMap &&) noexcept = default;
    SortedMap &operator=(const SortedMap &) = default;
    SortedMap &operator=(SortedMap &&) noexcept = default;
    ~SortedMap() = default;

    SortedMap(std::initializer_list<EntryType> initList)
    {
        for (const EntryType &entry : initList)
        {
            container.Put(entry);
        }
    }

    SortedMap &operator=(std::initializer_list<EntryType> initList)
    {
        SortedMap temp(initList);
        Swap(temp);
        return *this;
    }

public:
    SizeType Size() const
    {
        return container.Size();
    }

    bool IsEmpty() const
    {
        return container.IsEmpty();
    }

    void Swap(SortedMap &other)
    {
        container.Swap(other.container);
    }

    void Clear()
    {
        container.Clear();
    }

    bool Contains(const Key &key) const
    {
        return container.ContainsKey(key);
    }

    void Put(const Key &key, const Value &value)
    {
        container.Put(EntryType(key, value));
    }

    void Put(const Key &key, Value &&value)
    {
        container.Put(EntryType(key, std::move(value)));
    }

    void Put(Key &&key, const Value &value)
    {
        container.Put(EntryType(std::move(key), value));
    }

    void Put(Key &&key, Value &&value)
    {
        container.Put(EntryType(std::move(key), std::move(value)));
    }

    Value &Get(const Key &key)
    {
        return container.GetByKey(key).Value();
    }

    const Value &Get(const Key &key) const
    {
        return container.GetByKey(key).Value();
    }

    bool Remove(const Key &key)
    {
        return container.RemoveByKey(key);
    }

    Value &operator[](const Key &key)
    {
        return Get(key);
    }

    const Value &operator[](const Key &key) const
    {
        return Get(key);
    }

    bool operator==(const SortedMap &other) const
    {
        return container == other.container;
    }

    bool operator!=(const SortedMap &other) const
    {
        return !(*this == other);
    }

    //===================== STL STYLE =========================
public:
    auto begin()
    {
        return container.begin();
    }

    auto begin() const
    {
        return container.begin();
    }

    auto end()
    {
        return container.end();
    }

    auto end() const
    {
        return container.end();
    }

private:
    using KeyTraits = Container::MapKeyTraits<EntryType>;
    using ContainerType = SkipList<EntryType, Comparer, KeyTraits, Alloc>;
    //using ContainerType = AVLTree<EntryType, Comparer, KeyTraits, Alloc>;

    ContainerType container;
};

namespace std
{
template <typename K, typename V, typename C, template <typename T> class A>
inline void swap(SortedMap<K, V, C, A> &lhs, SortedMap<K, V, C, A> &rhs)
{
    lhs.Swap(rhs);
}
}