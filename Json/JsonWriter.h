#pragma once

#include "Json/.Package.h"
#include "Json/JsonValue.h"
#include "Core/Stack.h"

namespace Json
{
class JsonWriter
{
public:
    JsonWriter();
    ~JsonWriter() = default;

    JsonWriter& PushObject();
    JsonWriter& PushArray();
    JsonWriter& Name(const String& name);
    
    template<typename T>
    void Value(T&&);

private:
    bool CheckCanWriteName() const;

    int32 indentLevel = 0;
    bool named = false;
    Stack<JsonType> stack;
};     
}