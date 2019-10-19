#pragma once

#include "Core/General.h"
#include "Core/String.h"
#include "Core/Array.h"
#include "Core/HashMap.h"
#include "Core/Template.h"
#include "Utils/Name.h"

CT_SCOPE_BEGIN

class Type;

class MetaData
{
public:
    HashMap<Name, String> dataMap;
};

template <typename T>
struct ReflectTraits
{
    static const Type *GetType()
    {
        return T::GetType();
    }
};

template <>
struct ReflectTraits<std::nullptr_t>
{
    static const Type *GetType()
    {
        return nullptr;
    }
};

template <typename T>
CT_INLINE Type *TypeOf()
{
    return ReflectTraits<T>::GetType();
}

CT_SCOPE_END