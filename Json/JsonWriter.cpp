#include "Json/JsonWriter.h"

Json::JsonWriter::JsonWriter()
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
        if (GetCurrentState().array)
        {
            buffer += CT_TEXT(']');
        }
        else
        {
            buffer += CT_TEXT('}');
        }
        stack.Pop();
    }

    return *this;
}

Json::JsonWriter &Json::JsonWriter::Value(const String &value)
{
    if (CheckCanWriteValue())
    {
        buffer += value;
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

Json::JsonWriter::State &Json::JsonWriter::GetCurrentState()
{
    if (stack.IsEmpty())
    {
        stack.Push({true, true});
        buffer += CT_TEXT('[');
    }
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
            return false;
        else
            named = false;
    }
    return true;
}

bool Json::JsonWriter::CheckCanPop()
{
    if (named)
    {
        if(state.first)
            state.first = false;
        else
            buffer += CT_TEXT(", ");
    }
    else
    {
        if(!named)
            return false;
        else
            named = false; 
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
        return false;
    }
    return true;
}