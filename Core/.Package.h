#pragma once

#include <cassert>
#include <cstdint>
#include <initializer_list>

#ifdef _DEBUG
#define CT_DEBUG 1
#endif

#define CT_ALIGN(x, a) (((x) + ((a)-1)) & ~((a)-1))

#define CT_ASSERT(cond) assert(cond)

#ifdef CT_EXCEPTION
#define CT_THROW(msg) throw msg
#else
#define CT_THROW(...) CT_ASSERT(false)
#endif

#define CT_INLINE inline
#define CT_TEXT(text) L##text

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

static constexpr SizeType INDEX_NONE = static_cast<SizeType>(-1);