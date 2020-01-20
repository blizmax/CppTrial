#include "Json/JsonWriter.h"

Json::JsonWriter::JsonWriter()
{
    stack.Push(JsonType::Object);
}

Json::JsonWriter& Json::JsonWriter::Name(const String& name)
{
    if(CheckCanWriteName())
    {
        named = true;
    }


    return *this;
}

Json::JsonWriter& Json::JsonWriter::PushObject()
{

}

bool Json::JsonWriter::CheckCanWriteName() const
{
    if(named)
    {
        return false;
    }
    if(stack.Top() != JsonType::Object)
    {
        return false;
    }
    return true;
}