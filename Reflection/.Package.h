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

class MetaBase
{
protected:
    MetaBase() = default;

    MetaBase(const Name &name) : name(name)
    {
    }

public:
    virtual ~MetaBase() = default;

    Name GetName() const
    {
        return name;
    }

    void SetMetaData(const Name& name, const String& value)
    {
        dataMap.Put(name, value);
    }

protected:
    Name name;
    HashMap<Name, String> dataMap;
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