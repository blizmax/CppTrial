#pragma once

#include "Core/.Package.h"
#include "Core/Allocator.h"
#include "Core/Algo/Sort.h"

template <typename Element, typename Alloc = Allocator<Element>>
class Array
{
public:
    Array() = default;

    explicit Array(SizeType capacity)
    {
        Reserve(capacity);
    }

    Array(std::initializer_list<Element> initList)
    {
        SizeType minCapacity = initList.size();
        Reserve(minCapacity);
        for (const Element &value : initList)
        {
            Memory::UninitializedFill((data + size++), 1, value);
        }
    }

    Array(const Array &other)
    {
        SizeType minCapacity = other.size;
        Reserve(minCapacity);
        Memory::UninitializedCopy(data, other.data, other.size);
        size = other.size;
    }

    Array(Array &&other) noexcept : size(other.size), capacity(other.capacity), data(other.data)
    {
        other.size = 0;
        other.capacity = 0;
        other.data = nullptr;
    }

    Array &operator=(const Array &other)
    {
        if (this != &other)
        {
            if (other.size > capacity)
            {
                Array temp(other);
                Swap(temp);
            }
            else if (size > other.size)
            {
                Memory::Copy(data, other.data, other.size);
                Alloc::Destroy(data + other.size, size - other.size);
                size = other.size;
            }
            else
            {
                Memory::Copy(data, other.data, size);
                Memory::UninitializedCopy(data + size, other.data + size, other.size - size);
                size = other.size;
            }
        }
        return *this;
    }

    Array &operator=(Array &&other) noexcept
    {
        if (this != &other)
        {
            DestroyAndDeallocate(data, size, capacity);
            size = other.size;
            capacity = other.capacity;
            data = other.data;
            other.size = 0;
            other.capacity = 0;
            other.data = nullptr;
        }
        return *this;
    }

    Array &operator=(std::initializer_list<Element> initList)
    {
        Array temp(initList);
        Swap(temp);
        return *this;
    }

    ~Array()
    {
        DestroyAndDeallocate(data, size, capacity);
        data = nullptr;
        size = capacity = 0;
    }

    Element *GetData()
    {
        return data;
    }

    const Element *GetData() const
    {
        return data;
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

    void Swap(Array &other) noexcept
    {
        if (this != &other)
        {
            std::swap(size, other.size);
            std::swap(capacity, other.capacity);
            std::swap(data, other.data);
        }
    }

    void Clear()
    {
        Alloc::Destroy(data, size);
        size = 0;
    }

    void Shrink()
    {
        if (size != capacity)
        {
            SizeType newCapacity = FixCapacity(size);
            if (newCapacity < capacity)
            {
                ReservePrivate(newCapacity);
            }
        }
    }

    void Reverse()
    {
        if (size > 1)
        {
            Memory::Reverse(data, data + size);
        }
    }

    void Reserve(SizeType newCapacity)
    {
        if (newCapacity <= capacity)
            return;

        newCapacity = FixCapacity(newCapacity);
        if (newCapacity > capacity)
        {
            ReservePrivate(newCapacity);
        }
    }

    void Resize(SizeType newSize)
    {
        if (newSize < size)
        {
            RemovePrivate(newSize, size - newSize);
        }
        else if (newSize > size)
        {
            AppendUninitialized(newSize - size);
            //FIXME
            //Init use default value Element() ?
        }
    }

    void AppendUninitialized(SizeType appendSize)
    {
        const SizeType newSize = size + appendSize;
        if (newSize > capacity)
        {
            Reserve(newSize);
        }
        size = newSize;
    }

    void Remove(SizeType index)
    {
        CheckRange(index);
        RemovePrivate(index, 1);
    }

    void Remove(SizeType index, SizeType count)
    {
        CheckRange(index);
        CheckRange(index + count - 1);
        RemovePrivate(index, count);
    }

    void RemoveValue(const Element &value)
    {
        auto index = IndexOf(value);
        if (index != INDEX_NONE)
        {
            Remove(index);
        }
    }

    void Add(const Element &value)
    {
        InsertPrivate(size, value);
    }

    void Add(Element &&value)
    {
        InsertPrivate(size, std::move(value));
    }

    void Insert(SizeType index, const Element &value)
    {
        CheckRange(index);
        InsertPrivate(index, value);
    }

    void Insert(SizeType index, const Element &value, SizeType count)
    {
        CheckRange(index);
        if (count > 0)
        {
            InsertPrivate(index, value, count);
        }
    }

    void Insert(SizeType index, Element &&value)
    {
        CheckRange(index);
        InsertPrivate(index, std::move(value));
    }

    void Insert(SizeType index, const Element *src, SizeType count)
    {
        CheckRange(index);
        if (count > 0)
        {
            InsertPrivate(index, src, count);
        }
    }

    Element &First()
    {
        CheckRange(0);
        return data[0];
    }

    const Element &First() const
    {
        CheckRange(0);
        return data[0];
    }

    Element &Peek()
    {
        CheckRange(size - 1);
        return data[size - 1];
    }

    const Element &Peek() const
    {
        CheckRange(size - 1);
        return data[size - 1];
    }

    void Pop()
    {
        CheckRange(size - 1);
        Alloc::Destroy(data + (--size));
    }

    bool Find(const Element &value, SizeType *at = nullptr) const
    {
        for (SizeType i = 0; i < size; ++i)
        {
            if (data[i] == value)
            {
                if (at)
                {
                    *at = i;
                }
                return true;
            }
        }
        return false;
    }

    bool Find(const Element &value, SizeType &at) const
    {
        return Find(value, &at);
    }

    template <typename Predicate>
    bool Find(Predicate pred, SizeType *at = nullptr) const
    {
        for (SizeType i = 0; i < size; ++i)
        {
            if (pred(data[i]))
            {
                if (at)
                {
                    *at = i;
                }
                return true;
            }
        }
        return false;
    }

    template <typename Predicate>
    bool Find(Predicate pred, SizeType &at) const
    {
        return Find(pred, &at);
    }

    bool FindLast(const Element &value, SizeType *at) const
    {
        for (SizeType i = size; i >= 1;)
        {
            --i;
            if (data[i] == value)
            {
                if (at)
                {
                    *at = i;
                }
                return true;
            }
        }
        return false;
    }

    bool FindLast(const Element &value, SizeType &at) const
    {
        return Find(value, &at);
    }

    template <typename Predicate>
    bool FindLast(Predicate pred, SizeType *at = nullptr) const
    {
        for (SizeType i = size; i >= 1;)
        {
            --i;
            if (pred(data[i]))
            {
                if (at)
                {
                    *at = i;
                }
                return true;
            }
        }
        return false;
    }

    template <typename Predicate>
    bool FindLast(Predicate pred, SizeType &at) const
    {
        return FindLast(pred, &at);
    }

    bool Contains(const Element &value) const
    {
        return Find(value);
    }

    SizeType IndexOf(const Element &value) const
    {
        SizeType ret = 0;
        if (Find(value, ret))
        {
            return ret;
        }
        return INDEX_NONE;
    }

    SizeType LastIndexOf(const Element &value) const
    {
        SizeType ret = 0;
        if (FindLast(value, ret))
        {
            return ret;
        }
        return INDEX_NONE;
    }

    Element &At(SizeType index)
    {
        CheckRange(index);
        return data[index];
    }

    const Element &At(SizeType index) const
    {
        CheckRange(index);
        return data[index];
    }

    void Sort()
    {
        Algo::Sort(data, size, Less<Element>());
    }

    template <typename Compare>
    void Sort(Compare compare)
    {
        Algo::Sort(data, size, compare);
    }

    Element &operator[](SizeType index)
    {
        CheckRange(index);
        return data[index];
    }

    const Element &operator[](SizeType index) const
    {
        CheckRange(index);
        return data[index];
    }

    bool operator==(const Array &other) const
    {
        if (size != other.size)
        {
            return false;
        }

        for (SizeType i = 0; i < size; ++i)
        {
            if (data[i] != other.data[i])
                return false;
        }
        return true;
    }

    bool operator!=(const Array &other) const
    {
        return !(*this == other);
    }

    //===================== STL STYLE =========================
public:
    Element *begin()
    {
        return data;
    }

    const Element *begin() const
    {
        return data;
    }

    Element *end()
    {
        return data ? data + size : nullptr;
    }

    const Element *end() const
    {
        return data ? data + size : nullptr;
    }

private:
    void CheckRange(SizeType index) const
    {
        CT_ASSERT(index >= 0 && index < size);
    }

    SizeType FixCapacity(SizeType inputCapacity) const
    {
        return (inputCapacity < 8) ? 8 : CT_ALIGN(inputCapacity, 8);
    }

    void DestroyAndDeallocate(Element *ptr, SizeType destroySize, SizeType deallocSize)
    {
        Alloc::Destroy(ptr, destroySize);
        Alloc::Deallocate(ptr, deallocSize);
    }

    void ReservePrivate(SizeType newCapacity)
    {
        Element *newData = Alloc::Allocate(newCapacity);
        Memory::UninitializedMove(newData, data, size);
        DestroyAndDeallocate(data, size, capacity);
        data = newData;
        capacity = newCapacity;
    }

    void RemovePrivate(SizeType index, SizeType count)
    {
        const SizeType moveCount = size - index - count;
        size -= count;
        Memory::Move(data + index, data + index + count, moveCount);
        Alloc::Destroy(data + size, count);
    }

    void InsertPrivate(SizeType index, const Element &value, SizeType count = 1)
    {
        const SizeType oldSize = size;
        size += count;

        if (index == oldSize && size <= capacity)
        {
            Memory::UninitializedFill(data + oldSize, count, value);
        }
        else if (size <= capacity)
        {
            SizeType moveNum = oldSize - index;
            if (moveNum > count)
            {
                Memory::UninitializedMove(data + oldSize, data + oldSize - count, count);
                Memory::MoveBackward(data + oldSize - 1, data + oldSize - count - 1, moveNum - count);
                Memory::Fill(data + index, count, value);
            }
            else
            {
                Memory::UninitializedMove(data + index + count, data + index, moveNum);
                Memory::Fill(data + index, moveNum, value);
                Memory::UninitializedFill(data + index + moveNum, count - moveNum, value);
            }
        }
        else
        {
            const SizeType oldCapacity = capacity;
            Element *oldData = data;
            capacity = FixCapacity((oldCapacity * 2) >= size ? (oldCapacity * 2) : size);
            data = Alloc::Allocate(capacity);
            Memory::UninitializedMove(data, oldData, index);
            Memory::UninitializedFill(data + index, count, value);
            Memory::UninitializedMove(data + index + count, oldData + index, oldSize - index);
            DestroyAndDeallocate(oldData, oldSize, oldCapacity);
        }
    }

    void InsertPrivate(SizeType index, Element &&value)
    {
        const SizeType oldSize = size++;

        if (index == oldSize && size <= capacity)
        {
            Alloc::Construct(data + oldSize, std::move(value));
        }
        else if (size <= capacity)
        {
            SizeType moveNum = oldSize - index;
            Memory::UninitializedMove(data + oldSize, data + oldSize - 1, 1);
            Memory::MoveBackward(data + oldSize - 1, data + oldSize - 2, moveNum - 1);
            Alloc::Construct(data + index, std::move(value));
        }
        else
        {
            const SizeType oldCapacity = capacity;
            Element *oldData = data;
            capacity = FixCapacity((oldCapacity * 2) >= size ? (oldCapacity * 2) : size);
            data = Alloc::Allocate(capacity);
            Memory::UninitializedMove(data, oldData, index);
            Alloc::Construct(data + index, std::move(value));
            Memory::UninitializedMove(data + index + 1, oldData + index, oldSize - index);
            DestroyAndDeallocate(oldData, oldSize, oldCapacity);
        }
    }

    void InsertPrivate(SizeType index, const Element *src, SizeType count)
    {
        const SizeType oldSize = size;
        size += count;

        if (index == oldSize && size <= capacity)
        {
            Memory::UninitializedCopy(data + oldSize, src, count);
        }
        else if (size <= capacity)
        {
            SizeType moveNum = oldSize - index;
            if (moveNum > count)
            {
                Memory::UninitializedMove(data + oldSize, data + oldSize - count, count);
                Memory::MoveBackward(data + oldSize - 1, data + oldSize - count - 1, moveNum - count);
                Memory::Copy(data + index, src, count);
            }
            else
            {
                Memory::UninitializedMove(data + index + count, data + index, moveNum);
                Memory::Copy(data + index, src, moveNum);
                Memory::UninitializedCopy(data + index + moveNum, src + moveNum, count - moveNum);
            }
        }
        else
        {
            const SizeType oldCapacity = capacity;
            Element *oldData = data;
            capacity = FixCapacity((oldCapacity * 2) >= size ? (oldCapacity * 2) : size);
            data = Alloc::Allocate(capacity);
            Memory::UninitializedMove(data, oldData, index);
            Memory::Copy(data + index, src, count);
            Memory::UninitializedMove(data + index + count, oldData + index, oldSize - index);
            DestroyAndDeallocate(oldData, oldSize, oldCapacity);
        }
    }

private:
    SizeType size = 0;
    SizeType capacity = 0;
    Element *data = nullptr;
};

namespace std
{
template <typename T, typename A>
inline void swap(Array<T, A> &lhs, Array<T, A> &rhs)
{
    lhs.Swap(rhs);
}
}