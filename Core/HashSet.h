#pragma once

#include "Core/.Package.h"
#include "Core/Hash.h"
#include "Core/Hash/HashTable.h"

template <typename Key,
          typename Hasher = HashFunc<Key>,
          typename KeyEqual = EqualTo<Key>,
          template <typename T> class Alloc = Allocator>
class HashSet
{
public:
    HashSet() = default;
    HashSet(const HashSet &) = default;
    HashSet(HashSet &&) noexcept = default;
    HashSet &operator=(const HashSet &) = default;
    HashSet &operator=(HashSet &&) noexcept = default;
    ~HashSet() = default;

    explicit HashSet(SizeType initCapacity) : hashTable(HashTableType(initCapacity))
    {
    }

    HashSet(std::initializer_list<Key> initList)
    {
        SizeType initSize = initList.size();
        hashTable = HashTableType(initSize); // may make waste when has same keys
        for (const Key &key : initList)
        {
            hashTable.Add(key);
        }
    }

    HashSet &operator=(std::initializer_list<Key> initList)
    {
        HashSet temp(initList);
        Swap(temp);
        return *this;
    }

public:
    SizeType Size() const
    {
        return hashTable.Size();
    }

    SizeType Capacity() const
    {
        return hashTable.Capacity();
    }

    bool IsEmpty() const
    {
        return hashTable.IsEmpty();
    }

    bool IsFull() const
    {
        return hashTable.IsFull();
    }

    void Swap(HashSet &other)
    {
        hashTable.Swap(other.hashTable);
    }

    void Clear()
    {
        hashTable.Clear();
    }

    void Shrink()
    {
        hashTable.Shrink();
    }

    bool Contains(const Key &key) const
    {
        return hashTable.ContainsKey(key);
    }

    bool Add(const Key &key)
    {
        return hashTable.Add(key);
    }

    bool Add(Key &&key)
    {
        return hashTable.Add(std::move(key));
    }

    const Key &Get(const Key &key) const
    {
        return hashTable.GetByKey(key);
    }

    bool Remove(const Key &key)
    {
        return hashTable.RemoveByKey(key);
    }

    bool operator==(const HashSet &other) const
    {
        return hashTable == other.hashTable;
    }

    bool operator!=(const HashSet &other) const
    {
        return !(*this == other);
    }

    //===================== STL STYLE =========================
public:
    auto begin()
    {
        return hashTable.begin();
    }

    auto begin() const
    {
        return hashTable.begin();
    }

    auto end()
    {
        return hashTable.end();
    }

    auto end() const
    {
        return hashTable.end();
    }

private:
    using KeyTriats = Hash::SetKeyTraits<Key>;
    using HashTableType = HashTable<Key, Hasher, KeyEqual, KeyTriats, Alloc>;

    HashTableType hashTable;
};

namespace std
{
template <typename K, typename H, typename E, template <typename T> class A>
inline void swap(HashSet<K, H, E, A> &lhs, HashSet<K, H, E, A> &rhs)
{
    lhs.Swap(rhs);
}
}