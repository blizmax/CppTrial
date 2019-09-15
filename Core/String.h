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
    typedef Array<CharType> CharArray;

public:
    String() = default;
    String(const String &) = default;
    String(String &&) = default;
    String &operator=(const String &) = default;
    String &operator=(String &&) = default;

    String(const CharType *str)
    {
        if (str && *str)
        {
            size_t len = CharTraits::length(str);
            if (len > 0)
            {
                data.AppendUninitialized(len + 1);
                CharTraits::copy(data.GetData(), str, len);
                data.GetData()[len] = 0;
            }
        }
    }

    String &operator=(const CharType *str)
    {
        if (data.GetData() != str)
        {
            size_t len = (str && *str) ? CharTraits::length(str) + 1 : 0;
            data.Clear();
            data.AppendUninitialized(len);
            if (len > 0)
            {
                CharTraits::copy(data.GetData(), str, len);
            }
        }
        return *this;
    }

    void Clear()
    {
        data.Clear();
    }

    bool IsEmpty() const
    {
        return data.Size() <= 1;
    }

    size_t Length() const
    {
        return data.Size() ? data.Size() - 1 : 0;
    }

    size_t Size() const
    {
        return data.Size() ? data.Size() - 1 : 0;
    }

    CharArray &GetCharArray()
    {
        return data;
    }

    const CharArray &GetCharArray() const
    {
        return data;
    }

    String &operator+=(const CharType *str)
    {
        if (str && *str)
        {
            size_t len = CharTraits::length(str);
            if (len > 0)
            {
                const size_t curSize = data.Size();
                data.AppendUninitialized(len + (curSize ? 0 : 1));
                CharType *curPtr = data.GetData() + curSize - (curSize ? 1 : 0);
                CharTraits::copy(curPtr, str, len);
                *(curPtr + len) = 0;
            }
        }
        return *this;
    }

    String &operator+=(CharType chr)
    {
        if (chr != 0)
        {
            const size_t curSize = data.Size();
            data.AppendUninitialized(1 + (curSize ? 0 : 1));
            CharType *curPtr = data.GetData() + curSize - (curSize ? 1 : 0);
            *curPtr = chr;
            *(curPtr + 1) = 0;
        }
        return *this;
    }

    String &operator+=(const String &str)
    {
        *this += *str;
        return *this;
    }

    const CharType *operator*() const
    {
        static CharType EMPTY[] = {0, 0};
        return data.Size() ? data.GetData() : EMPTY;
    }

    CharType &operator[](size_t index)
    {
        CheckRange(index);
        return data.GetData()[index];
    }

    const CharType &operator[](size_t index) const
    {
        CheckRange(index);
        return data.GetData()[index];
    }

    String &Append(CharType chr)
    {
        *this += chr;
        return *this;
    }

    String &Append(const String &str)
    {
        *this += str;
        return *this;
    }

    void Remove(size_t index)
    {
        data.Remove(index);
    }

    void Remove(size_t index, size_t count)
    {
        data.Remove(index, count);
    }

    void Insert(size_t index, CharType chr)
    {
        if (chr != 0)
        {
            if (data.Size() == 0)
            {
                *this += chr;
            }
            else
            {
                data.Insert(index, chr);
            }
        }
    }

private:
    void CheckRange(size_t index) const
    {
        CT_ASSERT(index >= 0 && index < Size());
    }

public:
    CharArray data;
};

CT_SCOPE_END