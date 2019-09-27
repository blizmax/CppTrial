#pragma once

#include "Core/General.h"
#include "Core/Array.h"
#include <locale>

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
namespace StringConvert
{
// Convert one unicode
// return multibyte size, return 0 means convert failed
CT_INLINE size_t UTF8ToUTF32(const uint8 *start, const uint8 *end, uint32 *output)
{
    if (start >= end)
        return 0;

    size_t byteNum = 0;
    uint8 first = *start;

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

    uint32 temp = 0;
    size_t pos = 0;
    for (; pos < byteNum - 1; ++pos)
    {
        temp += start[pos];
        temp <<= 6;
    }
    temp += start[pos];

    constexpr uint32 offsets[6] = {0x00000000, 0x00003080, 0x000E2080, 0x03C82080, 0xFA082080, 0x82082080};
    temp -= offsets[byteNum - 1];
    *output = temp;

    return byteNum;
}

CT_INLINE size_t UTF32ToUTF8(const uint32 *start, const uint32 *end, uint8 *output)
{
    if (start >= end)
        return 0;

    size_t byteNum = 0;
    uint32 input = *start;

    if (input < 0x80)
        byteNum = 1;
    else if (input < 0x800)
        byteNum = 2;
    else if (input < 0x10000)
        byteNum = 3;
    else
        byteNum = 4;

    constexpr uint8 headers[7] = {0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC};

    switch (byteNum)
    {
    case 4:
        output[3] = (uint8)((input | 0x80) & 0xBF);
        input >>= 6;
    case 3:
        output[2] = (uint8)((input | 0x80) & 0xBF);
        input >>= 6;
    case 2:
        output[1] = (uint8)((input | 0x80) & 0xBF);
        input >>= 6;
    case 1:
        output[0] = (uint8)(input | headers[byteNum]);
    default:
        break;
    }

    return byteNum;
}

} // namespace StringConvert

CT_SCOPE_END