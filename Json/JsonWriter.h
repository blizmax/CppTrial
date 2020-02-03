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

    JsonWriter &Reset();
    JsonWriter &PushObject();
    JsonWriter &PushArray();
    JsonWriter &Pop();
    JsonWriter &Name(const String &name);

    JsonWriter &Value(const String &value);

    JsonWriter &Value(std::nullptr_t)
    {
        return Value(String(CT_TEXT("null")));
    }

    template <typename T, typename = typename TEnableIf<TIsIntegral<T>::value || TIsFloatingPoint<T>::value, T>::value>
    JsonWriter &Value(T value)
    {
        return Value(StringConvert::ToString(value));
    }

    void Write(String &dest);

private:
    struct State
    {
        bool array;
        bool first;
    };

    State &GetCurrentState();
    bool CheckCanWriteValue();
    bool CheckCanWriteName();
    bool CheckCanPop();

    bool named = false;
    Stack<State> stack;
    String buffer;
};
} // namespace Json