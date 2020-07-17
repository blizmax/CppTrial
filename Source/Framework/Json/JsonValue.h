#pragma once

#include "Json/.Package.h"
#include "Core/Variant.h"

namespace Json
{
class JsonValue
{
public:
    String name;
    JsonType type;
    Variant variant;

    SPtr<JsonValue> next;
    SPtr<JsonValue> child;
    SizeType size = 0;

    explicit JsonValue(JsonType type)
        : type(type)
    {
    }

    String AsString() const;
    float AsFloat() const;
    double AsDouble() const;
    int32 AsInt32() const;
    int64 AsInt64() const;
    bool AsBool() const;

    SPtr<JsonValue> GetChild(SizeType index) const;
    SPtr<JsonValue> GetChild(const String &name) const;

    String GetString(const String &name) const;
    String GetString(SizeType index) const;
    float GetFloat(const String &name) const;
    float GetFloat(SizeType index) const;
    double GetDouble(const String &name) const;
    double GetDouble(SizeType index) const;
    int32 GetInt32(const String &name) const;
    int32 GetInt32(SizeType index) const;
    int64 GetInt64(const String &name) const;
    int64 GetInt64(SizeType index) const;
    bool GetBool(const String &name) const;
    bool GetBool(SizeType index) const;

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
};
}