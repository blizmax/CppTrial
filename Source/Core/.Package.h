#pragma once

#include <cassert>
#include <cstdint>
#include <initializer_list>
#include <typeindex>
#include <functional>

#include "debugbreak.h"

#if defined(_WIN32)
#define CT_PLATFORM_WIN32 1
#elif defined(__APPLE__)
#define CT_PLATFORM_OSX 1
#elif defined(__linux__)
#define CT_PLATFORM_LINUX 1
#else
#error Unsupported platform
#endif

#if CT_PLATFORM_WIN32
#if defined(_MSC_VER)
#define CT_EXPORT __declspec(dllexport)
#define CT_IMPORT __declspec(dllimport)
#else
#define CT_EXPORT __attribute__((dllexport))
#define CT_IMPORT __attribute__((dllimport))
#endif
#else
#define CT_EXPORT __attribute__((visibility("default")))
#define CT_IMPORT __attribute__((visibility("default")))
#endif

#ifdef CT_PRIVATE_EXPORTS
#define CT_PRIVATE_API CT_EXPORT
#else
#define CT_PRIVATE_API CT_IMPORT
#endif

#ifdef _DEBUG
#define CT_DEBUG 1
#endif

#define CT_PROFILE 1

#define CT_DEBUG_BREAK() debug_break()
#define CT_ASSERT(cond) assert(cond)

#if CT_DEBUG
#define CT_CHECK(cond)    \
    if (!(cond))          \
    {                     \
        CT_DEBUG_BREAK(); \
    }
#else
#define CT_CHECK(cond)
#endif

//#define CT_ALIGN(x, a) (((x) + ((a)-1)) & ~((a)-1))
#define CT_ALIGN(x, a) ((((x) + (a)-1) / (a)) * (a))

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
typedef unsigned char uchar8;
typedef char16_t char16;
typedef char32_t char32;
typedef wchar_t wchar;

constexpr int32 INDEX_NONE = -1;

typedef wchar CharType;
typedef size_t SizeType;
typedef uint32 HashType;

using Runnable = std::function<void(void)>;

template <typename T>
CT_INLINE std::type_index TypeIndexOf()
{
    return std::type_index(typeid(T));
}