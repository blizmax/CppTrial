#pragma once

#include "Reflection/.Package.h"
#include "Reflection/QualifiedType.h"

namespace Reflection
{

class ParamInfo : public MetaBase
{
    friend struct Parameter;

public:
    ParamInfo(const QualifiedType &type, uint32 index)
        : type(type), index(index)
    {
    }

    QualifiedType GetType() const
    {
        return type;
    }

    uint32 GetIndex() const
    {
        return index;
    }

    Any GetDefaultValue() const
    {
        return defaultValue;
    }

    bool HasDefaultValue() const
    {
        return !defaultValue.IsEmpty();
    }

protected:
    QualifiedType type;
    uint32 index;
    Any defaultValue;
};

}