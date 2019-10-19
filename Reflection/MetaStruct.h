#pragma once

#include "Reflection/MetaType.h"
#include "Reflection/MetaProperty.h"

CT_SCOPE_BEGIN

class MetaStruct : public MetaType
{
public:


public : Array<MetaProperty> properties;
};


CT_SCOPE_END