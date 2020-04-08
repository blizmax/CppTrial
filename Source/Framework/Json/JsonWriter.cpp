#include "Json/JsonWriter.h"

Json::JsonWriter::JsonWriter(bool pretty) : pretty(pretty)
{
    stack.Push({false, true});
    buffer += CT_TEXT('{');
}

Json::JsonWriter &Json::JsonWriter::Reset()
{
    named = false;
    buffer.Clear();
    stack.Clear();

    stack.Push({false, true});
    buffer += CT_TEXT('{');

    return *this;
}

Json::JsonWriter &Json::JsonWriter::Name(const String &name)
{
    if (CheckCanWriteName())
    {
        named = true;

        if (GetCurrentState().first)
            GetCurrentState().first = false;
        else
            buffer += CT_TEXT(", ");

        if (pretty)
        {
            buffer += CT_TEXT('\n');
            buffer += String(CT_TEXT(' '), 4 * GetIndentLevel());
        }
        buffer += CT_TEXT('\"');
        buffer += name;
        buffer += CT_TEXT('\"');
        buffer += CT_TEXT(": ");
    }

    return *this;
}

Json::JsonWriter &Json::JsonWriter::PushObject()
{
    if (CheckCanWriteValue())
    {
        stack.Push({false, true});
        buffer += CT_TEXT('{');
    }

    return *this;
}

Json::JsonWriter &Json::JsonWriter::PushArray()
{
    if (CheckCanWriteValue())
    {
        stack.Push({true, true});
        buffer += CT_TEXT('[');
    }
    return *this;
}

Json::JsonWriter &Json::JsonWriter::Pop()
{
    if (CheckCanPop())
    {
        const bool array = GetCurrentState().array;
        const bool first = GetCurrentState().first;
        stack.Pop();

        if (array)
        {
            buffer += CT_TEXT(']');
        }
        else
        {
            if (pretty)
            {
                if (!first)
                {
                    buffer += CT_TEXT('\n');
                    buffer += String(CT_TEXT(' '), 4 * GetIndentLevel());
                }
            }
            buffer += CT_TEXT('}');
        }
    }

    return *this;
}

void Json::JsonWriter::Write(String &dest)
{
    while (!stack.IsEmpty())
    {
        Pop();
    }

    dest.Swap(buffer);
    Reset();
}

Json::JsonWriter &Json::JsonWriter::WriteValue(const String &value, bool quote)
{
    if (CheckCanWriteValue())
    {
        if (quote)
            buffer += CT_TEXT('\"');
        buffer += value;
        if (quote)
            buffer += CT_TEXT('\"');
    }

    return *this;
}

int32 Json::JsonWriter::GetIndentLevel() const
{
    return stack.Count();
}

Json::JsonWriter::State &Json::JsonWriter::GetCurrentState()
{
    return stack.Top();
}

bool Json::JsonWriter::CheckCanWriteValue()
{
    State &state = GetCurrentState();
    if (state.array)
    {
        if (state.first)
            state.first = false;
        else
            buffer += CT_TEXT(", ");
    }
    else
    {
        if (!named)
        {
            CT_EXCEPTION(Json, "Name must be set.");
            return false;
        }
        named = false;
    }
    return true;
}

bool Json::JsonWriter::CheckCanPop()
{
    if (named)
    {
        CT_EXCEPTION(Json, "Value must be set since named.");
        return false;
    }
    if (stack.IsEmpty())
    {
        return false;
    }
    return true;
}

bool Json::JsonWriter::CheckCanWriteName()
{
    if (named || GetCurrentState().array)
    {
        CT_EXCEPTION(Json, "Current state must be a json object.");
        return false;
    }
    return true;
}