#pragma once

#include "Core/.Package.h"

#define CT_DECL_FLAGS(e)     \
    namespace e              \
    {                        \
    enum _Enum : uint32;     \
    }                        \
    typedef uint32 e##Flags; \
    enum e::_Enum : uint32
