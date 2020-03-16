#pragma once

#include "Reflection/.Package.h"
#include "Reflection/ParamInfo.h"
#include "Reflection/Constructor.h"
#include "Reflection/Property.h"
#include "Reflection/Method.h"
#include "Reflection/Enum.h"

namespace Reflection
{

class Constructor;
class Property;
class Method;
class Enum;

class Type : public MetaBase
{
    template<typename T>
    friend class TypeRegistrar;
public:
    Type(const Name &name, Type *baseType, SizeType size)
        : MetaBase(name), baseType(baseType), size(size)
    {
    }

    Type(const Name &name, Type *baseType, SizeType size, const Array<QualifiedType> &templates)
        : MetaBase(name), baseType(baseType), size(size), templates(templates)
    {
    }

    explicit Type(Enum* e);

    bool IsEnum() const
    {
        return innerEnum != nullptr;
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
        return templates.Count() > 1;
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

    template<typename T>
    static Type *GetType()
    {
        return TypeOf<T>();
    }

    static Type *GetType(const Name &name);

protected:
    Type *SetConstructors(const Array<Constructor *> value);
    Type *SetProperties(const Array<Property *> value);
    Type *SetMethods(const Array<Method *> value);

    static bool MatchParams(const Array<ParamInfo> &params, const Array<QualifiedType> &types);

protected:
    Type *baseType = nullptr;
    SizeType size = 0;
    Array<Type *> derivedTypes;
    Array<Constructor *> constructors;
    Array<Property *> properties;
    Array<Method *> methods;
    Array<QualifiedType> templates;
    Enum *innerEnum = nullptr;
};

} // namespace Reflection