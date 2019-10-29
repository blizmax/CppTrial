#include "Reflection/Type.h"

CT_SCOPE_BEGIN

namespace Reflection
{

Type::Type(Enum *enumType)
    : MetaBase(enumType->GetName()), isEnum(true)
{
}

bool Type::MatchParams(const Array<ParamInfo> &params, const Array<QualifiedType> &types)
{
    if (params.Size() != types.Size())
    {
        return false;
    }
    for (decltype(params.Size()) i = 0; i < params.Size(); ++i)
    {
        if (params[i].GetType() != types[i])
        {
            return false;
        }
    }
    return true;
}

Type *Type::SetConstructors(const Array<Constructor *> value)
{
    constructors = value;
    return this;
}

Type *Type::SetProperties(const Array<Property *> value)
{
    properties = value;
    return this;
}

Type *Type::SetMethods(const Array<Method *> value)
{
    methods = value;
    return this;
}

Array<Constructor *> Type::GetConstructors() const
{
    return constructors;
}

Array<Property *> Type::GetProperties() const
{
    return properties;
}

Array<Method *> Type::GetMethods() const
{
    return methods;
}

Constructor *Type::GetConstructor(const Array<QualifiedType> &typeList) const
{
    for (const auto c : constructors)
    {
        if (MatchParams(c->GetParamInfos(), typeList))
        {
            return c;
        }
    }
    return nullptr;
}

Property *Type::GetProperty(const Name &name) const
{
    for (const auto v : properties)
    {
        if (v->GetName() == name)
        {
            return v;
        }
    }

    if (baseType)
    {
        return baseType->GetProperty(name);
    }
    return nullptr;
}

Method *Type::GetMethod(const Name &name) const
{
    for (const auto v : methods)
    {
        if (v->GetName() == name)
        {
            return v;
        }
    }

    if (baseType)
    {
        return baseType->GetMethod(name);
    }
    return nullptr;
}

Method *Type::GetMethod(const Name &name, const Array<QualifiedType> &typeList) const
{
    for (const auto v : methods)
    {
        if (v->GetName() == name && MatchParams(v->GetParamInfos(), typeList))
        {
            return v;
        }
    }

    if (baseType)
    {
        return baseType->GetMethod(name, typeList);
    }
    return nullptr;
}

Enum *Type::GetEnum() const
{
    //TODO
    return nullptr;
}

Type *Type::GetType(const Name &name)
{
    //TODO
    return nullptr;
}

} // namespace Reflection

CT_SCOPE_END