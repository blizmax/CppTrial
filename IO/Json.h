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

    bool IsNumber() const
    {
        return type == JsonType::Double || type == JsonType::Int64;
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
        default:
            CT_ASSERT(false);
        }
    }

    // String GetString(const String& name, const String& defaultValue)
    // {

    // }
};

} // namespace IO