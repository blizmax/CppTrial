#pragma once

#include "Reflection/.Package.h"
#include "Reflection/QualifiedType.h"

namespace Reflection
{

class Enum : public MetaBase
{
    template <typename T>
    friend class EnumRegistrar;

public:
    class Element : public MetaBase
    {
        friend class Enum;

    public:
        Element(const Name &name, int64 value)
            : MetaBase(name), value(value)
        {
        }

        int64 GetValue() const
        {
            return value;
        }

    protected:
        int64 value;
    };

public:
    Enum(const Name &name, Type *underlyingType, const Array<Element> &elements = {})
        : MetaBase(name), underlyingType(underlyingType), elements(elements)
    {
    }

    Type *GetUnderlyingType() const
    {
        return underlyingType;
    }

    int32 GetElementCount() const;
    bool IsValidIndex(int32 index) const;
    bool IsValidName(const Name &name) const;
    bool IsValidValue(int64 value) const;
    int32 GetIndexByName(const Name &name) const;
    int32 GetIndexByValue(int64 value) const;
    int64 GetValueByName(const Name &name) const;
    int64 GetValueByIndex(int32 index) const;
    Name GetNameByValue(int64 value) const;
    Name GetNameByIndex(int32 index) const;

protected:
    void CheckRange(int32 index) const;

    Type *underlyingType = nullptr;
    Array<Element> elements;
};

}