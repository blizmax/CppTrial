#pragma once

#include "Reflection/.Package.h"
#include "Reflection/QualifiedType.h"

CT_SCOPE_BEGIN

namespace Reflection
{

class ParamInfo : public MetaBase
{
    friend class Parameter;
public:
    ParamInfo(const QualifiedType &type, uint32 index) : type(type), index(index)
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

} // namespace Reflection

CT_SCOPE_END