#pragma once

#include "Core/General.h"
#include "Core/String.h"
#include "Core/Array.h"
#include "Core/HashMap.h"
#include "Core/Template.h"
#include "Core/Any.h"
#include "Core/Exception.h"
#include "Utils/Name.h"

CT_SCOPE_BEGIN

namespace Reflection
{

// If CT_REFLECTION_AUTO_POPULATE != 0, Type will auto populate after registered, more convenient but not thread safe.
// For multithread environment, set CT_REFLECTION_AUTO_POPULATE = 0 and call PopulateAllTypes() before use Reflection library.
#ifndef CT_REFLECTION_AUTO_POPULATE
#define CT_REFLECTION_AUTO_POPULATE 0
#endif


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