#pragma once

#include "Reflection/MetaType.h"

CT_SCOPE_BEGIN

class MetaProperty : public MetaType
{
public:
    template <typename StructType, typename FieldType>
    MetaProperty(FieldType StructType::*property, const Name &name) : MetaType(name)
    {
        size = sizeof(F);
        offset = GetOffset(property);
    }

protected:
    template <typename StructType, typename FieldType>
    uint32 GetOffset(FieldType StructType::*property)
    {
        return (char *)&((StructType *)nullptr->*property) - (char *)nullptr;
    }

public:
    using MetaType::name;

    uint32 size;
    uint32 offset;
};

CT_SCOPE_END