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
    String(String &&) = default;
    String &operator=(const String &) = default;
    String &operator=(String &&) = default;
    ~String() = default;

    String(const char8 *str);

    String(const CharType *str)
    {
        if (str && *str)
        {
            SizeType len = CString::Length(str);
            if (len > 0)
            {
                data.AppendUninitialized(len + 1);
                CString::Copy(data.GetData(), str, len);
                data.GetData()[len] = 0;
            }
        }
    }

    explicit String(const CharType *str, SizeType count)
    {
        if (str && *str)
        {
            SizeType len = CString::Length(str);
            len = len < count ? len : count;
            if (len > 0)
            {
                data.AppendUninitialized(len + 1);
                CString::Copy(data.GetData(), str, len);
                data.GetData()[len] = 0;
            }
        }
    }

    String(CharType chr, SizeType count = 1)
    {
        if (chr)
        {
            if (count > 0)
            {
                data.AppendUninitialized(count + 1);
                Memory::UninitializedFill(data.GetData(), count, chr);
                data.GetData()[count] = 0;
            }
        }
    }

    String &operator=(const CharType *str)
    {
        if (data.GetData() != str)
        {
            SizeType len = (str && *str) ? CString::Length(str) + 1 : 0;
            data.Clear();
            data.AppendUninitialized(len);
            if (len > 0)
            {
                CString::Copy(data.GetData(), str, len);
            }
        }
        return *this;
    }

    String &operator=(CharType chr)
    {
        String temp(chr);
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
        return data.Size() <= 1;
    }

    SizeType Length() const
    {
        return data.Size() ? data.Size() - 1 : 0;
    }

    SizeType Size() const
    {
        return data.Size() ? data.Size() - 1 : 0;
    }

    SizeType Capacity() const
    {
        return data.Capacity();
    }

    const CharType *CStr() const
    {
        constexpr static CharType EMPTY[] = {0, 0};
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

    void Reverse()
    {
        if (Length() > 0)
        {
            CharType *ptr = data.GetData();
            Memory::Reverse(ptr, ptr + Length() - 1);
        }
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

    void Remove(SizeType index)
    {
        data.Remove(index);
    }

    void Remove(SizeType index, SizeType count)
    {
        data.Remove(index, count);
    }

    void Insert(SizeType index, CharType chr)
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

    void Insert(SizeType index, const String &str)
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

    String Substring(SizeType index) const
    {
        return Substring(index, Length());
    }

    String Substring(SizeType index, SizeType count) const
    {
        const SizeType len = Length();
        index = index < len ? index : len;
        return String(CStr() + index, count);
    }

    String Replace(SizeType index, SizeType count, const String &str) const
    {
        CheckRange(index);

        String temp(*this);
        return temp.ReplacePrivate(index, count, str.CStr(), str.Length());
    }

    String Replace(SizeType index, SizeType count, const String &str, SizeType strIndex) const
    {
        CheckRange(index);
        str.CheckRange(strIndex);

        String temp(*this);
        return temp.ReplacePrivate(index, count, str.CStr() + strIndex, str.Length());
    }

    String Replace(SizeType index, SizeType count, const String &str, SizeType strIndex, SizeType strCount) const
    {
        CheckRange(index);
        str.CheckRange(strIndex);

        String temp(*this);
        return temp.ReplacePrivate(index, count, str.CStr() + strIndex, strCount);
    }

    String Replace(SizeType index, SizeType count, const CharType *str) const
    {
        CheckRange(index);

        String temp(*this);
        return temp.ReplacePrivate(index, count, str, CString::Length(str));
    }

    String Replace(SizeType index, SizeType count, const CharType *str, SizeType repCount) const
    {
        CheckRange(index);

        String temp(*this);
        return temp.ReplacePrivate(index, count, str, repCount);
    }

    String Replace(SizeType index, SizeType count, CharType chr, SizeType chrCount = 1) const
    {
        CheckRange(index);

        String temp(*this);
        return temp.Replace(index, count, String(chr, chrCount));
    }

    String Replace(const String &oldValue, const String &newValue) const
    {
        CheckNonEmpty(oldValue);

        SizeType index = 0;
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
        SizeType pos1 = 0;
        SizeType pos2 = 0;
        while (temp.Find(oldValue, pos1, pos2))
        {
            temp.ReplacePrivate(pos2, oldValue.Length(), newValue.CStr(), newValue.Length());
            pos1 = pos2 + newValue.Length();
        }
        return temp;
    }

    String TrimStart(CharType value = CT_TEXT(' ')) const
    {
        const SizeType len = Length();
        SizeType startIndex = 0;

        if (len > 0)
        {
            SizeType pos = 0;
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
        const SizeType len = Length();
        SizeType endIndex = len;

        if (len > 0)
        {
            SizeType pos = 0;
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
        const SizeType len = Length();
        SizeType startIndex = 0;
        SizeType endIndex = len;

        if (len > 0)
        {
            SizeType pos = 0;
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

    bool Contains(const CharType *str) const
    {
        return Find(str);
    }

    bool Contains(const CharType *str, SizeType count) const
    {
        return Find(str, 0, count, nullptr);
    }

    bool Contains(const String &str) const
    {
        return Find(str);
    }

    SizeType IndexOf(CharType value) const
    {
        SizeType ret = 0;
        if (Find(value, ret))
        {
            return ret;
        }
        return INDEX_NONE;
    }

    SizeType IndexOf(const CharType *str) const
    {
        SizeType ret = 0;
        if (Find(str, ret))
        {
            return ret;
        }
        return INDEX_NONE;
    }

    SizeType IndexOf(const CharType *str, SizeType count) const
    {
        SizeType ret = 0;
        if (Find(str, 0, count, ret))
        {
            return ret;
        }
        return INDEX_NONE;
    }

    SizeType IndexOf(const String &str) const
    {
        SizeType ret = 0;
        if (Find(str, ret))
        {
            return ret;
        }
        return INDEX_NONE;
    }

    SizeType LastIndexOf(CharType value) const
    {
        SizeType ret = 0;
        if (ReverseFind(value, ret))
        {
            return ret;
        }
        return INDEX_NONE;
    }

    SizeType LastIndexOf(const CharType *str) const
    {
        SizeType ret = 0;
        if (ReverseFind(str, ret))
        {
            return ret;
        }
        return INDEX_NONE;
    }

    SizeType LastIndexOf(const CharType *str, SizeType count) const
    {
        SizeType ret = 0;
        if (ReverseFind(str, 0, count, ret))
        {
            return ret;
        }
        return INDEX_NONE;
    }

    SizeType LastIndexOf(const String &str) const
    {
        SizeType ret = 0;
        if (ReverseFind(str, ret))
        {
            return ret;
        }
        return INDEX_NONE;
    }

    bool Find(CharType value) const
    {
        return Find(value, 0, nullptr);
    }

    bool Find(CharType value, SizeType &at) const
    {
        return Find(value, 0, &at);
    }

    bool Find(CharType value, SizeType startIndex, SizeType &at) const
    {
        return Find(value, startIndex, &at);
    }

    bool Find(CharType value, SizeType startIndex, SizeType *at) const
    {
        const CharType *ptr = CStr();
        for (SizeType i = startIndex; i < Length(); ++i)
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

    bool Find(const CharType *str) const
    {
        const SizeType len = CString::Length(str);
        return Find(str, 0, len, nullptr);
    }

    bool Find(const CharType *str, SizeType startIndex, SizeType *at) const
    {
        const SizeType len = CString::Length(str);
        return Find(str, startIndex, len, at);
    }

    bool Find(const CharType *str, SizeType &at) const
    {
        const SizeType len = CString::Length(str);
        return Find(str, 0, len, &at);
    }

    bool Find(const CharType *str, SizeType startIndex, SizeType &at) const
    {
        const SizeType len = CString::Length(str);
        return Find(str, startIndex, len, &at);
    }

    bool Find(const CharType *str, SizeType startIndex, SizeType count, SizeType &at) const
    {
        return Find(str, startIndex, count, &at);
    }

    bool Find(const CharType *str, SizeType startIndex, SizeType count, SizeType *at) const
    {
        const SizeType len = Length();
        const CharType *ptr = CStr();

        if (count == 0)
        {
            if (at)
            {
                *at = startIndex;
            }
            return true;
        }
        if (startIndex + count > len)
        {
            return false;
        }

        for (SizeType i = startIndex; i < len; ++i)
        {
            if (*(ptr + i) == *(str))
            {
                for (SizeType j = 1; j < count; ++j)
                {
                    if (*(ptr + i + j) != *(str + j))
                    {
                        return false;
                    }
                }
                if (at)
                {
                    *at = i;
                }
                return true;
            }
        }
        return false;
    }

    bool Find(const String &str) const
    {
        return Find(str, 0, str.Length(), nullptr);
    }

    bool Find(const String &str, SizeType startIndex, SizeType *at) const
    {
        return Find(str, 0, str.Length(), at);
    }

    bool Find(const String str, SizeType &at) const
    {
        return Find(str, 0, str.Length(), &at);
    }

    bool Find(const String &str, SizeType startIndex, SizeType &at) const
    {
        return Find(str, startIndex, str.Length(), &at);
    }

    bool Find(const String &str, SizeType startIndex, SizeType count, SizeType &at) const
    {
        return Find(str, startIndex, count, &at);
    }

    bool Find(const String &str, SizeType startIndex, SizeType count, SizeType *at) const
    {
        return Find(str.CStr(), startIndex, count, at);
    }

    bool ReverseFind(CharType value, SizeType &at) const
    {
        return ReverseFind(value, Length() - 1, &at);
    }

    bool ReverseFind(CharType value, SizeType startIndex, SizeType &at) const
    {
        return ReverseFind(value, startIndex, &at);
    }

    bool ReverseFind(CharType value, SizeType startIndex, SizeType *at) const
    {
        if (Length() == 0)
        {
            return false;
        }

        const CharType *ptr = CStr();
        startIndex = startIndex >= Length() ? Length() - 1 : startIndex;
        for (SizeType i = startIndex + 1; i >= 1;)
        {
            if (ptr[--i] == value)
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

    bool ReverseFind(const CharType *str) const
    {
        const SizeType len = CString::Length(str);
        return ReverseFind(str, Length() - 1, len, nullptr);
    }

    bool ReverseFind(const CharType *str, SizeType startIndex, SizeType *at) const
    {
        const SizeType len = CString::Length(str);
        return ReverseFind(str, Length() - 1, len, at);
    }

    bool ReverseFind(const CharType *str, SizeType &at) const
    {
        const SizeType len = CString::Length(str);
        return ReverseFind(str, Length() - 1, len, &at);
    }

    bool ReverseFind(const CharType *str, SizeType startIndex, SizeType &at) const
    {
        const SizeType len = CString::Length(str);
        return ReverseFind(str, startIndex, len, &at);
    }

    bool ReverseFind(const CharType *str, SizeType startIndex, SizeType count, SizeType &at) const
    {
        return ReverseFind(str, startIndex, count, &at);
    }

    bool ReverseFind(const CharType *str, SizeType startIndex, SizeType count, SizeType *at) const
    {
        if (Length() == 0)
        {
            return false;
        }

        const CharType *ptr = CStr();
        startIndex = startIndex >= Length() ? Length() - 1 : startIndex;

        if (count == 0)
        {
            if (at)
            {
                *at = startIndex;
            }
            return true;
        }
        if (startIndex < count - 1)
        {
            return false;
        }

        for (SizeType i = startIndex + 1; i >= count;)
        {
            --i;
            if (*(ptr + i) == *(str + count - 1))
            {
                for (SizeType j = 1; j < count; ++j)
                {
                    if (*(ptr + i - j) != *(str + count - 1 - j))
                    {
                        return false;
                    }
                }
                if (at)
                {
                    *at = i - count + 1;
                }
                return true;
            }
        }
        return false;
    }

    bool ReverseFind(const String &str) const
    {
        return ReverseFind(str, Length() - 1, str.Length(), nullptr);
    }

    bool ReverseFind(const String &str, SizeType startIndex, SizeType *at) const
    {
        return ReverseFind(str, Length() - 1, str.Length(), at);
    }

    bool ReverseFind(const String str, SizeType &at) const
    {
        return ReverseFind(str, Length() - 1, str.Length(), &at);
    }

    bool ReverseFind(const String &str, SizeType startIndex, SizeType &at) const
    {
        return ReverseFind(str, startIndex, str.Length(), &at);
    }

    bool ReverseFind(const String &str, SizeType startIndex, SizeType count, SizeType &at) const
    {
        return ReverseFind(str, startIndex, count, &at);
    }

    bool ReverseFind(const String &str, SizeType startIndex, SizeType count, SizeType *at) const
    {
        return ReverseFind(str.CStr(), startIndex, count, at);
    }

    bool StartsWith(const String &str, SizeType offset = 0) const
    {
        SizeType pos = 0;
        if (Find(str, offset, pos))
        {
            if (pos == offset)
            {
                return true;
            }
        }
        return false;
    }

    bool EndsWith(const String &str) const
    {
        SizeType pos = 0;
        if (ReverseFind(str, pos))
        {
            if (pos + str.Length() == Length())
            {
                return true;
            }
        }
        return false;
    }

    Array<String> Split(const String &delim)
    {
        CheckNonEmpty(delim);

        Array<String> arr;
        SizeType pos1 = 0;
        SizeType pos2 = 0;
        while (Find(delim, pos1, pos2))
        {
            if (pos2 - pos1) // skip more than one delims
            {
                arr.Add(Substring(pos1, pos2 - pos1));
            }
            pos1 = pos2 + delim.Length();
        }
        if (pos1 != Length())
        {
            arr.Add(Substring(pos1));
        }

        return arr;
    }

    int32 Compare(const String &other) const
    {
        return ComparePrivate(CStr(), Length(), other.CStr(), other.Length());
    }

    int32 Compare(const CharType *other) const
    {
        SizeType len = CString::Length(other);
        return ComparePrivate(CStr(), Length(), other, len);
    }

    uint32 HashCode() const
    {
        const CharType *ptr = CStr();
        return Hash::HashValue(ptr);
    }

public:
    template <typename... Args>
    static String Format(const String &src, Args &&... args);

public:
    String &operator+=(const CharType *str)
    {
        if (str && *str)
        {
            SizeType len = CString::Length(str);
            if (len > 0)
            {
                const SizeType curSize = data.Size();
                data.AppendUninitialized(len + (curSize ? 0 : 1));
                CharType *curPtr = data.GetData() + curSize - (curSize ? 1 : 0);
                CString::Copy(curPtr, str, len);
                *(curPtr + len) = 0;
            }
        }
        return *this;
    }

    String &operator+=(CharType chr)
    {
        if (chr != 0)
        {
            const SizeType curSize = data.Size();
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
        return CStr();
    }

    CharType &operator[](SizeType index)
    {
        CheckRange(index);
        return data.GetData()[index];
    }

    const CharType &operator[](SizeType index) const
    {
        CheckRange(index);
        return data.GetData()[index];
    }

    friend String operator+(const String &lhs, CharType rhs)
    {
        String ret(lhs);
        ret += rhs;
        return ret;
    }

    friend String operator+(CharType lhs, const String &rhs)
    {
        String ret;
        ret += lhs;
        ret += rhs;
        return ret;
    }

    friend String operator+(String &&lhs, CharType rhs)
    {
        String ret(std::move(lhs));
        ret += rhs;
        return ret;
    }

    friend String operator+(CharType lhs, String &&rhs)
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
    void CheckRange(SizeType index) const
    {
        CT_ASSERT(index >= 0 && index < Length());
    }

    void CheckNonEmpty(const String &value) const
    {
        CT_ASSERT(!value.IsEmpty());
    }

    String &ReplacePrivate(SizeType index, SizeType count, const CharType *repStr, SizeType repCount)
    {
        if (!data.GetData())
        {
            String temp(repStr, repCount);
            Swap(temp);
        }
        else
        {
            const SizeType len = Length();
            if (index + count > len)
            {
                count = len - index - 1;
            }

            if (count >= repCount)
            {
                auto ptr = data.GetData() + index;
                CString::Move(ptr + repCount, ptr + count, len - index - count + 1);
                CString::Copy(ptr, repStr, repCount);
                data.Resize(data.Size() - count + repCount);
            }
            else
            {
                data.AppendUninitialized(repCount - count);
                auto ptr = data.GetData() + index;
                CString::Move(ptr + repCount, ptr + count, len - index - count + 1);
                CString::Copy(ptr, repStr, repCount);
            }
        }

        return *this;
    }

    int32 ComparePrivate(const CharType *ptr1, SizeType len1, const CharType *ptr2, SizeType len2) const
    {
        SizeType len = len1 < len2 ? len1 : len2;
        int32 ret = CString::Compare(ptr1, ptr2, len);
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

private:
    CharArray data;
};

#include "Core/String/StringConvert.h"
#include "Core/String/StringFormat.h"
#include "Core/String/StringEncode.h"

CT_INLINE String::String(const char8 *str)
{
    String temp = StringEncode::FromUTF8(str);
    Swap(temp);
}

template <typename... Args>
CT_INLINE String String::Format(const String &src, Args &&... args)
{
    return StringFormat::Format(src, std::forward<Args>(args)...);
}

namespace std
{
inline void swap(String &lhs, String &rhs)
{
    lhs.Swap(rhs);
}
} // namespace std