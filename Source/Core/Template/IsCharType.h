#pragma once

#include "Core/Template/.Package.h"

template <typename T>
struct TIsCharType : public TFalseType
{
};

template <>
struct TIsCharType<char8> : public TTrueType
{
};

template <>
struct TIsCharType<char16> : public TTrueType
{
};

template <>
struct TIsCharType<char32> : public TTrueType
{
};

template <>
struct TIsCharType<wchar> : public TTrueType
{
};