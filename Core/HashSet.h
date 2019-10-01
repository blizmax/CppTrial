#pragma once

#include "Core/General.h"
#include "Core/Hash.h"
#include "Core/Hash/HashTable.h"

CT_SCOPE_BEGIN

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

    explicit HashSet(size_t initCapacity) : hashTable(HashTableType(initCapacity))
    {
    }

    HashSet(std::initializer_list<Key> initList)
    {
        size_t initSize = initList.size();
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
    size_t Size() const
    {
        return hashTable.Size();
    }

    size_t Capacity() const
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
    typedef typename Hash::SetKeyTraits<Key> KeyTriats;
    typedef HashTable<Key, Hasher, KeyEqual, KeyTriats, Alloc> HashTableType;

    HashTableType hashTable;
};

CT_SCOPE_END