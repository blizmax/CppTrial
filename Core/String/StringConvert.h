#pragma once

#include "Core/.Package.h"
#include "Core/String.h"

namespace StringConvert
{

CT_INLINE String ToString(int32 value)
{
    std::wstring wstr = std::to_wstring(value);
    return String(wstr.data(), wstr.size());
}

CT_INLINE String ToString(int64 value)
{
    std::wstring wstr = std::to_wstring(value);
    return String(wstr.data(), wstr.size());
}

CT_INLINE String ToString(uint32 value)
{
    std::wstring wstr = std::to_wstring(value);
    return String(wstr.data(), wstr.size());
}

CT_INLINE String ToString(uint64 value)
{
    std::wstring wstr = std::to_wstring(value);
    return String(wstr.data(), wstr.size());
}

CT_INLINE String ToString(float value)
{
    std::wstring wstr = std::to_wstring(value);
    return String(wstr.data(), wstr.size());
}

CT_INLINE String ToString(double value)
{
    std::wstring wstr = std::to_wstring(value);
    return String(wstr.data(), wstr.size());
}

CT_INLINE String ToString(const String &value)
{
    return value;
}

template <typename T>
CT_INLINE String ToString(const T &value)
{
    return value.ToString();
}

template <typename T>
CT_INLINE String ToString(const T *value)
{
    uint64 addr = value ? reinterpret_cast<uint64>(value) : 0;
    return ToString(addr);
}

CT_INLINE bool FastCheckIsIntPrivate(const String &str)
{
    return (str.Length() > 0 && CString::IsDigit(str[0])) ||
           (str.Length() > 1 && str[0] == L'-' && CString::IsDigit(str[1]));
}

CT_INLINE bool FastCheckIsUIntPrivate(const String &str)
{
    return (str.Length() > 0 && CString::IsDigit(str[0]));
}

CT_INLINE bool FastCheckIsFloatPrivate(const String &str)
{
    return (str.Length() > 0 && CString::IsDigit(str[0])) ||
           (str.Length() > 1 && str[0] == L'-' && CString::IsDigit(str[1])) ||
           (str.Length() > 1 && str[0] == L'.' && CString::IsDigit(str[1])) ||
           (str.Length() > 2 && str[0] == L'-' && str[1] == L'.' && CString::IsDigit(str[2]));
}

CT_INLINE bool TryParseInt32(const String &str, int32 &ret)
{
    if (FastCheckIsIntPrivate(str))
    {
        CharType *end;
        ret = CString::ToInt32(str.GetPtr(), end);
        if (end - str.GetPtr() == str.Length())
        {
            return true;
        }
    }
    return false;
}

CT_INLINE bool TryParseUInt32(const String &str, uint32 &ret)
{
    if (FastCheckIsUIntPrivate(str))
    {
        CharType *end;
        ret = CString::ToUInt32(str.GetPtr(), end);
        if (end - str.GetPtr() == str.Length())
        {
            return true;
        }
    }
    return false;
}

CT_INLINE bool TryParseFloat(const String &str, float &ret)
{
    if (FastCheckIsFloatPrivate(str))
    {
        CharType *end;
        ret = CString::ToFloat(str.GetPtr(), end);
        if (end - str.GetPtr() == str.Length())
        {
            return true;
        }
    }
    return false;
}

} // namespace StringConvert