#pragma once

#include "Core/General.h"
#include "Core/String.h"
#include "Core/Array.h"
#include "Core/HashMap.h"

CT_SCOPE_BEGIN

class MetaData
{
public:
protected:
    HashMap<String, String> dataMap;
};

class MetaType
{
protected:
    String name;
    MetaData metaData;
};

CT_SCOPE_END