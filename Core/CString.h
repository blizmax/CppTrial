#pragma once

#include "Core/General.h"
#include <cwchar>
#include <string>

CT_SCOPE_BEGIN

namespace CString
{

typedef wchar CharType;
typedef std::char_traits<CharType> CharTraits;

CT_INLINE bool IsDigit(CharType input)
{
    return input >= L'0' && input <= L'9';
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

CT_SCOPE_END