#pragma once

#include "Core/General.h"

CT_SCOPE_BEGIN

template <typename T>
struct EqualTo
{
    bool operator()(const T &a, const T &b)
    {
        return a == b;
    }
};

template <typename T>
struct Less
{
    bool operator()(const T &a, const T &b)
    {
        return a < b;
    }
};

CT_SCOPE_END