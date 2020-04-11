#pragma once

#include "Json/.Package.h"
#include "Json/JsonValue.h"
#include "Core/Stack.h"

namespace Json
{
class JsonWriter
{
public:
    JsonWriter(bool pretty = false);

    JsonWriter &Reset();
    JsonWriter &PushObject();
    JsonWriter &PushArray();
    JsonWriter &Pop();
    JsonWriter &Name(const String &name);

    JsonWriter &Value(const String &value)
    {
        return WriteValue(value, true);
    }

    JsonWriter &Value(std::nullptr_t)
    {
        return WriteValue(CT_TEXT("null"));
    }

    template <typename T, typename = typename TEnableIf<TIsIntegral<T>::value || TIsFloatingPoint<T>::value, T>::type>
    JsonWriter &Value(T value)
    {
        return WriteValue(StringConvert::ToString(value));
    }

    void Write(String &dest);

private:
    struct State
    {
        bool array;
        bool first;
    };

    JsonWriter &WriteValue(const String& value, bool quote = false);

    State &GetCurrentState();
    int32 GetIndentLevel() const;

    bool CheckCanWriteValue();
    bool CheckCanWriteName();
    bool CheckCanPop();

private:
    bool pretty = false;
    bool named = false;
    Stack<State> stack;
    String buffer;
};
} // namespace Json