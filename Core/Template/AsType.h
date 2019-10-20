#pragma once

#include "Core/Template/Base.h"

CT_SCOPE_BEGIN

template<typename T1, typename T2>
struct TAsType
{
    using value = T2;
};

CT_SCOPE_END