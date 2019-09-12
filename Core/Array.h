#pragma once

#include "General.h"
#include "Allocator.h"

CT_SCOPE_BEGIN

template <typename Type, typename Alloc = Allocator<Type>>
class Array
{
public:
    Array();
    explicit Array(const size_t cap);
    Array(std::initializer_list<Type> initList);
    Array(const Array &arr);
    Array(Array &&arr);
    Array &operator=(const Array &arr);
    Array &operator=(Array &&arr);
    ~Array();

    bool operator==(const Array &arr) const;
    bool operator!=(const Array &arr) const;

private:
    size_t size;
    size_t capacity;
    Type* data;
};



CT_SCOPE_END