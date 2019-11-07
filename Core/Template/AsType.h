#pragma once

#include "Core/Template/.Package.h"

template<typename T1, typename T2>
struct TAsType
{
    using value = T2;
};