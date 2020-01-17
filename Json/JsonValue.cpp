#include "Json/JsonValue.h"

String Json::JsonValue::AsString() const
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
    return String();
}

float Json::JsonValue::AsFloat() const
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
    return 0;
}

double Json::JsonValue::AsDouble() const
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
    return 0;
}


int32 Json::JsonValue::AsInt32() const
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
    return 0;
}

int64 Json::JsonValue::AsInt64() const
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
    return 0;
}

bool Json::JsonValue::AsBool() const
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
    return false;
}

SPtr<Json::JsonValue> Json::JsonValue::GetChild(SizeType index) const
{
    auto current = child;
    while (current && index > 0)
    {
        --index;
        current = current->next;
    }
    return current;
}

SPtr<Json::JsonValue> Json::JsonValue::GetChild(const String &name) const
{
    auto current = child;
    while (current && current->name != name)
    {
        current = current->next;
    }
    return current;
}

String Json::JsonValue::GetString(const String &name) const
{
    auto child = GetChild(name);
    return child->AsString();
}

String Json::JsonValue::GetString(SizeType index) const
{
    auto child = GetChild(index);
    return child->AsString();
}

float Json::JsonValue::GetFloat(const String &name) const
{
    auto child = GetChild(name);
    return child->AsFloat();
}

float Json::JsonValue::GetFloat(SizeType index) const
{
    auto child = GetChild(index);
    return child->AsFloat();
}

double Json::JsonValue::GetDouble(const String &name) const
{
    auto child = GetChild(name);
    return child->AsDouble();
}

double Json::JsonValue::GetDouble(SizeType index) const
{
    auto child = GetChild(index);
    return child->AsDouble();
}

int32 Json::JsonValue::GetInt32(const String &name) const
{
    auto child = GetChild(name);
    return child->AsInt32();
}

int32 Json::JsonValue::GetInt32(SizeType index) const
{
    auto child = GetChild(index);
    return child->AsInt32();
}

int64 Json::JsonValue::GetInt64(const String &name) const
{
    auto child = GetChild(name);
    return child->AsInt64();
}

int64 Json::JsonValue::GetInt64(SizeType index) const
{
    auto child = GetChild(index);
    return child->AsInt64();
}

bool Json::JsonValue::GetBool(const String &name) const
{
    auto child = GetChild(name);
    return child->AsBool();
}

bool Json::JsonValue::GetBool(SizeType index) const
{
    auto child = GetChild(index);
    return child->AsBool();
}