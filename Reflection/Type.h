#pragma once

#include "Reflection/.Package.h"
#include "Reflection/QualifiedType.h"

CT_SCOPE_BEGIN

namespace Reflection
{

class Type : public MetaBase
{
};

template <>
struct TypeTraits<int32>
{
    static const Type *GetType()
    {
        static Type type;
        return &type;
    }
};

template <>
struct TypeTraits<String>
{
    static const Type *GetType()
    {
        static Type type;
        return &type;
    }
};

} // namespace Reflection

CT_SCOPE_END