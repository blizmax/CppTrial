#pragma once

#include "Math/.Package.h"

namespace Math
{

// Range[0, bound], a neg bound will be set to 0
int32 RandInt(int32 bound);

// Range[min(a, b), max(a, b)]
int32 RandInt(int32 a, int32 b);

// Range[0.0, 1.0)
float Rand01();

// Range[0, max(uint32)]
uint32 Rand();

void RandSeed(uint32 seed);

}