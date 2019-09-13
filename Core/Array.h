#pragma once

#include "General.h"
#include "Allocator.h"
#include "Uninitialized.h"

CT_SCOPE_BEGIN

template <typename Type, typename Alloc = Allocator<Type>>
class Array
{
public:
    Array() = default;

    explicit Array(const size_t capacity)
    {
        Reserve(capacity);
    }

    Array(std::initializer_list<Type> initList)
    {
        size_t needCapacity = initList.size();
        Reserve(needCapacity);
        for (const Type &value : initList)
        {
            uninitialized_fill((data + size++), 1, value);
        }
    }

    Array(const Array &arr);
    Array(Array &&arr);
    Array &operator=(const Array &arr);
    Array &operator=(Array &&arr);

    ~Array()
    {
        DestroyAndDeallocate(data, size, capacity);
        data = nullptr;
        size = capacity = 0;
    }

    void Clear()
    {
        Alloc::Destroy(data, size);
        size = 0;
    }

    void Reserve(size_t newCapacity)
    {
        if (newCapacity <= capacity)
            return;

        newCapacity = FixCapacity(newCapacity);
        Type *newData = Alloc::Allocate(newCapacity);
        uninitialized_move(data, size, newData);
        DestroyAndDeallocate(data, size, capacity);
        data = newData;
        capacity = newCapacity;
    }

    void Remove(size_t index)
    {
        CheckRange(index);
        RemovePrivate(index);
    }

    void Add(const Type &value)
    {
        InsertPrivate(size, value);
    }

    void Add(Type &&value)
    {
        InsertPrivate(size, std::move(value));
    }

    void Insert(size_t index, const Type &value)
    {
        CheckRange(index);
        InsertPrivate(index, value);
    }

    void Insert(size_t index, Type &&value)
    {
        CheckRange(index);
        InsertPrivate(index, std::move(value));
    }

    Type &At(size_t index)
    {
        CheckRange(index);
        return data[index];
    }

    const Type &At(size_t index) const
    {
        CheckRange(index);
        return data[index];
    }

    Type &operator[](size_t index)
    {
        CheckRange(index);
        return data[index];
    }

    const Type &operator[](size_t index) const
    {
        CheckRange(index);
        return data[index];
    }

    // typedef Type value_type;
    // typedef Type &reference;
    // typedef const Type &const_reference;
    // typedef size_t size_type;
    // typedef Type *iterator;
    // typedef const Type *const_iterator;

    Type *begin() noexcept
    {
        return data;
    }

    Type *end() noexcept
    {
        return data ? data + size : nullptr;
    }

    bool operator==(const Array &arr) const;
    bool operator!=(const Array &arr) const;

private:
    void CheckRange(size_t index)
    {
        CT_ASSERT(index < size && index >= 0);
    }

    size_t FixCapacity(size_t inputCapacity)
    {
        return (inputCapacity < 8) ? 8 : CT_ALIGN(inputCapacity, 8);
    }

    void DestroyAndDeallocate(Type *ptr, size_t destroySize, size_t deallocSize)
    {
        Alloc::Destroy(ptr, destroySize);
        Alloc::Deallocate(ptr, deallocSize);
    }

    void RemovePrivate(size_t index)
    {
        for (size_t pos = index; pos < size - 1; ++pos)
        {
            data[pos] = std::move(data[pos + 1]);
        }

        Alloc::Destroy(data + (--size));
    }

    void InsertPrivate(size_t index, const Type &value)
    {
        const size_t oldSize = size++;

        if (index == oldSize && oldSize < capacity)
        {
            uninitialized_fill(data + size, 1, value);
        }
        else if (oldSize < capacity)
        {
            for (size_t pos = oldSize; pos > index; --pos)
            {
                data[pos] = data[pos - 1];
            }
            uninitialized_fill(data + index, 1, value);
        }
        else
        {
            const size_t oldCapacity = capacity;
            Type *oldData = data;
            capacity = FixCapacity(oldCapacity * 2);
            data = Alloc::Allocate(capacity);
            uninitialized_move(oldData, index, data);
            uninitialized_fill(data + index, 1, value);
            uninitialized_move(oldData + index, oldSize - index, data + index + 1);
            DestroyAndDeallocate(oldData, oldSize, oldCapacity);
        }
    }

    void InsertPrivate(size_t index, Type &&value)
    {
        const size_t oldSize = size++;

        if (index == oldSize && oldSize < capacity)
        {
            Alloc::Construct(data + size, std::move(value));
        }
        else if (oldSize < capacity)
        {
            for (size_t pos = oldSize; pos > index; --pos)
            {
                data[pos] = std::move(data[pos - 1]);
            }
            Alloc::Construct(data + index, std::move(value));
        }
        else
        {
            const size_t oldCapacity = capacity;
            Type *oldData = data;
            capacity = FixCapacity(oldCapacity * 2);
            data = Alloc::Allocate(capacity);
            uninitialized_move(oldData, index, data);
            Alloc::Construct(data + index, std::move(value));
            uninitialized_move(oldData + index, oldSize - index, data + index + 1);
            DestroyAndDeallocate(oldData, oldSize, oldCapacity);
        }
    }

public:
    size_t size = 0;
    size_t capacity = 0;
    Type *data = nullptr;
};

CT_SCOPE_END