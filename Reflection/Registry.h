#pragma once

#include "Reflection/Type.h"

CT_SCOPE_BEGIN

namespace Reflection
{
class Registry
{
private:
    Registry();
    ~Registry();

public:
    static Registry *GetInstance();

    bool RegisterType(Type *type);
    void UnregisterType(Type *type);

private:
    HashMap<Name, Type *> typeMap;
};

#define CT_REFLECTION_BUILTIN_TYPE_TRAITS(TYPE_)                                   \
    template <>                                                                    \
    struct TypeTraits<TYPE_>                                                       \
    {                                                                              \
        static Type *GetType()                                                     \
        {                                                                          \
            static Type *type = nullptr;                                           \
            if (!type)                                                             \
            {                                                                      \
                type = Memory::New<Type>(CT_TEXT(#TYPE_), nullptr, sizeof(TYPE_)); \
                Registry::GetInstance()->RegisterType(type);                       \
            }                                                                      \
            return type;                                                           \
        }                                                                          \
    };

CT_REFLECTION_BUILTIN_TYPE_TRAITS(int8);
CT_REFLECTION_BUILTIN_TYPE_TRAITS(int16);
CT_REFLECTION_BUILTIN_TYPE_TRAITS(int32);
CT_REFLECTION_BUILTIN_TYPE_TRAITS(int64);
CT_REFLECTION_BUILTIN_TYPE_TRAITS(uint8);
CT_REFLECTION_BUILTIN_TYPE_TRAITS(uint16);
CT_REFLECTION_BUILTIN_TYPE_TRAITS(uint32);
CT_REFLECTION_BUILTIN_TYPE_TRAITS(uint64);
CT_REFLECTION_BUILTIN_TYPE_TRAITS(char8);
CT_REFLECTION_BUILTIN_TYPE_TRAITS(char16);
CT_REFLECTION_BUILTIN_TYPE_TRAITS(char32);
CT_REFLECTION_BUILTIN_TYPE_TRAITS(wchar);
CT_REFLECTION_BUILTIN_TYPE_TRAITS(bool);
CT_REFLECTION_BUILTIN_TYPE_TRAITS(float);
CT_REFLECTION_BUILTIN_TYPE_TRAITS(double);
CT_REFLECTION_BUILTIN_TYPE_TRAITS(String);

} // namespace Reflection

CT_SCOPE_END