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

    explicit String(const CharType *str, size_t count)
    {
        if (str && *str)
        {
            size_t len = CharTraits::length(str);
            len = len < count ? len : count;
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

    void Shrink()
    {
        data.Shrink();
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

    size_t Capacity() const
    {
        return data.Capacity();
    }

    const CharType *GetPtr() const
    {
        static CharType EMPTY[] = {0, 0};
        return data.Size() ? data.GetData() : EMPTY;
    }

    CharArray &GetCharArray()
    {
        return data;
    }

    const CharArray &GetCharArray() const
    {
        return data;
    }

    void Swap(String &other) noexcept
    {
        if (this != &other)
        {
            data.Swap(other.data);
        }
    }

    String &Append(const CharType chr)
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

    void Insert(size_t index, const CharType chr)
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

    void Insert(size_t index, const String &str)
    {
        if (str.Length())
        {
            if (data.Size() == 0)
            {
                *this += str;
            }
            else
            {
                data.Insert(index, str.data.GetData(), str.Length());
            }
        }
    }

    String SubString(size_t index)
    {
        return SubString(index, Length());
    }

    String SubString(size_t index, size_t count)
    {
        const size_t len = Length();
        index = index < len ? index : len;
        return String(GetPtr() + index, count);
    }

    String &Replace(size_t index, size_t count, const String &str)
    {
        CheckRange(index);
        return ReplacePrivate(index, count, str.GetPtr(), str.Length());
    }

    String &Replace(size_t index, size_t count, const CharType *str)
    {
        CheckRange(index);
        return ReplacePrivate(index, count, str, CharTraits::length(str));
    }

    String &Replace(size_t index, size_t count, const CharType *str, size_t repCount)
    {
        CheckRange(index);
        return ReplacePrivate(index, count, str, repCount);
    }

    int Compare(const String &other) const
    {
        return ComparePrivate(GetPtr(), Length(), other.GetPtr(), other.Length());
    }

    int Compare(const CharType *other) const
    {
        size_t len = CharTraits::length(other);
        return ComparePrivate(GetPtr(), Length(), other, len);
    }

public:
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

    String &operator+=(const CharType chr)
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
        return GetPtr();
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

    friend String operator+(const String &lhs, const CharType rhs)
    {
        String ret(lhs);
        ret += rhs;
        return ret;
    }

    friend String operator+(const CharType lhs, const String &rhs)
    {
        String ret;
        ret += lhs;
        ret += rhs;
        return ret;
    }

    friend String operator+(String &&lhs, const CharType rhs)
    {
        String ret(std::move(lhs));
        ret += rhs;
        return ret;
    }

    friend String operator+(const CharType lhs, String &&rhs)
    {
        String ret;
        ret += lhs;
        ret += std::move(rhs);
        return ret;
    }

    friend String operator+(const String &lhs, const CharType *rhs)
    {
        String ret(lhs);
        ret += rhs;
        return ret;
    }

    friend String operator+(const CharType *lhs, const String &rhs)
    {
        String ret(lhs);
        ret += rhs;
        return ret;
    }

    friend String operator+(String &&lhs, const CharType *rhs)
    {
        String ret(std::move(lhs));
        ret += rhs;
        return ret;
    }

    friend String operator+(const CharType *lhs, String &&rhs)
    {
        String ret(lhs);
        ret += std::move(rhs);
        return ret;
    }

    friend String operator+(const String &lhs, const String &rhs)
    {
        String ret(lhs);
        ret += rhs;
        return ret;
    }

    friend String operator+(const String &lhs, String &&rhs)
    {
        String ret(lhs);
        ret += std::move(rhs);
        return ret;
    }

    friend String operator+(String &&lhs, const String &rhs)
    {
        String ret(std::move(lhs));
        ret += rhs;
        return ret;
    }

    friend String operator+(String &&lhs, String &&rhs)
    {
        String ret(std::move(lhs));
        ret += std::move(rhs);
        return ret;
    }

    // op <=
    friend bool operator<=(const String &lhs, const String &rhs)
    {
        return lhs.Compare(rhs) <= 0;
    }

    friend bool operator<=(const String &lhs, const CharType *rhs)
    {
        return lhs.Compare(rhs) <= 0;
    }

    friend bool operator<=(const CharType *lhs, const String &rhs)
    {
        return rhs.Compare(lhs) > 0;
    }

    // op <
    friend bool operator<(const String &lhs, const String &rhs)
    {
        return lhs.Compare(rhs) < 0;
    }

    friend bool operator<(const String &lhs, const CharType *rhs)
    {
        return lhs.Compare(rhs) < 0;
    }

    friend bool operator<(const CharType *lhs, const String &rhs)
    {
        return rhs.Compare(lhs) >= 0;
    }

    // op >=
    friend bool operator>=(const String &lhs, const String &rhs)
    {
        return lhs.Compare(rhs) >= 0;
    }

    friend bool operator>=(const String &lhs, const CharType *rhs)
    {
        return lhs.Compare(rhs) >= 0;
    }

    friend bool operator>=(const CharType *lhs, const String &rhs)
    {
        return rhs.Compare(lhs) < 0;
    }

    // op >
    friend bool operator>(const String &lhs, const String &rhs)
    {
        return lhs.Compare(rhs) > 0;
    }

    friend bool operator>(const String &lhs, const CharType *rhs)
    {
        return lhs.Compare(rhs) > 0;
    }

    friend bool operator>(const CharType *lhs, const String &rhs)
    {
        return rhs.Compare(lhs) <= 0;
    }

    // op ==
    friend bool operator==(const String &lhs, const String &rhs)
    {
        return lhs.Compare(rhs) == 0;
    }

    friend bool operator==(const String &lhs, const CharType *rhs)
    {
        return lhs.Compare(rhs) == 0;
    }

    friend bool operator==(const CharType *lhs, const String &rhs)
    {
        return rhs.Compare(lhs) == 0;
    }

    // op !=
    friend bool operator!=(const String &lhs, const String &rhs)
    {
        return lhs.Compare(rhs) != 0;
    }

    friend bool operator!=(const String &lhs, const CharType *rhs)
    {
        return lhs.Compare(rhs) != 0;
    }

    friend bool operator!=(const CharType *lhs, const String &rhs)
    {
        return rhs.Compare(lhs) != 0;
    }

    //===================== STL STYLE =========================
public:
    CharType *begin()
    {
        return data.begin();
    }

    const CharType *begin() const
    {
        return data.begin();
    }

    CharType *end()
    {
        auto ptr = data.end();
        if (data.Size())
        {
            --ptr;
        }
        return ptr;
    }

    const CharType *end() const
    {
        auto ptr = data.end();
        if (data.Size())
        {
            --ptr;
        }
        return ptr;
    }

private:
    void CheckRange(size_t index) const
    {
        CT_ASSERT(index >= 0 && index < Length());
    }

    String &ReplacePrivate(size_t index, size_t count, const CharType *repStr, size_t repCount)
    {
        CharType *ptr = data.GetData();

        if (!ptr)
        {
            String temp(repStr, repCount);
            Swap(temp);
        }
        else
        {
            const size_t len = Length();
            ptr += index;

            if (index + count > len)
            {
                count = len - index;
            }

            if (count >= repCount)
            {
                CharTraits::move(ptr + repCount, ptr + count, len - index - count + 1);
                CharTraits::copy(ptr, repStr, repCount);
                data.Resize(data.Size() - count + repCount);
            }
            else
            {
                data.AppendUninitialized(repCount - count);
                CharTraits::move(ptr + repCount, ptr + count, len - index - count + 1);
                CharTraits::copy(ptr, repStr, repCount);
            }
        }

        return *this;
    }

    int ComparePrivate(const CharType *ptr1, size_t len1, const CharType *ptr2, size_t len2) const
    {
        size_t len = len1 < len2 ? len1 : len2;
        int ret = CharTraits::compare(ptr1, ptr2, len);
        if (ret != 0)
        {
            return ret;
        }
        if (len1 != len2)
        {
            return len1 < len2 ? -1 : 1;
        }
        return 0;
    }

public:
    CharArray data;
};

CT_SCOPE_END