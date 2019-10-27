#pragma once

#include "Reflection/.Package.h"
#include "Reflection/QualifiedType.h"

CT_SCOPE_BEGIN

namespace Reflection
{

class Property : public MetaBase
{
protected:
    Property(const Name &name, Type *ownerType, const QualifiedType type, bool isStatic = false)
        : MetaBase(name), ownerType(ownerType), type(type), isStatic(isStatic)
    {
    }

public:
    Type *GetOwnerType() const
    {
        return ownerType;
    }

    QualifiedType GetType() const
    {
        return type;
    }

    bool IsStatic() const
    {
        return isStatic;
    }

    bool IsReadonly() const
    {
        return type.IsConst();
    }

    virtual void Set(Any obj, Any value) const
    {
        CT_THROW("Not implement");
    }

    virtual Any Get(Any obj) const
    {
        CT_THROW("Not implement");
        return Any();
    }

protected:
    Type *ownerType;
    QualifiedType type;
    bool isStatic;
};

template <typename OwnerType, typename PropertyType>
class MemberProperty : public Property
{
public:
    MemberProperty(const Name &name, PropertyType OwnerType::*property)
        : Property(name, TypeOf<OwnerType>(), GetQualifiedType<PropertyType>()), property(property)
    {
    }

    virtual void Set(Any obj, Any value) const override
    {
        ((OwnerType *)obj)->*property = value.Cast<PropertyType>();
    }

    virtual Any Get(Any obj) const override
    {
        return ((OwnerType *)obj)->*property;
    }

private:
    PropertyType OwnerType::*property;
};

template <typename OwnerType, typename PropertyType>
class ReadonlyProperty : public Property
{
public:
    ReadonlyProperty(const Name &name, PropertyType OwnerType::*property)
        : Property(name, TypeOf<OwnerType>(), GetQualifiedType<PropertyType>()), property(property)
    {
    }

    virtual void Set(Any obj, Any value) const override
    {
        CT_THROW("Attempt to modify a readonly property.");
    }

    virtual Any Get(Any obj) const override
    {
        return ((OwnerType *)obj)->*property;
    }

private:
    PropertyType OwnerType::*property;
};

template <typename PropertyType>
class StaticProperty : public Property
{
public:
    StaticProperty(const Name &name, PropertyType *property)
        : Property(name, nullptr, GetQualifiedType<PropertyType>(), true), property(property)
    {
    }

    virtual void Set(Any obj, Any value) const override
    {
        *property = value.Cast<PropertyType>();
    }

    virtual Any Get(Any obj) const override
    {
        return *property;
    }

private:
    PropertyType *property;
};

} // namespace Reflection

CT_SCOPE_END