#pragma once

#include "Core/.Package.h"
#include "Core/Allocator.h"
#include "Core/Algo/Sort.h"

template <typename Element, typename Alloc = Allocator<Element>>
class Array
{
public:
    Array() = default;

    explicit Array(int32 capacity)
    {
        Reserve(capacity);
    }

    Array(std::initializer_list<Element> initList)
    {
        int32 minCapacity = (int32)initList.size();
        Reserve(minCapacity);
        for (const Element &value : initList)
        {
            Memory::UninitializedFill((data + count++), 1, value);
        }
    }

    Array(const Array &other)
    {
        int32 minCapacity = other.count;
        Reserve(minCapacity);
        Memory::UninitializedCopy(data, other.data, other.count);
        count = other.count;
    }

    Array(Array &&other) noexcept : count(other.count), capacity(other.capacity), data(other.data)
    {
        other.count = 0;
        other.capacity = 0;
        other.data = nullptr;
    }

    Array &operator=(const Array &other)
    {
        if (this != &other)
        {
            if (other.count > capacity)
            {
                Array temp(other);
                Swap(temp);
            }
            else if (count > other.count)
            {
                Memory::Copy(data, other.data, other.count);
                Alloc::Destroy(data + other.count, count - other.count);
                count = other.count;
            }
            else
            {
                Memory::Copy(data, other.data, count);
                Memory::UninitializedCopy(data + count, other.data + count, other.count - count);
                count = other.count;
            }
        }
        return *this;
    }

    Array &operator=(Array &&other) noexcept
    {
        if (this != &other)
        {
            Array temp(std::move(other));
            Swap(temp);
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
        DestroyAndDeallocate(data, count, capacity);
        data = nullptr;
        count = capacity = 0;
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

    void Swap(Array &other) noexcept
    {
        if (this != &other)
        {
            std::swap(count, other.count);
            std::swap(capacity, other.capacity);
            std::swap(data, other.data);
        }
    }

    void Clear()
    {
        Alloc::Destroy(data, count);
        count = 0;
    }

    void Shrink()
    {
        if (count != capacity)
        {
            int32 newCapacity = FixCapacity(count);
            if (newCapacity < capacity)
            {
                ReservePrivate(newCapacity);
            }
        }
    }

    void Reverse()
    {
        if (count > 1)
        {
            Memory::Reverse(data, data + count);
        }
    }

    void Reserve(int32 newCapacity)
    {
        if (newCapacity <= capacity)
            return;

        newCapacity = FixCapacity(newCapacity);
        if (newCapacity > capacity)
        {
            ReservePrivate(newCapacity);
        }
    }

    void Resize(int32 newCount)
    {
        CT_CHECK(newCount >= 0);

        if (newCount < count)
        {
            RemovePrivate(newCount, count - newCount);
        }
        else if (newCount > count)
        {
            AppendUninitialized(newCount - count);
            //FIXME
            //Init use default value Element() ?
        }
    }

    void AppendUninitialized(int32 num)
    {
        CT_CHECK(num >= 0);

        const int32 newCount = count + num;
        if (newCount > capacity)
        {
            Reserve(newCount);
        }
        count = newCount;
    }

    bool Find(const Element &value, int32 *at = nullptr) const
    {
        for (int32 i = 0; i < count; ++i)
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

    bool Find(const Element &value, int32 &at) const
    {
        return Find(value, &at);
    }

    template <typename Predicate>
    bool Find(Predicate pred, int32 *at = nullptr) const
    {
        for (int32 i = 0; i < count; ++i)
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
    bool Find(Predicate pred, int32 &at) const
    {
        return Find(pred, &at);
    }

    bool FindLast(const Element &value, int32 *at) const
    {
        for (int32 i = count - 1; i >= 0; --i)
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

    bool FindLast(const Element &value, int32 &at) const
    {
        return Find(value, &at);
    }

    template <typename Predicate>
    bool FindLast(Predicate pred, int32 *at = nullptr) const
    {
        for (int32 i = count - 1; i >= 0; --i)
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
    bool FindLast(Predicate pred, int32 &at) const
    {
        return FindLast(pred, &at);
    }

    bool Contains(const Element &value) const
    {
        return Find(value);
    }

    int32 IndexOf(const Element &value) const
    {
        int32 result = 0;
        if (Find(value, result))
        {
            return result;
        }
        return INDEX_NONE;
    }

    int32 LastIndexOf(const Element &value) const
    {
        int32 result = 0;
        if (FindLast(value, result))
        {
            return result;
        }
        return INDEX_NONE;
    }

    void Add(const Element &value)
    {
        InsertPrivate(count, value);
    }

    void Add(Element &&value)
    {
        InsertPrivate(count, std::move(value));
    }

    void Insert(int32 index, const Element &value)
    {
        CheckRange(index);

        InsertPrivate(index, value);
    }

    void Insert(int32 index, const Element &value, int32 num)
    {
        CheckRange(index);
        CT_CHECK(num >= 0);

        if (num > 0)
        {
            InsertPrivate(index, value, num);
        }
    }

    void Insert(int32 index, Element &&value)
    {
        CT_CHECK(index >= 0 && index < count);

        InsertPrivate(index, std::move(value));
    }

    void Insert(int32 index, const Element *ptr, int32 num)
    {
        CheckRange(index);
        CT_CHECK(num >= 0);

        if (num > 0)
        {
            InsertPrivate(index, ptr, num);
        }
    }

    void RemoveLast()
    {
        CheckRange(count - 1);

        RemovePrivate(count - 1, 1);
    }

    void RemoveAt(int32 index)
    {
        CheckRange(index);

        RemovePrivate(index, 1);
    }

    void RemoveAt(int32 index, int32 num)
    {
        CheckRange(index);
        CT_CHECK(num >= 0 && num <= count);

        RemovePrivate(index, num);
    }

    bool RemoveValue(const Element &value)
    {
        int32 index = IndexOf(value);
        if (index != INDEX_NONE)
        {
            RemovePrivate(index, 1);
            return true;
        }
        return false;
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

    Element &Last()
    {
        CheckRange(count - 1);

        return data[count - 1];
    }

    const Element &Last() const
    {
        CheckRange(count - 1);

        return data[count - 1];
    }

    void Pop()
    {
        CheckRange(count - 1);

        Alloc::Destroy(data + (--count));
    }

    Element &At(int32 index)
    {
        CheckRange(index);

        return data[index];
    }

    const Element &At(int32 index) const
    {
        CheckRange(index);

        return data[index];
    }

    void Sort()
    {
        Algo::Sort(data, count, Less<Element>());
    }

    template <typename Compare>
    void Sort(Compare compare)
    {
        Algo::Sort(data, count, compare);
    }

    Element &operator[](int32 index)
    {
        CheckRange(index);

        return data[index];
    }

    const Element &operator[](int32 index) const
    {
        CheckRange(index);

        return data[index];
    }

    bool operator==(const Array &other) const
    {
        if (count != other.count)
        {
            return false;
        }

        for (int32 i = 0; i < count; ++i)
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
        return data ? data + count : nullptr;
    }

    const Element *end() const
    {
        return data ? data + count : nullptr;
    }

private:
    void CheckRange(int32 index) const
    {
        CT_CHECK(index >= 0 && index < count);
    }

    int32 FixCapacity(int32 newCapacity) const
    {
        return (newCapacity < 8) ? 8 : CT_ALIGN(newCapacity, 8);
    }

    void DestroyAndDeallocate(Element *ptr, int32 destroyNum, int32 deallocNum)
    {
        Alloc::Destroy(ptr, destroyNum);
        Alloc::Deallocate(ptr, deallocNum);
    }

    void ReservePrivate(int32 newCapacity)
    {
        Element *newData = Alloc::Allocate(newCapacity);
        Memory::UninitializedMove(newData, data, count);
        DestroyAndDeallocate(data, count, capacity);
        data = newData;
        capacity = newCapacity;
    }

    void RemovePrivate(int32 index, int32 num)
    {
        const int32 moveNum = count - index - num;
        count -= num;
        Memory::Move(data + index, data + index + num, moveNum);
        Alloc::Destroy(data + count, num);
    }

    void InsertPrivate(int32 index, const Element &value, int32 num = 1)
    {
        const int32 oldCount = count;
        count += num;

        if (index == oldCount && count <= capacity)
        {
            Memory::UninitializedFill(data + oldCount, num, value);
        }
        else if (count <= capacity)
        {
            const int32 moveNum = oldCount - index;
            if (moveNum > num)
            {
                Memory::UninitializedMove(data + oldCount, data + oldCount - num, num);
                Memory::MoveBackward(data + oldCount - 1, data + oldCount - num - 1, moveNum - num);
                Memory::Fill(data + index, num, value);
            }
            else
            {
                Memory::UninitializedMove(data + index + num, data + index, moveNum);
                Memory::Fill(data + index, moveNum, value);
                Memory::UninitializedFill(data + index + moveNum, num - moveNum, value);
            }
        }
        else
        {
            const int32 oldCapacity = capacity;
            Element *oldData = data;
            capacity = FixCapacity((oldCapacity * 2) >= count ? (oldCapacity * 2) : count);
            data = Alloc::Allocate(capacity);
            Memory::UninitializedMove(data, oldData, index);
            Memory::UninitializedFill(data + index, count, value);
            Memory::UninitializedMove(data + index + count, oldData + index, oldCount - index);
            DestroyAndDeallocate(oldData, oldCount, oldCapacity);
        }
    }

    void InsertPrivate(int32 index, Element &&value)
    {
        const int32 oldCount = count++;

        if (index == oldCount && count <= capacity)
        {
            Alloc::Construct(data + oldCount, std::move(value));
        }
        else if (count <= capacity)
        {
            const int32 moveNum = oldCount - index;
            Memory::UninitializedMove(data + oldCount, data + oldCount - 1, 1);
            Memory::MoveBackward(data + oldCount - 1, data + oldCount - 2, moveNum - 1);
            Alloc::Construct(data + index, std::move(value));
        }
        else
        {
            const int32 oldCapacity = capacity;
            Element *oldData = data;
            capacity = FixCapacity((oldCapacity * 2) >= count ? (oldCapacity * 2) : count);
            data = Alloc::Allocate(capacity);
            Memory::UninitializedMove(data, oldData, index);
            Alloc::Construct(data + index, std::move(value));
            Memory::UninitializedMove(data + index + 1, oldData + index, oldCount - index);
            DestroyAndDeallocate(oldData, oldCount, oldCapacity);
        }
    }

    void InsertPrivate(int32 index, const Element *ptr, int32 num)
    {
        const int32 oldCount = count;
        count += num;

        if (index == oldCount && count <= capacity)
        {
            Memory::UninitializedCopy(data + oldCount, ptr, num);
        }
        else if (count <= capacity)
        {
            const int32 moveNum = oldCount - index;
            if (moveNum > num)
            {
                Memory::UninitializedMove(data + oldCount, data + oldCount - num, num);
                Memory::MoveBackward(data + oldCount - 1, data + oldCount - num - 1, moveNum - num);
                Memory::Copy(data + index, ptr, num);
            }
            else
            {
                Memory::UninitializedMove(data + index + num, data + index, moveNum);
                Memory::Copy(data + index, ptr, moveNum);
                Memory::UninitializedCopy(data + index + moveNum, ptr + moveNum, num - moveNum);
            }
        }
        else
        {
            const int32 oldCapacity = capacity;
            Element *oldData = data;
            capacity = FixCapacity((oldCapacity * 2) >= count ? (oldCapacity * 2) : count);
            data = Alloc::Allocate(capacity);
            Memory::UninitializedMove(data, oldData, index);
            Memory::Copy(data + index, ptr, num);
            Memory::UninitializedMove(data + index + num, oldData + index, oldCount - index);
            DestroyAndDeallocate(oldData, oldCount, oldCapacity);
        }
    }

private:
    int32 count = 0;
    int32 capacity = 0;
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