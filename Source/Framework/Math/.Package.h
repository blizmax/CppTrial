#pragma once

#include "Core/.Package.h"
#include "Core/Hash.h"
#include "Core/Math.h"
#include "Core/String.h"

class Vector2;
class Vector3;
class Vector4;
class Matrix3;
class Matrix4;
class Quat;

namespace Math
{

CT_INLINE float Lerp(float a, float b, float t)
{
    return a * (1.0f - t) + b * t;
}

CT_INLINE float Smoothstep(float a, float b, float x)
{
    float t = Clamp((x - a) / (b - 1), 0.0f, 1.0f);
    return t * t * (3.0f - 2.0f * t);
}


}