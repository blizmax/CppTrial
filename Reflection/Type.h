#pragma once

#include "Core/General.h"
#include "Core/String.h"

CT_SCOPE_BEGIN

class Type
{
protected:
    String name;
    const Type* baseType;

public:
    String& GetName() const;
        

};

CT_SCOPE_END