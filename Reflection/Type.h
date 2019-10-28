#pragma once

#include "Reflection/.Package.h"
#include "Reflection/ParamInfo.h"

CT_SCOPE_BEGIN

namespace Reflection
{

class Constructor;
class Property;
class Method;
class Enum;

class Type : public MetaBase
{
public:
    Type(const Name &name, Type *baseType, SizeType size)
        : MetaBase(name), baseType(baseType), size(size)
    {
    }

    Type(const Name &name, Type *baseType, SizeType size, const Array<QualifiedType> &templates)
        : MetaBase(name), baseType(baseType), size(size), templates(templates)
    {
    }

    explicit Type(Enum *enumType);

    bool IsEnum() const
    {
        return isEnum;
    }

    bool IsBaseOf(const Type &other) const
    {
        Type *base = other.baseType;
        while (base)
        {
            if (this == base)
            {
                return true;
            }
            else
            {
                base = base->baseType;
            }
        }
        return false;
    }

    template <typename T>
    bool IsBaseOf() const
    {
        return IsBaseOf(TypeOf<T>());
    }

    bool IsDerivedFrom(const Type &other) const
    {
        return other.IsBaseOf(*this);
    }

    template <typename T>
    bool IsDerivedFrom() const
    {
        return IsDerivedFrom(TypeOf<T>());
    }

    SizeType GetSize() const
    {
        return size;
    }

    Type *GetBaseType() const
    {
        return baseType;
    }

    Array<Type *> GetDerivedTypes() const
    {
        return derivedTypes;
    }

    bool IsTemplate() const
    {
        return templates.Size() > 1;
    }

    const Array<QualifiedType> &GetTemplates() const
    {
        return templates;
    }

    Array<Constructor *> GetConstructors() const;
    Array<Property *> GetProperties() const;
    Array<Method *> GetMethods() const;

    Constructor *GetConstructor(const Array<QualifiedType> &typeList = {}) const;
    Property *GetProperty(const Name &name) const;
    Method *GetMethod(const Name &name) const;
    Method *GetMethod(const Name &name, const Array<QualifiedType> &typeList) const;

    Enum *GetEnum() const;

    static Type *GetType(const Name &name);

protected:
    Type *SetConstructors(const Array<Constructor *> value);
    Type *SetProperties(const Array<Property *> value);
    Type *SetMethods(const Array<Method *> value);

    static bool MatchParams(const Array<ParamInfo> &params, const Array<QualifiedType> &types);

protected:
    Array<Type *> derivedTypes;
    Array<Constructor *> constructors;
    Array<Property *> properties;
    Array<Method *> methods;
    Array<QualifiedType> templates;
    Type *baseType = nullptr;
    SizeType size = 0;
    bool isEnum = false;
};

} // namespace Reflection

CT_SCOPE_END