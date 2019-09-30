#pragma once

#include "Core/General.h"
#include "Core/Array.h"
#include "Core/CString.h"

CT_SCOPE_BEGIN

class String
{
public:
    using CharType = CString::CharType;
    using CharTraits = CString::CharTraits;
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

    String(CharType chr, size_t count = 1)
    {
        if (chr)
        {
            if (count > 0)
            {
                data.AppendUninitialized(count + 1);
                ThisScope::uninitialized_fill(data.GetData(), count, chr);
                data.GetData()[count] = 0;
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

    void Reverse()
    {
        if (Length() > 0)
        {
            CharType *ptr = data.GetData();
            ThisScope::reverse(ptr, ptr + Length() - 1);
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

    String &Replace(size_t index, size_t count, const String &str, size_t strIndex)
    {
        CheckRange(index);
        str.CheckRange(strIndex);
        return ReplacePrivate(index, count, str.GetPtr() + strIndex, str.Length());
    }

    String &Replace(size_t index, size_t count, const String &str, size_t strIndex, size_t strCount)
    {
        CheckRange(index);
        str.CheckRange(strIndex);
        return ReplacePrivate(index, count, str.GetPtr() + strIndex, strCount);
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

    String &Replace(size_t index, size_t count, CharType chr, size_t chrCount = 1)
    {
        CheckRange(index);
        String temp(chr, chrCount);
        return Replace(index, count, temp);
    }

    String &Replace(const String &oldValue, const String &newValue)
    {
        CheckNonEmpty(oldValue);

        size_t index = 0;
        if (Find(oldValue, index))
        {
            Replace(index, oldValue.Length(), newValue);
        }
        return *this;
    }

    String &ReplaceAll(const String &oldValue, const String &newValue)
    {
        CheckNonEmpty(oldValue);

        size_t pos1 = 0;
        size_t pos2 = 0;
        while (Find(oldValue, pos1, pos2))
        {
            Replace(pos1, oldValue.Length(), newValue);
            pos1 = pos2 + newValue.Length() - oldValue.Length();
        }
        return *this;
    }

    bool Contains(CharType value) const
    {
        return Find(value);
    }

    bool Contains(const CharType *str) const
    {
        return Find(str);
    }

    bool Contains(const CharType *str, size_t count) const
    {
        return Find(str, 0, count, nullptr);
    }

    bool Contains(const String &str) const
    {
        return Find(str);
    }

    size_t IndexOf(CharType value) const
    {
        size_t ret = 0;
        if (Find(value, ret))
        {
            return ret;
        }
        return INDEX_NONE;
    }

    size_t IndexOf(const CharType *str) const
    {
        size_t ret = 0;
        if (Find(str, ret))
        {
            return ret;
        }
        return INDEX_NONE;
    }

    size_t IndexOf(const CharType *str, size_t count) const
    {
        size_t ret = 0;
        if (Find(str, 0, count, ret))
        {
            return ret;
        }
        return INDEX_NONE;
    }

    size_t IndexOf(const String &str) const
    {
        size_t ret = 0;
        if (Find(str, ret))
        {
            return ret;
        }
        return INDEX_NONE;
    }

    size_t LastIndexOf(CharType value) const
    {
        size_t ret = 0;
        if (ReverseFind(value, ret))
        {
            return ret;
        }
        return INDEX_NONE;
    }

    size_t LastIndexOf(const CharType *str) const
    {
        size_t ret = 0;
        if (ReverseFind(str, ret))
        {
            return ret;
        }
        return INDEX_NONE;
    }

    size_t LastIndexOf(const CharType *str, size_t count) const
    {
        size_t ret = 0;
        if (ReverseFind(str, 0, count, ret))
        {
            return ret;
        }
        return INDEX_NONE;
    }

    size_t LastIndexOf(const String &str) const
    {
        size_t ret = 0;
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

    bool Find(CharType value, size_t &at) const
    {
        return Find(value, 0, &at);
    }

    bool Find(CharType value, size_t startIndex, size_t &at) const
    {
        return Find(value, startIndex, &at);
    }

    bool Find(CharType value, size_t startIndex, size_t *at) const
    {
        const CharType *ptr = GetPtr();
        for (size_t i = startIndex; i < Length(); ++i)
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
        const size_t len = CharTraits::length(str);
        return Find(str, 0, len, nullptr);
    }

    bool Find(const CharType *str, size_t startIndex, size_t *at) const
    {
        const size_t len = CharTraits::length(str);
        return Find(str, startIndex, len, at);
    }

    bool Find(const CharType *str, size_t &at) const
    {
        const size_t len = CharTraits::length(str);
        return Find(str, 0, len, &at);
    }

    bool Find(const CharType *str, size_t startIndex, size_t &at) const
    {
        const size_t len = CharTraits::length(str);
        return Find(str, startIndex, len, &at);
    }

    bool Find(const CharType *str, size_t startIndex, size_t count, size_t &at) const
    {
        return Find(str, startIndex, count, &at);
    }

    bool Find(const CharType *str, size_t startIndex, size_t count, size_t *at) const
    {
        const size_t len = Length();
        const CharType *ptr = GetPtr();

        if (count == 0)
        {
            if (at)
            {
                *at = startIndex;
            }
            return true;
        }
        if (len - startIndex < count)
        {
            return false;
        }

        for (size_t i = startIndex; i < len; ++i)
        {
            if (*(ptr + i) == *(str))
            {
                for (size_t j = 1; j < count; ++j)
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

    bool Find(const String &str, size_t startIndex, size_t *at) const
    {
        return Find(str, 0, str.Length(), at);
    }

    bool Find(const String str, size_t &at) const
    {
        return Find(str, 0, str.Length(), &at);
    }

    bool Find(const String &str, size_t startIndex, size_t &at) const
    {
        return Find(str, startIndex, str.Length(), &at);
    }

    bool Find(const String &str, size_t startIndex, size_t count, size_t &at) const
    {
        return Find(str, startIndex, count, &at);
    }

    bool Find(const String &str, size_t startIndex, size_t count, size_t *at) const
    {
        return Find(str.GetPtr(), startIndex, count, at);
    }

    bool ReverseFind(CharType value, size_t &at) const
    {
        return ReverseFind(value, 0, &at);
    }

    bool ReverseFind(CharType value, size_t startIndex, size_t &at) const
    {
        return ReverseFind(value, startIndex, &at);
    }

    bool ReverseFind(CharType value, size_t startIndex, size_t *at) const
    {
        const CharType *ptr = GetPtr();
        startIndex = startIndex >= Length() ? Length() - 1 : startIndex;
        for (size_t i = startIndex + 1; i >= 1;)
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
        const size_t len = CharTraits::length(str);
        return ReverseFind(str, Length() - 1, len, nullptr);
    }

    bool ReverseFind(const CharType *str, size_t startIndex, size_t *at) const
    {
        const size_t len = CharTraits::length(str);
        return ReverseFind(str, Length() - 1, len, at);
    }

    bool ReverseFind(const CharType *str, size_t &at) const
    {
        const size_t len = CharTraits::length(str);
        return ReverseFind(str, Length() - 1, len, &at);
    }

    bool ReverseFind(const CharType *str, size_t startIndex, size_t &at) const
    {
        const size_t len = CharTraits::length(str);
        return ReverseFind(str, startIndex, len, &at);
    }

    bool ReverseFind(const CharType *str, size_t startIndex, size_t count, size_t &at) const
    {
        return ReverseFind(str, startIndex, count, &at);
    }

    bool ReverseFind(const CharType *str, size_t startIndex, size_t count, size_t *at) const
    {
        const size_t len = Length();
        const CharType *ptr = GetPtr();
        startIndex = startIndex >= len ? len - 1 : startIndex;

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

        for (size_t i = startIndex + 1; i >= count;)
        {
            --i;
            if (*(ptr + i) == *(str + count - 1))
            {
                for (size_t j = 1; j < count; ++j)
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

    bool ReverseFind(const String &str, size_t startIndex, size_t *at) const
    {
        return ReverseFind(str, Length() - 1, str.Length(), at);
    }

    bool ReverseFind(const String str, size_t &at) const
    {
        return ReverseFind(str, Length() - 1, str.Length(), &at);
    }

    bool ReverseFind(const String &str, size_t startIndex, size_t &at) const
    {
        return ReverseFind(str, startIndex, str.Length(), &at);
    }

    bool ReverseFind(const String &str, size_t startIndex, size_t count, size_t &at) const
    {
        return ReverseFind(str, startIndex, count, &at);
    }

    bool ReverseFind(const String &str, size_t startIndex, size_t count, size_t *at) const
    {
        return ReverseFind(str.GetPtr(), startIndex, count, at);
    }

    bool StartsWith(const String &str, size_t offset = 0) const
    {
        size_t pos = 0;
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
        size_t pos = 0;
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
        size_t pos1 = 0;
        size_t pos2 = 0;
        while (Find(delim, pos1, pos2))
        {
            if (pos2 - pos1) // skip more than one delims
            {
                arr.Add(SubString(pos1, pos2 - pos1));
            }
            pos1 = pos2 + delim.Length();
        }
        if (pos1 != Length())
        {
            arr.Add(SubString(pos1));
        }

        return std::move(arr);
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

    uint32 HashCode() const
    {
        uint32 seed = 131;
        uint32 hash = 0;
        const CharType *ptr = GetPtr();
        while (*ptr)
        {
            hash = hash * seed + (*ptr++);
        }
        return (hash & 0x7FFFFFFF);
    }

public:
    template <typename... Args>
    static String Format(const String &src, Args &&... args);

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

    void CheckNonEmpty(const String &value) const
    {
        CT_ASSERT(!value.IsEmpty());
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

private:
    CharArray data;
};

template <>
class std::hash<String>
{
public:
    size_t operator()(const String &value) const
    {
        return value.HashCode();
    }
};

#include "Core/String/StringFormat.h"

template <typename... Args>
CT_INLINE String String::Format(const String &src, Args &&... args)
{
    return StringFormat::Format(src, std::forward<Args>(args)...);
}

CT_SCOPE_END