#pragma once

#include "General.h"

CPP_TRIAL_SCOPE_BEGIN

template <typename Type, typename Allocator>
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
};

CPP_TRIAL_SCOPE_END