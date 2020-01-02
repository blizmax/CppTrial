#pragma once

#include "Core/Container/.Package.h"
#include "Core/Allocator.h"
#include "Core/Math.h"

template <typename Element, typename Hasher, typename KeyEqual, typename KeyTraits, template <typename> class Alloc>
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
        uint32 hash;
        uint32 flag;
    };

public:
    using Key = typename KeyTraits::KeyType;

    HashTable() = default;

    explicit HashTable(SizeType initCapacity)
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

    SizeType Size() const
    {
        return size;
    }

    SizeType Capacity() const
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
            SizeType newCapacity = FixCapacity(size);
            if (newCapacity < capacity)
            {
                RehashPrivate(newCapacity);
            }
        }
    }

    SizeType Find(const Element &value) const
    {
        const Key &key = KeyTraits::GetKey(value);
        return FindPrivate(HashKey(key), key);
    }

    SizeType FindByKey(const Key &key) const
    {
        return FindPrivate(HashKey(key), key);
    }

    bool Contains(const Element &value) const
    {
        return Find(value) != INDEX_NONE;
    }

    bool ContainsKey(const Key &key) const
    {
        return FindByKey(key) != INDEX_NONE;
    }

    void Put(const Element &value)
    {
        const Key &key = KeyTraits::GetKey(value);
        auto hash = HashKey(key);
        SizeType pos = FindPrivate(hash, key);
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

    void Put(Element &&value)
    {
        const Key &key = KeyTraits::GetKey(value);
        auto hash = HashKey(key);
        SizeType pos = FindPrivate(hash, key);
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

    bool Add(const Element &value)
    {
        const Key &key = KeyTraits::GetKey(value);
        auto hash = HashKey(key);
        SizeType pos = FindPrivate(hash, key);
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

    bool Add(Element &&value)
    {
        const Key &key = KeyTraits::GetKey(value);
        auto hash = HashKey(key);
        SizeType pos = FindPrivate(hash, key);
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

    Element &Get(const Element &value)
    {
        SizeType pos = Find(value);
        CheckRange(pos);
        return data[pos];
    }

    const Element &Get(const Element &value) const
    {
        SizeType pos = Find(value);
        CheckRange(pos);
        return data[pos];
    }

    Element &GetByKey(const Key &key)
    {
        SizeType pos = FindByKey(key);
        CheckRange(pos);
        return data[pos];
    }

    const Element &GetByKey(const Key &key) const
    {
        SizeType pos = FindByKey(key);
        CheckRange(pos);
        return data[pos];
    }

    bool Remove(const Element &value)
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

        for (SizeType i = 0; i < capacity; ++i)
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
        SizeType index;

        void Step()
        {
            for (SizeType i = index; i < table->capacity; ++i)
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
        Iterator(HashTable *ht, SizeType idx) : table(ht), index(idx)
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

        Element &operator*()
        {
            table->CheckRange(index);
            return table->data[index];
        }

        Element *operator->()
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
        SizeType index;

        void Step()
        {
            for (SizeType i = index; i < table->capacity; ++i)
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
        ConstIterator(const HashTable *ht, SizeType idx) : table(ht), index(idx)
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

        const Element &operator*()
        {
            table->CheckRange(index);
            return table->data[index];
        }

        const Element *operator->()
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
    void CheckRange(SizeType index) const
    {
        CT_ASSERT(index < capacity);
    }

    uint32 HashKey(const Key &key) const
    {
        static Hasher hash;
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

    SizeType ProbeDistance(uint32 hash, SizeType pos) const
    {
        SizeType desiredPos = static_cast<SizeType>(hash) & mask;
        return (pos + capacity - desiredPos) & mask;
    }

    SizeType FixCapacity(SizeType newCapacity) const
    {
        newCapacity = newCapacity < 8 ? 8 : newCapacity;
        if (Math::IsPowerOfTwo(newCapacity))
        {
            return newCapacity;
        }
        return Math::NextPowerOfTwo(newCapacity);
    }

    void DestroyAllKeys()
    {
        for (SizeType i = 0; i < capacity; ++i)
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
            for (SizeType i = 0; i < other.capacity; ++i)
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
            for (SizeType i = 0; i < other.capacity; ++i)
            {
                if (IsInited(other.indexData[i].flag))
                {
                    InsertPrivate(other.indexData[i].hash, std::move(other.data[i]));
                }
            }
        }
    }

    SizeType FindPrivate(uint32 hash, const Key &key) const
    {
        if (size == 0)
        {
            return INDEX_NONE;
        }

        SizeType pos = static_cast<SizeType>(hash) & mask;
        SizeType dist = 0;
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
        SizeType index = FindByKey(key);
        if (index == INDEX_NONE)
        {
            return false;
        }
        DataAlloc::Destroy(data + index);
        indexData[index].flag |= DELETED;
        --size;
        return true;
    }

    bool PreInsert(uint32 hash, Element *&insertPtr)
    {
        SizeType pos = static_cast<SizeType>(hash) & mask;
        SizeType dist = 0;
        Element *tempPtr = nullptr;
        while (true)
        {
            if (!IsInited(indexData[pos].flag))
            {
                break;
            }
            SizeType existingDist = ProbeDistance(indexData[pos].hash, pos);
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
                    tempPtr = data + pos;
                }
                dist = existingDist;
            }

            pos = (pos + 1) & mask;
            ++dist;
        }

        indexData[pos].hash = hash;
        indexData[pos].flag = DEFAULT;

        if (tempPtr)
        {
            Memory::UninitializedMove(data + pos, tempPtr, 1);
            return true;
        }

        insertPtr = data + pos;
        return false;
    }

    void InsertPrivate(uint32 hash, const Element &value)
    {
        Element *insertPtr = nullptr;
        if (PreInsert(hash, insertPtr))
        {
            *insertPtr = value;
        }
        else
        {
            Memory::UninitializedFill(insertPtr, 1, value);
        }
    }

    void InsertPrivate(uint32 hash, Element &&value)
    {
        Element *insertPtr = nullptr;
        if (PreInsert(hash, insertPtr))
        {
            *insertPtr = std::move(value);
        }
        else
        {
            DataAlloc::Construct(insertPtr, std::move(value));
        }
    }

    void RehashPrivate(SizeType newCapacity)
    {
        HashTable temp(newCapacity);
        temp.size = size;
        temp.MoveFrom(std::move(*this));
        Swap(temp);
    }

private:
    using DataAlloc = Alloc<Element>;
    using IndexAlloc = Alloc<Index>;

    SizeType size = 0;
    SizeType capacity = 0;
    SizeType mask = 0;
    Element *data = nullptr;
    Index *indexData = nullptr;
};