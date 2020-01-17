#pragma once

#include "IO/.Package.h"
#include "Core/Variant.h"

namespace IO
{

enum class JsonType
{
    Null,
    Bool,
    Int64,
    Double,
    Object,
    Array,
    String,
};

class JsonValue
{
public:
    String name;
    JsonType type;
    Variant variant;

    SPtr<JsonValue> next;
    SPtr<JsonValue> child;

    JsonValue(JsonType type) : type(type)
    {
    }

    SPtr<JsonValue> GetChild(SizeType index) const
    {
        auto current = child;
        while (current && index > 0)
        {
            --index;
            current = current->next;
        }
        return current;
    }

    SPtr<JsonValue> GetChild(const String &name) const
    {
        auto current = child;
        while (current && current->name != name)
        {
            current = current->next;
        }
        return current;
    }

    bool HasChild(const String &name) const
    {
        return GetChild(name) != nullptr;
    }

    bool IsArray() const
    {
        return type == JsonType::Array;
    }

    bool IsObject() const
    {
        return type == JsonType::Object;
    }

    bool IsString() const
    {
        return type == JsonType::String;
    }

    bool IsInt() const
    {
        return type == JsonType::Int64;
    }

    bool IsFloat() const
    {
        return type == JsonType::Double;
    }

    bool IsNumber() const
    {
        return type == JsonType::Double || type == JsonType::Int64;
    }

    bool IsBool() const
    {
        return type == JsonType::Bool;
    }

    bool IsNull() const
    {
        return type == JsonType::Null;
    }

    bool IsValue() const
    {
        return type != JsonType::Array && type != JsonType::Object;
    }

    String AsString() const;
    float AsFloat() const;
    double AsDouble() const;
    int32 AsInt32() const;
    int64 AsInt64() const;
    bool AsBool() const;

    String GetString(const String &name) const
    {
        auto child = GetChild(name);
        return child->AsString();
    }

    String GetString(SizeType index) const
    {
        auto child = GetChild(index);
        return child->AsString();
    }

    float GetFloat(const String &name) const
    {
        auto child = GetChild(name);
        return child->AsFloat();
    }

    float GetFloat(SizeType index) const
    {
        auto child = GetChild(index);
        return child->AsFloat();
    }

    double GetDouble(const String &name) const
    {
        auto child = GetChild(name);
        return child->AsDouble();
    }

    double GetDouble(SizeType index) const
    {
        auto child = GetChild(index);
        return child->AsDouble();
    }

    int32 GetInt32(const String &name) const
    {
        auto child = GetChild(name);
        return child->AsInt32();
    }

    int32 GetInt32(SizeType index) const
    {
        auto child = GetChild(index);
        return child->AsInt32();
    }

    int64 GetInt64(const String &name) const
    {
        auto child = GetChild(name);
        return child->AsInt64();
    }

    int64 GetInt64(SizeType index) const
    {
        auto child = GetChild(index);
        return child->AsInt64();
    }

    bool GetBool(const String &name) const
    {
        auto child = GetChild(name);
        return child->AsBool();
    }

    bool GetBool(SizeType index) const
    {
        auto child = GetChild(index);
        return child->AsBool();
    }
};

class JsonReader
{
public:
    SPtr<JsonValue> Parse(const String &source)
    {
        return Parse(source.CStr(), 0, source.Length());
    }

    SPtr<JsonValue> Parse(const CharType *cstr, SizeType offset, SizeType count)
    {
        auto root = NewValue(JsonType::Object);

        if (count > 0)
        {
            ParseObject(root, cstr, offset, offset + count - 1);
        }
        return root;
    }

private:
    SPtr<JsonValue> NewValue(JsonType type) const
    {
        return Memory::MakeShared<JsonValue>(type);
    }

    SizeType ParseObject(SPtr<JsonValue> json, const CharType *cstr, SizeType begin, SizeType end);
    SizeType ParseArray(SPtr<JsonValue> json, const CharType *cstr, SizeType begin, SizeType end);
    SizeType ParseChild(SPtr<JsonValue> parent, const CharType *cstr, SizeType begin, SizeType end, SizeType nameBegin, SizeType nameEnd);
    SizeType ParseString(SPtr<JsonValue> json, const CharType *cstr, SizeType begin, SizeType end);
    SPtr<JsonValue> ParseOtherTypes(const CharType *cstr, SizeType begin, SizeType end);
};

} // namespace IO