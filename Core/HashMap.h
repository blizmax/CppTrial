#pragma once

#include "Core/General.h"
#include "Core/Hash.h"
#include "Core/Hash/HashTable.h"

CT_SCOPE_BEGIN

template <typename Key,
          typename Value,
          typename Hasher = HashFunc<Key>,
          typename KeyEqual = EqualTo<Key>,
          template <typename T> class Alloc = Allocator>
class HashMap
{
public:
    typedef typename Hash::MapEntry<Key, Value> EntryType;

public:
    HashMap() = default;
    HashMap(const HashMap &) = default;
    HashMap(HashMap &&) noexcept = default;
    HashMap &operator=(const HashMap &) = default;
    HashMap &operator=(HashMap &&) noexcept = default;
    ~HashMap() = default;

    explicit HashMap(SizeType initCapacity) : hashTable(HashTableType(initCapacity))
    {
    }

    HashMap(std::initializer_list<EntryType> initList)
    {
        SizeType initSize = initList.size();
        hashTable = HashTableType(initSize); // may make waste when has same keys
        for (const EntryType &entry : initList)
        {
            hashTable.Put(entry);
        }
    }

    HashMap &operator=(std::initializer_list<EntryType> initList)
    {
        HashMap temp(initList);
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

    void Swap(HashMap &other)
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

    void Put(const Key &key, const Value &value)
    {
        hashTable.Put(EntryType(key, value));
    }

    void Put(const Key &key, Value &&value)
    {
        hashTable.Put(EntryType(key, std::move(value)));
    }

    void Put(Key &&key, const Value &value)
    {
        hashTable.Put(EntryType(std::move(key), value));
    }

    void Put(Key &&key, Value &&value)
    {
        hashTable.Put(EntryType(std::move(key), std::move(value)));
    }

    Value &Get(const Key &key)
    {
        return hashTable.GetByKey(key).Value();
    }

    const Value &Get(const Key &key) const
    {
        return hashTable.GetByKey(key).Value();
    }

    bool Remove(const Key &key)
    {
        return hashTable.RemoveByKey(key);
    }

    Value &operator[](const Key &key)
    {
        return Get(key);
    }

    const Value &operator[](const Key &key) const
    {
        return Get(key);
    }

    bool operator==(const HashMap &other) const
    {
        return hashTable == other.hashTable;
    }

    bool operator!=(const HashMap &other) const
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
    typedef typename Hash::MapKeyTraits<EntryType> KeyTriats;
    typedef HashTable<EntryType, Hasher, KeyEqual, KeyTriats, Alloc> HashTableType;

    HashTableType hashTable;
};

CT_SCOPE_END