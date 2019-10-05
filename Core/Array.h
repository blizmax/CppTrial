#pragma once

#include "Core/General.h"
#include "Core/Allocator.h"
#include "Core/Algo/Sort.h"

CT_SCOPE_BEGIN

template <typename Type, typename Alloc = Allocator<Type>>
class Array
{
public:
    Array() = default;

    explicit Array(SizeType capacity)
    {
        Reserve(capacity);
    }

    Array(std::initializer_list<Type> initList)
    {
        SizeType minCapacity = initList.size();
        Reserve(minCapacity);
        for (const Type &value : initList)
        {
            ThisScope::uninitialized_fill((data + size++), 1, value);
        }
    }

    Array(const Array &other)
    {
        SizeType minCapacity = other.size;
        Reserve(minCapacity);
        ThisScope::uninitialized_copy(other.data, other.size, data);
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
                ThisScope::copy(other.data, other.size, data);
                Alloc::Destroy(data + other.size, size - other.size);
                size = other.size;
            }
            else
            {
                ThisScope::copy(other.data, size, data);
                ThisScope::uninitialized_copy(other.data + size, other.size - size, data + size);
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

    Array &operator=(std::initializer_list<Type> initList)
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

    Type *GetData()
    {
        return data;
    }

    const Type *GetData() const
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
            ThisScope::reverse(data, data + size);
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
            //Init use default value Type() ?
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

    void Add(const Type &value)
    {
        InsertPrivate(size, value);
    }

    void Add(Type &&value)
    {
        InsertPrivate(size, std::move(value));
    }

    void Insert(SizeType index, const Type &value)
    {
        CheckRange(index);
        InsertPrivate(index, value);
    }

    void Insert(SizeType index, const Type &value, SizeType count)
    {
        CheckRange(index);
        if (count > 0)
        {
            InsertPrivate(index, value, count);
        }
    }

    void Insert(SizeType index, Type &&value)
    {
        CheckRange(index);
        InsertPrivate(index, std::move(value));
    }

    void Insert(SizeType index, const Type *src, SizeType count)
    {
        CheckRange(index);
        if (count > 0)
        {
            InsertPrivate(index, src, count);
        }
    }

    Type &First()
    {
        CheckRange(0);
        return data[0];
    }

    const Type &First() const
    {
        CheckRange(0);
        return data[0];
    }

    Type &Peek()
    {
        CheckRange(size - 1);
        return data[size - 1];
    }

    const Type &Peek() const
    {
        CheckRange(size - 1);
        return data[size - 1];
    }

    void Pop()
    {
        CheckRange(size - 1);
        Alloc::Destroy(data + (--size));
    }

    bool Find(const Type &value, SizeType *at = nullptr) const
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

    bool Find(const Type &value, SizeType &at) const
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

    bool FindLast(const Type &value, SizeType *at) const
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

    bool FindLast(const Type &value, SizeType &at) const
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

    bool Contains(const Type &value) const
    {
        return Find(value);
    }

    SizeType IndexOf(const Type &value) const
    {
        SizeType ret = 0;
        if (Find(value, ret))
        {
            return ret;
        }
        return INDEX_NONE;
    }

    SizeType LastIndexOf(const Type &value) const
    {
        SizeType ret = 0;
        if (FindLast(value, ret))
        {
            return ret;
        }
        return INDEX_NONE;
    }

    Type &At(SizeType index)
    {
        CheckRange(index);
        return data[index];
    }

    const Type &At(SizeType index) const
    {
        CheckRange(index);
        return data[index];
    }

    void Sort()
    {
        Algo::Sort(data, size, Less<Type>());
    }

    template <typename Compare>
    void Sort(Compare compare)
    {
        Algo::Sort(data, size, compare);
    }

    Type &operator[](SizeType index)
    {
        CheckRange(index);
        return data[index];
    }

    const Type &operator[](SizeType index) const
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
    Type *begin()
    {
        return data;
    }

    const Type *begin() const
    {
        return data;
    }

    Type *end()
    {
        return data ? data + size : nullptr;
    }

    const Type *end() const
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

    void DestroyAndDeallocate(Type *ptr, SizeType destroySize, SizeType deallocSize)
    {
        Alloc::Destroy(ptr, destroySize);
        Alloc::Deallocate(ptr, deallocSize);
    }

    void ReservePrivate(SizeType newCapacity)
    {
        Type *newData = Alloc::Allocate(newCapacity);
        ThisScope::uninitialized_move(data, size, newData);
        DestroyAndDeallocate(data, size, capacity);
        data = newData;
        capacity = newCapacity;
    }

    void RemovePrivate(SizeType index, SizeType count)
    {
        const SizeType moveCount = size - index - count;
        size -= count;
        ThisScope::move(data + index + count, moveCount, data + index);
        Alloc::Destroy(data + size, count);
    }

    void InsertPrivate(SizeType index, const Type &value, SizeType count = 1)
    {
        const SizeType oldSize = size;
        size += count;

        if (index == oldSize && size <= capacity)
        {
            ThisScope::uninitialized_fill(data + oldSize, count, value);
        }
        else if (size <= capacity)
        {
            SizeType moveNum = oldSize - index;
            if (moveNum > count)
            {
                ThisScope::uninitialized_move(data + oldSize - count, count, data + oldSize);
                ThisScope::move_backward(data + oldSize - count - 1, moveNum - count, data + oldSize - 1);
                ThisScope::fill(data + index, count, value);
            }
            else
            {
                ThisScope::uninitialized_move(data + index, moveNum, data + index + count);
                ThisScope::fill(data + index, moveNum, value);
                ThisScope::uninitialized_fill(data + index + moveNum, count - moveNum, value);
            }
        }
        else
        {
            const SizeType oldCapacity = capacity;
            Type *oldData = data;
            capacity = FixCapacity((oldCapacity * 2) >= size ? (oldCapacity * 2) : size);
            data = Alloc::Allocate(capacity);
            ThisScope::uninitialized_move(oldData, index, data);
            ThisScope::uninitialized_fill(data + index, count, value);
            ThisScope::uninitialized_move(oldData + index, oldSize - index, data + index + count);
            DestroyAndDeallocate(oldData, oldSize, oldCapacity);
        }
    }

    void InsertPrivate(SizeType index, Type &&value)
    {
        const SizeType oldSize = size++;

        if (index == oldSize && size <= capacity)
        {
            Alloc::Construct(data + oldSize, std::move(value));
        }
        else if (size <= capacity)
        {
            SizeType moveNum = oldSize - index;
            ThisScope::uninitialized_move(data + oldSize - 1, 1, data + oldSize);
            ThisScope::move_backward(data + oldSize - 2, moveNum - 1, data + oldSize - 1);
            Alloc::Construct(data + index, std::move(value));
        }
        else
        {
            const SizeType oldCapacity = capacity;
            Type *oldData = data;
            capacity = FixCapacity((oldCapacity * 2) >= size ? (oldCapacity * 2) : size);
            data = Alloc::Allocate(capacity);
            ThisScope::uninitialized_move(oldData, index, data);
            Alloc::Construct(data + index, std::move(value));
            ThisScope::uninitialized_move(oldData + index, oldSize - index, data + index + 1);
            DestroyAndDeallocate(oldData, oldSize, oldCapacity);
        }
    }

    void InsertPrivate(SizeType index, const Type *src, SizeType count)
    {
        const SizeType oldSize = size;
        size += count;

        if (index == oldSize && size <= capacity)
        {
            ThisScope::uninitialized_copy(src, count, data + oldSize);
        }
        else if (size <= capacity)
        {
            SizeType moveNum = oldSize - index;
            if (moveNum > count)
            {
                ThisScope::uninitialized_move(data + oldSize - count, count, data + oldSize);
                ThisScope::move_backward(data + oldSize - count - 1, moveNum - count, data + oldSize - 1);
                ThisScope::copy(src, count, data + index);
            }
            else
            {
                ThisScope::uninitialized_move(data + index, moveNum, data + index + count);
                ThisScope::copy(src, moveNum, data + index);
                ThisScope::uninitialized_copy(src + moveNum, count - moveNum, data + index + moveNum);
            }
        }
        else
        {
            const SizeType oldCapacity = capacity;
            Type *oldData = data;
            capacity = FixCapacity((oldCapacity * 2) >= size ? (oldCapacity * 2) : size);
            data = Alloc::Allocate(capacity);
            ThisScope::uninitialized_move(oldData, index, data);
            ThisScope::copy(src, count, data + index);
            ThisScope::uninitialized_move(oldData + index, oldSize - index, data + index + count);
            DestroyAndDeallocate(oldData, oldSize, oldCapacity);
        }
    }

private:
    SizeType size = 0;
    SizeType capacity = 0;
    Type *data = nullptr;
};

CT_SCOPE_END