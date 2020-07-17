#pragma once

#include "Core/Allocator.h"
#include "Core/Container/.Package.h"
#include "Core/Math.h"

template <typename Element, typename Hasher, typename KeyEqual, typename KeyTraits, template <typename> class Alloc>
class HashTable
{
private:
    static constexpr uint32 DELETED = 0x80000000;
    static constexpr uint32 FREE = 0x00000000;
    static constexpr uint32 DEFAULT = 0x0123abcd;

    static constexpr float LOAD_FACTOR = 0.9f;

    struct Index
    {
        uint32 hash;
        uint32 flag;
    };

public:
    using Key = typename KeyTraits::KeyType;

    HashTable() = default;

    explicit HashTable(int32 initCapacity)
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

    HashTable(const HashTable &other)
        : count(other.count), capacity(other.capacity), mask(other.mask)
    {
        if (capacity > 0)
        {
            data = DataAlloc::Allocate(other.capacity);
            indexData = IndexAlloc::Allocate(other.capacity);
            FreeAllIndices();

            CopyFrom(other);
        }
    }

    HashTable(HashTable &&other) noexcept
        : count(other.count), capacity(other.capacity), mask(other.mask), data(other.data), indexData(other.indexData)
    {
        other.count = 0;
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
            HashTable temp(std::move(other));
            Swap(temp);
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
        count = capacity = mask = 0;
    }

    Element *GetData()
    {
        return data;
    }

    const Element *GetData() const
    {
        return data;
    }

    int32 Count() const
    {
        return count;
    }

    int32 Capacity() const
    {
        return capacity;
    }

    bool IsEmpty() const
    {
        return count == 0;
    }

    bool IsFull() const
    {
        return count >= capacity * LOAD_FACTOR;
    }

    void Swap(HashTable &other) noexcept
    {
        if (this != &other)
        {
            std::swap(count, other.count);
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
        count = 0;
    }

    void Shrink()
    {
        if (count != capacity)
        {
            int32 newCapacity = FixCapacity(count);
            if (newCapacity < capacity)
            {
                RehashPrivate(newCapacity);
            }
        }
    }

    int32 Find(const Element &value) const
    {
        const Key &key = KeyTraits::GetKey(value);
        return FindPrivate(HashKey(key), key);
    }

    int32 FindByKey(const Key &key) const
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
        int32 pos = FindPrivate(hash, key);
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
            ++count;
        }
    }

    void Put(Element &&value)
    {
        const Key &key = KeyTraits::GetKey(value);
        auto hash = HashKey(key);
        int32 pos = FindPrivate(hash, key);
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
            ++count;
        }
    }

    bool Add(const Element &value)
    {
        const Key &key = KeyTraits::GetKey(value);
        auto hash = HashKey(key);
        int32 pos = FindPrivate(hash, key);
        if (pos == INDEX_NONE)
        {
            if (IsFull())
            {
                RehashPrivate(FixCapacity(capacity * 2));
            }
            InsertPrivate(hash, value);
            ++count;
            return true;
        }
        return false;
    }

    bool Add(Element &&value)
    {
        const Key &key = KeyTraits::GetKey(value);
        auto hash = HashKey(key);
        int32 pos = FindPrivate(hash, key);
        if (pos == INDEX_NONE)
        {
            if (IsFull())
            {
                RehashPrivate(FixCapacity(capacity * 2));
            }
            InsertPrivate(hash, std::move(value));
            ++count;
            return true;
        }
        return false;
    }

    Element &Get(const Element &value)
    {
        auto pos = Find(value);
        CheckRange(pos);
        return data[pos];
    }

    const Element &Get(const Element &value) const
    {
        auto pos = Find(value);
        CheckRange(pos);
        return data[pos];
    }

    Element &GetByKey(const Key &key)
    {
        auto pos = FindByKey(key);
        CheckRange(pos);
        return data[pos];
    }

    const Element &GetByKey(const Key &key) const
    {
        auto pos = FindByKey(key);
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
        if (count != other.count)
        {
            return false;
        }

        for (int32 i = 0; i < capacity; ++i)
        {
            if (IsInited(indexData[i].flag))
            {
                const Element &value = data[i];
                const Key &key = KeyTraits::GetKey(value);
                if (!other.ContainsKey(key))
                {
                    return false;
                }
                if (other.GetByKey(key) != value)
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
        HashTable &table;
        int32 index;

        void Step()
        {
            for (int32 i = index; i < table.capacity; ++i)
            {
                if (table.IsInited(table.indexData[i].flag))
                {
                    index = i;
                    return;
                }
            }
            index = INDEX_NONE;
        }

    public:
        Iterator(HashTable &table, int32 index)
            : table(table), index(index)
        {
            if (index != INDEX_NONE)
            {
                Step();
            }
        }

        Iterator(const Iterator &other)
            : table(other.table), index(other.index)
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
            table.CheckRange(index);
            return table.data[index];
        }

        Element *operator->()
        {
            table.CheckRange(index);
            return &(table.data[index]);
        }

        bool operator==(const Iterator &other) const
        {
            return (&table == &other.table) && (index == other.index);
        }

        bool operator!=(const Iterator &other) const
        {
            return (&table != &other.table) || (index != other.index);
        }
    };

    class ConstIterator
    {
    private:
        const HashTable &table;
        int32 index;

        void Step()
        {
            for (int32 i = index; i < table.capacity; ++i)
            {
                if (table.IsInited(table.indexData[i].flag))
                {
                    index = i;
                    return;
                }
            }
            index = INDEX_NONE;
        }

    public:
        ConstIterator(const HashTable &table, int32 index)
            : table(table), index(index)
        {
            if (index != INDEX_NONE)
            {
                Step();
            }
        }

        ConstIterator(const ConstIterator &other)
            : table(other.table), index(other.index)
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
            ConstIterator temp(*this);
            if (index != INDEX_NONE)
            {
                ++index;
                Step();
            }
            return temp;
        }

        const Element &operator*()
        {
            table.CheckRange(index);
            return table.data[index];
        }

        const Element *operator->()
        {
            table.CheckRange(index);
            return &(table.data[index]);
        }

        bool operator==(const ConstIterator &other) const
        {
            return (&table == &other.table) && (index == other.index);
        }

        bool operator!=(const ConstIterator &other) const
        {
            return (&table != &other.table) || (index != other.index);
        }
    };

    Iterator begin()
    {
        return Iterator(*this, 0);
    }

    ConstIterator begin() const
    {
        return ConstIterator(*this, 0);
    }

    Iterator end()
    {
        return Iterator(*this, INDEX_NONE);
    }

    ConstIterator end() const
    {
        return ConstIterator(*this, INDEX_NONE);
    }

private:
    void CheckRange(int32 index) const
    {
        CT_CHECK(index < capacity);
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

    int32 ProbeDistance(uint32 hash, int32 pos) const
    {
        int32 desiredPos = static_cast<int32>(hash) & mask;
        return (pos + capacity - desiredPos) & mask;
    }

    int32 FixCapacity(int32 newCapacity) const
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
        for (int32 i = 0; i < capacity; ++i)
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
        if (other.count > 0)
        {
            for (int32 i = 0; i < other.capacity; ++i)
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
        if (other.count > 0)
        {
            for (int32 i = 0; i < other.capacity; ++i)
            {
                if (IsInited(other.indexData[i].flag))
                {
                    InsertPrivate(other.indexData[i].hash, std::move(other.data[i]));
                }
            }
        }
    }

    int32 FindPrivate(uint32 hash, const Key &key) const
    {
        if (count == 0)
        {
            return INDEX_NONE;
        }

        int32 pos = static_cast<int32>(hash) & mask;
        int32 dist = 0;
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
        int32 index = FindByKey(key);
        if (index == INDEX_NONE)
        {
            return false;
        }
        DataAlloc::Destroy(data + index);
        indexData[index].flag |= DELETED;
        --count;
        return true;
    }

    bool PreInsert(uint32 hash, Element *&insertPtr)
    {
        int32 pos = static_cast<int32>(hash) & mask;
        int32 dist = 0;
        Element *tempPtr = nullptr;
        while (true)
        {
            if (!IsInited(indexData[pos].flag))
            {
                break;
            }
            auto existingDist = ProbeDistance(indexData[pos].hash, pos);
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

    void RehashPrivate(int32 newCapacity)
    {
        HashTable temp(newCapacity);
        temp.count = count;
        temp.MoveFrom(std::move(*this));
        Swap(temp);
    }

private:
    using DataAlloc = Alloc<Element>;
    using IndexAlloc = Alloc<Index>;

    int32 count = 0;
    int32 capacity = 0;
    int32 mask = 0;
    Element *data = nullptr;
    Index *indexData = nullptr;
};