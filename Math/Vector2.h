#include "Math/.Package.h"

class Vector2
{
public:
    float x = 0.0f;
    float y = 0.0f;

public:
    Vector2() = default;

    Vector2(float x, float y) : x(x), y(y)
    {
    }

    float operator[](uint32 i) const
    {
        CT_ASSERT(i < 2);
        return *(&x + i);
    }

    float &operator[](uint32 i)
    {
        CT_ASSERT(i < 2);
        return *(&x + i);
    }
};