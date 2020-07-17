#pragma once

#include "Core/.Package.h"
#include "Core/Exception.h"
#include "Core/String.h"

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