#pragma once

#include "Core/.Package.h"
#include "Core/Array.h"
#include "Core/CString.h"
#include "Core/Hash.h"

class String
{
public:
    using CharArray = Array<CharType>;

public:
    String() = default;
    String(const String &) = default;
    String(String &&) noexcept = default;
    String &operator=(const String &) = default;
    String &operator=(String &&) noexcept = default;
    ~String() = default;

    String(const char8 *src);

    String(const CharType *src)
    {
        if (src && *src)
        {
            int32 len = CString::Length(src);
            if (len > 0)
            {
                data.AddUninitialized(len + 1);
                CString::Copy(data.GetData(), src, len);
                data.GetData()[len] = 0;
            }
        }
    }

    explicit String(const CharType *src, int32 num)
    {
        if (src && *src)
        {
            int32 len = CString::Length(src);
            len = len < num ? len : num;
            if (len > 0)
            {
                data.AddUninitialized(len + 1);
                CString::Copy(data.GetData(), src, len);
                data.GetData()[len] = 0;
            }
        }
    }

    String(CharType value, int32 num = 1)
    {
        if (value)
        {
            if (num > 0)
            {
                data.AddUninitialized(num + 1);
                Memory::UninitializedFill(data.GetData(), num, value);
                data.GetData()[num] = 0;
            }
        }
    }

    String &operator=(const CharType *src)
    {
        if (data.GetData() != src)
        {
            int32 len = (src && *src) ? CString::Length(src) + 1 : 0;
            data.Clear();
            data.AddUninitialized(len);
            if (len > 0)
            {
                CString::Copy(data.GetData(), src, len);
            }
        }
        return *this;
    }

    String &operator=(CharType value)
    {
        String temp(value);
        Swap(temp);
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
        return data.Count() <= 1;
    }

    int32 Length() const
    {
        return data.Count() ? data.Count() - 1 : 0;
    }

    int32 Count() const
    {
        return data.Count() ? data.Count() - 1 : 0;
    }

    int32 Capacity() const
    {
        return data.Capacity();
    }

    const CharType *CStr() const
    {
        constexpr static CharType EMPTY[] = { 0, 0 };
        return data.Count() ? data.GetData() : EMPTY;
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

    void Reverse()
    {
        if (Length() > 0)
        {
            CharType *ptr = data.GetData();
            Memory::Reverse(ptr, ptr + Length() - 1);
        }
    }

    String &Append(CharType value)
    {
        *this += value;
        return *this;
    }

    String &Append(const String &value)
    {
        *this += value;
        return *this;
    }

    void RemoveAt(int32 index)
    {
        data.RemoveAt(index);
    }

    void RemoveAt(int32 index, int32 num)
    {
        data.RemoveAt(index, num);
    }

    void Insert(int32 index, CharType value)
    {
        if (value != 0)
        {
            if (data.Count() == 0)
            {
                *this += value;
            }
            else
            {
                data.Insert(index, value);
            }
        }
    }

    void Insert(int32 index, const String &value)
    {
        if (value.Length())
        {
            if (data.Count() == 0)
            {
                *this += value;
            }
            else
            {
                data.Insert(index, value.data.GetData(), value.Length());
            }
        }
    }

    String Substring(int32 index) const
    {
        return Substring(index, Length());
    }

    String Substring(int32 index, int32 num) const
    {
        CT_CHECK(num >= 0);

        const auto len = Length();
        index = index < len ? index : len;
        return String(CStr() + index, num);
    }

    String Replace(int32 index, int32 num, const String &other) const
    {
        CT_CHECK(num >= 0);
        CheckRange(index);

        String temp(*this);
        return temp.ReplacePrivate(index, num, other.CStr(), other.Length());
    }

    String Replace(int32 index, int32 num, const String &other, int32 otherIndex) const
    {
        CT_CHECK(num >= 0);
        CheckRange(index);
        other.CheckRange(otherIndex);

        String temp(*this);
        return temp.ReplacePrivate(index, num, other.CStr() + otherIndex, other.Length());
    }

    String Replace(int32 index, int32 num, const String &other, int32 otherIndex, int32 otherNum) const
    {
        CT_CHECK(num >= 0 && otherNum >= 0);
        CheckRange(index);
        other.CheckRange(otherIndex);

        String temp(*this);
        return temp.ReplacePrivate(index, num, other.CStr() + otherIndex, otherNum);
    }

    String Replace(int32 index, int32 num, const CharType *otherSrc) const
    {
        CT_CHECK(num >= 0);
        CheckRange(index);

        String temp(*this);
        return temp.ReplacePrivate(index, num, otherSrc, CString::Length(otherSrc));
    }

    String Replace(int32 index, int32 num, const CharType *otherSrc, int32 otherNum) const
    {
        CT_CHECK(num >= 0 && otherNum >= 0);
        CheckRange(index);

        String temp(*this);
        return temp.ReplacePrivate(index, num, otherSrc, otherNum);
    }

    String Replace(int32 index, int32 num, CharType other, int32 otherNum = 1) const
    {
        CT_CHECK(num >= 0 && otherNum >= 0);
        CheckRange(index);

        String temp(*this);
        return temp.Replace(index, num, String(other, otherNum));
    }

    String Replace(const String &oldValue, const String &newValue) const
    {
        CheckNonEmpty(oldValue);

        int32 index = 0;
        if (Find(oldValue, index))
        {
            return Replace(index, oldValue.Length(), newValue);
        }
        return *this;
    }

    String ReplaceAll(const String &oldValue, const String &newValue) const
    {
        CheckNonEmpty(oldValue);

        String temp(*this);
        int32 pos1 = 0;
        int32 pos2 = 0;
        while (temp.Find(oldValue, pos1, pos2))
        {
            temp.ReplacePrivate(pos2, oldValue.Length(), newValue.CStr(), newValue.Length());
            pos1 = pos2 + newValue.Length();
        }
        return temp;
    }

    String TrimStart(CharType value = CT_TEXT(' ')) const
    {
        const int32 len = Length();
        int32 startIndex = 0;

        if (len > 0)
        {
            int32 pos = 0;
            while (Find(value, startIndex, pos))
            {
                if (pos == startIndex && startIndex < len)
                {
                    ++startIndex;
                }
                else
                {
                    break;
                }
            }
        }

        return Substring(startIndex);
    }

    String TrimEnd(CharType value = CT_TEXT(' ')) const
    {
        const int32 len = Length();
        int32 endIndex = len;

        if (len > 0)
        {
            int32 pos = 0;
            while (ReverseFind(value, endIndex - 1, pos))
            {
                if (pos != endIndex - 1)
                {
                    break;
                }
                if (--endIndex == 0)
                {
                    break;
                }
            }
        }

        return Substring(0, endIndex);
    }

    String Trim(CharType value = CT_TEXT(' ')) const
    {
        const int32 len = Length();
        int32 startIndex = 0;
        int32 endIndex = len;

        if (len > 0)
        {
            int32 pos = 0;
            while (Find(value, startIndex, pos))
            {
                if (pos == startIndex && startIndex < len)
                {
                    ++startIndex;
                }
                else
                {
                    break;
                }
            }

            while (ReverseFind(value, endIndex - 1, pos))
            {
                if (pos != endIndex - 1)
                {
                    break;
                }
                if (--endIndex <= startIndex)
                {
                    break;
                }
            }
        }

        return Substring(startIndex, endIndex - startIndex);
    }

    bool Contains(CharType value) const
    {
        return Find(value);
    }

    bool Contains(const CharType *src) const
    {
        return Find(src);
    }

    bool Contains(const CharType *src, int32 count) const
    {
        return Find(src, 0, count, nullptr);
    }

    bool Contains(const String &value) const
    {
        return Find(value);
    }

    int32 IndexOf(CharType value) const
    {
        int32 result = 0;
        if (Find(value, result))
        {
            return result;
        }
        return INDEX_NONE;
    }

    int32 IndexOf(const CharType *src) const
    {
        int32 result = 0;
        if (Find(src, result))
        {
            return result;
        }
        return INDEX_NONE;
    }

    int32 IndexOf(const CharType *src, int32 count) const
    {
        int32 result = 0;
        if (Find(src, 0, count, result))
        {
            return result;
        }
        return INDEX_NONE;
    }

    int32 IndexOf(const String &value) const
    {
        int32 result = 0;
        if (Find(value, result))
        {
            return result;
        }
        return INDEX_NONE;
    }

    int32 LastIndexOf(CharType value) const
    {
        int32 result = 0;
        if (ReverseFind(value, result))
        {
            return result;
        }
        return INDEX_NONE;
    }

    int32 LastIndexOf(const CharType *src) const
    {
        int32 result = 0;
        if (ReverseFind(src, result))
        {
            return result;
        }
        return INDEX_NONE;
    }

    int32 LastIndexOf(const CharType *src, int32 count) const
    {
        int32 result = 0;
        if (ReverseFind(src, 0, count, result))
        {
            return result;
        }
        return INDEX_NONE;
    }

    int32 LastIndexOf(const String &value) const
    {
        int32 result = 0;
        if (ReverseFind(value, result))
        {
            return result;
        }
        return INDEX_NONE;
    }

    bool Find(CharType value) const
    {
        return Find(value, 0, nullptr);
    }

    bool Find(CharType value, int32 &at) const
    {
        return Find(value, 0, &at);
    }

    bool Find(CharType value, int32 startIndex, int32 &at) const
    {
        return Find(value, startIndex, &at);
    }

    bool Find(CharType value, int32 startIndex, int32 *at) const
    {
        if (Length() == 0)
        {
            return false;
        }

        startIndex = startIndex < 0 ? 0 : startIndex;
        const CharType *ptr = CStr();
        for (int32 i = startIndex; i < Length(); ++i)
        {
            if (ptr[i] == value)
            {
                if (at)
                {
                    *at = i;
                }
                return true;
            }
        }
        return false;
    }

    bool Find(const CharType *src) const
    {
        return Find(src, 0, CString::Length(src), nullptr);
    }

    bool Find(const CharType *src, int32 startIndex, int32 *at) const
    {
        return Find(src, startIndex, CString::Length(src), at);
    }

    bool Find(const CharType *src, int32 &at) const
    {
        return Find(src, 0, CString::Length(src), &at);
    }

    bool Find(const CharType *src, int32 startIndex, int32 &at) const
    {
        return Find(src, startIndex, CString::Length(src), &at);
    }

    bool Find(const CharType *src, int32 startIndex, int32 count, int32 &at) const
    {
        return Find(src, startIndex, count, &at);
    }

    bool Find(const CharType *src, int32 startIndex, int32 count, int32 *at) const
    {
        CT_CHECK(count >= 0);

        startIndex = startIndex < 0 ? 0 : startIndex;
        if (startIndex > (Length() > 0 ? Length() - 1 : 0))
        {
            return false;
        }
        if (count == 0)
        {
            if (at)
            {
                *at = startIndex;
            }
            return true;
        }

        const CharType *ptr = CStr();
        for (int32 i = startIndex; i < Length() - count + 1; ++i)
        {
            if (*(ptr + i) == *(src))
            {
                for (int32 j = 1; j < count; ++j)
                {
                    if (*(ptr + i + j) != *(src + j))
                    {
                        goto out;
                    }
                }
                if (at)
                {
                    *at = i;
                }
                return true;
            }
        out:;
        }
        return false;
    }

    bool Find(const String &value) const
    {
        return Find(value, 0, value.Length(), nullptr);
    }

    bool Find(const String &value, int32 startIndex, int32 *at) const
    {
        return Find(value, 0, value.Length(), at);
    }

    bool Find(const String value, int32 &at) const
    {
        return Find(value, 0, value.Length(), &at);
    }

    bool Find(const String &value, int32 startIndex, int32 &at) const
    {
        return Find(value, startIndex, value.Length(), &at);
    }

    bool Find(const String &value, int32 startIndex, int32 count, int32 &at) const
    {
        return Find(value, startIndex, count, &at);
    }

    bool Find(const String &value, int32 startIndex, int32 count, int32 *at) const
    {
        return Find(value.CStr(), startIndex, count, at);
    }

    bool ReverseFind(CharType value, int32 &at) const
    {
        return ReverseFind(value, Length() - 1, &at);
    }

    bool ReverseFind(CharType value, int32 startIndex, int32 &at) const
    {
        return ReverseFind(value, startIndex, &at);
    }

    bool ReverseFind(CharType value, int32 startIndex, int32 *at) const
    {
        if (Length() == 0)
        {
            return false;
        }

        startIndex = startIndex >= Length() ? Length() - 1 : startIndex;
        const CharType *ptr = CStr();
        for (int32 i = startIndex; i >= 0; --i)
        {
            if (ptr[i] == value)
            {
                if (at)
                {
                    *at = i;
                }
                return true;
            }
        }
        return false;
    }

    bool ReverseFind(const CharType *src) const
    {
        return ReverseFind(src, Length() - 1, CString::Length(src), nullptr);
    }

    bool ReverseFind(const CharType *src, int32 startIndex, int32 *at) const
    {
        return ReverseFind(src, Length() - 1, CString::Length(src), at);
    }

    bool ReverseFind(const CharType *src, int32 &at) const
    {
        return ReverseFind(src, Length() - 1, CString::Length(src), &at);
    }

    bool ReverseFind(const CharType *src, int32 startIndex, int32 &at) const
    {
        return ReverseFind(src, startIndex, CString::Length(src), &at);
    }

    bool ReverseFind(const CharType *src, int32 startIndex, int32 count, int32 &at) const
    {
        return ReverseFind(src, startIndex, count, &at);
    }

    bool ReverseFind(const CharType *src, int32 startIndex, int32 count, int32 *at) const
    {
        CT_CHECK(count >= 0);

        startIndex = startIndex >= Length() ? (Length() > 0 ? Length() - 1 : 0) : startIndex;
        if (startIndex < 0)
        {
            return false;
        }
        if (count == 0)
        {
            if (at)
            {
                *at = startIndex;
            }
            return true;
        }

        const CharType *ptr = CStr();
        for (int32 i = startIndex; i >= count; --i)
        {
            if (*(ptr + i) == *(src + count - 1))
            {
                for (int32 j = 1; j < count; ++j)
                {
                    if (*(ptr + i - j) != *(src + count - 1 - j))
                    {
                        goto out;
                    }
                }
                if (at)
                {
                    *at = i - count + 1;
                }
                return true;
            }
        out:;
        }
        return false;
    }

    bool ReverseFind(const String &value) const
    {
        return ReverseFind(value, Length() - 1, value.Length(), nullptr);
    }

    bool ReverseFind(const String &value, int32 startIndex, int32 *at) const
    {
        return ReverseFind(value, Length() - 1, value.Length(), at);
    }

    bool ReverseFind(const String &value, int32 &at) const
    {
        return ReverseFind(value, Length() - 1, value.Length(), &at);
    }

    bool ReverseFind(const String &value, int32 startIndex, int32 &at) const
    {
        return ReverseFind(value, startIndex, value.Length(), &at);
    }

    bool ReverseFind(const String &value, int32 startIndex, int32 count, int32 &at) const
    {
        return ReverseFind(value, startIndex, count, &at);
    }

    bool ReverseFind(const String &value, int32 startIndex, int32 count, int32 *at) const
    {
        return ReverseFind(value.CStr(), startIndex, count, at);
    }

    bool StartsWith(const String &value, int32 offset = 0) const
    {
        int32 pos = 0;
        if (Find(value, offset, pos))
        {
            if (pos == offset)
            {
                return true;
            }
        }
        return false;
    }

    bool EndsWith(const String &value) const
    {
        int32 pos = 0;
        if (ReverseFind(value, pos))
        {
            if (pos + value.Length() == Length())
            {
                return true;
            }
        }
        return false;
    }

    Array<String> Split(const String &delim)
    {
        CheckNonEmpty(delim);

        Array<String> result;
        int32 pos1 = 0;
        int32 pos2 = 0;
        while (Find(delim, pos1, pos2))
        {
            if (pos2 - pos1) // skip more than one delims
            {
                result.Add(Substring(pos1, pos2 - pos1));
            }
            pos1 = pos2 + delim.Length();
        }
        if (pos1 != Length())
        {
            result.Add(Substring(pos1));
        }

        return result;
    }

    int32 Compare(const String &other) const
    {
        return ComparePrivate(CStr(), Length(), other.CStr(), other.Length());
    }

    int32 Compare(const CharType *other) const
    {
        int32 len = CString::Length(other);
        return ComparePrivate(CStr(), Length(), other, len);
    }

    uint32 HashCode() const
    {
        const CharType *ptr = CStr();
        return Hash::HashValue(ptr);
    }

public:
    template <typename... Args>
    static String Format(const String &fmt, Args &&... args);

public:
    String &operator+=(const CharType *src)
    {
        if (src && *src)
        {
            const int32 len = CString::Length(src);
            if (len > 0)
            {
                const auto curCount = data.Count();
                data.AddUninitialized(len + (curCount ? 0 : 1));
                CharType *curPtr = data.GetData() + curCount - (curCount ? 1 : 0);
                CString::Copy(curPtr, src, len);
                *(curPtr + len) = 0;
            }
        }
        return *this;
    }

    String &operator+=(CharType value)
    {
        if (value != 0)
        {
            const auto curCount = data.Count();
            data.AddUninitialized(1 + (curCount ? 0 : 1));
            CharType *curPtr = data.GetData() + curCount - (curCount ? 1 : 0);
            *curPtr = value;
            *(curPtr + 1) = 0;
        }
        return *this;
    }

    String &operator+=(const String &value)
    {
        *this += *value;
        return *this;
    }

    const CharType *operator*() const
    {
        return CStr();
    }

    CharType &operator[](int32 index)
    {
        CheckRange(index);
        return data.GetData()[index];
    }

    const CharType &operator[](int32 index) const
    {
        CheckRange(index);
        return data.GetData()[index];
    }

    friend String operator+(const String &lhs, CharType rhs)
    {
        String result(lhs);
        result += rhs;
        return result;
    }

    friend String operator+(CharType lhs, const String &rhs)
    {
        String result;
        result += lhs;
        result += rhs;
        return result;
    }

    friend String operator+(String &&lhs, CharType rhs)
    {
        String result(std::move(lhs));
        result += rhs;
        return result;
    }

    friend String operator+(CharType lhs, String &&rhs)
    {
        String result;
        result += lhs;
        result += std::move(rhs);
        return result;
    }

    friend String operator+(const String &lhs, const CharType *rhs)
    {
        String result(lhs);
        result += rhs;
        return result;
    }

    friend String operator+(const CharType *lhs, const String &rhs)
    {
        String result(lhs);
        result += rhs;
        return result;
    }

    friend String operator+(String &&lhs, const CharType *rhs)
    {
        String result(std::move(lhs));
        result += rhs;
        return result;
    }

    friend String operator+(const CharType *lhs, String &&rhs)
    {
        String result(lhs);
        result += std::move(rhs);
        return result;
    }

    friend String operator+(const String &lhs, const String &rhs)
    {
        String result(lhs);
        result += rhs;
        return result;
    }

    friend String operator+(const String &lhs, String &&rhs)
    {
        String result(lhs);
        result += std::move(rhs);
        return result;
    }

    friend String operator+(String &&lhs, const String &rhs)
    {
        String result(std::move(lhs));
        result += rhs;
        return result;
    }

    friend String operator+(String &&lhs, String &&rhs)
    {
        String result(std::move(lhs));
        result += std::move(rhs);
        return result;
    }

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
        if (data.Count())
        {
            --ptr;
        }
        return ptr;
    }

    const CharType *end() const
    {
        auto ptr = data.end();
        if (data.Count())
        {
            --ptr;
        }
        return ptr;
    }

private:
    void CheckRange(int32 index) const
    {
        CT_CHECK(index >= 0 && index < Length());
    }

    void CheckNonEmpty(const String &value) const
    {
        CT_CHECK(!value.IsEmpty());
    }

    String &ReplacePrivate(int32 index, int32 num, const CharType *otherSrc, int32 otherNum)
    {
        if (!data.GetData())
        {
            String temp(otherSrc, otherNum);
            Swap(temp);
        }
        else
        {
            const auto len = Length();
            if (index + num > len)
            {
                num = len - index - 1;
            }

            if (num >= otherNum)
            {
                auto ptr = data.GetData() + index;
                CString::Move(ptr + otherNum, ptr + num, len - index - num + 1);
                CString::Copy(ptr, otherSrc, otherNum);
                data.SetCount(data.Count() - num + otherNum);
            }
            else
            {
                data.AddUninitialized(otherNum - num);
                auto ptr = data.GetData() + index;
                CString::Move(ptr + otherNum, ptr + num, len - index - num + 1);
                CString::Copy(ptr, otherSrc, otherNum);
            }
        }

        return *this;
    }

    int32 ComparePrivate(const CharType *ptr1, int32 len1, const CharType *ptr2, int32 len2) const
    {
        int32 len = len1 < len2 ? len1 : len2;
        int32 result = CString::Compare(ptr1, ptr2, len);
        if (result != 0)
        {
            return result;
        }
        if (len1 != len2)
        {
            return len1 < len2 ? -1 : 1;
        }
        return 0;
    }

private:
    CharArray data;
};

#include "Core/String/StringConvert.h"
#include "Core/String/StringEncode.h"
#include "Core/String/StringFormat.h"

CT_INLINE String::String(const char8 *src)
{
    String temp = StringEncode::UTF8::FromChars(src);
    Swap(temp);
}

template <typename... Args>
CT_INLINE String String::Format(const String &fmt, Args &&... args)
{
    return StringFormat::Format(fmt, std::forward<Args>(args)...);
}

namespace std
{
inline void swap(String &lhs, String &rhs)
{
    lhs.Swap(rhs);
}
}