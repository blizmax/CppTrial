#pragma once

#include "Core/.Package.h"
#include "Core/Template.h"
#include <cwchar>
#include <string>

namespace CString
{

template <typename Char>
CT_INLINE SizeType Length(const Char *str)
{
    if constexpr (TIsSame<Char, char8>::value)
    {
        return std::strlen(str);
    }
    else if constexpr (TIsSame<Char, wchar>::value)
    {
        return std::wcslen(str);
    }
    else
    {
        SizeType len = 0;
        for (; *str != static_cast<Char>(0); ++str)
            ++len;
        return len;
    }
}

template <typename Char>
CT_INLINE int32 Compare(const Char *str1, const Char *str2, SizeType count)
{
    if constexpr (TIsSame<Char, char8>::value)
    {
        return std::memcmp(str1, str2, count);
    }
    else if constexpr (TIsSame<Char, wchar>::value)
    {
        return std::wmemcmp(str1, str2, count);
    }
    else
    {
        for (; count > 0; --count, ++str1, ++str2)
        {
            if (*str1 < *str2)
                return -1;
            if (*str2 < *str1)
                return 1;
        }
        return 0;
    }
}

template <typename Char>
CT_INLINE void Copy(Char *dst, const Char *src, SizeType count)
{
    if constexpr (TIsSame<Char, char8>::value)
    {
        std::memcpy(dst, src, count);
    }
    else if constexpr (TIsSame<Char, wchar>::value)
    {
        std::wmemcpy(dst, src, count);
    }
    else
    {
        std::memcpy(dst, src, count * sizeof(Char));
    }
}

template <typename Char>
CT_INLINE void Move(Char *dst, const Char *src, SizeType count)
{
    if constexpr (TIsSame<Char, char8>::value)
    {
        std::memmove(dst, src, count);
    }
    else if constexpr (TIsSame<Char, wchar>::value)
    {
        std::wmemmove(dst, src, count);
    }
    else
    {
        std::memmove(dst, src, count * sizeof(Char));
    }
}

CT_INLINE bool IsDigit(CharType input)
{
    return input >= CT_TEXT('0') && input <= CT_TEXT('9');
}

CT_INLINE int32 ToInt32(const CharType *str)
{
    return wcstol(str, nullptr, 10);
}

CT_INLINE int32 ToInt32(const CharType *str, CharType *&end)
{
    return wcstol(str, &end, 10);
}

CT_INLINE int64 ToInt64(const CharType *str)
{
    return wcstoll(str, nullptr, 10);
}

CT_INLINE int64 ToInt64(const CharType *str, CharType *&end)
{
    return wcstoll(str, &end, 10);
}

CT_INLINE uint32 ToUInt32(const CharType *str)
{
    return wcstoul(str, nullptr, 10);
}

CT_INLINE uint32 ToUInt32(const CharType *str, CharType *&end)
{
    return wcstoul(str, &end, 10);
}

CT_INLINE uint64 ToUInt64(const CharType *str)
{
    return wcstoull(str, nullptr, 10);
}

CT_INLINE uint64 ToUInt64(const CharType *str, CharType *&end)
{
    return wcstoull(str, &end, 10);
}

CT_INLINE float ToFloat(const CharType *str)
{
    return wcstof(str, nullptr);
}

CT_INLINE float ToFloat(const CharType *str, CharType *&end)
{
    return wcstof(str, &end);
}

CT_INLINE double ToDouble(const CharType *str)
{
    return wcstod(str, nullptr);
}

CT_INLINE double ToDouble(const CharType *str, CharType *&end)
{
    return wcstod(str, &end);
}

} // namespace CString