#pragma once

#include "Core/.Package.h"

template <typename T>
struct Plus
{
    T operator()(const T &a, const T &b) const
    {
        return a + b;
    }
};

template <typename T>
struct Minus
{
    T operator()(const T &a, const T &b) const
    {
        return a - b;
    }
};

template <typename T>
struct Multiplies
{
    T operator()(const T &a, const T &b) const
    {
        return a * b;
    }
};

template <typename T>
struct Divides
{
    T operator()(const T &a, const T &b) const
    {
        return a / b;
    }
};

template <typename T>
struct Modulus
{
    T operator()(const T &a, const T &b) const
    {
        return a % b;
    }
};

template <typename T>
struct EqualTo
{
    bool operator()(const T &a, const T &b) const
    {
        return a == b;
    }
};

template <typename T>
struct NotEqualTo
{
    bool operator()(const T &a, const T &b) const
    {
        return a != b;
    }
};

template <typename T>
struct Greater
{
    bool operator()(const T &a, const T &b) const
    {
        return a > b;
    }
};

template <typename T>
struct Less
{
    bool operator()(const T &a, const T &b) const
    {
        return a < b;
    }
};

template <typename T>
struct GreaterEqual
{
    bool operator()(const T &a, const T &b) const
    {
        return a >= b;
    }
};

template <typename T>
struct LessEqual
{
    bool operator()(const T &a, const T &b) const
    {
        return a <= b;
    }
};

template <typename T>
struct CompareTo
{
    int32 operator()(const T &a, const T &b) const
    {
        if (a < b)
            return -1;
        else if (a == b)
            return 0;
        else
            return 1;
    }
};