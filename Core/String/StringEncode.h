#pragma once

#include "Core/General.h"
#include "Core/String.h"
//#include <locale>

CT_SCOPE_BEGIN

/*
    Unicode             |  Utf-8
    ---------------------------------------------------------------------------    
    00000000 - 0000007F | 0xxxxxxx
    00000080 - 000007FF | 110xxxxx 10xxxxxx
    00000800 - 0000FFFF | 1110xxxx 10xxxxxx 10xxxxxx
    00010000 - 001FFFFF | 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
    00200000 - 03FFFFFF | 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
    04000000 - 7FFFFFFF | 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 
    */
namespace StringEncode
{
CT_INLINE SizeType UTF8ToUTF32(const char8 *start, const char8 *end, char32 *output)
{
    if (start >= end)
        return 0;

    SizeType byteNum = 0;
    uint8 first = (uint8)*start;

    if (first < 192)
        byteNum = 1;
    else if (first < 224)
        byteNum = 2;
    else if (first < 240)
        byteNum = 3;
    else if (first < 248)
        byteNum = 4;
    else if (first < 252)
        byteNum = 5;
    else
        byteNum = 6;

    if (start + byteNum > end)
        return 0;

    constexpr uint32 OFFSETS[6] = {0x00000000, 0x00003080, 0x000E2080, 0x03C82080, 0xFA082080, 0x82082080};
    char32 temp = 0;
    SizeType pos = 0;

    for (; pos < byteNum - 1; ++pos)
    {
        temp += (uint8)(start[pos]);
        temp <<= 6;
    }
    temp += (uint8)start[pos];
    temp -= OFFSETS[byteNum - 1];
    *output = temp;

    return byteNum;
}

CT_INLINE SizeType UTF32ToUTF8(const char32 *start, char8 *output)
{
    uint32 input = (uint32)*start;
    if (input > 0x0010FFFF)
        return 0;
    if (input >= 0xD800 && input <= 0xDBFF)
        return 0;

    constexpr uint8 HEADERS[7] = {0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC};
    SizeType byteNum = 0;

    if (input < 0x80)
        byteNum = 1;
    else if (input < 0x800)
        byteNum = 2;
    else if (input < 0x10000)
        byteNum = 3;
    else
        byteNum = 4;

    switch (byteNum)
    {
    case 4:
        output[3] = (char8)((input | 0x80) & 0xBF);
        input >>= 6;
    case 3:
        output[2] = (char8)((input | 0x80) & 0xBF);
        input >>= 6;
    case 2:
        output[1] = (char8)((input | 0x80) & 0xBF);
        input >>= 6;
    case 1:
        output[0] = (char8)(input | HEADERS[byteNum]);
    default:
        break;
    }

    return byteNum;
}

CT_INLINE SizeType UTF16ToUTF32(const char16 *start, const char16 *end, char32 *output)
{
    if (start >= end)
        return 0;

    uint32 first = (uint32)*start;
    if (first >= 0xD800 && first <= 0xDBFF)
    {
        if (start + 1 >= end)
            return 0;

        uint32 second = (uint32) * (start + 1);
        if (second >= 0xDC00 && second <= 0xDFFF)
        {
            *output = (char32)(((first - 0xD800) << 10) + (second - 0xDC00) + 0x0010000);
            return 2;
        }
        else
        {
            return 0;
        }
    }

    *output = (char32)first;
    return 1;
}

CT_INLINE SizeType UTF32ToUTF16(const char32 *start, char16 *output)
{
    uint32 input = (uint32)*start;
    if (input > 0x0010FFFF)
        return 0;

    if (input <= 0xFFFF)
    {
        if (input >= 0xD800 && input <= 0xDFFF)
        {
            return 0;
        }
        else
        {
            output[0] = (char16)input;
            return 1;
        }
    }

    input -= 0x0010000;
    output[0] = (char16)((input >> 10) + 0xD800);
    output[1] = (char16)((input & 0x3FF) + 0xDC00);
    return 2;
}

CT_INLINE SizeType WideToUTF32(const wchar *start, const wchar *end, char32 *output)
{
    if (start >= end)
        return 0;

    if (sizeof(wchar) == 4)
    {
        *output = (char32)*start;
        return 1;
    }

    return UTF16ToUTF32((const char16 *)start, (const char16 *)end, output);
}

CT_INLINE SizeType UTF32ToWide(const char32 *start, wchar *output)
{
    if (sizeof(wchar) == 4)
    {
        *output = (wchar)*start;
        return 1;
    }

    char16 buffer[2] = {0};
    SizeType size = UTF32ToUTF16(start, buffer);
    if (size > 0)
        output[0] = (wchar)buffer[0];
    if (size > 1)
        output[1] = (wchar)buffer[1];
    return size;
}

CT_INLINE String FromUTF8(const char8 *cstr)
{
    SizeType len = strlen(cstr);
    SizeType pos = 0;
    SizeType size;
    char32 charUTF32;
    wchar buffer[2] = {0};
    String str;

    while (true)
    {
        size = UTF8ToUTF32(cstr + pos, cstr + len, &charUTF32);
        if (size == 0)
            break;
        pos += size;
        size = UTF32ToWide(&charUTF32, buffer);
        if (size == 0)
            break;
        for (SizeType i = 0; i < size; ++i)
        {
            str += buffer[i];
        }
    }
    return str;
}

CT_INLINE Array<char8> ToUTF8(String &str)
{
    SizeType len = str.Length();
    SizeType pos = 0;
    SizeType size;
    const wchar *cstr = str.GetPtr();
    char32 charUTF32;
    char8 buffer[6] = {0};
    Array<char8> arr;

    while (true)
    {
        size = WideToUTF32(cstr + pos, cstr + len, &charUTF32);
        if (size == 0)
            break;
        pos += size;
        size = UTF32ToUTF8(&charUTF32, buffer);
        if (size == 0)
            break;
        for (SizeType i = 0; i < size; ++i)
        {
            arr.Add(buffer[i]);
        }
    }
    arr.Add(0);
    return arr;
}

} // namespace StringEncode

CT_SCOPE_END