#pragma once

#include "Core/General.h"
#include "Core/Allocator.h"
#include <utility>

CT_SCOPE_BEGIN

namespace HashTableInternal
{
template <typename T>
struct KeyEqual
{
    bool operator()(const T &a, const T &b)
    {
        return a == b;
    }
};

template <typename T>
struct SetKeyTraits
{
    typedef T KeyType;
    static const KeyType &GetKey(const T &value)
    {
        return value;
    }
};

template <typename T>
struct MapKeyTraits
{
    typedef typename std::remove_cv<typename T::first_type>::type KeyType;
    static const KeyType &GetKey(const T &value)
    {
        return value.first;
    }
};
} // namespace HashTableInternal

template <typename Type, typename HashFunc, typename KeyEqual, typename KeyTraits, template <typename T> class Alloc>
class HashTable
{
public:
    typedef typename KeyTraits::KeyType Key;

private:
    enum
    {
        DELETED = 0x80000000,
        FREE = 0x00000000,
        DEFAULT = 0x0123abcd,
    };

    struct Index
    {
        uint32 hash;
        uint32 flag;
    };

    typedef Alloc<Type> DataAlloc;
    typedef Alloc<Index> IndexAlloc;

public:
    HashTable() = default;

    explicit HashTable(size_t initCapacity)
    {
        if (initCapacity > 0)
        {
            capacity = FixCapacity(initCapacity);
            mask = capacity - 1;
            data = DataAlloc::Allocate(capacity);
            indexData = IndexAlloc::Allocate(capacity);
            FreeAllIndices();
        }
    }

    HashTable(const HashTable &other) : size(other.size), capacity(other.capacity), mask(other.mask)
    {
        if (capacity > 0)
        {
            data = DataAlloc::Allocate(other.capacity);
            indexData = IndexAlloc::Allocate(other.capacity);
            FreeAllIndices();

            CopyFrom(other);
        }
    }

    HashTable(HashTable &&other) noexcept : size(other.size), capacity(other.capacity), mask(other.mask), data(other.data), indexData(other.indexData)
    {
        other.size = 0;
        other.capacity = 0;
        other.mask = 0;
        other.data = nullptr;
        other.indexData = nullptr;
    }

    HashTable &operator=(const HashTable &other)
    {
        if (this != &other)
        {
            HashTable temp(other);
            Swap(temp);
        }
        return *this;
    }

    HashTable &operator=(HashTable &&other) noexcept
    {
        if (this != &other)
        {
            DestroyAllKeys();
            DataAlloc::Deallocate(data, capacity);
            IndexAlloc::Deallocate(indexData, capacity);
            size = other.size;
            capacity = other.capacity;
            mask = other.mask;
            data = other.data;
            indexData = other.indexData;
            other.size = 0;
            other.capacity = 0;
            other.mask = 0;
            other.data = nullptr;
            other.indexData = nullptr;
        }
        return *this;
    }

    ~HashTable()
    {
        DestroyAllKeys();
        DataAlloc::Deallocate(data, capacity);
        IndexAlloc::Deallocate(indexData, capacity);
        data = nullptr;
        indexData = nullptr;
        size = capacity = mask = 0;
    }

    size_t Size() const
    {
        return size;
    }

    size_t Capacity() const
    {
        return capacity;
    }

    bool IsEmpty() const
    {
        return size == 0;
    }

    bool IsFull() const
    {
        return size >= capacity * 0.9;
    }

    void Swap(HashTable &other)
    {
        if (this != &other)
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

    void Shrink()
    {
        if (size != capacity)
        {
            size_t newCapacity = FixCapacity(size);
            if (newCapacity < capacity)
            {
                RehashPrivate(newCapacity);
            }
        }
    }

    size_t Find(const Type &value) const
    {
        const Key &key = KeyTraits::GetKey(value);
        return FindPrivate(HashKey(key), key);
    }

    size_t FindByKey(const Key &key) const
    {
        return FindPrivate(HashKey(key), key);
    }

    bool Contains(const Type &value) const
    {
        return Find(value) != INDEX_NONE;
    }

    bool ContainsKey(const Key &key) const
    {
        return FindByKey(key) != INDEX_NONE;
    }

    void Put(const Type &value)
    {
        const Key &key = KeyTraits::GetKey(value);
        auto hash = HashKey(key);
        size_t pos = FindPrivate(hash, key);
        if (pos != INDEX_NONE)
        {
            data[pos] = value;
        }
        else
        {
            if (IsFull())
            {
                RehashPrivate(FixCapacity(capacity * 2));
            }
            InsertPrivate(hash, value);
            ++size;
        }
    }

    void Put(Type &&value)
    {
        const Key &key = KeyTraits::GetKey(value);
        auto hash = HashKey(key);
        size_t pos = FindPrivate(hash, key);
        if (pos != INDEX_NONE)
        {
            data[pos] = std::move(value);
        }
        else
        {
            if (IsFull())
            {
                RehashPrivate(FixCapacity(capacity * 2));
            }
            InsertPrivate(hash, std::move(value));
            ++size;
        }
    }

    bool Add(const Type &value)
    {
        const Key &key = KeyTraits::GetKey(value);
        auto hash = HashKey(key);
        size_t pos = FindPrivate(hash, key);
        if (pos == INDEX_NONE)
        {
            if (IsFull())
            {
                RehashPrivate(FixCapacity(capacity * 2));
            }
            InsertPrivate(hash, value);
            ++size;
            return true;
        }
        return false;
    }

    bool Add(Type &&value)
    {
        const Key &key = KeyTraits::GetKey(value);
        auto hash = HashKey(key);
        size_t pos = FindPrivate(hash, key);
        if (pos == INDEX_NONE)
        {
            if (IsFull())
            {
                RehashPrivate(FixCapacity(capacity * 2));
            }
            InsertPrivate(hash, std::move(value));
            ++size;
            return true;
        }
        return false;
    }

    Type &Get(const Type &value)
    {
        size_t pos = Find(value);
        CheckRange(pos);
        return data[pos];
    }

    const Type &Get(const Type &value) const
    {
        size_t pos = Find(value);
        CheckRange(pos);
        return data[pos];
    }

    Type &GetByKey(const Key &key)
    {
        size_t pos = FindByKey(key);
        CheckRange(pos);
        return data[pos];
    }

    const Type &GetByKey(const Key &key) const
    {
        size_t pos = FindByKey(key);
        CheckRange(pos);
        return data[pos];
    }

    bool Remove(const Type &value)
    {
        return RemovePrivate(KeyTraits::GetKey(value));
    }

    bool RemoveByKey(const Key &key)
    {
        return RemovePrivate(key);
    }

    bool operator==(const HashTable &other) const
    {
        if (size != other.size)
        {
            return false;
        }

        for (size_t i = 0; i < capacity; ++i)
        {
            if (IsInited(indexData[i].flag))
            {
                if (!other.Contains(data[i]))
                {
                    return false;
                }
            }
        }
        return true;
    }

    bool operator!=(const HashTable &other) const
    {
        return !(*this == other);
    }

    //===================== STL STYLE =========================
public:
    friend class Iterator;
    friend class ConstIterator;

    class Iterator
    {
    private:
        HashTable *table;
        size_t index;

        void Step()
        {
            for (size_t i = index; i < table->capacity; ++i)
            {
                if (table->IsInited(table->indexData[i].flag))
                {
                    index = i;
                    return;
                }
            }
            index = INDEX_NONE;
        }

    public:
        Iterator(HashTable *ht, size_t idx) : table(ht), index(idx)
        {
            Step();
        }

        Iterator(const Iterator &other) : table(other.table), index(other.index)
        {
        }

        Iterator &operator++()
        {
            if (index != INDEX_NONE)
            {
                ++index;
                Step();
            }
            return *this;
        }

        Iterator operator++(int)
        {
            Iterator temp(*this);
            if (index != INDEX_NONE)
            {
                ++index;
                Step();
            }
            return temp;
        }

        Type &operator*()
        {
            table->CheckRange(index);
            return table->data[index];
        }

        Type *operator->()
        {
            table->CheckRange(index);
            return &(table->data[index]);
        }

        bool operator==(const Iterator &other) const
        {
            return (table == other.table) && (index == other.index);
        }

        bool operator!=(const Iterator &other) const
        {
            return (table != other.table) || (index != other.index);
        }
    };

    class ConstIterator
    {
    private:
        const HashTable *table;
        size_t index;

        void Step()
        {
            for (size_t i = index; i < table->capacity; ++i)
            {
                if (table->IsInited(table->indexData[i].flag))
                {
                    index = i;
                    return;
                }
            }
            index = INDEX_NONE;
        }

    public:
        ConstIterator(const HashTable *ht, size_t idx) : table(ht), index(idx)
        {
            Step();
        }

        ConstIterator(const ConstIterator &other) : table(other.table), index(other.index)
        {
        }

        ConstIterator &operator++()
        {
            if (index != INDEX_NONE)
            {
                ++index;
                Step();
            }
            return *this;
        }

        ConstIterator operator++(int)
        {
            Iterator temp(*this);
            if (index != INDEX_NONE)
            {
                ++index;
                Step();
            }
            return temp;
        }

        const Type &operator*()
        {
            table->CheckRange(index);
            return table->data[index];
        }

        const Type *operator->()
        {
            table->CheckRange(index);
            return &(table->data[index]);
        }

        bool operator==(const ConstIterator &other) const
        {
            return (table == other.table) && (index == other.index);
        }

        bool operator!=(const ConstIterator &other) const
        {
            return (table != other.table) || (index != other.index);
        }
    };

    Iterator begin()
    {
        return Iterator(this, 0);
    }

    ConstIterator begin() const
    {
        return ConstIterator(const_cast<HashTable *>(this), 0);
    }

    Iterator end()
    {
        return Iterator(this, INDEX_NONE);
    }

    ConstIterator end() const
    {
        return ConstIterator(const_cast<HashTable *>(this), INDEX_NONE);
    }

private:
    void CheckRange(size_t index) const
    {
        CT_ASSERT(index < capacity);
    }

    uint32 HashKey(const Key &key) const
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

    bool IsInited(uint32 flag) const
    {
        return flag && !IsDeleted(flag);
    }

    size_t ProbeDistance(uint32 hash, size_t pos) const
    {
        size_t desiredPos = static_cast<size_t>(hash) & mask;
        return (pos + capacity - desiredPos) & mask;
    }

    size_t FixCapacity(size_t newCapacity) const
    {
        newCapacity = newCapacity < 8 ? 8 : newCapacity;
        if (IsPowerOfTwo(newCapacity))
        {
            return newCapacity;
        }
        return NextPowerOfTwo(newCapacity);
    }

    void DestroyAllKeys()
    {
        for (size_t i = 0; i < capacity; ++i)
        {
            if (indexData[i].flag == DEFAULT)
            {
                DataAlloc::Destroy(data + i);
            }
        }
    }

    void FreeAllIndices()
    {
        std::memset(indexData, 0, sizeof(Index) * capacity);
    }

    void CopyFrom(const HashTable &other)
    {
        if (other.size > 0)
        {
            for (size_t i = 0; i < other.capacity; ++i)
            {
                if (IsInited(other.indexData[i].flag))
                {
                    InsertPrivate(other.indexData[i].hash, other.data[i]);
                }
            }
        }
    }

    void MoveFrom(HashTable &&other)
    {
        if (other.size > 0)
        {
            for (size_t i = 0; i < other.capacity; ++i)
            {
                if (IsInited(other.indexData[i].flag))
                {
                    InsertPrivate(other.indexData[i].hash, std::move(other.data[i]));
                }
            }
        }
    }

    size_t FindPrivate(uint32 hash, const Key &key) const
    {
        if (size == 0)
        {
            return INDEX_NONE;
        }

        size_t pos = static_cast<size_t>(hash) & mask;
        size_t dist = 0;
        while (true)
        {
            const auto flag = indexData[pos].flag;
            if (flag == FREE)
            {
                return INDEX_NONE;
            }
            if (dist > ProbeDistance(indexData[pos].hash, pos))
            {
                return INDEX_NONE;
            }
            if (!IsDeleted(flag) && indexData[pos].hash == hash && IsEqual(KeyTraits::GetKey(data[pos]), key))
            {
                return pos;
            }
            pos = (pos + 1) & mask;
            ++dist;
        }
    }

    bool RemovePrivate(const Key &key)
    {
        size_t index = FindByKey(key);
        if (index == INDEX_NONE)
        {
            return false;
        }
        DataAlloc::Destroy(data + index);
        indexData[index].flag |= DELETED;
        --size;
        return true;
    }

    void InsertPrivate(uint32 hash, const Type &value)
    {
        size_t pos = static_cast<size_t>(hash) & mask;
        size_t dist = 0;
        Type *insertPtr = nullptr;
        Type *tempPtr = nullptr;
        while (true)
        {
            if (!IsInited(indexData[pos].flag))
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
                    std::swap(*(data + pos), *tempPtr);
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

        if (tempPtr)
        {
            ThisScope::uninitialized_move(tempPtr, 1, data + pos);
            *insertPtr = value;
        }
        else
        {
            ThisScope::uninitialized_fill(data + pos, 1, value);
        }

        indexData[pos].hash = hash;
        indexData[pos].flag = DEFAULT;
    }

    void InsertPrivate(uint32 hash, Type &&value)
    {
        size_t pos = static_cast<size_t>(hash) & mask;
        size_t dist = 0;
        Type *insertPtr = nullptr;
        Type *tempPtr = nullptr;
        while (true)
        {
            if (!IsInited(indexData[pos].flag))
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
                    std::swap(*(data + pos), *tempPtr);
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

        if (tempPtr)
        {
            ThisScope::uninitialized_move(tempPtr, 1, data + pos);
            *insertPtr = std::move(value);
        }
        else
        {
            DataAlloc::Construct(data + pos, std::move(value));
        }

        indexData[pos].hash = hash;
        indexData[pos].flag = DEFAULT;
    }

    void RehashPrivate(size_t newCapacity)
    {
        HashTable temp(newCapacity);
        temp.size = size;
        temp.MoveFrom(std::move(*this));
        Swap(temp);
    }

private:
    size_t size = 0;
    size_t capacity = 0;
    size_t mask = 0;
    Type *data = nullptr;
    Index *indexData = nullptr;
};

CT_SCOPE_END
