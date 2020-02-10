#pragma once

#include <cassert>
#include <cstdint>
#include <initializer_list>
#include <memory>

#include "debugbreak.h"

#ifdef _DEBUG
#define CT_DEBUG 1
#endif

#define CT_DEBUG_BREAK() debug_break()

#define CT_ASSERT(cond) assert(cond)

#define CT_ALIGN(x, a) (((x) + ((a)-1)) & ~((a)-1))

#define CT_INLINE inline
#define CT_TEXT(text) L##text

#define CT_FALLTHROUGH [[fallthrough]]

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;
typedef char char8;
typedef char16_t char16;
typedef char32_t char32;
typedef wchar_t wchar;

typedef wchar CharType;
typedef size_t SizeType;

constexpr SizeType INDEX_NONE = static_cast<SizeType>(-1);

template <typename T>
using SPtr = std::shared_ptr<T>;
template <typename T>
using WPtr = std::weak_ptr<T>;
template <typename T>
using UPtr = std::unique_ptr<T>;