#include "JsonReader.h"

SPtr<Json::JsonValue> Json::JsonReader::Parse(const CharType *cstr, SizeType offset, SizeType count)
{
    auto root = NewValue(JsonType::Object);

    if (count > 0)
    {
        SizeType end = offset + count;
        SizeType current = offset;

        auto c = cstr[current];
        while (current < end)
        {
            if (IsWhitespace(c))
            {
                ++current;
                continue;
            }
            if (c == CT_TEXT('{'))
            {
                ParseObject(root.get(), cstr, current + 1, end);
            }
            else
            {
                CT_EXCEPTION(Json, "Bad json format.");
            }
            break;
        }
    }
    return root;
}

SizeType Json::JsonReader::ParseObject(JsonValue *json, const CharType *cstr, SizeType begin, SizeType end)
{
    bool nameFlag = false;
    bool commaFlag = false;
    String childName;

    SizeType current = begin;
    while (current < end)
    {
        auto c = cstr[current];
        if (IsWhitespace(c))
        {
            ++current;
            continue;
        }

        switch (c)
        {
        case CT_TEXT('\"'):
        {
            if (!nameFlag)
            {
                SizeType dqPos = current + 1;
                while (dqPos < end)
                {
                    if (cstr[dqPos++] == CT_TEXT('\"'))
                    {
                        childName = String(cstr + current + 1, dqPos - current - 2);
                        nameFlag = true;
                        current = dqPos;
                        break;
                    }
                }
                if (!nameFlag)
                {
                    CT_EXCEPTION(Json, "Parse name error.");
                    return end;
                }
            }
            else
            {
                CT_EXCEPTION(Json, "Name already set.");
                return end;
            }
            break;
        }
        case CT_TEXT(':'):
        {
            if (!nameFlag)
            {
                CT_EXCEPTION(Json, "Name expected.");
                return end;
            }
            else
            {
                current = ParseChild(json, childName, cstr, current + 1, end);
                nameFlag = false;
            }
            break;
        }
        case CT_TEXT(','):
        {
            if (json->size == 0)
            {
                CT_EXCEPTION(Json, "Cannot add comma since children size is 0.");
                return end;
            }
            else if (nameFlag)
            {
                CT_EXCEPTION(Json, "Cannot add comma between name and value.");
                return end;
            }
            ++current;
            break;
        }
        case CT_TEXT('}'):
        {
            if (nameFlag)
            {
                CT_EXCEPTION(Json, "Value expected.");
                return end;
            }
            return current + 1;
        }
        default:
        {
            CT_EXCEPTION(Json, "Illegal character on parse object.");
            return end;
        }
        }
    }

    CT_EXCEPTION(Json, "Unfinished json object.");
    return current;
}

SizeType Json::JsonReader::ParseArray(JsonValue *json, const CharType *cstr, SizeType begin, SizeType end)
{
    auto current = ParseChild(json, String(), cstr, begin, end);
    while (current < end)
    {
        auto c = cstr[current];
        if (IsWhitespace(c))
        {
            ++current;
            continue;
        }

        switch (c)
        {
        case CT_TEXT(','):
        {
            current = ParseChild(json, String(), cstr, current + 1, end);
            break;
        }
        case CT_TEXT(']'):
        {
            return current + 1;
        }
        default:
        {
            CT_EXCEPTION(Json, "Illegal character on parse array.");
            return end;
        }
        }
    }

    CT_EXCEPTION(Json, "Unfinished json array.");
    return current;
}

SizeType Json::JsonReader::ParseChild(JsonValue *parent, const String &name, const CharType *cstr, SizeType begin, SizeType end)
{
    SPtr<JsonValue> child = nullptr;
    SizeType retPos;
    SizeType current = begin;
    bool charFlag = false;

    while (current)
    {
        auto c = cstr[current];
        if (IsWhitespace(c))
        {
            ++current;
            continue;
        }

        switch (c)
        {
        case CT_TEXT('\"'):
        {
            if (charFlag)
            {
                CT_EXCEPTION(Json, "Parse value error");
                return end;
            }
            child = NewValue(JsonType::String);
            retPos = ParseString(child.get(), cstr, current + 1, end);
            break;
        }
        case CT_TEXT('['):
        {
            if (charFlag)
            {
                CT_EXCEPTION(Json, "Parse value error");
                return end;
            }
            child = NewValue(JsonType::Array);
            retPos = ParseArray(child.get(), cstr, current + 1, end);
            break;
        }
        case CT_TEXT('{'):
        {
            if (charFlag)
            {
                CT_EXCEPTION(Json, "Parse value error");
                return end;
            }
            child = NewValue(JsonType::Object);
            retPos = ParseObject(child.get(), cstr, current + 1, end);
            break;
        }
        case CT_TEXT('}'):
        case CT_TEXT(']'):
        case CT_TEXT(','):
        {
            if (c == CT_TEXT('}') && parent->IsArray())
            {
                CT_EXCEPTION(Json, "Can not close object since current state is array.");
                return end;
            }
            else if (c == CT_TEXT(']') && parent->IsObject())
            {
                CT_EXCEPTION(Json, "Can not close array since current state is object.");
                return end;
            }

            retPos = current;
            String str = String(cstr + begin, current - begin);
            str = str.Trim(CT_TEXT(' ')).Trim(CT_TEXT('\t')).Trim(CT_TEXT('\n')).Trim(CT_TEXT('\r'));

            if (!str.IsEmpty())
            {
                child = ParseOtherTypes(str);
            }
            if (!child)
            {
                return retPos;
            }
            break;
        }
        default:
        {
            charFlag = true;
            break;
        }
        } // !switch

        if (child)
        {
            child->name = name;
            if (parent->size == 0)
            {
                parent->child = child;
            }
            else
            {
                auto prev = parent->GetChild(parent->size - 1);
                prev->next = child;
            }
            ++parent->size;

            return retPos;
        }

        ++current;
    }

    CT_EXCEPTION(Json, "Unfinished json value.");
    return end;
}

SizeType Json::JsonReader::ParseString(JsonValue *json, const CharType *cstr, SizeType begin, SizeType end)
{
    SizeType current = begin;
    while (current < end)
    {
        auto c = cstr[current++];
        if (c == CT_TEXT('\"'))
        {
            json->variant = String(cstr + begin, current - begin);
            return current;
        }
    }
    CT_EXCEPTION(Json, "Parse string value error");
    return end;
}

SPtr<Json::JsonValue> Json::JsonReader::ParseOtherTypes(const String &str)
{
    SPtr<JsonValue> ret = nullptr;

    if (str == CT_TEXT("true"))
    {
        ret = NewValue(JsonType::Bool);
        ret->variant = true;
        return ret;
    }

    if (str == CT_TEXT("false"))
    {
        ret = NewValue(JsonType::Bool);
        ret->variant = false;
        return ret;
    }

    if (str == CT_TEXT("null"))
    {
        ret = NewValue(JsonType::Null);
        return ret;
    }

    int64 tempInt = 0;
    if (StringConvert::TryParseInt64(str, tempInt))
    {
        ret = NewValue(JsonType::Int64);
        ret->variant = tempInt;
        return ret;
    }

    double tempDouble = 0;
    if (StringConvert::TryParseDouble(str, tempDouble))
    {
        ret = NewValue(JsonType::Double);
        ret->variant = tempDouble;
        return ret;
    }

    CT_EXCEPTION(Json, "Parse other types error.");
    return ret;
}