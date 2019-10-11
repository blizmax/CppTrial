#pragma once

#include "Core/General.h"
#include "Core/Array.h"

CT_SCOPE_BEGIN

enum class VariantType
{
    Empty = 0,
    Bool,
    Int8,
    Int16,
    Int32,
    Int64,
    UInt8,
    UInt16,
    UInt32,
    UInt64,
    Char,
    WChar,
    Float,
    Double,
    String,
};

namespace VariantInternal
{

template <typename T>
struct VariantTypeTraits : public TFalseType
{

};

template <>
struct VariantTypeTraits<bool> : public TTrueType
{
    static VariantType GetType()
    {
        return VariantType::Bool;
    }
};

template <>
struct VariantTypeTraits<int8> : public TTrueType
{
    static VariantType GetType()
    {
        return VariantType::Int8;
    }
};

template <>
struct VariantTypeTraits<int16> : public TTrueType
{
    static VariantType GetType()
    {
        return VariantType::Int16;
    }
};

template <>
struct VariantTypeTraits<int32> : public TTrueType
{
    static VariantType GetType()
    {
        return VariantType::Int32;
    }
};

template <>
struct VariantTypeTraits<int64> : public TTrueType
{
    static VariantType GetType()
    {
        return VariantType::Int64;
    }
};

template <>
struct VariantTypeTraits<uint8> : public TTrueType
{
    static VariantType GetType()
    {
        return VariantType::UInt8;
    }
};

template <>
struct VariantTypeTraits<uint16> : public TTrueType
{
    static VariantType GetType()
    {
        return VariantType::UInt16;
    }
};

template <>
struct VariantTypeTraits<uint32> : public TTrueType
{
    static VariantType GetType()
    {
        return VariantType::UInt32;
    }
};

template <>
struct VariantTypeTraits<uint64> : public TTrueType
{
    static VariantType GetType()
    {
        return VariantType::UInt64;
    }
};

template <>
struct VariantTypeTraits<char8> : public TTrueType
{
    static VariantType GetType()
    {
        return VariantType::Char;
    }
};

template <>
struct VariantTypeTraits<wchar> : public TTrueType
{
    static VariantType GetType()
    {
        return VariantType::WChar;
    }
};

template <>
struct VariantTypeTraits<float> : public TTrueType
{
    static VariantType GetType()
    {
        return VariantType::Float;
    }
};

template <>
struct VariantTypeTraits<double> : public TTrueType
{
    static VariantType GetType()
    {
        return VariantType::Double;
    }
};

template <>
struct VariantTypeTraits<String> : public TTrueType
{
    static VariantType GetType()
    {
        return VariantType::String;
    }
};

template <typename T>
CT_INLINE void WriteBytes(Array<uint8> &bytes, const T &value)
{
    bytes.AppendUninitialized(sizeof(T));
    std::memcpy(bytes.GetData(), &value, sizeof(T));
}

CT_INLINE void WriteBytes(Array<uint8> &bytes, const String &value)
{
    auto strArr = value.GetCharArray();
    SizeType byteNum = sizeof(CharType) * strArr.Size();
    bytes.AppendUninitialized(byteNum);
    std::memcpy(bytes.GetData(), strArr.GetData(), byteNum);
}

template <typename T>
CT_INLINE void ReadBytes(const Array<uint8> &bytes, T &value)
{
    std::memcpy(&value, bytes.GetData(), sizeof(T));
}

CT_INLINE void ReadBytes(const Array<uint8> &bytes, String &value)
{
    SizeType charNum = bytes.Size() / sizeof(CharType);
    Array<CharType> temp;
    temp.AppendUninitialized(charNum);
    std::memcpy(temp.GetData(), bytes.GetData(), bytes.Size());
    value = temp.GetData();
}

} // namespace VariantInternal


template <typename T>
struct TIsVariantType : public TConditional<VariantInternal::VariantTypeTraits<T>::value, TTrueType, TFalseType>::type
{

};


class Variant
{
public:
    Variant() = default;
    Variant(const Variant &) = default;
    Variant &operator=(const Variant &) = default;
    ~Variant() = default;

    Variant(Variant &&other) : type(other.type), data(std::move(other.data))
    {
        other.type = VariantType::Empty;
    }

    Variant &operator=(Variant &&other)
    {
        if (this != &other)
        {
            type = other.type;
            data = std::move(other.data);
            other.type = VariantType::Empty;
        }
        return *this;
    }

    template <typename T>
    Variant(T value)
    {
        VariantInternal::WriteBytes(data, value);
        type = VariantInternal::VariantTypeTraits<T>::GetType();
    }

    template <typename T>
    Variant &operator=(T &&value)
    {
        Variant temp(value);
        Swap(temp);
        return *this;
    }

    void Swap(Variant &other)
    {
        std::swap(type, other.type);
        std::swap(data, other.data);
    }

    void Clear()
    {
        data.Clear();
        type = VariantType::Empty;
    }

    bool IsEmpty() const
    {
        return type == VariantType::Empty;
    }

    VariantType GetType() const
    {
        return type;
    }

    template <typename T>
    T GetValue() const
    {
        CT_ASSERT(VariantInternal::VariantTypeTraits<T>::GetType() == type);
        T ret;
        VariantInternal::ReadBytes(data, ret);
        return ret;
    }

    bool operator==(const Variant &other) const
    {
        return (type == other.type) && (data == other.data);
    }

    bool operator!=(const Variant &other) const
    {
        return !(*this == other);
    }

private:
    VariantType type = VariantType::Empty;
    Array<uint8> data;
};

CT_SCOPE_END