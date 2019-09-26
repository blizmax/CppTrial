#pragma once

#include "Core/General.h"
#include "Core/Array.h"
#include <locale>

CT_SCOPE_BEGIN

namespace StringConvert
{
    // convert one unicode
    // return multibyte size, return 0 means convert failed
    CT_INLINE size_t UTF8ToUTF32(const uint8* src, uint32* dst)
    {

    }

    CT_INLINE size_t UTF32ToUTF8(const uint32 *src, uint8 *dst)
    {
    }

    void Example(const uint32* src)
    {
        Array<uint8> arr;
        uint8 buffer[6] = {0};
        size_t bytes = 0;
        size_t pos = 0;
        while (src[pos])
        {
            bytes = UTF32ToUTF8(src + pos, buffer);
            if(bytes == 0)
            {
                break;
            }
            else
            {
                for(size_t i = 0; i < bytes; ++i)
                {
                    arr.Add(buffer[i]); //TODO Add Range
                }
            }
        }
    }
}


CT_SCOPE_END