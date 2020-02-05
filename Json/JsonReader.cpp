#include "JsonReader.h"

SPtr<Json::JsonValue> Json::JsonReader::Parse(const CharType *cstr, SizeType offset, SizeType count)
{
    auto root = NewValue(JsonType::Object);

    if (count > 0)
    {
        ParseObject(root.get(), cstr, offset, offset + count - 1);
    }
    return root;
}

SizeType Json::JsonReader::ParseObject(JsonValue *json, const CharType *cstr, SizeType begin, SizeType end)
{
    bool dqOpen = false;
    SizeType dqPos;
    String childName;

    SizeType current = begin;
    while (current < end)
    {
        auto c = cstr[current];
        switch (c)
        {
        case CT_TEXT('\"'):
        {
            if (!dqOpen)
            {
                dqPos = current;
            }
            else
            {
                childName = String(cstr + dqPos + 1, current - dqPos - 1);
            }
            dqOpen = !dqOpen;
            ++current;
            break;
        }
        case CT_TEXT(':'):
        {
            current = ParseChild(json, childName, cstr, current + 1, end);
            break;
        }
        case CT_TEXT('}'):
        {
            return current + 1;
        }
        //TODO Check Error
        default:
        {
            ++current;
            break;
        }
        }
    }

    return current;
}

SizeType Json::JsonReader::ParseArray(JsonValue *json, const CharType *cstr, SizeType begin, SizeType end)
{
    auto current = ParseChild(json, String(), cstr, begin, end);
    while (current < end)
    {
        auto c = cstr[current];
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
        //TODO Check Error
        default:
        {
            ++current;
            break;
        }
        }
    }

    return current;
}

SizeType Json::JsonReader::ParseChild(JsonValue *parent, const String &name, const CharType *cstr, SizeType begin, SizeType end)
{
    SPtr<JsonValue> child = nullptr;
    SizeType retPos;
    SizeType current = begin;
    while (current)
    {
        auto c = cstr[current];
        switch (c)
        {
        case CT_TEXT('\"'):
        {
            child = NewValue(JsonType::String);
            retPos = ParseString(child.get(), cstr, current + 1, end);
            break;
        }
        case CT_TEXT('['):
        {
            child = NewValue(JsonType::Array);
            retPos = ParseArray(child.get(), cstr, current + 1, end);
            break;
        }
        case CT_TEXT('{'):
        {
            child = NewValue(JsonType::Object);
            retPos = ParseObject(child.get(), cstr, current + 1, end);
            break;
        }
        case CT_TEXT('}'): //TODO Check
        case CT_TEXT(']'): //TODO Check
        case CT_TEXT(','):
        {
            child = ParseOtherTypes(String(cstr + begin, current - begin));
            retPos = current;
            if (!child)
            {
                return retPos;
            }
            break;
        }
        default:
            break;
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

    //TODO Error
    return end;
}

SPtr<Json::JsonValue> Json::JsonReader::ParseOtherTypes(const String &str)
{
    SPtr<JsonValue> ret = nullptr;
    String tempStr = str.Trim();

    if (tempStr == CT_TEXT("true"))
    {
        ret = NewValue(JsonType::Bool);
        ret->variant = true;
        return ret;
    }

    if (tempStr == CT_TEXT("false"))
    {
        ret = NewValue(JsonType::Bool);
        ret->variant = false;
        return ret;
    }

    if (tempStr == CT_TEXT("null"))
    {
        ret = NewValue(JsonType::Null);
        return ret;
    }

    int64 tempInt = 0;
    if (StringConvert::TryParseInt64(tempStr, tempInt))
    {
        ret = NewValue(JsonType::Int64);
        ret->variant = tempInt;
        return ret;
    }

    double tempDouble = 0;
    if (StringConvert::TryParseDouble(tempStr, tempDouble))
    {
        ret = NewValue(JsonType::Double);
        ret->variant = tempDouble;
        return ret;
    }

    return ret;
}