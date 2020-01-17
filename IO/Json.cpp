#include "IO/Json.h"

String IO::JsonValue::AsString() const
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

float IO::JsonValue::AsFloat() const
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

double IO::JsonValue::AsDouble() const
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

double IO::JsonValue::AsDouble() const
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

int32 IO::JsonValue::AsInt32() const
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

int64 IO::JsonValue::AsInt64() const
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

bool IO::JsonValue::AsBool() const
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

// SizeType IO::JsonReader::ParseObject(JsonValue *json, const CharType *cstr, SizeType begin, SizeType end)
// {

// }