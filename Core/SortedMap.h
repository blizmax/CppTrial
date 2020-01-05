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
            skipList.Put(entry);
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
        return skipList.Size();
    }

    bool IsEmpty() const
    {
        return skipList.IsEmpty();
    }

    void Swap(SortedMap &other)
    {
        skipList.Swap(other.skipList);
    }

    void Clear()
    {
        skipList.Clear();
    }

    bool Contains(const Key &key) const
    {
        return skipList.ContainsKey(key);
    }

    void Put(const Key &key, const Value &value)
    {
        skipList.Put(EntryType(key, value));
    }

    void Put(const Key &key, Value &&value)
    {
        skipList.Put(EntryType(key, std::move(value)));
    }

    void Put(Key &&key, const Value &value)
    {
        skipList.Put(EntryType(std::move(key), value));
    }

    void Put(Key &&key, Value &&value)
    {
        skipList.Put(EntryType(std::move(key), std::move(value)));
    }

    Value &Get(const Key &key)
    {
        return skipList.GetByKey(key).Value();
    }

    const Value &Get(const Key &key) const
    {
        return skipList.GetByKey(key).Value();
    }

    bool Remove(const Key &key)
    {
        return skipList.RemoveByKey(key);
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
        return skipList == other.skipList;
    }

    bool operator!=(const SortedMap &other) const
    {
        return !(*this == other);
    }

    //===================== STL STYLE =========================
public:
    auto begin()
    {
        return skipList.begin();
    }

    auto begin() const
    {
        return skipList.begin();
    }

    auto end()
    {
        return skipList.end();
    }

    auto end() const
    {
        return skipList.end();
    }

private:
    using KeyTraits = Container::MapKeyTraits<EntryType>;
    using SkipListType = SkipList<EntryType, Comparer, KeyTraits, Alloc>;

    SkipListType skipList;
};

namespace std
{
template <typename K, typename V, typename C, template <typename T> class A>
inline void swap(SortedMap<K, V, C, A> &lhs, SortedMap<K, V, C, A> &rhs)
{
    lhs.Swap(rhs);
}
}