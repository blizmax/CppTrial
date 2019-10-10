#pragma once

#include "Core/General.h"
#include "Core/Array.h"
#include <variant>

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

template<typename T>
CT_INLINE void WriteBytes(Array<uint8>& bytes, T value)
{
    bytes.AppendUninitialized(sizeof(T));
    std::memcpy(bytes.GetData(), &value, sizeof(T));
}

template<typename T>
CT_INLINE bool ReadBytes(Array<uint8>& bytes, T& value)
{
    if(bytes.Size() >= sizeof(T))
    {
        std::memcpy(&value, bytes.GetData(), sizeof(T));
        return true;
    }
    return false;
}

CT_INLINE VariantType ToVariant(Array<uint8>& bytes, bool value)
{
    WriteBytes(bytes, value);
    return VariantType::Bool;
}

CT_INLINE VariantType ToVariant(Array<uint8>& bytes, int8 value)
{
    WriteBytes(bytes, value);
    return VariantType::Int8;
}

CT_INLINE VariantType ToVariant(Array<uint8>& bytes, int16 value)
{
    WriteBytes(bytes, value);
    return VariantType::Int16;
}

CT_INLINE VariantType ToVariant(Array<uint8>& bytes, int32 value)
{
    WriteBytes(bytes, value);
    return VariantType::Int32;
}

CT_INLINE VariantType ToVariant(Array<uint8>& bytes, int64 value)
{
    WriteBytes(bytes, value);
    return VariantType::Int64;
}

CT_INLINE VariantType ToVariant(Array<uint8>& bytes, uint8 value)
{
    WriteBytes(bytes, value);
    return VariantType::UInt8;
}

CT_INLINE VariantType ToVariant(Array<uint8>& bytes, uint16 value)
{
    WriteBytes(bytes, value);
    return VariantType::UInt16;
}

CT_INLINE VariantType ToVariant(Array<uint8>& bytes, uint32 value)
{
    WriteBytes(bytes, value);
    return VariantType::UInt32;
}

CT_INLINE VariantType ToVariant(Array<uint8>& bytes, uint64 value)
{
    WriteBytes(bytes, value);
    return VariantType::UInt64;
}

CT_INLINE VariantType ToVariant(Array<uint8>& bytes, char8 value)
{
    WriteBytes(bytes, value);
    return VariantType::Char;
}

CT_INLINE VariantType ToVariant(Array<uint8>& bytes, wchar value)
{
    WriteBytes(bytes, value);
    return VariantType::WChar;
}

CT_INLINE VariantType ToVariant(Array<uint8>& bytes, float value)
{
    WriteBytes(bytes, value);
    return VariantType::Float;
}

CT_INLINE VariantType ToVariant(Array<uint8>& bytes, double value)
{
    WriteBytes(bytes, value);
    return VariantType::Double;
}

CT_INLINE VariantType ToVariant(Array<uint8>& bytes, const String& value)
{
    SizeType byteNum = sizeof(CharType) * value.Length();
    bytes.AppendUninitialized(byteNum);
    std::memcpy(bytes.GetData(), value.GetPtr(), byteNum);
    return VariantType::String;
}

CT_INLINE bool FromVariant(Array<uint8>& bytes, bool& value)
{
    return ReadBytes(bytes, value);
}

CT_INLINE bool FromVariant(Array<uint8>& bytes, int8& value)
{
    return ReadBytes(bytes, value);
}

CT_INLINE bool FromVariant(Array<uint8>& bytes, int16& value)
{
    return ReadBytes(bytes, value);
}

CT_INLINE bool FromVariant(Array<uint8>& bytes, int32& value)
{
    return ReadBytes(bytes, value);
}

CT_INLINE bool FromVariant(Array<uint8>& bytes, int64& value)
{
    return ReadBytes(bytes, value);
}

CT_INLINE bool FromVariant(Array<uint8>& bytes, uint8& value)
{
    return ReadBytes(bytes, value);
}

CT_INLINE bool FromVariant(Array<uint8>& bytes, uint16& value)
{
    return ReadBytes(bytes, value);
}

CT_INLINE bool FromVariant(Array<uint8>& bytes, uint32& value)
{
    return ReadBytes(bytes, value);
}

CT_INLINE bool FromVariant(Array<uint8>& bytes, uint64& value)
{
    return ReadBytes(bytes, value);
}

CT_INLINE bool FromVariant(Array<uint8>& bytes, char8& value)
{
    return ReadBytes(bytes, value);
}

CT_INLINE bool FromVariant(Array<uint8>& bytes, wchar& value)
{
    return ReadBytes(bytes, value);
}

CT_INLINE bool FromVariant(Array<uint8>& bytes, float& value)
{
    return ReadBytes(bytes, value);
}

CT_INLINE bool FromVariant(Array<uint8>& bytes, double& value)
{
    return ReadBytes(bytes, value);
}

CT_INLINE bool FromVariant(Array<uint8>& bytes, String& value)
{
    SizeType charNum = bytes.Size() / sizeof(CharType);
    Array<CharType> temp;
    temp.AppendUninitialized(charNum);
    std::memcpy(temp.GetData(), bytes.GetData(), bytes.Size());
    value = temp.GetData();
    return true;
}

}

class Variant
{
public:
    Variant() = default;
    Variant(const Variant&) = default;
    Variant& operator=(const Variant&) = default;
    ~Variant() = default;

    Variant(Variant&& other) : type(other.type), data(std::move(other.data))
    {
        other.type = VariantType::Empty;
    }

    Variant& operator=(Variant&& other)
    {
        if(this != &other)
        {
            type = other.type;
            data = std::move(other.data);
            other.type = VariantType::Empty;
        }
        return *this;
    }

    template<typename T>
    Variant(T&& value)
    {
        type = VariantInternal::ToVariant(data, std::forward(value));
    }

    template<typename T>
    Variant& operator=(T&& value)
    {
        Variant temp(value);
        Swap(temp);
        return *this;
    }

    void Swap(Variant& other)
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

    template<typename T>
    T GetValue() const
    {
        T ret;
        VariantInternal::FromVariant(data, ret);
        return ret;
    }

    bool operator==(const Variant& other) const
    {
        return (type == other.type) && (data == other.data);
    }
    
    bool operator!=(const Variant& other) const
    {
        return !(*this == other);
    }

private:
    VariantType type = VariantType::Empty;
    Array<uint8> data;

};

CT_SCOPE_END