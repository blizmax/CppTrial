#pragma once

#include "Core/General.h"
#include "Core/Allocator.h"
#include <algorithm>

CT_SCOPE_BEGIN

template <typename Key, typename HashFunc = std::hash<Key>, typename KeyEqual = std::equal_to<Key>, template <typename T> class Alloc = Allocator>
class HashTable
{
private:
    enum
    {
        DELETED = 0x80000000,
        FREE = 0x00000000,
        DEFAULT = 0x0123abcd,
    };

    struct Index
    {
        size_t hash;
        uint32 flag;
    };

    typedef Alloc<Key> KeyAlloc;
    typedef Alloc<Index> IndexAlloc;

public:
    HashTable() = default;

    //TODO
    // HashTable(const HashTable &) = default;
    // HashTable(HashTable &&) = default;
    // HashTable &operator=(const HashTable &) = default;
    // HashTable &operator=(HashTable &&) = default;

    bool IsFull() const
    {
        return size >= capacity * 0.9;
    }

    void Swap(HashTable& other)
    {
        if(this != &other)
        {
            std::swap(size, other.size);
            std::swap(capacity, other.capacity);
            std::swap(mask, other.mask);
            std::swap(data, other.data);
            std::swap(indexData, other.indexData);
        }
    }

    void Clear()
    {
        DestroyAllKeys();
        FreeAllIndices();
        size = 0;
    }

    size_t Find(const Key& key) const
    {
        if(size == 0)
        {
            return INDEX_NONE;
        }

        auto hash = HashKey(key);
        size_t pos = hash & mask;
        size_t dist = 0;
        while(true)
        {
            const auto flag = indexData[pos].flag;
            if(flag == FREE)
            {
                return INDEX_NONE;
            }
            if(dist > ProbeDistance(indexData[pos].hash, pos))
            {
                return INDEX_NONE;
            }
            if (!IsDeleted(flag) && indexData[pos].hash == hash && IsEqual(data[pos], key))
            {
                return pos;
            }
            pos = (pos + 1) & mask;
            ++dist;
        }
    }

private:
    size_t HashKey(const Key &key) const
    {
        static HashFunc hash;
        return hash(key);
    }

    bool IsEqual(const Key &key1, const Key &key2) const
    {
        static KeyEqual equal;
        return equal(key1, key2);
    }

    bool IsDeleted(uint32 flag) const
    {
        return (flag >> 31) != 0;
    }

    size_t ProbeDistance(size_t hash, size_t pos) const
    {
        auto desiredPos = hash & mask;
        return (pos + capacity - desiredPos) & mask;
    }

    void DestroyAllKeys()
    {
        for (size_t i = 0; i < capacity; ++i)
        {
            if (indexData[i].flag == DEFAULT)
            {
                KeyAlloc::Destroy(data[i]);
            }
        }
    }

    void FreeAllIndices()
    {
        std::memset(indexData, 0, sizeof(Index) * capacity);
    }

    bool RemovePrivate(const Key& key)
    {
        size_t index = Find(key);
        if(index == INDEX_NONE)
        {
            return false;
        }
        KeyAlloc::Destroy(data[index]);
        indexData[index].flag |= DELETED;
        --size;
        return true;
    }

    void InsertPrivate(size_t hash, const Key& key)
    {  
        size_t pos = hash & mask;
        size_t dist = 0;
        Key *insertPtr = nullptr;
        Key *tempPtr = nullptr;
        while(true)
        {
            const auto flag = indexData[pos].flag;
            if (!flag || IsDeleted(flag))
            {
                break;
            }
            size_t existingDist = ProbeDistance(indexData[pos].hash, pos);
            if (existingDist < dist)
            {
                std::swap(hash, indexData[pos].hash);
                indexData[pos].flag = DEFAULT;
                if (insertPtr)
                {
                    std::swap(data + pos, tempPtr);
                }
                else
                {
                    insertPtr = data + pos;
                }
                tempPtr = data + pos;
                dist = existingDist;
            }

            pos = (pos + 1) & mask;
            ++dist;
        }

        if(tempPtr)
        {
            ThisScope::uninitialized_move(tempPtr, 1, data + pos);
            *insertPtr = key;            
        }
        else
        {
            ThisScope::uninitialized_fill(data + pos, 1, key);
        }

        indexData[pos].hash = hash;
        indexData[pos].flag = DEFAULT;
        ++size;
    }

    void RehashPrivate(size_t newCapacity)
    {
        HashTable newTable;
        newTable.data = KeyAlloc::Allocate(newCapacity);
        newTable.indexData = IndexAlloc::Allocate(newCapacity);
        newTable.FreeAllIndices();
        newTable.capacity = newCapacity;
        newTable.size = size;
        newTable.mask = newCapacity - 1;

        for (size_t i = 0; i < capacity; ++i)
        {
            const auto flag = indexData[i].flag;
            if (flag && !IsDeleted(flag))
            {
                Key &key = data[i];
                newTable.InsertPrivate(indexData[i].hash, std::move(key));
            }
        }

        Swap(newTable);
    }

private: 
    size_t size = 0;
    size_t capacity = 0;
    size_t mask = 0;
    Key *data = nullptr;
    Index *indexData = nullptr;
};

CT_SCOPE_END
