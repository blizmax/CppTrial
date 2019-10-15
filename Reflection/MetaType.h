#pragma once

#include "Core/General.h"
#include "Core/String.h"
#include "Core/Array.h"
#include "Core/HashMap.h"
#include "Utils/Name.h"

CT_SCOPE_BEGIN

class MetaData
{
public:
protected:
    HashMap<Name, String> dataMap;
};

class MetaType
{
protected:
    Name name;
    MetaData metaData;
};

CT_SCOPE_END