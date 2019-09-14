#pragma once

#include "Core/General.h"
#include "Core/Array.h"
#include <string>

CT_SCOPE_BEGIN

class String
{
public:
    typedef wchar_t CharType;
    typedef std::char_traits<CharType> CharTraits;

public: 
    String() = default;
    String(const String&) = default;
    String(String&&) = default;
    String& operator=(const String&) = default;
    String& operator=(String&&) = default;

    String(const CharType* str)
    {
        if(str && *str)
        {
            size_t len = CharTraits::length(str);
            if(len > 0)
            {
                data.AppendUninitialized(len + 1);
                CharTraits::copy(data.GetData(), str, len);
                data[len] = 0;
            }
        }
    }

public:
    Array<CharType> data;
};

CT_SCOPE_END