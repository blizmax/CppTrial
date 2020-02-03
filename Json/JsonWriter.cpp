#include "Json/JsonWriter.h"

Json::JsonWriter& Json::JsonWriter::Name(const String& name)
{
    if(CheckCanWriteName())
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
    if (CheckCanPushState())
    {
        stack.Push({false, true});
        buffer += CT_TEXT('{');
    }
    return *this;
}

Json::JsonWriter::State &Json::JsonWriter::GetCurrentState()
{
    if(stack.IsEmpty())
    {
        PushObject();
    }
    return stack.Top();
}

bool Json::JsonWriter::CheckCanPushState()
{
    if (stack.IsEmpty())
        return true;

    State &state = stack.Top();    
    if(state.array)
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