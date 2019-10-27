#pragma once

#include "Core/General.h"
#include "Core/String.h"
#include "Core/Array.h"
#include "Core/HashMap.h"
#include "Core/Template.h"
#include "Core/Any.h"
#include "Utils/Name.h"

CT_SCOPE_BEGIN

namespace Reflection
{

class Type;

class MetaData
{
public:
    HashMap<Name, String> dataMap;
};

class MetaBase
{
public:
    MetaBase() = default;

    MetaBase(const Name &name) : name(name)
    {
    }

    Name GetName() const
    {
        return name;
    }

protected:
    Name name;
    MetaData metaData;
};

template <typename T>
struct TypeTraits
{
    static Type *GetType()
    {
        return T::GetType();
    }
};

template <>
struct TypeTraits<std::nullptr_t>
{
    static Type *GetType()
    {
        return nullptr;
    }
};

template <typename T>
CT_INLINE Type *TypeOf()
{
    return Reflection::TypeTraits<T>::GetType();
}

} // namespace Reflection

CT_SCOPE_END