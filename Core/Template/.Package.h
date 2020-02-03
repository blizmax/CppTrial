#pragma once

#include "Core/.Package.h"
#include <type_traits>

using TTrueType = std::true_type;
using TFalseType = std::false_type;

template <bool B, typename T>
using TEnableIf = std::enable_if<B, T>;

template <bool B, typename T, typename F>
using TConditional = std::conditional<B, T, F>;

template <typename T1, typename T2>
using TIsSame = std::is_same<T1, T2>;

template <typename T>
using TDecay = std::decay<T>;

template <typename T>
using TIsVoid = std::is_void<T>;
template <typename T>
using TIsPointer = std::is_pointer<T>;
template <typename T>
using TIsNullPointer = std::is_null_pointer<T>;
template <typename T>
using TIsIntegral = std::is_integral<T>;
template <typename T>
using TIsFloatingPoint = std::is_floating_point<T>;

template <typename T, typename... Args>
using TIsConstructible = std::is_constructible<T, Args...>;
template <typename T>
using TIsTriviallyDestructible = std::is_trivially_destructible<T>;
template <typename T>
using TIsTriviallyCopyAssignable = std::is_trivially_copy_assignable<T>;
template <typename T>
using TIsTriviallyMoveAssignable = std::is_trivially_move_assignable<T>;

struct TNullType;

template <bool T>
struct TBoolType;
template <>
struct TBoolType<true> : public TTrueType
{
};
template <>
struct TBoolType<false> : public TFalseType
{
};

template <typename T>
struct TIsValid : public TTrueType
{
};
template <>
struct TIsValid<TNullType> : public TFalseType
{
};

template <typename...>
struct TAnd;
template <>
struct TAnd<> : public TTrueType
{
};
template <typename B1>
struct TAnd<B1> : public B1
{
};
template <typename B1, typename B2>
struct TAnd<B1, B2> : public TConditional<B1::value, B2, B1>::type
{
};
template <typename B1, typename B2, typename B3, typename... Bn>
struct TAnd<B1, B2, B3, Bn...> : public TConditional<B1::value, TAnd<B2, B3, Bn...>, B1>::type
{
};

template <typename...>
struct TOr;
template <>
struct TOr<> : public TFalseType
{
};
template <typename B1>
struct TOr<B1> : public B1
{
};
template <typename B1, typename B2>
struct TOr<B1, B2> : public TConditional<B1::value, B1, B2>::type
{
};
template <typename B1, typename B2, typename B3, typename... Bn>
struct TOr<B1, B2, B3, Bn...> : public TConditional<B1::value, B1, TOr<B2, B3, Bn...>>::type
{
};

template <bool B>
struct TNot : public TFalseType
{
};
template <>
struct TNot<false> : public TTrueType
{
};