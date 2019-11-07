#pragma once

#include "Core/.Package.h"
#include <cwchar>
#include <string>

namespace CString
{

using CharTraits = std::char_traits<CharType>;

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

CT_INLINE uint32 ToUInt32(const CharType *str)
{
    return wcstoul(str, nullptr, 10);
}

CT_INLINE uint32 ToUInt32(const CharType *str, CharType *&end)
{
    return wcstoul(str, &end, 10);
}

CT_INLINE float ToFloat(const CharType *str)
{
    return wcstof(str, nullptr);
}

CT_INLINE float ToFloat(const CharType *str, CharType *&end)
{
    return wcstof(str, &end);
}

} // namespace CString