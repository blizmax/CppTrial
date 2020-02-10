#pragma once

#include "Json/.Package.h"
#include "Json/JsonValue.h"

namespace Json
{
class JsonReader
{
public:
    SPtr<JsonValue> Parse(const CharType *cstr, SizeType offset, SizeType count);

    SPtr<JsonValue> Parse(const String &source)
    {
        return Parse(source.CStr(), 0, source.Length());
    }

private:
    SPtr<JsonValue> NewValue(JsonType type) const
    {
        return Memory::MakeShared<JsonValue>(type);
    }

    SizeType ParseObject(JsonValue *json, const CharType *cstr, SizeType begin, SizeType end);
    SizeType ParseArray(JsonValue *json, const CharType *cstr, SizeType begin, SizeType end);
    SizeType ParseChild(JsonValue *parent, const String &name, const CharType *cstr, SizeType begin, SizeType end);
    SizeType ParseString(JsonValue *json, const CharType *cstr, SizeType begin, SizeType end);
    SPtr<JsonValue> ParseOtherTypes(const String &str);

    bool IsWhitespace(CharType value) const
    {
        return value == CT_TEXT(' ') || value == CT_TEXT('\t') ||
               value == CT_TEXT('\n') || value == CT_TEXT('\r');
    }
};

} // namespace Json
