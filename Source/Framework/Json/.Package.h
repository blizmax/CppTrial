#pragma once

#include "Core/.Package.h"
#include "Core/String.h"
#include "Core/Exception.h"

namespace Json
{

enum class JsonType
{
    Null,
    Bool,
    Int64,
    Double,
    Object,
    Array,
    String,
};

}