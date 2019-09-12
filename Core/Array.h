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
    explicit Array(const size_t capacity) {Reserve(capacity);}
    Array(std::initializer_list<Type> initList);
    Array(const Array &arr);
    Array(Array &&arr);
    Array &operator=(const Array &arr);
    Array &operator=(Array &&arr);

    ~Array()
    {
        Alloc::Destroy(data, size);
        Alloc::Deallocate(data, capacity);
        data = nullptr;
        size = capacity = 0;
    }

    void Reserve(size_t newCapacity)
    {
        Type* newData = Alloc::Allocate(newCapacity);
        uninitialized_move(data, size, newData);
        Alloc::Destroy(data, size);
        Alloc::Deallocate(data, capacity);
        data = newData;
        capacity = newCapacity;
    }

    void Grow()
    {
        size_t newCapacity = capacity ? (capacity + capacity / 2) : 8;
        Reserve(newCapacity);
    }

    void Add(const Type& value)
    {
        if(size == capacity) 
            Grow();
        uninitialized_fill((data + size++), 1, value);
    }

    void Insert(size_t index, const Type& value)
    {
        CT_ASSERT(index < size && index >= 0);
        //TODO
    }

    Type& Get(size_t index)
    {
        CT_ASSERT(index < size && index >= 0);
        return data[index];
    }
    
    const Type& Get(size_t index) const
    {
        CT_ASSERT(index < size && index >= 0);
        return data[index];
    }

    Type& operator[](size_t index)
    {
        CT_ASSERT(index < size && index >= 0);
        return data[index];
    }

    const Type& operator[](size_t index) const
    {
        CT_ASSERT(index < size && index >= 0);
        return data[index];
    }

    bool operator==(const Array &arr) const;
    bool operator!=(const Array &arr) const;

public:
    size_t size = 0;
    size_t capacity = 0;
    Type* data = nullptr;
};



CT_SCOPE_END