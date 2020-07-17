#include "Reflection/QualifiedType.h"
#include "Reflection/Registry.h"
#include "Reflection/Type.h"

namespace Reflection
{

String QualifiedType::ToString() const
{
    String ret;
    if (!type)
        ret = CT_TEXT("null");

    if (IsConst())
        ret += CT_TEXT("const ");
    if (IsVolatile())
        ret += CT_TEXT("volatile ");
    ret += type->GetName().ToString();
    for (uint32 i = 0; i < pointerCount; ++i)
        ret += CT_TEXT('*');
    if (IsLValueReference())
        ret += CT_TEXT('&');
    else if (IsRValueReference())
        ret += CT_TEXT("&&");
    return ret;
}

bool QualifiedType::CanCast(const QualifiedType &other) const
{
    if (!type || !other.type)
        return false;

    if (*this == other)
        return true;

    // TODO number(int32 -> int64)
    // TODO enum value
    // TODO class dynamic cast
    return false;
}

bool QualifiedType::IsEnum() const
{
    return type != nullptr ? type->IsEnum() : false;
}

bool QualifiedType::IsBool() const
{
    return type == TypeOf<bool>();
}

bool QualifiedType::IsNumber() const
{
    return type == TypeOf<int8>() || type == TypeOf<int16>() || type == TypeOf<int32>() || type == TypeOf<int64>() || type == TypeOf<uint8>() || type == TypeOf<uint16>() || type == TypeOf<uint32>() || type == TypeOf<uint64>() || type == TypeOf<float>() || type == TypeOf<double>();
}

bool QualifiedType::IsInteger() const
{
    return type == TypeOf<int8>() || type == TypeOf<int16>() || type == TypeOf<int32>() || type == TypeOf<int64>() || type == TypeOf<uint8>() || type == TypeOf<uint16>() || type == TypeOf<uint32>() || type == TypeOf<uint64>();
}

bool QualifiedType::IsFloat() const
{
    return type == TypeOf<float>() || type == TypeOf<double>();
}

}