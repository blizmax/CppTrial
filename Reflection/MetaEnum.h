#pragma once

#include "Reflection/MetaType.h"

CT_SCOPE_BEGIN

class MetaEnum : public MetaType
{
public:
    struct Element
    {
        String name;
        int64 value;
    };

protected:
    Array<Element> elements;
};

CT_SCOPE_END