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

    JsonValue *next = nullptr;
    JsonValue *prev = nullptr;
    JsonValue *parent = nullptr;
    JsonValue *child = nullptr;

    JsonValue(JsonType type) : type(type)
    {
    }

    JsonValue *GetChild(SizeType index) const
    {
        JsonValue *current = child;
        while (current && index > 0)
        {
            --index;
            current = current->next;
        }
        return current;
    }

    JsonValue *GetChild(const String &name) const
    {
        JsonValue *current = child;
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

    String AsString() const
    {
        switch (type)
        {
        case JsonType::String:
            return variant.GetValue<String>();
        case JsonType::Double:
            return StringConvert::ToString(variant.GetValue<double>());
        case JsonType::Int64:
            return StringConvert::ToString(variant.GetValue<int64>());
        case JsonType::Bool:
            return StringConvert::ToString(variant.GetValue<bool>());
        case JsonType::Null:
            return CT_TEXT("null");
        }
        CT_ASSERT(false);
    }

    float AsFloat() const
    {
        switch (type)
        {
        case JsonType::String:
            return StringConvert::ParseFloat(variant.GetValue<String>());
        case JsonType::Double:
            return static_cast<float>(variant.GetValue<double>());
        case JsonType::Int64:
            return static_cast<float>(variant.GetValue<int64>());
        case JsonType::Bool:
            return variant.GetValue<bool>() ? 0 : 1;
        }
        CT_ASSERT(false);
    }

    double AsDouble() const
    {
        switch (type)
        {
        case JsonType::String:
            return StringConvert::ParseDouble(variant.GetValue<String>());
        case JsonType::Double:
            return variant.GetValue<double>();
        case JsonType::Int64:
            return static_cast<double>(variant.GetValue<int64>());
        case JsonType::Bool:
            return variant.GetValue<bool>() ? 0 : 1;
        }
        CT_ASSERT(false);
    }

    int32 AsInt32() const
    {
        switch (type)
        {
        case JsonType::String:
            return StringConvert::ParseInt32(variant.GetValue<String>());
        case JsonType::Double:
            return static_cast<int32>(variant.GetValue<double>());
        case JsonType::Int64:
            return static_cast<int32>(variant.GetValue<int64>());
        case JsonType::Bool:
            return variant.GetValue<bool>() ? 0 : 1;
        }
        CT_ASSERT(false);
    }

    int64 AsInt64() const
    {
        switch (type)
        {
        case JsonType::String:
            return StringConvert::ParseInt64(variant.GetValue<String>());
        case JsonType::Double:
            return static_cast<int64>(variant.GetValue<double>());
        case JsonType::Int64:
            return variant.GetValue<int64>();
        case JsonType::Bool:
            return variant.GetValue<bool>() ? 0 : 1;
        }
        CT_ASSERT(false);
    }

    bool AsBool() const
    {
        switch (type)
        {
        case JsonType::String:
            return variant.GetValue<String>() == CT_TEXT("true");
        case JsonType::Double:
            return variant.GetValue<double>() != 0;
        case JsonType::Int64:
            return variant.GetValue<int64>() != 0;
        case JsonType::Bool:
            return variant.GetValue<bool>();
        }
        CT_ASSERT(false);
    }

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

} // namespace IO